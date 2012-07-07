#ifdef GL_ES
precision highp float;
#endif

varying vec2 vTextureuv; 
varying vec4 vNormal; 

uniform sampler2D uTexture; 
 
void main(void) {
	vec3 c = texture2D(uTexture, vTextureuv).xyz; 

	gl_FragColor = vec4(c, 1.0); 
}

