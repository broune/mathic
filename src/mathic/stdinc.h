#ifndef MATHIC_STDINC_GUARD
#define MATHIC_STDINC_GUARD

#ifdef MATHIC_SLOW_DEBUG
// for asserts that take a long time.
#define MATHIC_SLOW_ASSERT(X) MATHIC_ASSERT(X)
#ifndef MATHIC_DEBUG
#define MATHIC_DEBUG
#endif
#else
#define MATHIC_SLOW_ASSERT(X)
#endif

#ifdef MATHIC_DEBUG
#include <cassert>
#define MATHIC_ASSERT(X) assert(X)
#else
#define MATHIC_ASSERT(X)
#endif

namespace mathic {
  static unsigned long const BitsPerByte = 8;
}

#ifndef MATHIC_NO_MIC_NAMESPACE
namespace mic {
  using namespace mathic;
}
#endif

#endif
