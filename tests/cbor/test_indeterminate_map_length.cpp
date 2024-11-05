#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"


#include <rfl/json.hpp>

namespace test_indeterminate_map_length {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::map<std::string, Person> children;
};


TEST(cbor, test_indeterminate_map_length) {
  auto children = std::map<std::string, Person>();
  children.insert(std::make_pair("child1", Person{.first_name = "Bart"}));
  // children.insert(std::make_pair("child2", Person{.first_name = "Lisa"}));
  // children.insert(std::make_pair("child3", Person{.first_name = "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};


  // write_and_read(homer);

  const auto serialized1 = rfl::cbor::write(homer);
  const auto serialized2 = rfl::json::write(homer);

  std::cout<<serialized2<<std::endl;

  // const auto res = rfl::cbor::read<T, Ps...>(serialized1);
  // EXPECT_TRUE(res && true) << "Test failed on read. Error: "
  //                          << res.error().value().what();
}
}  // namespace test_indeterminate_map_length
