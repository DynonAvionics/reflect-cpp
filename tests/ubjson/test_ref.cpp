#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_ref {

struct DecisionTree {
  struct Leaf {
    using Tag = rfl::Literal<"Leaf">;
    double value;
  };

  struct Node {
    using Tag = rfl::Literal<"Node">;
    rfl::Rename<"criticalValue", double> critical_value;
    rfl::Ref<DecisionTree> lesser;
    rfl::Ref<DecisionTree> greater;
  };

  using LeafOrNode = rfl::TaggedUnion<"type", Leaf, Node>;

  rfl::Field<"leafOrNode", LeafOrNode> leaf_or_node;
};

TEST(ubjson, test_ref) { 
  const auto leaf1 = DecisionTree::Leaf{.value = 3.0};

  const auto leaf2 = DecisionTree::Leaf{.value = 5.0};

  auto node = DecisionTree::Node{
      .critical_value = 10.0,
      .lesser = rfl::make_ref<DecisionTree>(DecisionTree{leaf1}),
      .greater = rfl::make_ref<DecisionTree>(DecisionTree{leaf2})};

  const DecisionTree tree{.leaf_or_node = std::move(node)};

  write_and_read(tree);

}
}  // namespace test_ref
