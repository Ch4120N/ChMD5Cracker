#include "color.h"

std::string FailMessage(const std::string RequestMessage, Color color = Color::FG_BRIGHT_WHITE)
{
    return color_red() + " [ " + color_bwhite() + "-" + color_red() + " ] " + color_code(color) + RequestMessage + reset();
}

std::string PassMessage(const std::string RequestMessage, Color color = Color::FG_BRIGHT_WHITE)
{
    return color_green() + " [ " + color_bwhite() + "+" + color_green() + " ] " + color_code(color) + RequestMessage + reset();
}

std::string InfoMessage(const std::string RequestMessage, Color color = Color::FG_BRIGHT_WHITE)
{
    return color_cyan() + " [ " + color_bwhite() + "*" + color_cyan() + " ] " + color_code(color) + RequestMessage + reset();
}

std::string WarningMessage(const std::string RequestMessage, Color color = Color::FG_BRIGHT_WHITE)
{
    return color_orange() + " [ " + color_bwhite() + "!" + color_orange() + " ] " + color_code(color) + RequestMessage + reset();
}

std::string DoubleWarningMessage(const std::string RequestMessage, Color color = Color::FG_BRIGHT_WHITE)
{
    return color_orange() + " [ " + color_bwhite() + "!" + color_orange() + " ] " + color_code(color) + RequestMessage + color_orange() + " [ " + color_bwhite() + "!" + color_orange() + " ] " + reset();
}

std::string TipMessage(const std::string RequestMessage, Color color = Color::FG_BRIGHT_WHITE)
{
    return color_magenta() + " [ " + color_bwhite() + "!" + color_magenta() + " ] " + color_code(color) + RequestMessage + reset();
}

std::string ProgressMessage(const std::string RequestMessage, Color color = Color::FG_BRIGHT_WHITE)
{
    return color_magenta() + "\r [ " + color_bwhite() + ">" + color_magenta() + " ] " + color_code(color) + RequestMessage + reset();
}
