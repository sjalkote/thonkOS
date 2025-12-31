#pragma once

#include <stdint.h>
#include <limine.h>

void init_screen(struct limine_framebuffer *fb);
void put_char(char c);
void set_color(uint32_t color);
void set_color_rgb(uint8_t r, uint8_t g, uint8_t b);
void print(const char *str);
void println(const char *str);
