#ifndef RFL_CBOR_READ_HPP_
#define RFL_CBOR_READ_HPP_

#include <bit>
#include <istream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/cbor/decode_cbor.hpp>
#include <rfl/thirdparty/jsoncons_config.hpp>
#include <string>

#include "../Processors.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::cbor {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

#if RFL_JSONCONS_PROVIDES_TRY_DECODE
// Newer versions of jsoncons provide std::expected-like for error handling instead of exceptions

/// Parses an object from CBOR data blob, handling jsoncons std::expected-like in newer versions
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(const std::vector<char>& _bytes) {
  auto result = jsoncons::cbor::try_decode_cbor<jsoncons::json>(_bytes);
  if (result.has_value()) {
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&result.value()});
  } else {
    // std::string error("Could not parse CBOR: ");
    // FIXME: Remove temporary test output discriminator
    std::string error("Could not parse CBOR (result had error): ");
    error.append(jsoncons::to_string(result.error()));
    return rfl::error(error);
  }
}

/// Parses an object from a stream, handling jsoncons std::expected-like in newer versions
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(std::istream& _stream) {
  auto result = jsoncons::cbor::try_decode_cbor<jsoncons::json>(_stream);
  if (result.has_value()) {
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&result.value()});
  } else {
    // std::string error("Could not parse CBOR: ");
    // FIXME: Remove temporary test output discriminator
    std::string error("Could not parse CBOR (result had error): ");
    error.append(jsoncons::to_string(result.error()));
    return rfl::error(error);
  }
}

#else // RFL_JSONCONS_PROVIDES_TRY_DECODE
// Older versions of jsoncons throw exceptions on errors, so we need to catch them

/// Parses an object from CBOR data blob, catching jsoncons exceptions in older versions
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(const std::vector<char>& _bytes) {
  try {
    auto val = jsoncons::cbor::decode_cbor<jsoncons::json>(_bytes);
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&val});
  } catch(const jsoncons::ser_error& e)  {
    // std::string error("Could not parse CBOR: ");
    // FIXME: Remove temporary test output discriminator
    std::string error("Could not parse CBOR (trapped exception): ");
    error.append(e.what());
    return rfl::error(error);
  }
}

/// Parses an object from a stream, catching jsoncons exceptions in older versions
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(std::istream& _stream) {
  try {
    auto val = jsoncons::cbor::decode_cbor<jsoncons::json>(_stream);
    auto r = Reader();
    return Parser<T, Processors<Ps...>>::read(r, InputVarType{&val});
  } catch(const jsoncons::ser_error& e)  {
    // std::string error("Could not parse CBOR: ");
    // FIXME: Remove temporary test output discriminator
    std::string error("Could not parse CBOR (trapped exception): ");
    error.append(e.what());
    return rfl::error(error);
  }
}

#endif // RFL_JSONCONS_PROVIDES_TRY_DECODE

}  // namespace rfl::cbor

#endif
