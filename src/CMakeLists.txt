if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_library(main_test SHARED main.test.cpp)
  target_link_libraries(main_test PUBLIC doctest project_properties)
endif()

add_executable(eval eval.cpp)
target_link_libraries(eval PRIVATE xml operation)
