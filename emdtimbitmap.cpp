#include "emdtimbitmap.h"
#include "emdtypes.h"
#include "gpubuffer.h"
#include <QByteArray>

#define TIM_TYPE_4BIT (8)
#define TIM_TYPE_8BIT_PALETTE (9)
#define TIM_TYPE_TRUE_COLOR (2)
#define BIT_5_TO_8(x) ((x) << 3 | (x) >> 2)

EmdTimBitmap::EmdTimBitmap()
    : m_width(0), m_height(0), m_raw(NULL) {
}

int EmdTimBitmap::width() const {
    return m_width;
}

int EmdTimBitmap::height() const {
    return m_height;
}

pixelRGBA* EmdTimBitmap::raw() const {
    return m_raw;
}

EmdTimBitmap::EmdTimBitmap(QByteArray array, bool *success) {
    void* buffer = (void*) array.data();

    int offset = 0;
    tim_header_t header = *((tim_header_t*)((int)buffer + offset));
    offset += sizeof(tim_header_t);

    if(header.magic != 0x10) {
        //fprintf(stderr, "magic number fail.\n");
        *success = false;
        return;
    }

    if(header.type != TIM_TYPE_8BIT_PALETTE) {
        //fprintf(stderr, "Type not supported.\n");
        *success = false;
        return;
    }

    int colorOffset = offset;
    //high_color_t* colors = (high_color_t*) ((int)buffer + offset);
    offset += header.palette_colors * header.nb_palettes * sizeof(high_color_t);

    //image header
    tim_image_head_t image_info = *(tim_image_head_t*) ( (int)buffer + offset);
    offset += sizeof(tim_image_head_t);

    u8* pixel = (u8*)((int)buffer + offset);

    high_color_t* paletteA = (high_color_t*) ((int)buffer + colorOffset);
    high_color_t* paletteB = 0;
    if(header.nb_palettes == 2)
        paletteB = (high_color_t*) ((int)buffer + colorOffset + sizeof(high_color_t) * header.palette_colors);

    //create Image
    pixelRGBA* rgba = (pixelRGBA*) malloc(image_info.size * sizeof(pixelRGBA));
    int rgbIndex = 0;
    int x = 0;
    for(unsigned int i = 0; i != image_info.size; i++) {
        high_color_t color;
        pixelRGBA pixelrgba;

        x = (i % (image_info.width * 2));
        if(x < image_info.width || header.nb_palettes == 1 || !paletteB)
            color = paletteA[ pixel[i] ];
        else
            color = paletteB[ pixel[i] ];

        char c;
        c = (char)(color.R);
        pixelrgba.r = BIT_5_TO_8(c);
        c = (char)(color.G);
        pixelrgba.g = BIT_5_TO_8(c);
        c = (char)(color.B);
        pixelrgba.b = BIT_5_TO_8(c);
        pixelrgba.a = 0xFF;

        rgba[rgbIndex++] = pixelrgba;
    }

    this->m_width  = image_info.width * 2;
    this->m_height = image_info.height;

    qDebug() << "w " << m_width << " h: " << m_height;

    this->m_raw = rgba;

    *success = true;
}

EmdTimBitmap::~EmdTimBitmap() {
    delete m_raw;
}

#undef TIM_TYPE_4BIT
#undef TIM_TYPE_8BIT_PALETTE
#undef TIM_TYPE_TRUE_COLOR
#undef BIT_5_TO_8

