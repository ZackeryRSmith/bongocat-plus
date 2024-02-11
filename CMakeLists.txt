CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(bongocat)

# used to hide the lua cmake deprecation warning
SET(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

# setup cmake directories to keep root clean
SET(CMAKE_CACHEFILE_DIR "${CMAKE_SOURCE_DIR}/CMakeFiles")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")
SET(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/CMakeFiles")
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
SET(CMAKE_DEPS_DIR "${CMAKE_BINARY_DIR}/_deps")

# for my clangd users out there
SET(CMAKE_EXPORT_COMPILE_COMMANDS 1)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE(FetchContent)
SET(FETCHCONTENT_BASE_DIR ${CMAKE_DEPS_DIR})

# allow easy differentiation between OS'
IF(UNIX AND NOT APPLE)
    SET(LINUX TRUE)
ENDIF()

# ##############################################################################
# ANSI TEXT STYLING                                                            #
# ##############################################################################
IF(NOT WIN32 OR FORCE_ANSI STREQUAL True) # force ANSI regardless of OS with -DFORCE_ANSI=True
    STRING(ASCII 27 Esc)
    SET(ColorReset "${Esc}[m")
    SET(ColorBold "${Esc}[1m")
    SET(Red "${Esc}[31m")
    SET(Green "${Esc}[32m")
    SET(Yellow "${Esc}[33m")
    SET(Blue "${Esc}[34m")
    SET(Magenta "${Esc}[35m")
    SET(Cyan "${Esc}[36m")
    SET(White "${Esc}[37m")
    SET(BoldRed "${Esc}[1;31m")
    SET(BoldGreen "${Esc}[1;32m")
    SET(BoldYellow "${Esc}[1;33m")
    SET(BoldBlue "${Esc}[1;34m")
    SET(BoldMagenta "${Esc}[1;35m")
    SET(BoldCyan "${Esc}[1;36m")
    SET(BoldWhite "${Esc}[1;37m")
ENDIF()
# ##############################################################################

# ##############################################################################
# CUSTOM LOGGING                                                               #
# ##############################################################################
FUNCTION(LOG)
    LIST(GET ARGV 0 MessageType)
    IF(MessageType STREQUAL FATAL_ERROR OR MessageType STREQUAL SEND_ERROR)
        LIST(REMOVE_AT ARGV 0)
        MESSAGE("[${BoldRed}${MessageType}${ColorReset}] ${Red}${ARGV}${ColorReset}")
    ELSEIF(MessageType STREQUAL WARNING)
        LIST(REMOVE_AT ARGV 0)
        MESSAGE("[${BoldYellow}${MessageType}${ColorReset}] ${Yellow}${ARGV}${ColorReset}")
    ELSEIF(MessageType STREQUAL AUTHOR_WARNING)
        LIST(REMOVE_AT ARGV 0)
        MESSAGE("[${BoldCyan}${MessageType}${ColorReset}] ${Cyan}${ARGV}${ColorReset}")
    ELSEIF(MessageType STREQUAL STATUS)
        LIST(REMOVE_AT ARGV 0)
        MESSAGE("[${BoldGreen}${MessageType}${ColorReset}] ${Green}${ARGV}${ColorReset}")
    ELSE()
        MESSAGE("${ARGV}")
    ENDIF()
ENDFUNCTION()
# ##############################################################################

INCLUDE_DIRECTORIES(
    include # expose all headers
    ${CMAKE_SOURCE_DIR}/include/json # for parsing config.json
    ${CMAKE_DEPS_DIR}/luabridge3-src/Source # for binding cpp to lua
)

# source files (anything under ./src/)
FILE(GLOB_RECURSE SOURCES src/*.cpp)

# define the executable target
ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES})

# ##############################################################################
# FIND OR INSTALL LUA & LUA BRIDGE 3 (for cat scripts)                         #
# ##############################################################################
FIND_PACKAGE(Lua 5.4 QUIET)
IF(LUA_FOUND)
    INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})
    LOG(STATUS "Found LUA")
ELSE()
    # download lua cmake
    FetchContent_Declare(
        lua
        QUIET REQUIRED
        GIT_REPOSITORY "https://github.com/walterschell/Lua"
        GIT_TAG master)
    FetchContent_MakeAvailable(lua)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE lua_static)
    LOG(STATUS "Made LUA available")
ENDIF()

FetchContent_Declare(
    LuaBridge3
    QUIET REQUIRED
    GIT_REPOSITORY "https://github.com/kunitoki/LuaBridge3"
    GIT_TAG master)
FetchContent_MakeAvailable(LuaBridge3)
LOG(STATUS "Made LUA BRIDGE 3 available")

# ------------------------#
# INSTALL SFML (windows)  #
# ------------------------#
IF(WIN32)
    FetchContent_Declare(
        SFML
        QUIET REQUIRED
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
    FetchContent_MakeAvailable(SFML)
    LOG(STATUS "Made SFML ${SFML_VERSION} available")

    # don't build as it's unused
    SET(SFML_BUILD_NETWORK FALSE)
ELSE()
    # ----------------------------#
    # FIND SFML (MacOSX & Linux)  #
    # ----------------------------#
    FIND_PACKAGE(
        SFML 2.5
        COMPONENTS graphics window system audio
        QUIET REQUIRED)
    LOG(STATUS "Found SFML ${SFML_VERSION}")
ENDIF()

# link SFML
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

# ---------------------------------------------------------------------#
# Ensure that the runtime DLLs are copied to the same directory as the #
# executable when building on Windows.  #------------------------------#
# -------------------------------------/
IF(WIN32 AND BUILD_SHARED_LIBS)
    ADD_CUSTOM_COMMAND(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS)
ENDIF()

IF(UNIX)
    EXECUTE_PROCESS(
        COMMAND
        ${CMAKE_COMMAND} -E echo_append
        "\n${BoldWhite}___|${ColorReset} ${BoldBlue}*${ColorReset}NIX SPECIFIC'S ${BoldWhite}|___${ColorReset} : "
    )

    IF(LINUX)
        MESSAGE("${BoldCyan}LINUX${ColorReset}")

        FIND_PACKAGE(X11 REQUIRED)
        INCLUDE_DIRECTORIES(${X11_INCLUDE_DIR})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})

        FIND_PACKAGE(Xrandr REQUIRED)
        INCLUDE_DIRECTORIES(${XRANDR_INCLUDE_DIR})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${XRANDR_LIBRARIES})

        FIND_LIBRARY(XDO_LIBRARY xdo)
        IF(XDO_LIBRARY)
            TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${XDO_LIBRARY})
        ENDIF()

        MESSAGE("${BoldMagenta}X11 DIR:${ColorReset} ${X11_INCLUDE_DIR}")
        MESSAGE("${BoldMagenta}XRANDR DIR:${ColorReset} ${XRANDR_INCLUDE_DIR}")
        MESSAGE("${BoldMagenta}XDO DIR:${ColorReset} ${XDO_LIBRARY}")
    ENDIF()

    IF(APPLE)
        MESSAGE("${BoldWhite}APPLE${ColorReset}")

        FIND_LIBRARY(COREGRAPHICS_LIB CoreGraphics)
        IF(COREGRAPHICS_LIB)
            TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${COREGRAPHICS_LIB})
        ENDIF()

        FIND_LIBRARY(COREFOUNDATION_LIB CoreFoundation)
        IF(COREFOUNDATION_LIB)
            TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${COREFOUNDATION_LIB})
        ENDIF()

        FIND_LIBRARY(APPLICATIONSERVICES_LIB ApplicationServices)
        IF(COREFOUNDATION_LIBRARY)
            TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${APPLICATIONSERVICES_LIB})
        ENDIF()

        MESSAGE("${BoldMagenta}COREGRAPHICS DIR:${ColorReset} ${COREGRAPHICS_LIB}")
        MESSAGE("${BoldMagenta}COREFOUNDATION DIR:${ColorReset} ${COREFOUNDATION_LIB}")
        MESSAGE("${BoldMagenta}APPLICATIONSERVICES DIR:${ColorReset} ${APPLICATIONSERVICES_LIB}")
    ENDIF()

    MESSAGE("${BoldWhite}-------------------------${ColorReset}\n")
ENDIF()

# Print some useful information about the build
MESSAGE("${BoldRed}Application dir:${ColorReset} ${CMAKE_CURRENT_BINARY_DIR}")
MESSAGE("${BoldGreen}Sources:${ColorReset}")
FOREACH(source ${SOURCES})
    STRING(
        REPLACE
        "bongocat-plus"
        "${BoldWhite}bongo${BoldMagenta}cat${ColorReset}-${BoldYellow}plus${ColorReset}"
        modified_source
        ${source})
    STRING(REPLACE "src" "${BoldGreen}src${ColorReset}" modified_source ${modified_source})
    MESSAGE("  [${BoldCyan}*${ColorReset}] ${modified_source}")
ENDFOREACH()
MESSAGE("${BoldBlue}Compiler:${BoldWhite} ${CMAKE_CXX_COMPILER_ID} ${BoldYellow}${CMAKE_CXX_COMPILER_VERSION}${ColorReset}")
