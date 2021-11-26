#include <CBrain/Image.h>
#include <CBrain/ImgProc.h>
#include <CBrain/NeuralNet.h>

#include <stdio.h>

CBTensor *preprocessData(const char *in) {
  CBImage *img = CBImageRead(in);
  CBImgProcTransform(img, CBIPT_RGB_TO_GRAY);
  CBImgProcBinarize(img);
  CBImgProcResize(img, 28, 28);
  CBImgProcInvert(img);
  CBTensor *data = CBTensorFlatten(img);

  CBImageDelete(&img);

  return data;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage:\n"
           "\t%s [in_image]\n", argv[0]);
    return -1;
  }
  const char *in = argv[1];

  CBTensor *input = preprocessData(in);
  i32 inputSize = CastTo(CBTensorTotal(input), i32);

  CBNeuralNet *model = CBNeuralNetFrom(inputSize);

  CBNeuralNetAddLayer(model, 392, 0.3F, CBLAF_ArcTan, -1);
  CBNeuralNetAddLayer(model, 56, 0.4F, CBLAF_Sigmoid, -2);
  CBNeuralNetAddLayer(model, 28, 0.4F, CBLAF_ReLU, -3);
  CBNeuralNetAddLayer(model, 10, 1.2F, CBLAF_ELU, -4);

  CBNeuralNetSummary(model);

  CBTensor *output = CBNeuralNetPredict(model, input);

  printf("---\n"
         "Output:\n"
         "---\n");

  printf("shape = (%d, %d)\n", output->shape[0], output->shape[1]);

  for (i32 i = 0; i < output->shape[0]; i += 1) {
    printf("[ class = %02d, strength = % g ]\n", i, *CBTensorElemAt(output, f32, i, 0));
  }

  CBTensorDelete(&output);

  CBTensorDelete(&input);
  CBNeuralNetDelete(&model);
  return 0;
}
