#pragma once

#include <stdint.h>
#include <limine.h>

void init_screen(struct limine_framebuffer *fb);
void put_char(char c);
