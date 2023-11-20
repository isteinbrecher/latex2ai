target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

target_sources(
  ${PROJECT_NAME}
  PRIVATE "test_base64.cpp"
          "test_file_system.cpp"     
          "test_forms.cpp"
          "test_framework.cpp"       
          "test_parameter_list.cpp"  
          "test_string_functions.cpp"
          "test_utility.cpp"
          "testing.cpp"
          "testing_utility.cpp")
