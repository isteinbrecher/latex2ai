cmake_minimum_required(VERSION 3.21)

project(LaTeX2AI)


add_library(base64 STATIC)
target_sources(base64 PRIVATE
	    "tpl/base64/src/base64.cpp"
)
target_include_directories(base64 PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/tpl/base64/src>)

add_library(tinyxml2 STATIC)
target_sources(tinyxml2 PRIVATE
	    "tpl/tinyxml2/tinyxml2.cpp"
)
target_include_directories(tinyxml2 PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/tpl/tinyxml2>)



add_library(IllustratorSDK STATIC)
target_sources(IllustratorSDK PRIVATE
	    "../../common/source/SDKAboutPluginsHelper.cpp"
	    "../../common/source/Suites.cpp"	
        "../../common/source/IllustratorSDK.cpp"
        "../../common/source/FlashUIController.cpp"
        "../../common/source/SDKPlugPlug.cpp"
        "../../common/source/SDKErrors.cpp"
        "../../common/source/AppContext.cpp"
        "../../../illustratorapi/illustrator/AIAssert.cpp"
        "../../../illustratorapi/ate/IThrowException.cpp"
        "../../../illustratorapi/illustrator/IAIFilePath.cpp"
        "../../../illustratorapi/illustrator/IAIUnicodeString.cpp"
        "../../../illustratorapi/illustrator/IAIStringFormatUtils.cpp"
)
set_target_properties(IllustratorSDK PROPERTIES CXX_STANDARD 17)
target_include_directories(IllustratorSDK PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../common/includes>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../illustratorapi>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../illustratorapi/pica_sp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../illustratorapi/illustrator>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../illustratorapi/ate>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../illustratorapi/ate/slotextdomtypes>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../illustratorapi/ate/legacy>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../illustratorapi/illustrator/config>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../illustratorapi/illustrator/config/compiler>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../illustratorapi/illustrator/actions>
)
target_include_directories(IllustratorSDK PUBLIC Resources/raw)
target_include_directories(IllustratorSDK PUBLIC Resources/Mac)
set_target_properties(IllustratorSDK PROPERTIES CLANG_LINK_OBJC_RUNTIME "NO")


add_library(IllustratorObjects OBJECT)
target_link_libraries(IllustratorObjects PUBLIC IllustratorSDK)
target_sources(IllustratorObjects PRIVATE
	"../../common/source/Main.cpp"
    "../../common/source/Plugin.cpp"
)
set_target_properties(IllustratorObjects PROPERTIES CXX_STANDARD 17)
target_include_directories(IllustratorObjects PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../common/mac>)
target_include_directories(IllustratorObjects PUBLIC Resources/raw)
target_include_directories(IllustratorObjects PUBLIC Resources/Mac)


add_library(${PROJECT_NAME} SHARED)
set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".aip")
target_link_libraries(${PROJECT_NAME} PRIVATE IllustratorObjects)
target_link_libraries(${PROJECT_NAME} PRIVATE base64)
target_link_libraries(${PROJECT_NAME} PRIVATE tinyxml2)




set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# This can be removed in the future, when the correct library is imported
#target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../common/win>)



target_sources(${PROJECT_NAME} PRIVATE
   "src/l2a_suites.cpp"
   "src/l2a_plugin.cpp"
    "resources/l2a.rc"
)



target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>/src/utility)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>/src/l2a_property)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>/src/l2a_global)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>/src)

add_subdirectory(src/tests)



set_source_files_properties(MenuPlay.rc PROPERTIES LANGUAGE RC)

foreach(LANG  C CXX RC)
    set(CMAKE_${LANG}_STANDARD_INCLUDE_DIRECTORIES ${CUSTOM_INCLUDE_DIRECTORIES})
endforeach()
