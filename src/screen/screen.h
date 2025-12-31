#pragma once

#include <stdint.h>
#include <limine.h>

void init_screen(struct limine_framebuffer *fb);
void put_char(char c);
void print(const char *str);
void println(const char *str);
void set_color(uint32_t color);
