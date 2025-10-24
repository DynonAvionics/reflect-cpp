#ifndef RFL_THIRDPARTY_JSONCONS_CONFIG_HPP_
#define RFL_THIRDPARTY_JSONCONS_CONFIG_HPP_

#include <jsoncons/config/version.hpp>


#ifndef JSONCONS_VERSION
// Use multiplier of thousands to separate version components, to handle 0.177.0 for example
#define JSONCONS_VERSION (JSONCONS_VERSION_MAJOR * 1000000LL + \
                          JSONCONS_VERSION_MINOR * 1000LL + \
                          JSONCONS_VERSION_PATCH)

#endif

#if JSONCONS_VERSION >= 1003003LL
// Newer versions of jsoncons provide for std::expected-like interfaces for error handling via
// `try_decode_*()` functions, so we can use those results directly instead of catching exceptions
#   define RFL_JSONCONS_PROVIDES_TRY_DECODE 1
#else
// Older versions of jsoncons only throw exceptions on errors, so we need to catch them
#   define RFL_JSONCONS_PROVIDES_TRY_DECODE 0
#endif // JSONCONS_VERSION >= 1003003LL

#endif // RFL_THIRDPARTY_JSONCONS_CONFIG_HPP_
