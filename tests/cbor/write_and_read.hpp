#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <iostream>
#include <rfl/cbor.hpp>
#include <string>

template <class... Ps>
void write_and_read(const auto& _struct) {
  using T = std::remove_cvref_t<decltype(_struct)>;
  const auto serialized1 = rfl::cbor::write<Ps...>(_struct);
  const auto res = rfl::cbor::read<T, Ps...>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::cbor::write<Ps...>(res.value());
  EXPECT_EQ(serialized1, serialized2);
}
#endif
