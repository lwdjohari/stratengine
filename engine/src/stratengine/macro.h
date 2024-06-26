#pragma once

#define STRATE_NAMESPACE namespace stratengine
#define STRATE_BEGIN_NAMESPACE STRATE_NAMESPACE {
#define STRATE_END_NAMESPACE }
#define STRATE_INNER_NAMESPACE(arg) \
  STRATE_BEGIN_NAMESPACE            \
  namespace arg {
#define STRATE_INNER_END_NAMESPACE \
  }                                \
  }

// Architecture
#define STRATE_ARCH_32BIT 0
#define STRATE_ARCH_64BIT 0

// Compiler
#define STRATE_COMPILER_CLANG 0
#define STRATE_COMPILER_CLANG_ANALYZER 0
#define STRATE_COMPILER_GCC 0
#define STRATE_COMPILER_MSVC 0

// Endianness
#define STRATE_ENDIAN_BIG 0
#define STRATE_ENDIAN_LITTLE 0

// CPU
#define STRATE_CPU_ARM 0
#define STRATE_CPU_JIT 0
#define STRATE_CPU_MIPS 0
#define STRATE_CPU_PPC 0
#define STRATE_CPU_RISCV 0
#define STRATE_CPU_X86 0

// C Runtime
#define STRATE_CRT_BIONIC 0
#define STRATE_CRT_BSD 0
#define STRATE_CRT_GLIBC 0
#define STRATE_CRT_LIBCXX 0
#define STRATE_CRT_MINGW 0
#define STRATE_CRT_MSVC 0
#define STRATE_CRT_NEWLIB 0

#ifndef STRATE_CRT_NONE
#define STRATE_CRT_NONE 0
#endif  // STRATE_CRT_NONE

// Language standard version
#define STRATE_CPP17 201703L
#define STRATE_CPP20 202002L
#define STRATE_CPP23 202207L

// Platform
#define STRATE_PLATFORM_ANDROID 0
#define STRATE_PLATFORM_BSD 0
#define STRATE_PLATFORM_WEBGL 0
#define STRATE_PLATFORM_HAIKU 0
#define STRATE_PLATFORM_HURD 0
#define STRATE_PLATFORM_IOS 0
#define STRATE_PLATFORM_LINUX 0
#define STRATE_PLATFORM_NX 0
#define STRATE_PLATFORM_OSX 0
#define STRATE_PLATFORM_PS4 0
#define STRATE_PLATFORM_PS5 0
#define STRATE_PLATFORM_RPI 0
#define STRATE_PLATFORM_VISIONOS 0
#define STRATE_PLATFORM_WINDOWS 0
#define STRATE_PLATFORM_WINRT 0
#define STRATE_PLATFORM_XBOXONE 0

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Compilers
#if defined(__clang__)
// clang defines __GNUC__ or _MSC_VER
#undef STRATE_COMPILER_CLANG
#define STRATE_COMPILER_CLANG \
  (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#if defined(__clang_analyzer__)
#undef STRATE_COMPILER_CLANG_ANALYZER
#define STRATE_COMPILER_CLANG_ANALYZER 1
#endif  // defined(__clang_analyzer__)
#elif defined(_MSC_VER)
#undef STRATE_COMPILER_MSVC
#define STRATE_COMPILER_MSVC _MSC_VER
#elif defined(__GNUC__)
#undef STRATE_COMPILER_GCC
#define STRATE_COMPILER_GCC \
  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#error "STRATE_COMPILER_* is not defined!"
#endif  //

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Architectures
#if defined(__arm__) || defined(__aarch64__) || defined(_M_ARM)
#undef STRATE_CPU_ARM
#define STRATE_CPU_ARM 1
#define STRATE_CACHE_LINE_SIZE 64
#elif defined(__MIPSEL__) || defined(__mips_isa_rev) || defined(__mips64)
#undef STRATE_CPU_MIPS
#define STRATE_CPU_MIPS 1
#define STRATE_CACHE_LINE_SIZE 64
#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
#undef STRATE_CPU_PPC
#define STRATE_CPU_PPC 1
#define STRATE_CACHE_LINE_SIZE 128
#elif defined(__riscv) || defined(__riscv__) || defined(RISCVEL)
#undef STRATE_CPU_RISCV
#define STRATE_CPU_RISCV 1
#define STRATE_CACHE_LINE_SIZE 64
#elif defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || \
    defined(__x86_64__)
#undef STRATE_CPU_X86
#define STRATE_CPU_X86 1
#define STRATE_CACHE_LINE_SIZE 64
#else  // Doesn't have CPU defined.
#undef STRATE_CPU_JIT
#define STRATE_CPU_JIT 1
#define STRATE_CACHE_LINE_SIZE 64
#endif  //

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) ||    \
    defined(__64BIT__) || defined(__mips64) || defined(__powerpc64__) || \
    defined(__ppc64__) || defined(__LP64__)
