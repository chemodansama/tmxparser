cmake_minimum_required(VERSION 3.14)

project(tmxparser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX11_EXTENSION_COMPILE_OPTION "-std=c++20")

include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_MODULE_PATH})

set(TMXPARSER_VERSION_MAJOR 2)
set(TMXPARSER_VERSION_MINOR 1)
set(TMXPARSER_VERSION_PATCH 0)

set(TMXPARSER_VERSION "${TMXPARSER_VERSION_MAJOR}.${TMXPARSER_VERSION_MINOR}.${TMXPARSER_VERSION_PATCH}")

message("Configuring tmxparser version ${VERSION}")

configure_file(
  "include/Tmx.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/Tmx.h")

option(USE_MINIZ  "Use miniz.c instead of zlib (default: OFF)" OFF)
option(BUILD_SHARED_LIBS "Build shared libs, otherwise static libs. (default: ON)" ON)
option(BUILD_TINYXML2  "Build tinyxml2 as external project (default: OFF)" OFF)
option(BUILD_TESTS  "Build tests. (default: OFF)" OFF)
option(BUILD_DOCS  "Build documentation. (default: OFF)" OFF)

#Dependencies Settings
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/deps.cmake)

find_package(tinyxml2 REQUIRED)

if(NOT USE_MINIZ)
  find_package(ZLIB) #<-- build it as external project?
endif()

add_library(tmxparser "")

if(BUILD_SHARED_LIBS)
  set_target_properties(tmxparser PROPERTIES
    SOVERSION ${TMXPARSER_VERSION_MAJOR}
    VERSION ${TMXPARSER_VERSION})
endif()

target_sources(tmxparser
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include/Tmx.h
  PRIVATE src/TmxColor.cpp
  PRIVATE include/TmxColor.h
  PRIVATE src/TmxEllipse.cpp
  PRIVATE include/TmxEllipse.h
  PRIVATE src/TmxGroupLayer.cpp
  PRIVATE include/TmxGroupLayer.h
  PRIVATE src/TmxImage.cpp
  PRIVATE include/TmxImage.h
  PRIVATE src/TmxImageLayer.cpp
  PRIVATE include/TmxImageLayer.h
  PRIVATE src/TmxLayer.cpp
  PRIVATE include/TmxLayer.h
  PRIVATE src/TmxMap.cpp
  PRIVATE include/TmxMap.h
  PRIVATE src/TmxObject.cpp
  PRIVATE include/TmxObject.h
  PRIVATE src/TmxObjectGroup.cpp
  PRIVATE include/TmxObjectGroup.h
  PRIVATE src/TmxPoint.cpp
  PRIVATE include/TmxPoint.h
  PRIVATE src/TmxPolygon.cpp
  PRIVATE include/TmxPolygon.h
  PRIVATE src/TmxPolyline.cpp
  PRIVATE include/TmxPolyline.h
  PRIVATE src/TmxProperty.cpp
  PRIVATE include/TmxProperty.h
  PRIVATE src/TmxPropertySet.cpp
  PRIVATE include/TmxPropertySet.h
  PRIVATE src/TmxTerrain.cpp
  PRIVATE include/TmxTerrain.h
  PRIVATE src/TmxTerrainArray.cpp
  PRIVATE include/TmxTerrainArray.h
  PRIVATE src/TmxText.cpp
  PRIVATE include/TmxText.h
  PRIVATE src/TmxTile.cpp
  PRIVATE include/TmxTile.h
  PRIVATE src/TmxTileset.cpp
  PRIVATE include/TmxTileset.h
  PRIVATE src/TmxTileLayer.cpp
  PRIVATE include/TmxTileLayer.h
  PRIVATE src/TmxTileOffset.cpp
  PRIVATE include/TmxTileOffset.h
  PRIVATE src/TmxUtil.cpp
  PRIVATE include/TmxUtil.h
  PRIVATE src/base64/base64.cpp
  PRIVATE include/base64/base64.h)

if(USE_MINIZ)
  target_sources(tmxparser
    PRIVATE src/miniz.c)
  target_compile_definitions(tmxparser
    PRIVATE -DUSE_MINIZ)
endif()

target_include_directories(tmxparser PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(tmxparser
    PRIVATE -pedantic
    PRIVATE -Werror=strict-prototypes
    PRIVATE -Werror=old-style-definition
    PRIVATE -Werror=missing-prototypes)
endif()

if(NOT USE_MINIZ)
  if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(tmxparser
      PRIVATE -Werror)
  endif()
endif()

target_compile_features(tmxparser
  PRIVATE cxx_std_20)

target_link_libraries(tmxparser
  PRIVATE tinyxml2::tinyxml2)

if(NOT USE_MINIZ)
  target_link_libraries(tmxparser
    PRIVATE ZLIB::ZLIB)
endif()

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN Tmx.h)

install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS tmxparser EXPORT tmxparserTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT tmxparserTargets
  FILE tmxparserTargets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tmxparser)

# pkgconfig is okay but export a CMake config file, too
configure_file(src/tmxparser.pc.in tmxparser.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tmxparser.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

include(CMakePackageConfigHelpers)

configure_package_config_file(cmake/tmxparserConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/tmxparserConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tmxparser
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/tmxparserConfigVersion.cmake
  VERSION ${TMXPARSER_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tmxparserConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/tmxparserConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tmxparser)

if(BUILD_TESTS)
    add_executable(run_tests test/test.cpp)

    if(CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(run_tests
            PRIVATE -pedantic
            PRIVATE -Werror
            PRIVATE -Wall
            PRIVATE -g)
    endif()

    target_compile_features(run_tests PRIVATE cxx_std_20)
    target_link_libraries(run_tests tmxparser tinyxml2::tinyxml2)
    file(COPY test/example DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

if(BUILD_GTESTS)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(
        tmx_gtests
        gtests/gtests_polygon.cpp
        gtests/gtests_property.cpp
        gtests/gtests_tileset.cpp
        gtests/gtests_tmx.cpp
    )
    target_link_libraries(
        tmx_gtests
        GTest::gtest_main
        tmxparser
        tinyxml2::tinyxml2
    )

    include(GoogleTest)
    gtest_discover_tests(tmx_gtests)
endif()

if(BUILD_DOCS)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    configure_file(${PROJECT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ALL ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile  COMMENT "Generating API documentation with Doxygen" VERBATIM)
  else()
    message(WARNING "Doxygen not found! No documentation was build.")
  endif()
endif()