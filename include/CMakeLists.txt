add_library(xml INTERFACE)
target_include_directories(
  xml INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/xml>
                $<INSTALL_INTERFACE:include/xml>)
target_link_libraries(xml INTERFACE project_properties)

add_library(operation INTERFACE)
target_include_directories(
  operation INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/operation>
                      $<INSTALL_INTERFACE:include/operation>)
target_link_libraries(operation INTERFACE project_properties xml)
