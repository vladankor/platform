cmake_minimum_required(VERSION 3.8)
set(PLATFORM_VERSION 1.000)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(Platform VERSION ${PLATFORM_VERSION} LANGUAGES CXX)
message("Start building platform library version ${PLATFORM_VERSION}")

set(SRC_FILES
  "source/scope.cpp"
  "source/spin_lock.cpp"
  "source/utilities/configuration/configuration.cpp"
  "source/utilities/configuration/single_configuration.cpp"
  "source/utilities/log/log.cpp"
  "source/utilities/log/single_log.cpp")

set(INCLUDE_FILES
  "include/platform/exception.hpp"
  "include/platform/scope.hpp"
  "include/platform/singleton.hpp"
  "include/platform/spin_lock.hpp"
  "include/platform/type_traits.hpp"
  "include.platform/utilities.hpp"
  "include/platform/utilities/configuration/reader.hpp"
  "include/platform/utilities/configuration/configuration.hpp"
  "include/platform/utilities/configuration/exception.hpp"
  "include/platform/utilities/configuration/single_configuration.hpp"
  "include/platform/log/exception.hpp"
  "include/platform/log/log.hpp"
  "include/platform/log/single_log.hpp")
include_directories(include)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost COMPONENTS system filesystem REQUIRED)
if (NOT Boost_FOUND)
  message(FATAL_ERROR "Fatal error: Boost required.")
else()
  message(STATUS "Setting up BOOST")
  message(STATUS " Includes - ${Boost_INCLUDE_DIRS}")
  message(STATUS " Library  - ${Boost_LIBRARY_DIRS}")
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif (NOT Boost_FOUND)

add_library(platform STATIC ${SRC_FILES})
target_link_libraries(platform ${Boost_LIBRARIES})

set_target_properties(platform PROPERTIES VERSION ${PROJECT_VERSION}
                                          SOVERSION ${PROJECT_VERSION_MAJOR})
