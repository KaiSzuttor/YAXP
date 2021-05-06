#include <doctest/doctest.h>
#include <fstream>
#include <iostream> // toolchain issues on osx: https://github.com/onqtam/doctest/issues/356

#include "lexer.hpp"

/* clang-format off */
/* we expect the following tokens with their respective values
StartTagBegin name: data
StartTagEnd
StartTagBegin name: city
Attribute: key: name value: Stuttgart
Attribute: key: population value: 601646
StartTagEnd
StartTagBegin name: area
StartTagEnd
Content 207.36
EndTag name: area
EndTag name: city
StartTagBegin name: city
Attribute: key: name value: Moskau
Attribute: key: population value: 10563038
StartTagEnd
StartTagBegin name: area
StartTagEnd
Content 1081.5
EndTag name: area
EndTag name: city
EndTag name: data
*/
/* clang-format on */

TEST_CASE("data") {
  std::ifstream istrm("../../data/data_small.xml", std::ios::in);
  REQUIRE(istrm.is_open());
  XML::Lexer lexer(istrm);
  auto const tokens = lexer.tokenize();
  REQUIRE(std::get<XML::StartTagBegin>(tokens[0]).name == "data");
  REQUIRE(std::holds_alternative<XML::StartTagEnd>(tokens[1]));
  REQUIRE(std::get<XML::StartTagBegin>(tokens[2]).name == "city");
  auto attr = std::get<XML::Attribute>(tokens[3]);
  REQUIRE(attr.key_val.first == "name");
  REQUIRE(attr.key_val.second == "Stuttgart");
  attr = std::get<XML::Attribute>(tokens[4]);
  REQUIRE(attr.key_val.first == "population");
  REQUIRE(attr.key_val.second == "601646");
  REQUIRE(std::holds_alternative<XML::StartTagEnd>(tokens[5]));
  REQUIRE(std::get<XML::StartTagBegin>(tokens[6]).name == "area");
  REQUIRE(std::holds_alternative<XML::StartTagEnd>(tokens[7]));
  auto cont = std::get<XML::Content>(tokens[8]);
  REQUIRE(cont.content == "207.36");
  REQUIRE(std::get<XML::EndTag>(tokens[9]).name == "area");
  REQUIRE(std::get<XML::EndTag>(tokens[10]).name == "city");
  REQUIRE(std::get<XML::StartTagBegin>(tokens[11]).name == "city");
  attr = std::get<XML::Attribute>(tokens[12]);
  REQUIRE(attr.key_val.first == "name");
  REQUIRE(attr.key_val.second == "Moskau");
  attr = std::get<XML::Attribute>(tokens[13]);
  REQUIRE(attr.key_val.first == "population");
  REQUIRE(attr.key_val.second == "10563038");
  REQUIRE(std::holds_alternative<XML::StartTagEnd>(tokens[14]));
  REQUIRE(std::get<XML::StartTagBegin>(tokens[15]).name == "area");
  REQUIRE(std::holds_alternative<XML::StartTagEnd>(tokens[16]));
  cont = std::get<XML::Content>(tokens[17]);
  REQUIRE(cont.content == "1081.5");
  REQUIRE(std::get<XML::EndTag>(tokens[18]).name == "area");
  REQUIRE(std::get<XML::EndTag>(tokens[19]).name == "city");
  REQUIRE(std::get<XML::StartTagBegin>(tokens[20]).name == "city");
  attr = std::get<XML::Attribute>(tokens[21]);
  REQUIRE(attr.key_val.first == "name");
  REQUIRE(attr.key_val.second == "München");
  attr = std::get<XML::Attribute>(tokens[22]);
  REQUIRE(attr.key_val.first == "population");
  REQUIRE(attr.key_val.second == "1330440");
  REQUIRE(std::holds_alternative<XML::StartTagEnd>(tokens[23]));
  REQUIRE(std::get<XML::StartTagBegin>(tokens[24]).name == "area");
  REQUIRE(std::holds_alternative<XML::StartTagEnd>(tokens[25]));
  cont = std::get<XML::Content>(tokens[26]);
  REQUIRE(cont.content == "310.43");
  REQUIRE(std::get<XML::EndTag>(tokens[27]).name == "area");
  REQUIRE(std::get<XML::EndTag>(tokens[28]).name == "city");
}

