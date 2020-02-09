#pragma once

#include <stdint.h>

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
    uint8_t glyphs;
} __attribute__((packed)) psf2_t;

typedef struct {
	uint32_t width;
	uint32_t height;
}psf2_size_t;

extern psf2_t *kernel_default_font;

void kernel_init_text();
void kernel_render_text(int x, int y, const char *text, psf2_t *font);
psf2_size_t kernel_measure_text(const char *text, psf2_t *font);

