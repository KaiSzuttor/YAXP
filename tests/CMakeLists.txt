if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_executable(lexer_test lexer.test.cpp)
  target_link_libraries(lexer_test PRIVATE main_test xml)
  add_test(NAME lexer COMMAND lexer_test)

  add_executable(parser_test parser.test.cpp)
  target_link_libraries(parser_test PRIVATE main_test xml)
  add_test(NAME parser COMMAND parser_test)

  add_executable(operation_test operation.test.cpp)
  target_link_libraries(operation_test PRIVATE main_test operation)
  add_test(NAME operation COMMAND operation_test)
endif()
