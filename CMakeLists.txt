cmake_minimum_required(VERSION 3.10)
project(bongocat)

# used to hide the lua cmake deprecation warning
set(CMAKE_WARN_DEPRECATED
    OFF
    CACHE BOOL "" FORCE)

# setup cmake directories to keep root clean
set(CMAKE_CACHEFILE_DIR "${CMAKE_SOURCE_DIR}/CMakeFiles")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/CMakeFiles")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_DEPS_DIR "${CMAKE_BINARY_DIR}/_deps")

# for my clangd users out there
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_DEPS_DIR})

# allow easy differentiation between OS'
if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

# ##############################################################################
# *NIX TEXT STYLING                                                            #
# ##############################################################################
if(NOT WIN32 OR FORCE_ANSI STREQUAL True) # force ANSI regardless of OS with
                                          # -DFORCE_ANSI=True
  string(ASCII 27 Esc)
  set(ColorReset "${Esc}[m")
  set(ColorBold "${Esc}[1m")
  set(Red "${Esc}[31m")
  set(Green "${Esc}[32m")
  set(Yellow "${Esc}[33m")
  set(Blue "${Esc}[34m")
  set(Magenta "${Esc}[35m")
  set(Cyan "${Esc}[36m")
  set(White "${Esc}[37m")
  set(BoldRed "${Esc}[1;31m")
  set(BoldGreen "${Esc}[1;32m")
  set(BoldYellow "${Esc}[1;33m")
  set(BoldBlue "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan "${Esc}[1;36m")
  set(BoldWhite "${Esc}[1;37m")
endif()
# ##############################################################################

# ##############################################################################
# CUSTOM LOGGING                                                               #
# ##############################################################################
function(log)
  list(GET ARGV 0 MessageType)
  if(MessageType STREQUAL FATAL_ERROR OR MessageType STREQUAL SEND_ERROR)
    list(REMOVE_AT ARGV 0)
    message(
      "[${BoldRed}${MessageType}${ColorReset}] ${Red}${ARGV}${ColorReset}")
  elseif(MessageType STREQUAL WARNING)
    list(REMOVE_AT ARGV 0)
    message(
      "[${BoldYellow}${MessageType}${ColorReset}] ${Yellow}${ARGV}${ColorReset}"
    )
  elseif(MessageType STREQUAL AUTHOR_WARNING)
    list(REMOVE_AT ARGV 0)
    message(
      "[${BoldCyan}${MessageType}${ColorReset}] ${Cyan}${ARGV}${ColorReset}")
  elseif(MessageType STREQUAL STATUS)
    list(REMOVE_AT ARGV 0)
    message(
      "[${BoldGreen}${MessageType}${ColorReset}] ${Green}${ARGV}${ColorReset}")
  else()
    message("${ARGV}")
  endif()
endfunction()
# ##############################################################################

include_directories(
  include # expose all headers
  ${CMAKE_SOURCE_DIR}/include/json # for parsing config.json
  ${CMAKE_DEPS_DIR}/luabridge3-src/Source)

# source files (anything under ./src/)
file(GLOB_RECURSE SOURCES src/*.cpp)

# define the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# ##############################################################################
# INSTALL LUA AND LUA BRIDGE 3 (for cat scripts) #
# ##############################################################################
FetchContent_Declare(
  lua
  QUIET REQUIRED
  GIT_REPOSITORY "https://github.com/walterschell/Lua"
  GIT_TAG master)
FetchContent_MakeAvailable(lua)
target_link_libraries(${PROJECT_NAME} PRIVATE lua_static)
log(STATUS "Made LUA available")

FetchContent_Declare(
  LuaBridge3
  QUIET REQUIRED
  GIT_REPOSITORY "https://github.com/kunitoki/LuaBridge3"
  GIT_TAG master)
FetchContent_MakeAvailable(LuaBridge3)
log(STATUS "Made LUA BRIDGE 3 available")

# ------------------------#
# INSTALL SFML (windows)  #
# ------------------------#
if(WIN32)
  FetchContent_Declare(
    SFML
    QUIET REQUIRED
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
  FetchContent_MakeAvailable(SFML)
  log(STATUS "Made SFML available")

  # don't build as it's unused
  set(SFML_BUILD_NETWORK FALSE)

else()
  # ----------------------------#
  # FIND SFML (MacOSX & Linux) #
  # ----------------------------#
  find_package(
    SFML 2.5
    COMPONENTS graphics window system audio
    QUIET REQUIRED)
endif()

# link SFML
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window
                                              sfml-system sfml-audio)
log(STATUS "Linked SFML ${SFML_VERSION}")

# ---------------------------------------------------------------------#
# Ensure that the runtime DLLs are copied to the same directory as the #
# executable when building on Windows.  #------------------------------#
# -------------------------------------/
if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS)
endif()

if(UNIX)
  execute_process(
    COMMAND
      ${CMAKE_COMMAND} -E echo_append
      "\n${BoldWhite}___|${ColorReset} ${BoldBlue}*${ColorReset}NIX SPECIFIC'S ${BoldWhite}|___${ColorReset} : "
  )

  if(LINUX)
    message("${BoldCyan}LINUX${ColorReset}")

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
    message("${BoldWhite}APPLE${ColorReset}")

    find_library(COREGRAPHICS_LIB CoreGraphics)
    if(COREGRAPHICS_LIB)
      target_link_libraries(${PROJECT_NAME} PRIVATE ${COREGRAPHICS_LIB})
    endif()

    find_library(COREFOUNDATION_LIB CoreFoundation)
    if(COREFOUNDATION_LIB)
      target_link_libraries(${PROJECT_NAME} PRIVATE ${COREFOUNDATION_LIB})
    endif()

    find_library(APPLICATIONSERVICES_LIB ApplicationServices)
    if(COREFOUNDATION_LIBRARY)
      target_link_libraries(${PROJECT_NAME} PRIVATE ${APPLICATIONSERVICES_LIB})
    endif()

    message("${BoldMagenta}COREGRAPHICS DIR:${ColorReset} ${COREGRAPHICS_LIB}")
    message(
      "${BoldMagenta}COREFOUNDATION DIR:${ColorReset} ${COREFOUNDATION_LIB}")
    message(
      "${BoldMagenta}APPLICATIONSERVICES DIR:${ColorReset} ${APPLICATIONSERVICES_LIB}"
    )

  endif()

  message("${BoldWhite}-------------------------${ColorReset}\n")
endif()

# Print some useful information about the build
message("${BoldRed}Application dir:${ColorReset} ${CMAKE_CURRENT_BINARY_DIR}")
message("${BoldGreen}Sources:${ColorReset}")
foreach(source ${SOURCES})
  string(
    REPLACE
      "bongocat-plus"
      "${BoldWhite}bongo${BoldMagenta}cat${ColorReset}-${BoldYellow}plus${ColorReset}"
      modified_source
      ${source})
  string(REPLACE "src" "${BoldGreen}src${ColorReset}" modified_source
                 ${modified_source})
  message("  [${BoldCyan}*${ColorReset}] ${modified_source}")
endforeach()
message(
  "${BoldBlue}Compiler:${BoldWhite} ${CMAKE_CXX_COMPILER_ID} ${BoldYellow}${CMAKE_CXX_COMPILER_VERSION}${ColorReset}"
)
