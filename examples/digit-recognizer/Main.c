#include <CBrain/Image.h>
#include <CBrain/ImgProc.h>
#include <CBrain/NeuralNet.h>

#include <time.h>
#include <stdio.h>

CBTensor *preprocessData(const char *in) {
  CBImage *img = CBImageRead(in);
  CBImgProcTransform(img, CBIPT_RGB_TO_GRAY);
  CBImgProcBinarize(img);
  CBImgProcResize(img, 28, 28);
  CBImgProcInvert(img);

  CBImageWrite(img, "processed.jpg");

  CBTensor *data = CBTensorFlatten(img);

  CBImageDelete(&img);

  return data;
}

int reverseInt(int i) {
  u8 c1 = 0, c2 = 0, c3 = 0, c4 = 0;

  c1 = i & 255;
  c2 = (i >> 8) & 255;
  c3 = (i >> 16) & 255;
  c4 = (i >> 24) & 255;

  return ((i32) c1 << 24) + ((i32) c2 << 16) + ((i32) c3 << 8) + c4;
}

CBTensor *readMNISTDataset(const char *path, i32 size) {
  FILE *fptr = fopen(path, "rb");

  if (fptr == nil) {
    printf("Error: Could not load dataset!\n");
    return nil;
  }

  i32 magicNumber = 0, noOfImages = 0, rows = 0, cols = 0;

  fread((char *) &magicNumber, sizeof(magicNumber), 1, fptr);
  magicNumber = reverseInt(magicNumber);

  fread((char *) &noOfImages, sizeof(noOfImages), 1, fptr);
  noOfImages = reverseInt(noOfImages);

  fread((char *) &rows, sizeof(rows), 1, fptr);
  rows = reverseInt(rows);

  fread((char *) &cols, sizeof(cols), 1, fptr);
  cols = reverseInt(cols);

  if (size > noOfImages) { size = noOfImages; }

  i32 totalSizeOfImg = rows * cols;

  CBTensor *dataset_u8 = CBTensorFrom(u8, 2, size, totalSizeOfImg);

  fread((u8 *) dataset_u8->data, sizeof(u8), CBTensorTotal(dataset_u8), fptr);

  printf("Dataset => magic_number=%d, total=%d, image=%dx%d, size=%d, shape=(%dx%d)\n",
         magicNumber,
         noOfImages,
         rows,
         cols,
         size,
         size,
         totalSizeOfImg);

  // CBImage *img = CBImageNew();
  //
  // img->channels = 1;
  // img->width = 28;
  // img->height = 28;
  //
  // char buf[100];
  //
  // for (i32 i = 0; i < size; i += 1) {
  //   img->data = (u8 *)dataset_u8->data + (totalSizeOfImg * i);
  //   snprintf(buf, 100, "out_%d.jpg", i);
  //   CBImageWrite(img, buf);
  // }
  //
  // img->data = nil;
  //
  // CBImageDelete(&img);

  CBTensor *dataset = CBTensorFrom(f32, 2, dataset_u8->shape[0], dataset_u8->shape[1]);

  for (i32 i = 0; i < dataset_u8->shape[0]; i += 1) {
    for (i32 j = 0; j < dataset_u8->shape[1]; j += 1) {
      *CBTensorElemAt(dataset, f32, i, j) = CastTo(*CBTensorElemAt(dataset_u8, u8, i, j), f32);
    }
  }
  CBTensorDelete(&dataset_u8);

  fclose(fptr);
  return dataset;
}

CBTensor *readMNISTDatasetLabels(const char *path, i32 size) {
  FILE *fptr = fopen(path, "rb");

  if (fptr == nil) {
    printf("Error: Could not load dataset!\n");
    return nil;
  }

  i32 magicNumber = 0, noOfLabels = 0;

  fread((char *) &magicNumber, sizeof(magicNumber), 1, fptr);
  magicNumber = reverseInt(magicNumber);

  fread((char *) &noOfLabels, sizeof(noOfLabels), 1, fptr);
  noOfLabels = reverseInt(noOfLabels);

  if (size > noOfLabels) { size = noOfLabels; }

  printf("Labels => magic_number=%d, total=%d, size=%d\n", magicNumber, noOfLabels, size);

  CBTensor *labels = CBTensorFrom(u8, 1, size);
  fread((u8 *) labels->data, sizeof(u8), size, fptr);

  fclose(fptr);
  return labels;
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Usage:\n"
           "\t%s [in_image] [train_images] [train_labels]\n", argv[0]);
    return -1;
  }

  const char *in = argv[1];
  const char *trainImagesPath = argv[2];
  const char *trainLabelsPath = argv[3];

  printf("---\n"
         "Input: %s\n"
         "---\n", in);

  printf("\n");

  CBTensor *input = preprocessData(in);
  i32 inputSize = CastTo(CBTensorTotal(input), i32);

  CBNeuralNet *model = CBNeuralNetFrom(inputSize);

  CBNeuralNetAddLayer(model, 56, 0.0F, CBLAF_Linear, 82);
  CBNeuralNetAddLayer(model, 28, 0.0F, CBLAF_ELU, 6488);
  CBNeuralNetAddLayer(model, 10, 0.0F, CBLAF_ArcTan, 748);

  CBNeuralNetSummary(model);

  printf("\n");

  CBTensor *trainData = readMNISTDataset(trainImagesPath, 1024);
  CBTensor *trainLabels = readMNISTDatasetLabels(trainLabelsPath, 1024);

  printf("Training...\n");
  clock_t begin = clock();

  CBNeuralNetTrain(model, 0.001, 400, CBCF_MeanCrossEntropy, 3, trainData, trainLabels);

  clock_t end = clock();
  double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

  printf("Time taken: %.2lfs\n", time_spent);

  // CBTensor *output = CBNeuralNetPredict(model, input, true);
  //
  // i32 oClass = CBFindMax(output);
  // f32 strength = *CBTensorElemAt(output, f32, oClass, 0);
  //
  // printf("---\n"
  //        "Output: shape = (%d, %d)\n"
  //        "---\n", output->shape[0], output->shape[1]);
  //
  // for (i32 i = 0; i < output->shape[0]; i += 1) {
  //   printf("[ class = %02d, strength = % .4f ]\n", i, *CBTensorElemAt(output, f32, i, 0));
  // }

  // printf("---\n"
  //        "Prediction: %d, Strength: %g (%.2f%%)\n"
  //        "---\n", oClass, strength, strength * 100);

  CBTensorDelete(&trainData);
  CBTensorDelete(&trainLabels);

  // CBTensorDelete(&output);

  // CBTensorDelete(&input);
  CBNeuralNetDelete(&model);

  return 0;
}
