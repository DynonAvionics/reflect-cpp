#include <array>
#include <iostream>
#include <rfl/cbor.hpp>
#include <span>

// Make sure things still compile when
// rfl.hpp is included after rfl/cbor.hpp.
#include <rfl.hpp>

namespace test_from_span
{

TEST(cbor, test_read_from_span)
{
    struct S
    {
        int one;
        int two;
    };

    S s = {
        .one = 1,
        .two = 2,
    };

    std::vector<char> rfl_buffer = rfl::cbor::write(s);

    std::array<char, 64> my_buffer;
    std::copy(rfl_buffer.begin(), rfl_buffer.end(), my_buffer.begin());

    // I don't want to be forced to copy to a std::vector<char> if that's not what data strucure I use.
    // So, allow reading from a std::span
    std::span<char> span(my_buffer.data(), rfl_buffer.size());

    auto result = rfl::cbor::read<S>(span);
    EXPECT_TRUE(result);
    EXPECT_EQ(result->one, 1);
    EXPECT_EQ(result->two, 2);
}

}