#undef STRATE_ARCH_64BIT
#define STRATE_ARCH_64BIT 64
#else
#undef STRATE_ARCH_32BIT
#define STRATE_ARCH_32BIT 32
#endif  //

#if STRATE_CPU_PPC
// __BIG_ENDIAN__ is gcc predefined macro
#if defined(__BIG_ENDIAN__)
#undef STRATE_ENDIAN_BIG
#define STRATE_ENDIAN_BIG 1
#else
#undef STRATE_ENDIAN_LITTLE
#define STRATE_ENDIAN_LITTLE 1
#endif
#else
#undef STRATE_ENDIAN_LITTLE
#define STRATE_ENDIAN_LITTLE 1
#endif  // STRATE_CPU_PPC

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Operating_Systems
#if defined(_DURANGO) || defined(_XBOX_ONE)
#undef STRATE_PLATFORM_XBOXONE
#define STRATE_PLATFORM_XBOXONE 1
#elif defined(_WIN32) || defined(_WIN64)
// http://msdn.microsoft.com/en-us/library/6sehtctf.aspx
#ifndef NOMINMAX
#define NOMINMAX
#endif  // NOMINMAX
//  If _USING_V110_SDK71_ is defined it means we are using the v110_xp or
//  v120_xp toolset.
#if defined(_MSC_VER) && (_MSC_VER >= 1700) && !defined(_USING_V110_SDK71_)
#include <winapifamily.h>
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1700) && (!_USING_V110_SDK71_)
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
#undef STRATE_PLATFORM_WINDOWS
#if !defined(WINVER) && !defined(_WIN32_WINNT)
#if STRATE_ARCH_64BIT
//				When building 64-bit target Win7 and above.
#define WINVER 0x0601
#define _WIN32_WINNT 0x0601
#else
//				Windows Server 2003 with SP1, Windows XP with
// SP2 and above
#define WINVER 0x0502
#define _WIN32_WINNT 0x0502
#endif  // STRATE_ARCH_64BIT
#endif  // !defined(WINVER) && !defined(_WIN32_WINNT)
#define STRATE_PLATFORM_WINDOWS _WIN32_WINNT
#else
#undef STRATE_PLATFORM_WINRT
#define STRATE_PLATFORM_WINRT 1
#endif
#elif defined(__ANDROID__)
// Android compiler defines __linux__
#include <sys/cdefs.h>  // Defines __BIONIC__ and includes android/api-level.h
#undef STRATE_PLATFORM_ANDROID
#define STRATE_PLATFORM_ANDROID __ANDROID_API__
#elif defined(__VCCOREVER__)
// RaspberryPi compiler defines __linux__
#undef STRATE_PLATFORM_RPI
#define STRATE_PLATFORM_RPI 1
#elif defined(__linux__)
#undef STRATE_PLATFORM_LINUX
#define STRATE_PLATFORM_LINUX 1
#elif defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) || \
    defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#undef STRATE_PLATFORM_IOS
#define STRATE_PLATFORM_IOS 1
#elif defined(__has_builtin) && __has_builtin(__is_target_os) && \
    __is_target_os(xros)
