#include <iostream>
#include <string>

enum class Color
{
    // Foreground colors
    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_MAGENTA = 35,
    FG_CYAN = 36,
    FG_WHITE = 37,

    // Bright foreground colors
    FG_BRIGHT_BLACK = 90,
    FG_BRIGHT_RED = 91,
    FG_BRIGHT_GREEN = 92,
    FG_BRIGHT_YELLOW = 93,
    FG_BRIGHT_BLUE = 94,
    FG_BRIGHT_MAGENTA = 95,
    FG_BRIGHT_CYAN = 96,
    FG_BRIGHT_WHITE = 97, // \033[97m

    // Background colors
    BG_BLACK = 40,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_MAGENTA = 45,
    BG_CYAN = 46,
    BG_WHITE = 47,

    // Bright background colors
    BG_BRIGHT_BLACK = 100,
    BG_BRIGHT_RED = 101,
    BG_BRIGHT_GREEN = 102,
    BG_BRIGHT_YELLOW = 103,
    BG_BRIGHT_BLUE = 104,
    BG_BRIGHT_MAGENTA = 105,
    BG_BRIGHT_CYAN = 106,
    BG_BRIGHT_WHITE = 107,

    // Special codes
    RESET = 0,
    BOLD = 1,
    UNDERLINE = 4,
    REVERSED = 7
};

// Get ANSI escape sequence
static std::string color_code(Color color)
{
    return "\033[" + std::to_string(static_cast<int>(color)) + "m";
}

static std::string color_white(){
    return color_code(Color::FG_WHITE);
}

static std::string color_green()
{
    return color_code(Color::FG_BRIGHT_GREEN);
}

static std::string color_red()
{
    return color_code(Color::FG_BRIGHT_RED);
}

static std::string color_blue()
{
    return color_code(Color::FG_BRIGHT_BLUE);
}

static std::string color_cyan()
{
    return color_code(Color::FG_BRIGHT_CYAN);
}

static std::string color_magenta()
{
    return color_code(Color::FG_BRIGHT_MAGENTA);
}

static std::string color_yellow()
{
    return color_code(Color::FG_BRIGHT_YELLOW);
}
