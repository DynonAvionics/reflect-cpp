#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_default_if_missing_no_field_names {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::string town;
};

TEST(json, test_default_if_missing_no_field_names) {
  auto homer =
      rfl::json::read<Person, rfl::DefaultIfMissing, rfl::NoFieldNames>(
          R"(["Homer"])")
          .value();

  EXPECT_EQ(homer.first_name, "Homer");
  EXPECT_EQ(homer.last_name, "Simpson");
  EXPECT_EQ(homer.town, "");
}
}  // namespace test_default_if_missing_no_field_names