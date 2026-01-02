#include "color.h"

std::string FailMessage(const std::string RequestMessage, Color color = Color::FG_WHITE)
{
    return color_code(Color::FG_BRIGHT_RED) + " [ " + color_code(Color::FG_BRIGHT_WHITE) + "-" + color_code(Color::FG_BRIGHT_RED) + " ] " + color_code(color) + RequestMessage + color_code(Color::RESET);
}
