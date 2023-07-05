cmake_minimum_required(VERSION 3.10)
project(bongocat)

# Install SFML, this may become optinal in the future
# as installing and compiling SFML (if already installed
# on the system) is usless. But this makes windows compiling
# much easier.
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

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

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}> COMMAND_EXPAND_LISTS)
endif()

if(UNIX)
  find_package(SDL2 REQUIRED)
  include_directories(${SDL2_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})

  message("\n${BoldWhite}___| ${BoldBlue}*${ColorReset}NIX SPECIFIC'S ${BoldWhite}|___")
  message("${BoldMagenta}SDL2 DIR:${ColorReset} ${SDL2_INCLUDE_DIR}")
  
  if(LINUX)
    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
    
    find_package(Xrandr REQUIRED)
    include_directories(${XRANDR_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${XRANDR_LIBRARIES})
    
    find_library(XDO_LIBRARY xdo)
    if(XDO_LIBRARY)
      target_link_libraries(${PROJECT_NAME} PRIVATE ${XDO_LIBRARY})
    endif()
  
    message("${BoldMagenta}X11 DIR:${ColorReset} ${X11_INCLUDE_DIR}")
    message("${BoldMagenta}XRANDR DIR:${ColorReset} ${XRANDR_INCLUDE_DIR}")
    message("${BoldMagenta}XDO DIR:${ColorReset} ${XDO_LIBRARY}")
  endif()
  
  if(APPLE)
    find_library(COREGRAPHICS_LIBRARY CoreGraphics)
    if(COREGRAPHICS_LIBRARY)
      target_link_libraries(${PROJECT_NAME} PRIVATE ${COREGRAPHICS_LIBRARY})
    endif()

    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    if(COREFOUNDATION_LIBRARY)
      target_link_libraries(${PROJECT_NAME} PRIVATE ${COREFOUNDATION_LIBRARY})
    endif()

    message("${BoldMagenta}COREGRAPHICS DIR:${ColorReset} ${COREGRAPHICS_LIBRARY}")
    message("${BoldMagenta}COREFOUNDATION DIR:${ColorReset} ${COREFOUNDATION_LIBRARY}")
  endif()

  message("${BoldWhite}-------------------------${ColorReset}\n")
endif()

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

install(TARGETS ${PROJECT_NAME})