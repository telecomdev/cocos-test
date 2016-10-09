varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 u_uvScale;
uniform float u_vOpacity;
uniform vec2 u_vVelocity;
uniform int u_vAlphaFilter;
void main()
{
	vec2 uv_Coord = v_texCoord * u_uvScale;
	vec4 color0 = texture2D(CC_Texture0, v_texCoord);
	vec4 color1 = texture2D(CC_Texture1, uv_Coord + u_vVelocity);
	if(u_vAlphaFilter == 0)
	{
		gl_FragColor = v_fragmentColor * (color0 + u_vOpacity * color1);
	}
	else
	{
		gl_FragColor = v_fragmentColor * (color0 + color0.a * u_vOpacity * color1);
	}
}