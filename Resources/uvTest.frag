varying vec4 v_fragmentColor;
varying vec2 v_coord;

uniform vec4 u_color;

uniform vec2 v_animLight;
void main()
{
    gl_FragColor = v_fragmentColor * texture2D(CC_Texture0,v_coord) + texture2D(CC_Texture1,v_coord + v_animLight.xy);

}