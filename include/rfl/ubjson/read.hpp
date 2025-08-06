#ifndef RFL_UBJSON_READ_HPP_
#define RFL_UBJSON_READ_HPP_

#include <bit>
#include <istream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/ubjson/decode_ubjson.hpp>
#include <rfl/thirdparty/jsoncons_config.hpp>
#include <string>

#include "../Processors.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::ubjson {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

#if RFL_JSONCONS_PROVIDES_TRY_DECODE
// Newer versions of jsoncons provide std::expected-like for error handling instead of exceptions

/// Parses an object from UBJSON data blob, handling jsoncons std::expected-like in newer versions
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(const std::vector<char>& _bytes) {
  auto result = jsoncons::ubjson::try_decode_ubjson<jsoncons::json>(_bytes);
  if (result.has_value()) {
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&result.value()});
  } else {
    // std::string error("Could not parse UBJSON: ");
    // FIXME: Remove temporary test output discriminator
    std::string error("Could not parse UBJSON (result had error): ");
    error.append(jsoncons::to_string(result.error()));
    return rfl::error(error);
  }
}

/// Parses an object from a stream, handling jsoncons std::expected-like in newer versions
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(std::istream& _stream) {
  auto result = jsoncons::ubjson::try_decode_ubjson<jsoncons::json>(_stream);
  if (result.has_value()) {
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&result.value()});
  } else {
    // std::string error("Could not parse UBJSON: ");
    // FIXME: Remove temporary test output discriminator
    std::string error("Could not parse UBJSON (result had error): ");
    error.append(jsoncons::to_string(result.error()));
    return rfl::error(error);
  }
}

#else // RFL_JSONCONS_PROVIDES_TRY_DECODE
// Older versions of jsoncons throw exceptions on errors, so we need to catch them

/// Parses an object from UBJSON data blob, catching jsoncons exceptions in older versions
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(const std::vector<char>& _bytes) {
  try {
    auto val = jsoncons::ubjson::decode_ubjson<jsoncons::json>(_bytes);
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&val});
  } catch(const jsoncons::ser_error& e)  {
    // std::string error("Could not parse UBJSON: ");
    // FIXME: Remove temporary test output discriminator
    std::string error("Could not parse UBJSON (trapped exception): ");
    error.append(e.what());
    return rfl::error(error);
  }
}

/// Parses an object from a stream, catching jsoncons exceptions in older versions
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(std::istream& _stream) {
  try {
    auto val = jsoncons::ubjson::decode_ubjson<jsoncons::json>(_stream);
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&val});
  } catch(const jsoncons::ser_error& e)  {
    // std::string error("Could not parse UBJSON: ");
    // FIXME: Remove temporary test output discriminator
    std::string error("Could not parse UBJSON (trapped exception): ");
    error.append(e.what());
    return rfl::error(error);
  }
}

#endif // RFL_JSONCONS_PROVIDES_TRY_DECODE

}  // namespace rfl::ubjson

#endif
