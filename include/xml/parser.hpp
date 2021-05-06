#ifndef PARSER_HPP
#define PARSER_HPP

#include <cassert>
#include <memory>
#include <regex>
#include <stack>
#include <string>
#include <vector>

#include "lexer.hpp"

/** @file parser.hpp
 *  @brief This file contains the parser class and XML representations.
 */

namespace XML {

/**
 * @brief Representation of a XML attribute.
 */
struct XML_Attribute {
  std::pair<std::string, std::string> key_val;
};

/**
 * @brief Representation of a XML element.
 *
 * The data members contain all information that is needed to represent
 * a node in the hierarchical structure of a XML document.
 */
struct XML_Element {
  std::string name;
  std::shared_ptr<XML_Element> parent;
  std::size_t nesting_level;
  std::vector<std::shared_ptr<XML_Element>> children;
  std::vector<XML_Attribute> attributes;
  std::string content;
  XML_Element(std::string n, std::shared_ptr<XML_Element> p, std::size_t l)
      : name(std::move(n)), parent(std::move(p)), nesting_level(l) {}

  std::string get_attribute(std::string attr_name) const {
    auto const res = std::find_if(attributes.begin(), attributes.end(),
                                  [attr_name](auto const &attr) {
                                    return attr.key_val.first == attr_name;
                                  });
    if (res != attributes.end())
      return res->key_val.second;
    return {};
  }

  XML_Element get_child(std::string child_name) {
    auto const res = std::find_if(
        children.begin(), children.end(),
        [child_name](auto child) { return (*child).name == child_name; });
    if (res != children.end())
      return **res;
    throw std::runtime_error("Child not found");
  }
};

struct XML_Doc {
  using value_type = std::shared_ptr<XML_Element>;
  using container_type = std::vector<value_type>;
  using iterator = container_type::iterator;
  using const_iterator = container_type::const_iterator;
  using size_type = container_type::size_type;

  container_type m_data;

  value_type back() { return m_data.back(); }

  const value_type back() const { return m_data.back(); }

  iterator begin() { return m_data.begin(); }

  const_iterator begin() const { return m_data.begin(); }

  iterator end() { return m_data.end(); }

  const_iterator end() const { return m_data.end(); }

  size_type size() const { return m_data.size(); }

  std::shared_ptr<XML_Element> operator[](std::size_t ind) const {
    assert(ind < m_data.size());
    return m_data[ind];
  }

  void push_back(std::shared_ptr<XML_Element> element) {
    m_data.push_back(element);
  }

  void add_element(std::shared_ptr<XML_Element> element) { push_back(element); }
};

/**
 * @brief Find elements with matching names.
 *
 * @param doc The parsed XML document.
 * @param name_regex A regex string to match against the XML element names in
 * the given doc.
 * @return A XML document containing matched elements.
 */
XML_Doc name_filter(XML_Doc const &doc, std::string name_regex) {
  std::regex regex(name_regex);
  XML_Doc result;
  std::copy_if(
      doc.begin(), doc.end(), std::back_inserter(result),
      [regex](auto elem) { return std::regex_match(elem->name, regex); });
  return result;
}

/**
 * @brief Find elements with matching attributes names.
 *
 * @param doc The parsed XML document.
 * @param attr The name of the attribute to filter for.
 * @param val_regex A regex string to match against the XML element names in
 * the given doc.
 * @return A XML document containing matched elements.
 */
XML_Doc attr_filter(XML_Doc const &doc, std::string attr,
                    std::string val_regex) {
  std::regex regex(val_regex);
  XML_Doc result;
  std::copy_if(doc.begin(), doc.end(), std::back_inserter(result),
               [regex, attr](auto elem) {
                 return std::regex_match(elem->get_attribute(attr), regex);
               });
  return result;
}

/**
 * @brief Class to transform tokens to a XML document representation.
 *
 * The XML document is represented by a vector of XML_Element instances.
 */
struct Parser {
  std::vector<XML::Token> m_tokens;

  /**
   * @brief Constructor of the parser class.
   *
   * @param tokens A vector of XML tokens.
   */
  Parser(std::vector<XML::Token> tokens) : m_tokens(std::move(tokens)) {}

  /**
   * @brief Map the vector of XML tokens to a vector of XML elements.
   *
   * @return The vector of XML elements.
   */
  XML_Doc parse() {
    std::stack<std::shared_ptr<XML_Element>> parents;
    XML_Doc doc;
    for (auto &t : m_tokens) {
      std::visit(
          [&parents, &doc](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, XML::StartTagBegin>) {
              // update the current parent
              if (parents.empty()) {
                parents.push(
                    std::make_shared<XML_Element>(arg.name, nullptr, 0));
              } else {
                auto new_elem = std::make_shared<XML_Element>(
                    arg.name, parents.top(), parents.size());
                parents.top()->children.push_back(new_elem);
                parents.push(new_elem);
              }
            } else if constexpr (std::is_same_v<T, XML::Attribute>) {
              parents.top()->attributes.push_back(XML_Attribute{arg.key_val});
            } else if constexpr (std::is_same_v<T, XML::Content>) {
              parents.top()->content = arg.content;
            } else if constexpr (std::is_same_v<T, XML::CloseTag> or
                                 std::is_same_v<T, XML::EndTag>) {
              // we are closing the current parent and store it
              doc.add_element(parents.top());
              parents.pop();
            }
          },
          t);
    }
    return doc;
  }
};

} // namespace XML

#endif
