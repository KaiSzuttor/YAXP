#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "lexer.hpp"
#include "parser.hpp"

namespace XML {

/** I/O capability. */
std::ostream &operator<<(std::ostream &os, StartTagBegin const &m) {
  return os << "StartTagBegin name: " << m.name;
}
/** I/O capability. */
std::ostream &operator<<(std::ostream &os, StartTagEnd const &m) {
  return os << "StartTagEnd";
}
/** I/O capability. */
std::ostream &operator<<(std::ostream &os, Attribute const &m) {
  return os << "Attribute: key: " << m.key_val.first
            << " value: " << m.key_val.second;
}
/** I/O capability. */
std::ostream &operator<<(std::ostream &os, Content const &m) {
  return os << "Content " << m.content;
}

/** I/O capability. */
std::ostream &operator<<(std::ostream &os, EndTag const &m) {
  return os << "EndTag name: " << m.name;
}
/** I/O capability. */
std::ostream &operator<<(std::ostream &os, CloseTag const &m) {
  return os << "CloseTag";
}

/** I/O capability. */
std::ostream &operator<<(std::ostream &os, XML_Attribute const &m) {
  return os << " " << m.key_val.first << "=\"" << m.key_val.second << "\"";
}

/** I/O capability. */
std::ostream &operator<<(std::ostream &os, XML_Element const &m) {
  os << std::string(2 * m.nesting_level, ' ');
  os << "<" << m.name;
  std::ostream_iterator<XML_Attribute> out_it_attr(os, "");
  std::copy(m.attributes.begin(), m.attributes.end(), out_it_attr);
  os << ">\n";
  if (not m.content.empty()) {
    os << std::string(4 * m.nesting_level, ' ');
    os << m.content << "\n";
  }
  std::ostream_iterator<std::shared_ptr<XML_Element>> out_it_elem(os, "\n");
  std::copy(m.children.begin(), m.children.end(), out_it_elem);
  os << std::string(2 * m.nesting_level, ' ');
  os << "</" << m.name << ">";
  return os;
}

/** I/O capability. */
std::ostream &operator<<(std::ostream &os,
                         std::shared_ptr<XML_Element> const &m) {
  return os << *m;
}

std::ostream &operator<<(std::ostream &os, XML_Doc const &m) {
  return os << *(m.back());
}

} // namespace XML

#endif