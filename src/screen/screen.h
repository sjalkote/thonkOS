#pragma once

#include <stdint.h>
#include <limine.h>

void put_char(const struct limine_framebuffer *framebuffer, char c, const uint32_t x, const uint32_t y);
