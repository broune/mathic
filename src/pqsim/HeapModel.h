#ifndef HEAP_MODEL_GUARD
#define HEAP_MODEL_GUARD

#include "Model.h"
#include "mathic/Heap.h"

template<bool FastIndex>
struct HeapModelBase {
  static const bool fastIndex = FastIndex;
};

template<bool OnSpans, bool Deduplicate, bool FastIndex>
class HeapModel : public Model<
  OnSpans, Deduplicate, true, mathic::Heap, HeapModelBase<FastIndex> > {};

#endif
