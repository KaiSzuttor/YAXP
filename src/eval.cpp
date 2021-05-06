
#include <fstream>
#include <iostream>
#include <string>

#include "eval.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " data.xml operations.xml\n";
    std::exit(1);
  }

  // read xml files
  std::ifstream data_stream(argv[1], std::ios::in);
  std::ifstream op_stream(argv[2], std::ios::in);
  // Evaluate and write resulting XML to standard out.
  Operation::eval(data_stream, op_stream, std::cout);
}