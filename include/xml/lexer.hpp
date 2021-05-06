#ifndef LEXER_HPP
#define LEXER_HPP

#include <cctype>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

/** @file lexer.hpp
 *  @brief This file contains the lexer class and XML token representations.
 */

namespace XML {

/**
 * @brief "<" Token
 */
struct StartTagBegin {
  std::string name;
};

/**
 * @brief ">" Token
 */
struct StartTagEnd {};

/**
 * @brief "/>" Token
 */
struct CloseTag {};

/**
 * @brief Attribute of a StartTag
 */
struct Attribute {
  std::pair<std::string, std::string> key_val;
};

/**
 * @brief The content of an element.
 */
struct Content {
  std::string content;
};

/**
 * @brief "</name>" Token
 */
struct EndTag {
  std::string name;
};

/** Type used for all possible tokens. */
using Token = std::variant<StartTagBegin, StartTagEnd, CloseTag, Attribute,
                           Content, EndTag>;

namespace detail {

/**
 * @brief Extract the next string and return it.
 *
 * The string is assumed to end at the next non-alphanumeric char.
 *
 * @param[in,out] stream The input stream to read from.
 * @return The extracted string.
 */
std::string extract_string(std::istream &stream) {
  std::stringstream result;
  char c;
  while (std::isalpha(stream.peek())) {
    stream >> c;
    result << c;
  }
  return result.str();
}

/**
 * @brief Extract the next XML content.
 *
 * The non XML element content is read and returned.
 * It is assumed to end at the next tag.
 *
 * @param[in,out] stream The input stream to read from.
 * @return The XML content as a string.
 */
std::string extract_content(std::istream &stream) {
  std::stringstream result;
  char c;
  while (stream.peek() != '<') {
    stream >> c;
    result << c;
  }
  return result.str();
}

/**
 * @brief Extract the next XML attribute.
 *
 * The entire XML attribute is read and returned.
 *
 * @param[in,out] stream The input stream to read from.
 * @return A pair of identifier and value of the attribute.
 */
std::pair<std::string, std::string> extract_attribute(std::istream &stream) {
  // read the identifier
  std::string key;
  std::getline(stream, key, '=');
  // read the value of the attribute
  std::string value;
  stream.ignore(); // ignore the apostrophe
  std::getline(stream, value, '"');
  return {key, value};
}

} // namespace detail

/**
 * @brief Lexer to generate a list of tokens from the stream of characters.
 */
struct Lexer {
  std::vector<Token> m_tokens;
  std::istream &m_istream;

  /**
   * @brief The lexer is constructed with a file stream.
   * @param[in] stream The file stream to read from.
   */
  Lexer(std::istream &stream) : m_istream(stream) {}

  /**
   * @brief Create a vector of tokens from the input stream.
   *
   * The stream is read char by char and tokens are identified,
   * instances of the respective token classes are instantiated
   * returned in a vector.
   *
   * @return A vector of token instances.
   */
  std::vector<Token> tokenize() {
    char c;
    while (true) {
      m_istream >> c;
      if (c == '<') {
        if (std::isalpha(m_istream.peek())) {
          // we are reading the name of a StartTagBegin
          auto const name = detail::extract_string(m_istream);
          m_tokens.push_back(StartTagBegin{name});
        } else if (m_istream.peek() == '/') {
          m_istream >> c;
          // we are reading the name of an EndTag
          auto const name = detail::extract_string(m_istream);
          m_tokens.push_back(EndTag{name});
          m_istream.ignore(); // skip the closing bracket
          continue;
        }
      } else if (c == '/') {
        if (m_istream.peek() == '>') {
          m_istream.ignore(); // consume the '>'
          m_tokens.push_back(CloseTag());
          continue;
        }
      } else if (c == '>') {
        m_tokens.push_back(StartTagEnd());
        if (std::isalnum(m_istream.peek())) {
          std::string cont = detail::extract_content(m_istream);
          m_tokens.push_back(Content{cont});
          continue;
        }
      } else if (std::isalpha(c)) {
        m_istream.putback(c);
        // we are reading an Attribute
        auto const key_val = detail::extract_attribute(m_istream);
        m_tokens.push_back(Attribute{key_val});
        continue;
      }
      if (m_istream.eof())
        break;
    }
    return m_tokens;
  }
};

} // namespace XML

#endif