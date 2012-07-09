varying vec2 vTextureuv;
uniform sampler2D uTexture;

void main(void) {
    vec3 color = texture2D(uTexture, vTextureuv).rgb;

    gl_FragColor = vec4(color, 1.0);
}
