#include <doctest/doctest.h>
#include <fstream>
#include <iostream> // toolchain issues on osx: https://github.com/onqtam/doctest/issues/356

#include "parser.hpp"

TEST_CASE("data") {
  std::ifstream stream("../../data/data_small.xml", std::ios::in);
  REQUIRE(stream.is_open());
  XML::Lexer lexer(stream);
  XML::Parser parser(lexer.tokenize());
  auto const doc = parser.parse();
  REQUIRE(doc.size() == 7);
  REQUIRE(doc[0]->name == "area");
  REQUIRE(doc[0]->parent->name == "city");
  REQUIRE(doc[1]->name == "city");
  REQUIRE(doc[1]->parent->name == "data");
  REQUIRE(doc[2]->name == "area");
  REQUIRE(doc[2]->parent->name == "city");
  REQUIRE(doc[3]->name == "city");
  REQUIRE(doc[3]->parent->name == "data");
  REQUIRE(doc[6]->name == "data");

  auto const filtered2 = XML::attr_filter(doc, "name", "M.*");
  std::cout << filtered2.size() << std::endl;

  auto const filtered = XML::name_filter(doc, "cit.*");
  REQUIRE(filtered.size() == 3);
  REQUIRE(filtered[0]->name == "city");
  REQUIRE(filtered[1]->name == "city");
}

TEST_CASE("operations") {
  std::ifstream stream("../../data/operations.xml", std::ios::in);
  REQUIRE(stream.is_open());
  XML::Lexer lexer(stream);
  XML::Parser parser(lexer.tokenize());
  auto const doc = parser.parse();
  REQUIRE(doc.size() == 5);
}