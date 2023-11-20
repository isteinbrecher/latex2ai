target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

target_sources(
  ${PROJECT_NAME}
  PRIVATE "l2a_annotator.cpp"
          "l2a_forms.cpp"
          "l2a_global.cpp"
          "l2a_item.cpp"
          "l2a_latex.cpp"
          "l2a_plugin.cpp"
          "l2a_property.cpp"
          "l2a_suites.cpp")

add_subdirectory(utils)
add_subdirectory(tests)
