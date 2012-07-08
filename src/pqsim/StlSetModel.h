#ifndef STL_SET_MODEL_GUARD
#define STL_SET_MODEL_GUARD

#include "Model.h"
#include "mathic/StlSet.h"

template<bool OnSpans>
class StlSetModel : public Model<OnSpans, false, false, mathic::StlSet> {};

#endif
