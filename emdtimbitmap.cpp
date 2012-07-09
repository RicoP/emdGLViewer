#include "emdtimbitmap.h"
#include "emdtypes.h"
#include "gpubuffer.h"
#include <QByteArray>
#include <QFile>
#include <QDataStream>

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
        c = (char)(color.R) & 0x1F;
        pixelrgba.r = BIT_5_TO_8(c) & 0xFF;        
        c = (char)(color.G) & 0x1F;
        pixelrgba.g = BIT_5_TO_8(c) & 0xFF;        
        c = (char)(color.B) & 0x1F;
        pixelrgba.b = BIT_5_TO_8(c) & 0xFF;        
        pixelrgba.a = 0xFF;

        rgba[rgbIndex++] = pixelrgba;
    }

    this->m_width  = image_info.width * 2;
    this->m_height = image_info.height;
    this->m_raw = rgba;

    qDebug() << "w " << m_width << " h: " << m_height;


    *success = true;
}

EmdTimBitmap::~EmdTimBitmap() {
    delete m_raw;
}



struct BMPHeader
{
    char bfType[2];       /* "BM" */
    int bfSize;           /* Size of file in bytes */
    int bfReserved;       /* set to 0 */
    int bfOffBits;        /* Byte offset to actual bitmap data (= 54) */
    int biSize;           /* Size of BITMAPINFOHEADER, in bytes (= 40) */
    int biWidth;          /* Width of image, in pixels */
    int biHeight;         /* Height of images, in pixels */
    short biPlanes;       /* Number of planes in target device (set to 1) */
    short biBitCount;     /* Bits per pixel (24 in this case) */
    int biCompression;    /* Type of compression (0 if no compression) */
    int biSizeImage;      /* Image size, in bytes (0 if no compression) */
    int biXPelsPerMeter;  /* Resolution in pixels/meter of display device */
    int biYPelsPerMeter;  /* Resolution in pixels/meter of display device */
    int biClrUsed;        /* Number of colors in the color table (if 0, use maximum allowed by biBitCount) */
    int biClrImportant;   /* Number of important colors.  If 0, all colors are important */
};

static int write_bmp(QDataStream& stream, int width, int height, pixelRGBA* rgba) {
    int i, j;
    int bytesPerLine;    
    struct BMPHeader bmph;

    /* The length of each line must be a multiple of 4 bytes */
    bytesPerLine = (3 * (width + 1) / 4) * 4;

    bmph.bfType[0] = 'B';
    bmph.bfType[1] = 'M';
    bmph.bfOffBits = 54;
    bmph.bfSize = bmph.bfOffBits + bytesPerLine * height;
    bmph.bfReserved = 0;
    bmph.biSize = 40;
    bmph.biWidth = width;
    bmph.biHeight = height;
    bmph.biPlanes = 1;
    bmph.biBitCount = 24;
    bmph.biCompression = 0;
    bmph.biSizeImage = bytesPerLine * height;
    bmph.biXPelsPerMeter = 0;
    bmph.biYPelsPerMeter = 0;
    bmph.biClrUsed = 0;
    bmph.biClrImportant = 0;

    stream.writeRawData( (const char*)(&bmph), sizeof(BMPHeader) );

    for (i = height - 1; i >= 0; i--)
    {
        for (j = 0; j < width; j++)
        {
            pixelRGBA pixel = rgba[width * i + j];
            stream << (quint8)pixel.b;
            stream << (quint8)pixel.g;
            stream << (quint8)pixel.r;
        }        
    }

    return 1;
}


bool EmdTimBitmap::writeBmpFile(QString path) {
    QFile file(path);

    if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
        return false;
    }

    QDataStream out(&file);    

    bool stat = write_bmp(out, this->width(), this->height(), this->raw());
    return stat;
}

#undef TIM_TYPE_4BIT
#undef TIM_TYPE_8BIT_PALETTE
#undef TIM_TYPE_TRUE_COLOR
#undef BIT_5_TO_8