#undef STRATE_PLATFORM_VISIONOS
#define STRATE_PLATFORM_VISIONOS 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#undef STRATE_PLATFORM_OSX
#define STRATE_PLATFORM_OSX __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#elif defined(__EMSCRIPTEN__)
#include <emscripten/version.h>
#undef STRATE_PLATFORM_WEBGL
#define STRATE_PLATFORM_WEBGL                                  \
  (__EMSCRIPTEN_major__ * 10000 + __EMSCRIPTEN_minor__ * 100 + \
   __EMSCRIPTEN_tiny__)
#elif defined(__ORBIS__)
#undef STRATE_PLATFORM_PS4
#define STRATE_PLATFORM_PS4 1
#elif defined(__PROSPERO__)
#undef STRATE_PLATFORM_PS5
#define STRATE_PLATFORM_PS5 1
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || \
    defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#undef STRATE_PLATFORM_BSD
#define STRATE_PLATFORM_BSD 1
#elif defined(__GNU__)
#undef STRATE_PLATFORM_HURD
#define STRATE_PLATFORM_HURD 1
#elif defined(__NX__)
#undef STRATE_PLATFORM_NX
#define STRATE_PLATFORM_NX 1
#elif defined(__HAIKU__)
#undef STRATE_PLATFORM_HAIKU
#define STRATE_PLATFORM_HAIKU 1
#endif  //

#if !STRATE_CRT_NONE
// https://sourceforge.net/p/predef/wiki/Libraries/
#if defined(__BIONIC__)
#undef STRATE_CRT_BIONIC
#define STRATE_CRT_BIONIC 1
#elif defined(_MSC_VER)
#undef STRATE_CRT_MSVC
#define STRATE_CRT_MSVC 1
#elif defined(__GLIBC__)
#undef STRATE_CRT_GLIBC
#define STRATE_CRT_GLIBC (__GLIBC__ * 10000 + __GLIBC_MINOR__ * 100)
#elif defined(__MINGW32__) || defined(__MINGW64__)
#undef STRATE_CRT_MINGW
#define STRATE_CRT_MINGW 1
#elif defined(__apple_build_version__) || defined(__ORBIS__) || \
    defined(__EMSCRIPTEN__) || defined(__llvm__) || defined(__HAIKU__)
#undef STRATE_CRT_LIBCXX
#define STRATE_CRT_LIBCXX 1
#elif STRATE_PLATFORM_BSD
#undef STRATE_CRT_BSD
#define STRATE_CRT_BSD 1
#endif  //

#if !STRATE_CRT_BIONIC && !STRATE_CRT_BSD && !STRATE_CRT_GLIBC &&  \
    !STRATE_CRT_LIBCXX && !STRATE_CRT_MINGW && !STRATE_CRT_MSVC && \
    !STRATE_CRT_NEWLIB
#undef STRATE_CRT_NONE
#define STRATE_CRT_NONE 1
#endif  // STRATE_CRT_*
#endif  // !STRATE_CRT_NONE

///
#define STRATE_PLATFORM_POSIX                                                \
  (0 || STRATE_PLATFORM_ANDROID || STRATE_PLATFORM_BSD ||                    \
   STRATE_PLATFORM_WEBGL || STRATE_PLATFORM_HAIKU || STRATE_PLATFORM_HURD || \
   STRATE_PLATFORM_IOS || STRATE_PLATFORM_LINUX || STRATE_PLATFORM_NX ||     \
   STRATE_PLATFORM_OSX || STRATE_PLATFORM_PS4 || STRATE_PLATFORM_PS5 ||      \
   STRATE_PLATFORM_RPI || STRATE_PLATFORM_VISIONOS)

///
#define STRATE_PLATFORM_NONE                                                  \
  !(0 || STRATE_PLATFORM_ANDROID || STRATE_PLATFORM_BSD ||                    \
    STRATE_PLATFORM_WEBGL || STRATE_PLATFORM_HAIKU || STRATE_PLATFORM_HURD || \
    STRATE_PLATFORM_IOS || STRATE_PLATFORM_LINUX || STRATE_PLATFORM_NX ||     \
    STRATE_PLATFORM_OSX || STRATE_PLATFORM_PS4 || STRATE_PLATFORM_PS5 ||      \
    STRATE_PLATFORM_RPI || STRATE_PLATFORM_VISIONOS ||                        \
    STRATE_PLATFORM_WINDOWS || STRATE_PLATFORM_WINRT ||                       \
    STRATE_PLATFORM_XBOXONE)

