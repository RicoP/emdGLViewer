#ifndef GPUBUFFER_H
#define GPUBUFFER_H

struct vector4 {
    float x;
    float y;
    float z;
    float w;
};

struct textureuv {
    float u;
    float v;
};

struct tuplevt {
    vector4 vertex;
    textureuv textcoord;
};

struct trianglevt {
    tuplevt p1;
    tuplevt p2;
    tuplevt p3;
};

#endif // GPUBUFFER_H
