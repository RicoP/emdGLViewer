attribute vec4 aVertex;
attribute vec2 aTextureuv;
//attribute vec4 aNormal;

uniform mat4 uModelview;
uniform mat4 uProjection;

varying vec2 vTextureuv;

void main(void) {
    vTextureuv = aTextureuv;
    gl_Position = uProjection * uModelview * aVertex;
}
