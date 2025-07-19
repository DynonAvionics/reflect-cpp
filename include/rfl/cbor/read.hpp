#ifndef RFL_CBOR_READ_HPP_
#define RFL_CBOR_READ_HPP_

#include <bit>
#include <istream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/cbor/decode_cbor.hpp>
#include <string>

#include "../Processors.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"
#include "../concepts.hpp"

namespace rfl::cbor {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

#ifndef JSONCONS_VERSION
// Use multiplier of thousands to separate version components, to handle 0.177.0 for example
#define JSONCONS_VERSION (JSONCONS_VERSION_MAJOR * 1000000LL + \
                          JSONCONS_VERSION_MINOR * 1000LL + \
                          JSONCONS_VERSION_PATCH)

#endif



#if JSONCONS_VERSION < 1003003LL
// Older versions of jsoncons only throw exceptions on errors, so we need to catch them
// TODO: Remove this when we drop support for jsoncons < 1.3.3

/// Parses an object from CBOR data blob, catching jsoncons exceptions in older versions
template <class T, ContiguousByteContainer Bytes, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(const Bytes& _bytes) {
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

#else
// Newer versions of jsoncons use std::expected-like for error handling, so we can use that directly

/// Parses an object from CBOR data blob, handling jsoncons std::expected-like in newer versions
template <class T, ContiguousByteContainer Bytes, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(const Bytes& _bytes) {
  using namespace jsoncons::cbor;
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

/// Parses an object from a stream handling jsoncons std::expected-like in newer versions
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

#endif

}  // namespace rfl::cbor

#endif
