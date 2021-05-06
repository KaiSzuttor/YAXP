#ifndef EVAL_HPP
#define EVAL_HPP

#include <algorithm>
#include <iomanip>
#include <istream>
#include <numeric>
#include <string>
#include <vector>

#include "operation.hpp"
#include "output.hpp"
#include "parser.hpp"

namespace Operation {

/**
 * @brief Map the function name given as a string to the resp. function and
 * apply it on the data.
 *
 * @param name The name of a function to apply on @p data. Name \f$\in\f$
 * {"min", "max", "sum", "average"}.
 * @param data Data to operate on.
 * @return The return value of the function with name @p name applied on the @p
 * data.
 */
double apply_func(std::string name, std::vector<double> const &data) {
  if (name == "min")
    return *std::min_element(data.begin(), data.end());
  if (name == "max")
    return *std::max_element(data.begin(), data.end());
  if (name == "sum")
    return std::accumulate(data.begin(), data.end(), 0.0);
  if (name == "average")
    return apply_func("sum", data) / data.size();
  throw std::runtime_error("Unsupported function operation: " + name);
}

void eval(std::istream &data, std::istream &ops, std::ostream &output) {
  auto const data_doc = XML::Parser(XML::Lexer(data).tokenize()).parse();
  auto const op_doc = XML::Parser(XML::Lexer(ops).tokenize()).parse();

  // collect all operations
  std::vector<Operation> operations;
  for (auto const e : op_doc) {
    if (e->name == "operation")
      operations.emplace_back(Operation(*e));
  }
  assert(not operations.empty());

  XML::XML_Doc results;
  auto const root = std::make_shared<XML::XML_Element>("results", nullptr, 0);
  results.add_element(root);
  for (auto const &op : operations) {
    // filter elements
    auto const elements = XML::attr_filter(data_doc, "name", op.m_filter);
    // gather values
    std::vector<double> values;
    for (auto e : elements) {
      if (op.m_type == "sub") {
        auto const val = std::stod(e->get_child(op.m_attrib).content);
        values.push_back(val);
        continue;
      }
      if (op.m_type == "attrib") {
        auto const val = std::stod(e->get_attribute(op.m_attrib));
        values.push_back(val);
        continue;
      }
      throw std::runtime_error("Unsuported operation type");
    }
    assert(not values.empty());

    auto res_elem = std::make_shared<XML::XML_Element>("result", root, 1);
    res_elem->attributes.push_back(
        XML::XML_Attribute{std::make_pair("name", op.m_name)});
    std::ostringstream res_s;
    res_s << std::fixed << std::setprecision(2)
          << apply_func(op.m_func, values);

    res_elem->content = res_s.str();
    root->children.push_back(res_elem);
  }
  output << results;
}

} // namespace Operation

#endif