target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

target_sources(
  ${PROJECT_NAME}
  PRIVATE "l2a.rc")
