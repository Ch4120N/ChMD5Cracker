#include "color.h"

std::string FailMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_red() + " [ " + color_bwhite() + "-" + color_red() + " ] " + color_code(color) + RequestMessage + color_code(Color::RESET);
}

std::string PassMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_code(Color::FG_BRIGHT_GREEN) + " [ " + color_bwhite() + "+" + color_code(Color::FG_BRIGHT_GREEN) + " ] " + color_code(color) + RequestMessage + color_code(Color::RESET);
}

std::string InfoMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_code(Color::FG_BRIGHT_CYAN) + " [ " + color_bwhite() + "*" + color_code(Color::FG_BRIGHT_CYAN) + " ] " + color_code(color) + RequestMessage + color_code(Color::RESET);
}

std::string WarningMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_code(Color::FG_YELLOW) + " [ " + color_bwhite() + "!" + color_code(Color::FG_YELLOW) + " ] " + color_code(color) + RequestMessage + color_code(Color::RESET);
}

std::string DoubleWarningMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_code(Color::FG_YELLOW) + " [ " + color_bwhite() + "!" + color_code(Color::FG_YELLOW) + " ] " + color_code(color) + RequestMessage + color_code(Color::FG_YELLOW) + " [ " + color_bwhite() + "!" + color_code(Color::FG_YELLOW) + " ] " + color_code(Color::RESET);
}

std::string TipMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_code(Color::FG_BRIGHT_MAGENTA) + " [ " + color_bwhite() + "!" + color_code(Color::FG_BRIGHT_MAGENTA) + " ] " + color_code(color) + RequestMessage + color_code(Color::RESET);
}

std::string ProgressMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_code(Color::FG_BRIGHT_MAGENTA) + "\r [ " + color_bwhite() + ">" + color_code(Color::FG_BRIGHT_MAGENTA) + " ] " + color_code(color) + RequestMessage + color_code(Color::RESET);
}
