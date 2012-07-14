#ifndef MATHIC_STDINC_GUARD
#define MATHIC_STDINC_GUARD

#if (defined MATHIC_DEBUG) && (defined MATHIC_NDEBUG)
#error Both MATHIC_DEBUG and MATHIC_NDEBUG defined
#endif

#ifdef MATHIC_DEBUG
#include <cassert>
#define MATHIC_ASSERT(X) assert(X)
#else
#define MATHIC_ASSERT(X)
#endif

namespace mathic {
  static const unsigned long BitsPerByte = 8;
}

#ifndef MATHIC_NO_MIC_NAMESPACE
namespace mic {
  using namespace mathic;
}
#endif

#endif
