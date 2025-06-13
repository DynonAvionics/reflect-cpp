#include <gtest/gtest.h>

#include <rfl/cbor.hpp>

namespace test_optional_fields {

struct optional_test_t {
    int i;
    std::optional<int> opt_i;
};

TEST(cbor, test_optional_read) {

    const optional_test_t expected = { .i = 42 };
    std::vector<uint8_t> cbor_bytes = {
        0xA1,      // map(1)
        0x61,      // text(1)
            0x69,  // "i"
        0x18, 0x2A // unsigned(42)
    };

    auto from_cbor_res = rfl::cbor::read<optional_test_t>(std::vector<char>(cbor_bytes.begin(), cbor_bytes.end()));
    try
    {
        optional_test_t optional_from_cbor = from_cbor_res.value();
        EXPECT_EQ(expected.i, optional_from_cbor.i);
    }
    catch (const std::runtime_error& e)
    {
        EXPECT_EQ("Field named 'opt_i' not found.", std::string(e.what()));
    }
}

TEST(cbor, test_optional_write)
{
    // Writing
    const optional_test_t test_write = { .i = 42 };
    const std::vector<uint8_t> expected_cbor = {
        0xA1,      // map(1)
        0x61,      // text(1)
            0x69,  // "i"
        0x18, 0x2A // unsigned(42)
    };

    const std::vector<char> cbor = rfl::cbor::write(test_write);
    EXPECT_EQ(expected_cbor, std::vector<uint8_t>(cbor.begin(), cbor.end())); // we get "{"i": 42, "opt_i": null}" but expect "{"i": 42}"
}

}  // namespace test_optional_fields
