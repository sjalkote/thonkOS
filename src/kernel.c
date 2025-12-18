#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

// Set the base revision to 4, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(4);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimize them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};


// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
    uint8_t *restrict pdest = (uint8_t *restrict)dest;
    const uint8_t *restrict psrc = (const uint8_t *restrict)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

// Halt and catch fire function.
static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}

static const uint8_t font[128][8] = {
    ['H'] = {
        0b01000010,
        0b01000010,
        0b01000010,
        0b01111110,
        0b01000010,
        0b01000010,
        0b01000010,
        0b01000010
    },
    ['M'] = {
        0b01000010,
        0b01100110,
        0b01011010,
        0b01000010,
        0b01000010,
        0b01000010,
        0b01000010,
        0b01000010
    }
};

/**
 * Writes the specified character to the framebuffer at the given (x, y) coordinate position.
 * @param framebuffer The limine framebuffer to write to.
 * @param c The character to write. Must be a standard ASCII between [0, 127].
 * @param x The x-coordinate position to write at.
 * @param y The y-coordinate position to write at.
 */
void put_char(const struct limine_framebuffer *framebuffer, const char c, const uint32_t x, const uint32_t y) {
    const uint8_t *glyph = font[(uint8_t)c];
    volatile uint32_t *fb_ptr = framebuffer->address;

    for (uint32_t row = 0; row < 8; row++) {
        for (uint32_t col = 0; col < 8; col++) {
            if (glyph[row] & (1 << (7 - col))) {
                const uint32_t pixel_index = (y + row) * (framebuffer->pitch / 4) + (x + col);
                fb_ptr[pixel_index] = 0xffffff;
            }
        }
    }
}

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void kmain(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        hcf();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    uint64_t minDimension = framebuffer->height < framebuffer->width ? framebuffer->height : framebuffer->width;
    const int colors[] = {0xff0000, 0xff7f00, 0xffff00, 0x00ff00, 0x0000ff, 0xff00ff};
    for (size_t i = 0; i < minDimension; i++) {
        volatile uint32_t *fb_ptr = framebuffer->address;
        fb_ptr[i * (framebuffer->pitch / 4) + i] = (i / 100 < sizeof(colors) / sizeof(colors[0]) ? colors[i / 100] : 0xffffff);
    }

    // hmm
    put_char(framebuffer, 'H', framebuffer->width / 2 - 8, framebuffer->height / 2);
    put_char(framebuffer, 'M', framebuffer->width / 2, framebuffer->height / 2);
    put_char(framebuffer, 'M', framebuffer->width / 2 + 8, framebuffer->height / 2);

    // We're done, just hang...
    hcf();
}
