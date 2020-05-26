#ifndef TOUR_TREE_MODEL_GUARD
#define TOUR_TREE_MODEL_GUARD

#include "mathic/TourTree.h"
#include "Model.h"

template<bool FastIndex>
struct TourTreeModelBase {
  static const bool fastIndex = FastIndex;
};

template<bool OnSpans, bool FastIndex>
class TourTreeModel : public Model<
  OnSpans, false, true, mathic::TourTree, TourTreeModelBase<FastIndex> > {};

#endif
