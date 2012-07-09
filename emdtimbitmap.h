#ifndef EMDTIMBITMAP_H
#define EMDTIMBITMAP_H

#include "gpubuffer.h"
#include <QByteArray>

class EmdTimBitmap
{
public:
    EmdTimBitmap();
    EmdTimBitmap(QByteArray array, bool* success);
    ~EmdTimBitmap();        

    bool writeBmpFile(QString path);

    int width() const;
    int height() const;
    pixelRGBA* raw() const;

private:
    int m_width;
    int m_height;
    pixelRGBA* m_raw;
};

#endif // EMDTIMBITMAP_H
