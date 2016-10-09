const char* uvSprite_frag = STRINGIFY(

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform float u_vOpacity;
void main()
{
	vec4 color0 = texture2D(CC_Texture0, v_texCoord);
	gl_FragColor = v_fragmentColor * color0 * u_vOpacity;
}
);