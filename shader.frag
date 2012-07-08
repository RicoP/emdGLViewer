varying vec2 vTextureuv;

void main(void) {
        gl_FragColor = vec4(1.0, vTextureuv.x, vTextureuv.y, 1.0);
}