///
#define STRATE_PLATFORM_OS_CONSOLE                                          \
  (0 || STRATE_PLATFORM_NX || STRATE_PLATFORM_PS4 || STRATE_PLATFORM_PS5 || \
   STRATE_PLATFORM_WINRT || STRATE_PLATFORM_XBOXONE)

///
#define STRATE_PLATFORM_OS_DESKTOP                                         \
  (0 || STRATE_PLATFORM_BSD || STRATE_PLATFORM_HAIKU ||                    \
   STRATE_PLATFORM_HURD || STRATE_PLATFORM_LINUX || STRATE_PLATFORM_OSX || \
   STRATE_PLATFORM_WINDOWS)

///
#define STRATE_PLATFORM_OS_EMBEDDED (0 || STRATE_PLATFORM_RPI)

///
#define STRATE_PLATFORM_OS_MOBILE \
  (0 || STRATE_PLATFORM_ANDROID || STRATE_PLATFORM_IOS)

///
#define STRATE_PLATFORM_OS_WEB (0 || STRATE_PLATFORM_WEBGL)

///
#if STRATE_COMPILER_GCC
#define STRATE_COMPILER_NAME                              \
  "GCC " STRATE_STRINGIZE(__GNUC__) "." STRATE_STRINGIZE( \
      __GNUC_MINOR__) "." STRATE_STRINGIZE(__GNUC_PATCHLEVEL__)
#elif STRATE_COMPILER_CLANG
#define STRATE_COMPILER_NAME                                       \
  "Clang " STRATE_STRINGIZE(__clang_major__) "." STRATE_STRINGIZE( \
      __clang_minor__) "." STRATE_STRINGIZE(__clang_patchlevel__)
#elif STRATE_COMPILER_MSVC
#if STRATE_COMPILER_MSVC >= 1930  // Visual Studio 2022
#define STRATE_COMPILER_NAME "MSVC 17.0"
#elif STRATE_COMPILER_MSVC >= 1920  // Visual Studio 2019
#define STRATE_COMPILER_NAME "MSVC 16.0"
#elif STRATE_COMPILER_MSVC >= 1910  // Visual Studio 2017
#define STRATE_COMPILER_NAME "MSVC 15.0"
#elif STRATE_COMPILER_MSVC >= 1900  // Visual Studio 2015
#define STRATE_COMPILER_NAME "MSVC 14.0"
#elif STRATE_COMPILER_MSVC >= 1800  // Visual Studio 2013
#define STRATE_COMPILER_NAME "MSVC 12.0"
#elif STRATE_COMPILER_MSVC >= 1700  // Visual Studio 2012
#define STRATE_COMPILER_NAME "MSVC 11.0"
#elif STRATE_COMPILER_MSVC >= 1600  // Visual Studio 2010
#define STRATE_COMPILER_NAME "MSVC 10.0"
#elif STRATE_COMPILER_MSVC >= 1500  // Visual Studio 2008
#define STRATE_COMPILER_NAME "MSVC 9.0"
#else
#define STRATE_COMPILER_NAME "MSVC"
#endif  //
#endif  // STRATE_COMPILER_

#if STRATE_PLATFORM_ANDROID
#define STRATE_PLATFORM_NAME \
  "Android " STRATE_STRINGIZE(STRATE_PLATFORM_ANDROID)
#elif STRATE_PLATFORM_BSD
#define STRATE_PLATFORM_NAME "BSD"
#elif STRATE_PLATFORM_WEBGL
#define STRATE_PLATFORM_NAME                                                 \
  "Emscripten " STRATE_STRINGIZE(__EMSCRIPTEN_major__) "." STRATE_STRINGIZE( \
      __EMSCRIPTEN_minor__) "." STRATE_STRINGIZE(__EMSCRIPTEN_tiny__)
