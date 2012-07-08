#include "emdobjectset.h"
#include "emdtypes.h"
#include <assert.h>
#include <vector>

#define snormalize(x) ((x)/((float)0x500))
#define cnormalize(x) ((x)/((float)0xFF))

EmdObjectSet::EmdObjectSet()
    : parts(), numParts(0) {
}

EmdObjectSet::EmdObjectSet(QByteArray buffer, bool* success) {


    int blob = (int)(buffer.data());
    tmd_header_t* header = (tmd_header_t*) blob;
    blob += sizeof(tmd_header_t);

    //int vi  = 1;
    //int vti = 1;
    //int vni = 1;

    //fprintf(stderr, "id %x, unknown %x, count %x\n", header->id, header->unknown, header->num_objects);
    if(header->id != 0x41) {
        *success = false;
        return;
    }

    this->numParts = header->num_objects;
    this->parts    = (objectvtn*) malloc(sizeof(objectvtn) * header->num_objects);

    tmd_object_t* objects = (tmd_object_t*) blob;
    int firstObject = blob;

    for(int iobject = 0; iobject != header->num_objects; iobject++) {
        std::vector<trianglevtn> list;
        //fprintf(stderr, "  %i\n", iobject);
        tmd_object_t obj = objects[iobject];

        int primoffset = firstObject + obj.primitive_offset;
        int vertoffset = firstObject + obj.vertex_offset;
        int normoffset = firstObject + obj.normal_offset;

        tmd_vertex_t*      vertice    = (tmd_vertex_t*)(vertoffset);
        tmd_vertex_t*      normals    = (tmd_vertex_t*)(normoffset);
        tmd_txprimitive_t* primitives = (tmd_txprimitive_t*)primoffset;

        //fprintf(file, "o object_%i\n", iobject);

        for(int ipri = 0; ipri != obj.primitive_count; ipri++) {
            trianglevtn triangle;
            vertexvtn* vectors = (vertexvtn*)(&triangle); // trianglevtn == vertexvtn[3]

            tmd_txprimitive_t primitive = primitives[ipri];

            tmd_prim_header_t head = primitive.header;
            tmd_txtriangle_t  tri  = primitive.triangle;

            assert(head.type == 0x34);

            int vertexindice[] = { tri.v0, tri.v1, tri.v2 };
            for(int i = 0; i != 3; i++) {
                tmd_vertex_t v = vertice[ vertexindice[i] ];
                assert(v.zero == 0);
                vectors[i].position.x = snormalize( v.x );
                vectors[i].position.y = snormalize( v.y );
                vectors[i].position.z = snormalize( v.z );
                vectors[i].position.w = 1.0f;
            }

            int normalindice[] = { tri.n0, tri.n1, tri.n2 };
            for(int i = 0; i != 3; i++) {
                tmd_vertex_t n = normals[ normalindice[i] ];
                assert(n.zero == 0);
                //fprintf(file, FVN, snormalize(n.x), snormalize(n.y), snormalize(n.z));
                vectors[i].normal.x = snormalize( n.x );
                vectors[i].normal.y = snormalize( n.y );
                vectors[i].normal.z = snormalize( n.z );
                vectors[i].normal.w = 0.0f;
            }

            float Rp = (tri.TSB & 0x1F) * 0.5f;
            //fprintf(stderr, "TSB: %i\n", tri.TSB);
            tmd_textureuv_t textures[] = { tri.tex1, tri.tex2, tri.tex3 };
            for(int i = 0; i != 3; i++) {
                tmd_textureuv_t tex = textures[i];
                //fprintf(file, FVT, cnormalize(tex.u) + Rp, 1 - cnormalize(tex.v));
                vectors[i].uv.u = cnormalize(tex.u) + Rp;
                vectors[i].uv.v = 1.0f - cnormalize(tex.v);
            }

            //fprintf(file, "f");
            for(int i = 0; i != 3; i++) {
                //fprintf(file, " %i/%i/%i", vi++, vti++, vni++);
            }
            //fprintf(file, "\n");

            list.push_back(triangle);
        }

        objectvtn* object = this->parts + iobject;
        object->numTriangles = list.size();
        object->triangles = (trianglevtn*) malloc(list.size() * sizeof(trianglevtn));

        int i = 0;
        for(std::vector<trianglevtn>::iterator ite = list.begin(); ite != list.end(); ite++) {
            object->triangles[i++] = *ite;
        }

        list.clear();
    }

    *success = true;
}

EmdObjectSet::EmdObjectSet(objectvtn *parts, int numParts) {
    this->parts = parts;
    this->numParts = numParts;
}

EmdObjectSet::~EmdObjectSet() {
    if(parts) {
        if(parts->triangles) {
            free(parts->triangles);
        }

        free(parts);
    }
}

#undef snormalize
#undef cnormalize
