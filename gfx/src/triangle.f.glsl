varying vec3 f_color;
uniform float fade;

void main(void) {
    gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, fade);
    // gl_FragColor[3] = floor(mod(gl_FragCoord.y, 2.0));
}
