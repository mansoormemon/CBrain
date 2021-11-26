#include "CBrain/Layer.h"

#include "Private/Memory.h"
#include "Private/Layer.h"

CBLayer *CBLayerNew() {
  return CBAllocate_Memory(sizeof(CBLayer));
}

void CBLayerDelete(CBLayer **layerRef) {
  // Todo: Refactor.
  if (layerRef != nil) {
    CBLayer *previous = nil;
    bool flag = false;
    CBReset_Layer(*layerRef);
    if (*layerRef != nil) {
      if ((*layerRef)->previous != nil) {
        previous = (*layerRef)->previous;
        flag = true;
      }
    }
    CBFree_Memory(CastTo(layerRef, void **));
    if (flag) {
      previous->next = nil;
    }
  }
}
