#pragma once
#include "mathstructs.h"
#include "Framebuffer.hpp"
#include <text/psf2.hpp>
#include <stdint.h>

struct FramebufferArea
{
    int x;
    int y;
    int width;
    int height;
    int targetX;
    int targetY;
    int targetWidth;
    int targetHeight;
    uint32_t *buffer;

    inline bool IsValid() const
    {
        return x >= 0 && y >= 0 && width > 0 && height > 0 &&
            targetX >= 0 && targetY >= 0 && targetWidth > 0 && targetHeight > 0 &&
            buffer != NULL;
    }
};

union Color32
{
    struct
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } pixels;

    uint32_t value;
};

class FramebufferRenderer
{
private:

    Framebuffer* targetFramebuffer;
    psf2_font_t* font;

public:

    Point cursorPosition;
    uint32_t colour;

    FramebufferRenderer(Framebuffer* targetFramebuffer, psf2_font_t* font);
    void Print(const char* str);
    void PutChar(char chr, unsigned int xOff, unsigned int yOff);
    void PutChar(char chr);
    void Clear(uint32_t colour);
    void Newline();
    void ClearChar();
    FramebufferArea FrameBufferAreaAt(int x, int y, int width, int height);

    inline int Width() const
    {
        if(targetFramebuffer != NULL)
        {
            return targetFramebuffer->width;
        }

        return 0;
    }

    inline int Height() const
    {
        if(targetFramebuffer != NULL)
        {
            return targetFramebuffer->height;
        }

        return 0;
    }

    inline int FontWidth() const
    {
        if(font != NULL)
        {
            return font->header->width;
        }

        return 0;
    }

    inline int FontHeight() const
    {
        if(font != NULL)
        {
            return font->header->height;
        }

        return 0;
    }
};

extern FramebufferRenderer* globalRenderer;