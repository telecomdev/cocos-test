varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform vec2 u_uvScale;
uniform vec2 u_vVelocity;
uniform int u_iRepeat;
void main()
{
	vec2 uv_Coord = v_texCoord * u_uvScale;
	vec4 color0 = texture2D(CC_Texture0, v_texCoord);
	vec4 color1 = texture2D(CC_Texture1, uv_Coord + u_vVelocity);
	if(u_iRepeat == 0 && (uv_Coord.x > 1.0 || uv_Coord.y > 1.0))
	{
		gl_FragColor = vec4(0.0,0.0,0.0,0.0);
	}	
	else
	{
		gl_FragColor = color0.a * color1;
	}
}