#ifndef BONGO_CAT_HELPER_HPP
#define BONGO_CAT_HELPER_HPP
#include <global.hpp>

namespace BongoHelper {
// TODO: make this work with any type's not just bool
bool ternary(bool condition, bool if_true, bool if_false);

// these functions are binded to bongo... for example bongoWarn
// simply to avoid naming conflicts
void debug(std::string msg);
void error(std::string msg);
void fatal(std::string msg);
void info(std::string msg);
void warn(std::string msg);

//============================================================================
//  DRAW ...
//============================================================================
// void drawCircle(double x, double y, double radius, sf::Color fillColor);
// void drawLine(double x1, double y1, double x2, double y2, sf::Color color,
//              int thickness);
// void drawArc(double x1, double y1, double x2, double y2, int width,
//             sf::Color color);
// void drawQuadraticBezier(double x1, double y1, double cx, double cy, double
// x2,
//                         double y2, int segments, int thickness,
//                         sf::Color color);
//============================================================================
//  CALC ...
//============================================================================
// std::array<double, 2> calcBezierPoint(double ratio, std::vector<double>
// &points,
//                                      int length);
//============================================================================
//  BIND TO LUA
//============================================================================
void bindToLua();
} // namespace BongoHelper
#endif
