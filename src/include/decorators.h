#include "color.h"

std::string FailMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_code(Color::FG_BRIGHT_RED) + " [ " + color_code(Color::FG_BRIGHT_WHITE) + "-" + color_code(Color::FG_BRIGHT_RED) + " ] " + color_code(color) + RequestMessage + color_code(Color::RESET);
}

std::string PassMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_code(Color::FG_BRIGHT_GREEN) + " [ " + color_code(Color::FG_BRIGHT_WHITE) + "+" + color_code(Color::FG_BRIGHT_GREEN) + " ] " + color_code(color) + RequestMessage + color_code(Color::RESET);
}

std::string InfoMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_code(Color::FG_BRIGHT_CYAN) + " [ " + color_code(Color::FG_BRIGHT_WHITE) + "*" + color_code(Color::FG_BRIGHT_CYAN) + " ] " + color_code(color) + RequestMessage + color_code(Color::RESET);
}

std::string WarningMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_code(Color::FG_YELLOW) + " [ " + color_code(Color::FG_BRIGHT_WHITE) + "!" + color_code(Color::FG_YELLOW) + " ] " + color_code(color) + RequestMessage + color_code(Color::RESET);
}

std::string DoubleWarningMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_code(Color::FG_YELLOW) + " [ " + color_code(Color::FG_BRIGHT_WHITE) + "!" + color_code(Color::FG_YELLOW) + " ] " + color_code(color) + RequestMessage + color_code(Color::FG_YELLOW) + " [ " + color_code(Color::FG_BRIGHT_WHITE) + "!" + color_code(Color::FG_YELLOW) + " ] " + color_code(Color::RESET);
}