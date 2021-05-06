#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <string>

#include "parser.hpp"

namespace Operation {

struct Operation {
  std::string m_name;
  std::string m_type;
  std::string m_func;
  std::string m_attrib;
  std::string m_filter;
  Operation(XML::XML_Element const &op) {
    m_name = op.get_attribute("name");
    m_type = op.get_attribute("type");
    m_func = op.get_attribute("func");
    m_attrib = op.get_attribute("attrib");
    m_filter = op.get_attribute("filter");
  }
};

} // namespace Operation

#endif