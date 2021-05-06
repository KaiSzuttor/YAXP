#include <doctest/doctest.h>
#include <fstream>
#include <iostream> // toolchain issues on osx: https://github.com/onqtam/doctest/issues/356

#include "eval.hpp"
#include "operation.hpp"
#include "parser.hpp"

TEST_CASE("operations") {
  std::ifstream stream("../../data/operations.xml", std::ios::in);
  REQUIRE(stream.is_open());
  XML::Lexer lexer(stream);
  auto parser = XML::Parser(lexer.tokenize());
  auto const doc = parser.parse();
  REQUIRE(doc.size() == 5);
  {
    Operation::Operation op(*doc[0]);
    REQUIRE(op.m_name == "important");
    REQUIRE(op.m_type == "attrib");
    REQUIRE(op.m_func == "average");
    REQUIRE(op.m_attrib == "population");
    REQUIRE(op.m_filter == "M.*");
  }
  {
    Operation::Operation op(*doc[1]);
    REQUIRE(op.m_name == "information");
    REQUIRE(op.m_type == "sub");
    REQUIRE(op.m_func == "sum");
    REQUIRE(op.m_attrib == "area");
    REQUIRE(op.m_filter == ".*burg");
  }
  {
    Operation::Operation op(*doc[2]);
    REQUIRE(op.m_name == "for");
    REQUIRE(op.m_type == "sub");
    REQUIRE(op.m_func == "min");
    REQUIRE(op.m_attrib == "area");
    REQUIRE(op.m_filter == ".*n.+");
  }
  {
    Operation::Operation op(*doc[3]);
    REQUIRE(op.m_name == "future");
    REQUIRE(op.m_type == "attrib");
    REQUIRE(op.m_func == "max");
    REQUIRE(op.m_attrib == "population");
    REQUIRE(op.m_filter == ".*e.*n.*");
  }
}