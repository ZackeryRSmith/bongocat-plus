cmake_minimum_required(VERSION 3.10)
project(bongocat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColorReset  "${Esc}[m")
  set(ColorBold   "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

include_directories(include ${CMAKE_SOURCE_DIR}/include/json)

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp src/custom/*.cpp)

# Define the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Check for SFML and add it as a dependency
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)

# Print some useful information about the build
message("${BoldRed}Application dir:${ColorReset} ${CMAKE_CURRENT_BINARY_DIR}")
message("${BoldGreen}Sources:${ColorReset}")
foreach(source ${SOURCES})
    string(REPLACE "bongocat-plus" "${BoldWhite}bongo${BoldMagenta}cat${ColorReset}-${BoldYellow}plus${ColorReset}" modified_source ${source})
    string(REPLACE "src" "${BoldGreen}src${ColorReset}" modified_source ${modified_source})
    string(REPLACE "custom" "${BoldCyan}custom${ColorReset}" modified_source ${modified_source})
    message("  [${BoldCyan}*${ColorReset}] ${modified_source}")
endforeach()
message("${BoldBlue}Compiler:${BoldWhite} ${CMAKE_CXX_COMPILER_ID} ${BoldYellow}${CMAKE_CXX_COMPILER_VERSION}${ColorReset}")