#elif STRATE_PLATFORM_HAIKU
#define STRATE_PLATFORM_NAME "Haiku"
#elif STRATE_PLATFORM_HURD
#define STRATE_PLATFORM_NAME "Hurd"
#elif STRATE_PLATFORM_IOS
#define STRATE_PLATFORM_NAME "iOS"
#elif STRATE_PLATFORM_LINUX
#define STRATE_PLATFORM_NAME "Linux"
#elif STRATE_PLATFORM_NONE
#define STRATE_PLATFORM_NAME "None"
#elif STRATE_PLATFORM_NX
#define STRATE_PLATFORM_NAME "NX"
#elif STRATE_PLATFORM_OSX
#define STRATE_PLATFORM_NAME "macOS"
#elif STRATE_PLATFORM_PS4
#define STRATE_PLATFORM_NAME "PlayStation 4"
#elif STRATE_PLATFORM_PS5
#define STRATE_PLATFORM_NAME "PlayStation 5"
#elif STRATE_PLATFORM_RPI
#define STRATE_PLATFORM_NAME "RaspberryPi"
#elif STRATE_PLATFORM_VISIONOS
#define STRATE_PLATFORM_NAME "visionOS"
#elif STRATE_PLATFORM_WINDOWS
#define STRATE_PLATFORM_NAME "Windows"
#elif STRATE_PLATFORM_WINRT
#define STRATE_PLATFORM_NAME "WinRT"
#elif STRATE_PLATFORM_XBOXONE
#define STRATE_PLATFORM_NAME "Xbox One"
#else
#error "Unknown STRATE_PLATFORM!"
#endif  // STRATE_PLATFORM_

#if STRATE_CPU_ARM
#define STRATE_CPU_NAME "ARM"
#elif STRATE_CPU_JIT
#define STRATE_CPU_NAME "JIT-VM"
#elif STRATE_CPU_MIPS
#define STRATE_CPU_NAME "MIPS"
#elif STRATE_CPU_PPC
#define STRATE_CPU_NAME "PowerPC"
#elif STRATE_CPU_RISCV
#define STRATE_CPU_NAME "RISC-V"
#elif STRATE_CPU_X86
#define STRATE_CPU_NAME "x86"
#endif  // STRATE_CPU_

#if STRATE_CRT_BIONIC
#define STRATE_CRT_NAME "Bionic libc"
#elif STRATE_CRT_BSD
#define STRATE_CRT_NAME "BSD libc"
#elif STRATE_CRT_GLIBC
#define STRATE_CRT_NAME "GNU C Library"
#elif STRATE_CRT_MSVC
#define STRATE_CRT_NAME "MSVC C Runtime"
#elif STRATE_CRT_MINGW
#define STRATE_CRT_NAME "MinGW C Runtime"
#elif STRATE_CRT_LIBCXX
#define STRATE_CRT_NAME "Clang C Library"
#elif STRATE_CRT_NEWLIB
#define STRATE_CRT_NAME "Newlib"
#elif STRATE_CRT_NONE
#define STRATE_CRT_NAME "None"
#else
#error "Unknown STRATE_CRT!"
#endif  // STRATE_CRT_

#if STRATE_ARCH_32BIT
#define STRATE_ARCH_NAME "32-bit"
#elif STRATE_ARCH_64BIT
#define STRATE_ARCH_NAME "64-bit"
#endif  // STRATE_ARCH_

#if defined(__cplusplus)
#if STRATE_COMPILER_MSVC && defined(_MSVC_LANG) && _MSVC_LANG != __cplusplus
#error "When using MSVC you must set /Zc:__cplusplus compiler option."
#endif  // STRATE_COMPILER_MSVC && defined(_MSVC_LANG) && _MSVC_LANG !=
        // __cplusplus