/* clang-format off */
/* we expect the following tokens with their respective values
StartTagBegin name: operations
StartTagEnd
StartTagBegin name: operation
Attribute: key: name value: important
Attribute: key: type value: attrib
Attribute: key: func value: average
Attribute: key: attrib value: population
Attribute: key: filter value: M.*
StartTagEnd
StartTagBegin name: operation
Attribute: key: name value: information
Attribute: key: type value: sub
Attribute: key: func value: sum
Attribute: key: attrib value: area
Attribute: key: filter value: .*burg
StartTagEnd
StartTagBegin name: operation
Attribute: key: name value: for
Attribute: key: type value: sub
Attribute: key: func value: min
Attribute: key: attrib value: area
Attribute: key: filter value: .*n.+
StartTagEnd
StartTagBegin name: operation
Attribute: key: name value: future
Attribute: key: type value: attrib
Attribute: key: func value: max
Attribute: key: attrib value: population
Attribute: key: filter value: .*e.*n.*
StartTagEnd
EndTag name: operations
*/
/* clang-format on */
TEST_CASE("operations") {
  std::ifstream istrm("../../data/operations.xml", std::ios::in);
  REQUIRE(istrm.is_open());
  XML::Lexer lexer(istrm);
  auto const tokens = lexer.tokenize();
  REQUIRE(std::get<XML::StartTagBegin>(tokens[0]).name == "operations");
  REQUIRE(std::holds_alternative<XML::StartTagEnd>(tokens[1]));
  REQUIRE(std::get<XML::StartTagBegin>(tokens[2]).name == "operation");
  auto attr = std::get<XML::Attribute>(tokens[3]);
  REQUIRE(attr.key_val.first == "name");
  REQUIRE(attr.key_val.second == "important");
  attr = std::get<XML::Attribute>(tokens[4]);
  REQUIRE(attr.key_val.first == "type");
  REQUIRE(attr.key_val.second == "attrib");
  attr = std::get<XML::Attribute>(tokens[5]);
  REQUIRE(attr.key_val.first == "func");
  REQUIRE(attr.key_val.second == "average");
  attr = std::get<XML::Attribute>(tokens[6]);
  REQUIRE(attr.key_val.first == "attrib");
  REQUIRE(attr.key_val.second == "population");
  attr = std::get<XML::Attribute>(tokens[7]);
  REQUIRE(attr.key_val.first == "filter");
  REQUIRE(attr.key_val.second == "M.*");
  REQUIRE(std::holds_alternative<XML::CloseTag>(tokens[8]));
  REQUIRE(std::get<XML::StartTagBegin>(tokens[9]).name == "operation");
  attr = std::get<XML::Attribute>(tokens[10]);
  REQUIRE(attr.key_val.first == "name");
  REQUIRE(attr.key_val.second == "information");
  attr = std::get<XML::Attribute>(tokens[11]);
  REQUIRE(attr.key_val.first == "type");
  REQUIRE(attr.key_val.second == "sub");
  attr = std::get<XML::Attribute>(tokens[12]);
  REQUIRE(attr.key_val.first == "func");
  REQUIRE(attr.key_val.second == "sum");
  attr = std::get<XML::Attribute>(tokens[13]);
  REQUIRE(attr.key_val.first == "attrib");
  REQUIRE(attr.key_val.second == "area");
  attr = std::get<XML::Attribute>(tokens[14]);
  REQUIRE(attr.key_val.first == "filter");
  REQUIRE(attr.key_val.second == ".*burg");
  REQUIRE(std::holds_alternative<XML::CloseTag>(tokens[15]));
  REQUIRE(std::get<XML::StartTagBegin>(tokens[16]).name == "operation");
  attr = std::get<XML::Attribute>(tokens[17]);
  REQUIRE(attr.key_val.first == "name");
  REQUIRE(attr.key_val.second == "for");
  attr = std::get<XML::Attribute>(tokens[18]);
  REQUIRE(attr.key_val.first == "type");
  REQUIRE(attr.key_val.second == "sub");
  attr = std::get<XML::Attribute>(tokens[19]);
  REQUIRE(attr.key_val.first == "func");
  REQUIRE(attr.key_val.second == "min");
  attr = std::get<XML::Attribute>(tokens[20]);
  REQUIRE(attr.key_val.first == "attrib");
  REQUIRE(attr.key_val.second == "area");
  attr = std::get<XML::Attribute>(tokens[21]);
  REQUIRE(attr.key_val.first == "filter");
  REQUIRE(attr.key_val.second == ".*n.+");
  REQUIRE(std::holds_alternative<XML::CloseTag>(tokens[22]));
  REQUIRE(std::get<XML::StartTagBegin>(tokens[23]).name == "operation");
  attr = std::get<XML::Attribute>(tokens[24]);
  REQUIRE(attr.key_val.first == "name");
  REQUIRE(attr.key_val.second == "future");
  attr = std::get<XML::Attribute>(tokens[25]);
  REQUIRE(attr.key_val.first == "type");
  REQUIRE(attr.key_val.second == "attrib");
  attr = std::get<XML::Attribute>(tokens[26]);
  REQUIRE(attr.key_val.first == "func");
  REQUIRE(attr.key_val.second == "max");
  attr = std::get<XML::Attribute>(tokens[27]);
  REQUIRE(attr.key_val.first == "attrib");
  REQUIRE(attr.key_val.second == "population");
  attr = std::get<XML::Attribute>(tokens[28]);
  REQUIRE(attr.key_val.first == "filter");
  REQUIRE(attr.key_val.second == ".*e.*n.*");
  REQUIRE(std::holds_alternative<XML::CloseTag>(tokens[29]));
  REQUIRE(std::get<XML::EndTag>(tokens[30]).name == "operations");
}