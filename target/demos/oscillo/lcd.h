#ifndef LCD_H
#define LCD_H

struct color {
	int r;
	int g;
	int b;
};

int init_lcd(void);
unsigned long lcd_get_xres(void);
unsigned long lcd_get_yres(void);
void close_lcd(int fbfd);
void draw_pixel(unsigned int x, unsigned int y, struct color rgb);
void clear_vline(unsigned int x);

#endif /* LCD_H */