#if __cplusplus < STRATE_CPP17
#error "C++17 standard support is required to build."
#elif __cplusplus < STRATE_CPP20
#define STRATE_CPP_NAME "C++17"
#elif __cplusplus < STRATE_CPP23
#define STRATE_CPP_NAME "C++20"
#else
// See: https://gist.github.com/bkaradzic/2e39896bc7d8c34e042b#orthodox-c
#define STRATE_CPP_NAME "C++WayTooModern"
#endif  // STRATE_CPP_NAME
#else
#define STRATE_CPP_NAME "C++Unknown"
#endif  // defined(__cplusplus)

#if STRATE_COMPILER_MSVC && (!defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL)
#error "When using MSVC you must set /Zc:preprocessor compiler option."
#endif  // STRATE_COMPILER_MSVC && (!defined(_MSVC_TRADITIONAL) ||
        // _MSVC_TRADITIONAL)

#if STRATE_PLATFORM_OSX && STRATE_PLATFORM_OSX < 130000
// #error "Minimum supported macOS version is 13.00.\n"
#elif STRATE_PLATFORM_IOS && STRATE_PLATFORM_IOS < 160000
// #error "Minimum supported macOS version is 16.00.\n"
#endif  // STRATE_PLATFORM_OSX < 130000

#if STRATE_ENDIAN_BIG
static_assert(false, "\n\n"
                     "\t** IMPORTANT! **\n\n"
                     "\tThe code was not tested for big endian, and big endian "
                     "CPU is considered unsupported.\n"
                     "\t\n");
#endif  // STRATE_ENDIAN_BIG

#if STRATE_PLATFORM_BSD || STRATE_PLATFORM_HAIKU || STRATE_PLATFORM_HURD
static_assert(false, "\n\n"
                     "\t** IMPORTANT! **\n\n"
                     "\tYou're compiling for unsupported platform!\n"
                     "\tIf you wish to support this platform, make your own "
                     "fork, and modify code for _yourself_.\n"
                     "\t\n"
                     "\tDo not submit PR to main repo, it won't be considered, "
                     "and it would code rot anyway. I have no ability\n"
                     "\tto test on these platforms, and over years there "
                     "wasn't any serious contributor who wanted to take\n"
                     "\tburden of maintaining code for these platforms.\n"
                     "\t\n");
#endif  // STRATE_PLATFORM_*

#include <iostream>
#include <type_traits>

#define STRATE_ENUM_CLASS_ENABLE_BITMASK_OPERATORS(T)                    \
  inline T operator|(T lhs, T rhs) {                                     \
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) |  \
                          static_cast<std::underlying_type_t<T>>(rhs));  \
  }                                                                      \
                                                                         \
  inline T operator&(T lhs, T rhs) {                                     \
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) &  \
                          static_cast<std::underlying_type_t<T>>(rhs));  \
  }                                                                      \
                                                                         \
  inline T operator^(T lhs, T rhs) {                                     \
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) ^  \
                          static_cast<std::underlying_type_t<T>>(rhs));  \
  }                                                                      \
                                                                         \
  inline T operator~(T rhs) {                                            \
    return static_cast<T>(~static_cast<std::underlying_type_t<T>>(rhs)); \
  }

#define STRATE_ENUM_CLASS_DISPLAY_TRAIT(E)                               \
  inline std::ostream& operator<<(std::ostream& os, E e) {               \
    return os << static_cast<typename std::underlying_type<E>::type>(e); \
  }

namespace stratengine {
namespace impl {
template <typename Enum, typename Lambda>
std::string enum_to_string_impl(Enum e, Lambda lambda) {
  return lambda(e);
}
}
}

#define STRATE_ENUM_CLASS_TO_STRING_FORMATTER(EnumType, ...)         \
                                                                     \
  inline std::string ToStringEnum##EnumType(EnumType e) {            \
    static const auto toStringFunc = [](EnumType e) -> std::string { \
      switch (e) {                                                   \
        __VA_ARGS__                                                  \
        default:                                                     \
          throw std::invalid_argument("Unsupported enum value");     \
      }                                                              \
    };                                                               \
    return stratengine::impl::enum_to_string_impl(e, toStringFunc);        \
  }