#include <rfl/cbor.hpp>

// Make sure things still compile when
// rfl.hpp is included after rfl/cbor.hpp.
#include <rfl.hpp>

#include <gtest/gtest.h>

namespace test_floating_point {

struct Float {
    float f32;
    double f64;
};

TEST(cbor, test_floating_point) {

  auto res = rfl::cbor::write( Float{.f32 = 123456.789f, .f64 = 1234567.89123456} );

  // TODO: Is there a way to programmatically check for type instead of knowing the exact byte?
  EXPECT_TRUE((unsigned char)res[5] == CborFloatType);
  EXPECT_TRUE((unsigned char)res[14] == CborDoubleType);
}

}  // namespace test_floating_point
