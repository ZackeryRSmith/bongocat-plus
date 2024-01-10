#include <global.hpp>
#include <helper.hpp>
#include <window.hpp>

#define RESET "\033[0m"
#define RESET_BOLD "\033[22m"
#define BOLD "\033[1m"

#define FGRED "\033[0;31m"
#define FGGREEN "\033[0;32m"
#define FGYELLOW "\033[0;33m"
#define FGBLUE "\033[0;34m"
#define FGMAG "\033[0;35m" // magenta

#define BGRED "\033[0;41m"
#define BGGREEN "\033[0;42m"
#define BGYELLOW "\033[0;43m"
#define BGBLUE "\033[0;44m"
#define BGMAG "\033[0;45m" // magenta

#define DEBUG_STRING BGBLUE "[" BOLD "DEBUG" RESET_BOLD "]" RESET " " FGBLUE
#define ERROR_STRING BGRED "[" BOLD "ERROR" RESET_BOLD "]" RESET " " FGRED
#define FATAL_STRING BGMAG "[" BOLD "FATAL" RESET_BOLD "]" RESET " " FGMAG
#define INFO_STRING BGGREEN "[" BOLD "INFO" RESET_BOLD "]" RESET " " FGGREEN
#define WARN_STRING BGYELLOW "[" BOLD "WARN" RESET_BOLD "]" RESET " " FGYELLOW

// ternary operation for lua
bool BongoHelper::ternary(bool condition, bool if_true, bool if_false) {
    return condition ? if_true : if_false;
}

//============================================================================
// LOGGING ~ add color & quit on fatal
//============================================================================
void BongoHelper::debug(std::string msg) {
    std::cout << DEBUG_STRING << msg << RESET << std::endl;
}

void BongoHelper::error(std::string msg) {
    std::cout << ERROR_STRING << msg << RESET << std::endl;
}

void BongoHelper::fatal(std::string msg) {
    std::cout << FATAL_STRING << msg << RESET << std::endl;
}

void BongoHelper::info(std::string msg) {
    std::cout << INFO_STRING << msg << RESET << std::endl;
}

void BongoHelper::warn(std::string msg) {
    std::cout << WARN_STRING << msg << RESET << std::endl;
}

//============================================================================
//  BIND TO LUA
//============================================================================
void BongoHelper::bindToLua() {
    luabridge::getGlobalNamespace(LuaState)
        .addFunction("ternary", &BongoHelper::ternary)
        // logging
        .addFunction("bongoDebug", &BongoHelper::debug)
        .addFunction("bongoError", &BongoHelper::error)
        .addFunction("bongoFatal", &BongoHelper::fatal)
        .addFunction("bongoInfo", &BongoHelper::info)
        .addFunction("bongoWarn", &BongoHelper::warn)
        .beginNamespace("BongoHelper")
        // draw ...
        //.addFunction("draw_circle", &BongoHelper::drawCircle)
        //.addFunction("draw_line", &BongoHelper::drawLine)
        //.addFunction("draw_arc", &BongoHelper::drawArc)
        //.addFunction("draw_quadratic_bezier", &BongoHelper::drawQuad)
        // calc ...
        //.addFunction("calc_bezier_point", &BongoHelper::calcBezierPoint)
        .endNamespace();
}
