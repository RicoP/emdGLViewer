#ifndef TYPES_H
#define TYPES_H

typedef unsigned char u8; 

typedef unsigned short u16; 
typedef signed short s16; 

typedef unsigned int u32; 
typedef signed int s32; 


#pragma pack(1)
typedef struct {
    u32 id; /* Constant = 0x41 */
    u32 unknown;
    s32 num_objects; /* Number of objects in file */
} tmd_header_t;

#pragma pack(1)
typedef struct {
    u32 vertex_offset; /* Offset to vertex array coordinates */
    u32 vertex_count; /* Number of vertices */
    u32 normal_offset; /* Offset to normal array coordinates */
    u32 normal_count; /* Number of normals */
    u32 primitive_offset; /* Offsets to primitives */
    s32 primitive_count; /* Number of primitives */
    u32 unknown;
} tmd_object_t;

#pragma pack(1)
typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 zero;
} tmd_vertex_t;

#pragma pack(1)
typedef struct {
    u8 unknown0;
    u8 length; /* Length of following primitive in 32bit words */
    u8 unknown1;
    u8 type;	/* primitive type */
} tmd_prim_header_t;

#pragma pack(1)
typedef struct {
    u8 r,g,b,unknown; /* R,G,B triangle color */

    u16 n0; /* Normal index */

    u16 v0; /* Vertex index */
    u16 v1;
    u16 v2;
} tmd_flattriangle_t;

#pragma pack(1)
typedef struct {
    u8 r0,g0,b0,unk0; /* R,G,B vertex color */
    u8 r1,g1,b1,unk1;
    u8 r2,g2,b2,unk2;

    u16 n0; /* Normal index */

    u16 v0; /* Vertex index */
    u16 v1;
    u16 v2;
} tmd_gouraudtriangle_t;

#pragma pack(1)
typedef struct {
    u8 r,g,b,unknown; /* R,G,B quad color */

    u16 n0; /* Normal index */

    u16 v0; /* Vertex index */
    u16 v1;
    u16 v2;
    u16 v3;
} tmd_flatquad_t;

#pragma pack(1)
typedef struct {
    u8 r0,g0,b0,unk0; /* R,G,B vertex color */
    u8 r1,g1,b1,unk1;
    u8 r2,g2,b2,unk2;
    u8 r3,g3,b3,unk3;

    u16 n0; /* Normal index */

    u16 v0; /* Vertex index */
    u16 v1;
    u16 v2;
    u16 v3;

    u16 unk5;
} tmd_gouraudquad_t;

#pragma pack(1)
typedef struct {
    u8 r,g,b,unk; /* R,G,B triangle color */

    u16 n0; /* Normal index */
    u16 v0; /* Vertex index */
    u16 n1;
    u16 v1;
    u16 n2;
    u16 v2;
} tmd_flattriangle2_t;

#pragma pack(1)
typedef struct {
    u8 v; /* textures coordinates */
    u8 u;
} __tmd_textureuv_t;


#pragma pack(1)
typedef struct {
    u8 u;
    u8 v; /* textures coordinates */
} tmd_textureuv_t;

#pragma pack(1)
typedef struct {
    u16 unknown1;
    tmd_textureuv_t tex1;
    u16 unknown2;
    tmd_textureuv_t tex2;
    u16 unknown3;
    tmd_textureuv_t tex3;

    u16 v0; /* Vertex index */
    u16 n0; /* Normal index */
    u16 v1;
    u16 n1;
    u16 v2;
    u16 n2;
} __tmd_txtriangle_t;

#pragma pack(1)
typedef struct {
    tmd_textureuv_t tex1;         //- X and Y coordinate of texture for Vertex0
    u16 CBA;            //- position of CLUT for texture in VRAM (see earlier)
    //u8  U1, V1;         //- X and Y coordinate of texture for Vertex1
    tmd_textureuv_t tex2;
    u16 TSB;            //- information about texture in VRAM (see earlier)
    //u8  U2, V2;         //- X and Y coordinate of texture for Vertex2
    tmd_textureuv_t tex3;
    u16 pad;            //- ignored
    u16 n0;        //- index value of normal element
    u16 v0;        //- index value of vertex element
    u16 n1;
    u16 v1;
    u16 n2;
    u16 v2;
} tmd_txtriangle_t;

#pragma pack(1)
typedef struct {
    tmd_prim_header_t header;
    tmd_txtriangle_t triangle;
} tmd_txprimitive_t;

#pragma pack(1)
typedef struct {
    u8 r,g,b,unk; /* R,G,B quad color */

    u16 n0; /* Normal index */
    u16 v0; /* Vertex index */
    u16 n1;
    u16 v1;
    u16 n2;
    u16 v2;
    u16 n3;
    u16 v3;
} tmd_flatquad2_t;



#pragma pack(1)
typedef struct {
    unsigned int	magic;	/* Constant = 0x10 */
    unsigned int	type;
    unsigned int	length;
    unsigned short	unknown1;
    unsigned short	unknown2;
    unsigned short	palette_colors;
    unsigned short	nb_palettes;
} tim_header_t;

#pragma pack(1)
typedef struct {
    unsigned int    size;
    unsigned short  x;
    unsigned short  y;
    unsigned short	width;	/* Width of image in 16-bits words */
    unsigned short	height; /* Height of image in pixels */
} tim_image_head_t;

#pragma pack(1)
typedef struct {
    unsigned short R : 5;
    unsigned short G : 5;
    unsigned short B : 5;
    unsigned char A : 1;
} high_color_t;
//typedef unsigned short high_color_t;


#endif 
