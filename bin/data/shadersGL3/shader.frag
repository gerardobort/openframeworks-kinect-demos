#version 120
 
//attribute vec4 outputColor;
//out vec4 outputColor; // version 150

uniform int u_worldWidth;
uniform int u_worldHeight;
uniform int u_farThreshold;
uniform float u_time;
uniform sampler2DRect u_sampler2d;

varying vec4 v_color;
varying vec2 v_texCoord0;
 
void main()
{
    float r = v_color.x * gl_FragCoord.x / u_worldWidth;
    float g = v_color.y * gl_FragCoord.y / u_worldHeight;
    float b = v_color.z * (0.5 + 0.5 *cos(u_time));
    float a = v_color.w;

    vec4 mapColor = texture2DRect(u_sampler2d, v_texCoord0);
    gl_FragColor = vec4(mapColor.rgb, a);
}
