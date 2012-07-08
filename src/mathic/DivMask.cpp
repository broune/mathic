#include "DivMask.h"

#ifdef MATHIC_TRACK_DIV_MASK_HIT_RATIO
namespace mathic {
  namespace DivMaskStats {
    unsigned long maskComputes = 0;
    unsigned long maskChecks = 0;
    unsigned long maskHits = 0;
    unsigned long divChecks = 0;
    unsigned long divDivides = 0;
    unsigned long divHits = 0;
  }
}
#endif
