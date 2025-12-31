#pragma once

#include <stdint.h>
#include <limine.h>

void init_screen(struct limine_framebuffer *fb);
void put_char(char c, const uint32_t x, const uint32_t y);
