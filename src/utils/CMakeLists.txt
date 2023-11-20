target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

target_sources(
  ${PROJECT_NAME}
  PRIVATE "l2a_ai_functions.cpp"
          "l2a_error.cpp"     
          "l2a_file_system.cpp"
          "l2a_math.cpp"       
          "l2a_parameter_list.cpp"  
          "l2a_string_functions.cpp"
          "l2a_version.cpp")
