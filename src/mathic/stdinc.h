#ifndef MATHIC_STDINC_GUARD
#define MATHIC_STDINC_GUARD

#if (defined DEBUG || defined _DEBUG) && (!(defined MATCHIC_NDEBUG))
#ifndef MATHIC_DEBUG
#define MATHIC_DEBUG
#endif
#include <cassert>
#define MATHIC_ASSERT(X) assert(X)
#endif

#ifndef MATHIC_ASSERT
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
