#include <algorithm>
#include <array>
#include <iostream>
#include <span>
#include <rfl/cbor.hpp>

#include <gtest/gtest.h>

// Make sure things still compile when
// rfl.hpp is included after rfl/cbor.hpp.
#include <rfl.hpp>

namespace test_read_byte_containers
{

// TODO: Apparently the jsoncons trait is_byte_sequence<T> is not working for std::span.
// TEST(cbor, test_read_from_char_span)
// {
//     struct S
//     {
//         int one;
//         int two;
//     };

//     S s = {
//         .one = 1,
//         .two = 2,
//     };

//     std::vector<char> rfl_buffer = rfl::cbor::write(s);

//     // I don't want to be forced to copy to a std::vector<char> if that's not what data strucure I use.
//     // So, allow reading from a std::span
//     std::span<char> span(rfl_buffer.data(), rfl_buffer.size());

//     auto result = rfl::cbor::read<S>(span);
//     EXPECT_TRUE(result);
//     EXPECT_EQ(result->one, 1);
//     EXPECT_EQ(result->two, 2);
// }

TEST(cbor, test_read_from_byte_view)
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
    // TODO: Write directly into desired container, once rfl::cbor::write supports it.
    std::array<std::byte, 64> my_buffer;
    std::transform(rfl_buffer.begin(), rfl_buffer.end(), my_buffer.begin(),
        [](char c) { return static_cast<std::byte>(c); });

    std::basic_string_view<std::byte> byte_view(my_buffer.data(), rfl_buffer.size());

    auto result = rfl::cbor::read<S>(byte_view);
    EXPECT_TRUE(result);
    EXPECT_EQ(result->one, 1);
    EXPECT_EQ(result->two, 2);
}

TEST(cbor, test_read_from_uint8_array)
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
    // TODO: Write directly into desired container, once rfl::cbor::write supports it.
    std::array<std::uint8_t, 64> my_buffer;
    std::transform(rfl_buffer.begin(), rfl_buffer.end(), my_buffer.begin(),
        [](char c) { return static_cast<std::uint8_t>(c); });

    auto result = rfl::cbor::read<S>(my_buffer);
    EXPECT_TRUE(result);
    EXPECT_EQ(result->one, 1);
    EXPECT_EQ(result->two, 2);
}

}