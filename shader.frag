varying vec2 vTextureuv;
uniform sampler2D uTexture;

void main(void) {
    vec3 color = texture2D(uTexture, vec2(vTextureuv.x, 1.0 - vTextureuv.y)).rgb;

    gl_FragColor = vec4(color, 1.0);
}
