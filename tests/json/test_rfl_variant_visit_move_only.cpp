#include <cassert>
#include <iostream>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_rfl_variant_visit_move_only {

struct Rectangle {
  double height;
  std::unique_ptr<double> width;
};

struct Square {
  std::unique_ptr<double> width;
};

using Shapes = rfl::Variant<Rectangle, Square>;

TEST(json, test_rfl_variant_visit_move_only) {
  const Shapes r =
      Rectangle{.height = 10, .width = std::make_unique<double>(5.0)};
  const auto get_width = [](const auto& _s) -> const std::unique_ptr<double>& {
    return _s.width;
  };
  EXPECT_EQ(*rfl::visit(get_width, r), 5.0);
}
}  // namespace test_rfl_variant_visit_move_only