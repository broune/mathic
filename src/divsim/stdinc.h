#ifdef STDINC_GUARD
#error stdinc.h included twice
#endif
#define STDINC_GUARD

#ifdef _MSC_VER // For Microsoft Compiler in Visual Studio C++.
#pragma warning (push, 1) // Reduce warning level for GMP headers.
#endif

#ifdef PROFILE
#define NO_PINLINE NO_INLINE
#else
#define NO_PINLINE
#endif

#ifndef _MSC_VER
#define NO_INLINE __attribute__ ((noinline))
#endif

#ifdef _MSC_VER // For Microsoft Compiler in Visual Studio C++.
#define NO_INLINE __declspec(noinline)
#pragma warning (pop) // Go back to previous warning level.
#pragma warning (disable: 4996) // std::copy is flagged as dangerous.
#pragma warning (disable: 4290) // VC++ ignores throw () specification.
#pragma warning (disable: 4127) // Warns about using "while (true)".
#pragma warning (disable: 4100) // Warns about unused parameters.
#pragma warning (disable: 4800) // Warns on int to bool conversion.
#pragma warning (disable: 4146) // Warns on unary minus on unsigned (bit trick)

// This warning warns about using the this pointer in base member
// initializer lists. This is a pretty good warning as that can
// obviously easily go wrong, but it is pretty useful to do as well,
// so the warning is turned off.
#pragma warning (disable: 4355)

#ifdef _DEBUG
#define DEBUG
#endif

#endif

#include <cstddef>
#include <memory>

#ifdef DEBUG
#include <iostream> // Useful for debugging.
#define PRINT
#include <cassert>
#define ASSERT(X) assert(X);
#define IF_DEBUG(X) X
#else
#define ASSERT(X)
#define IF_DEBUG(X)
#endif

static const size_t BitsPerByte = 8;
static const size_t MemoryAlignment = sizeof(void*);
