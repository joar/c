varying vec2 f_texcoord;
uniform sampler2D mytexture;

void main(void) {
    vec2 flipped_texcoord = vec2(f_texcoord.x, 1.0 - f_texcoord.y);
    gl_FragColor = texture2D(mytexture, flipped_texcoord);
    // gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, 1.0);
    // gl_FragColor[3] = floor(mod(gl_FragCoord.y, 2.0));
}
