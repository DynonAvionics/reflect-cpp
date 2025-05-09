#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_box {

struct DecisionTree {
  struct Leaf {
    using Tag = rfl::Literal<"Leaf">;
    double value;
  };

  struct Node {
    using Tag = rfl::Literal<"Node">;
    rfl::Rename<"criticalValue", double> critical_value;
    rfl::Box<DecisionTree> lesser;
    rfl::Box<DecisionTree> greater;
  };

  using LeafOrNode = rfl::TaggedUnion<"type", Leaf, Node>;

  rfl::Field<"leafOrNode", LeafOrNode> leaf_or_node;
};


TEST(generic, test_box) {
  auto leaf1 = DecisionTree::Leaf{.value = 3.0};

  auto leaf2 = DecisionTree::Leaf{.value = 5.0};

  auto node = DecisionTree::Node{
      .critical_value = 10.0,
      .lesser = rfl::make_box<DecisionTree>(DecisionTree{leaf1}),
      .greater = rfl::make_box<DecisionTree>(DecisionTree{leaf2})};

  const DecisionTree tree{.leaf_or_node = std::move(node)};

  write_and_read(tree);

}
}  // namespace test_box


namespace test_copyable_box
{
struct DecisionTree {
  struct Leaf {
    using Tag = rfl::Literal<"Leaf">;
    double value;
  };

  struct Node {
    using Tag = rfl::Literal<"Node">;
    rfl::Rename<"criticalValue", double> critical_value;
    rfl::CopyableBox<DecisionTree> lesser;
    rfl::CopyableBox<DecisionTree> greater;
  };

  using LeafOrNode = rfl::TaggedUnion<"type", Leaf, Node>;

  rfl::Field<"leafOrNode", LeafOrNode> leaf_or_node;
};
TEST(generic, copyable_box) {
  auto leaf1 = DecisionTree::Leaf{.value = 3.0};

  auto leaf2 = DecisionTree::Leaf{.value = 5.0};

  auto node = DecisionTree::Node{
      .critical_value = 10.0,
      .lesser = rfl::make_box<DecisionTree>(DecisionTree{leaf1}),
      .greater = rfl::make_box<DecisionTree>(DecisionTree{leaf2})};

  const DecisionTree tree1{.leaf_or_node = std::move(node)};

  write_and_read(tree1);

  const DecisionTree tree2 = tree1;

  auto lesser_leaf_tree2 = rfl::get<DecisionTree::Leaf>(rfl::get<DecisionTree::Node>(tree2.leaf_or_node.get().variant()).lesser.get()->leaf_or_node.get().variant());
  lesser_leaf_tree2.value = 1.0;


  auto lesser_leaf_tree1 = rfl::get<DecisionTree::Leaf>(rfl::get<DecisionTree::Node>(tree1.leaf_or_node.get().variant()).lesser.get()->leaf_or_node.get().variant());
  EXPECT_NE(lesser_leaf_tree1.value, lesser_leaf_tree2.value);

}
} // namespace test_copyable_box
