
#include <string.h>
#include <noos/display.h>

#define DISPLAY_MAKE_ATTR(_textcolor_, _backcolor_) ((char) 0 | ((_textcolor_) & 0xF) | (((_backcolor_) & 0xF) << 4))

struct display_char
{
    char c;
    char attr;
};

static char g_back_color = NOOS_DISPLAY_BLACK;
static char g_text_color = NOOS_DISPLAY_GREEN;

static unsigned int g_display_row = 0;
static unsigned int g_display_col = 0;

static struct display_char* g_display_buffer = (struct display_char*) 0xb8000;

// Scroll one row up
static void scroll_row(void)
{
    /* Move all rows by 1 */
    for (int i = 1; i < NOOS_DISPLAY_ROWS; ++i)
    {
        struct display_char* src_row = &g_display_buffer[i * NOOS_DISPLAY_COLS];
        struct display_char* dst_row = &g_display_buffer[(i - 1) * NOOS_DISPLAY_COLS];
        memcpy(dst_row, src_row, NOOS_DISPLAY_COLS * 2);
    }

    /* Erase lower row */
    struct display_char* lower_row = &g_display_buffer[(NOOS_DISPLAY_ROWS - 1) * NOOS_DISPLAY_COLS];
    for (int j = 0; j < NOOS_DISPLAY_COLS; ++j)
    {
        lower_row[j].c = 0;
        lower_row[j].attr = DISPLAY_MAKE_ATTR(g_text_color, g_back_color);
    }
}

static void handle_new_line(void)
{
    if ((g_display_row + 1) == NOOS_DISPLAY_ROWS)
    {
        scroll_row();
    }
    else
    {
        ++g_display_row;
    }
}

void display_clear()
{
    for (int i = 0; i < (80 * 25); ++i)
    {
        g_display_buffer[i].c = 0x0;
        g_display_buffer[i].attr = DISPLAY_MAKE_ATTR(g_back_color, g_back_color);
    }    

    g_display_row = 0;
    g_display_col = 0;
}

void display_putchar(char c)
{
    // Need newline?
    if (g_display_col == NOOS_DISPLAY_COLS)
    {
        handle_new_line();
        g_display_col = 0;
    }

    struct display_char* ptr = &g_display_buffer[g_display_row * NOOS_DISPLAY_COLS + g_display_col];
    ptr->c = c;
    ptr->attr = DISPLAY_MAKE_ATTR(g_text_color, g_back_color);

    ++g_display_col;
}

void display_putstr(const char* str)
{
    while(*str != '\0')
    {
        switch (*str)
        {
        case '\r':  g_display_col = 0; break;
        case '\n':  handle_new_line(); break;
        default:    display_putchar(*str); break;
        };

        ++str;
    }
}

