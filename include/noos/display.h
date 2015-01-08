#ifndef NOOS_DISPLAY_H
#define NOOS_DISPLAY_H

#define NOOS_DISPLAY_ROWS       25
#define NOOS_DISPLAY_COLS       80

#define NOOS_DISPLAY_BLACK      0
#define NOOS_DISPLAY_BLUE       1
#define NOOS_DISPLAY_GREEN      2
#define NOOS_DISPLAY_CYAN       3
#define NOOS_DISPLAY_RED        4
#define NOOS_DISPLAY_MAGENTA    5
#define NOOS_DISPLAY_BROWN      6
#define NOOS_DISPLAY_GRAY       7
#define NOOS_DISPLAY_WHITE      15

#define NOOS_DISPLAY_LIGHT      8  /* NOOS_DISPLAY_LIGHT | NOOS_DISPLAY_RED = light red, etc. */

void display_clear(void);
void display_putchar(char c);
void display_putstr(const char* str);
#define display_putstr_ln(str) do { display_putstr((str)); display_putstr("\r\n"); } while(0)

#endif // NOOS_DISPLAY_H

