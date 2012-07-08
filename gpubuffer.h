#ifndef GPUBUFFER_H
#define GPUBUFFER_H

#include <QtOpenGL>

struct vector4 {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
};

struct textureuv {
    GLfloat u;
    GLfloat v;
};

struct vertexvtn {
    vector4 position;
    textureuv uv;
    vector4 normal;
};

struct trianglevtn {
    vertexvtn v1;
    vertexvtn v2;
    vertexvtn v3;
};

struct objectvtn {
    trianglevtn* triangles;
    int numTriangles;
};

struct objectset {
    objectvtn* parts;
    int numParts;
};

struct pixelRGBA {
    char r;
    char g;
    char b;
    char a;
};

struct bitmapRaw {
    int width;
    int height;

    pixelRGBA* raw;
};

#endif // GPUBUFFER_H
