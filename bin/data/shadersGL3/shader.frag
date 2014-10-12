#version 120
 
//attribute vec4 outputColor;
//out vec4 outputColor; // version 150

uniform int u_worldWidth;
uniform int u_worldHeight;
uniform int u_farThreshold;
uniform float u_time;

varying vec4 v_color;
 
void main()
{
    float r = v_color.x * gl_FragCoord.x / u_worldWidth;
    float g = v_color.y * gl_FragCoord.y / u_worldHeight;
    float b = v_color.z * (0.5 + 0.5 *cos(u_time));
    float a = v_color.w;

    gl_FragColor = vec4(r, g, b, a);
}
