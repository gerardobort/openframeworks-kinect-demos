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
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.
 
    //float windowWidth = 1024.0;
    //float windowHeight = 768.0;
 
    float r = v_color.x * gl_FragCoord.x / u_worldWidth;
    float g = v_color.y * gl_FragCoord.y / u_worldHeight;
    float b = v_color.z * (0.5 + 0.5 *cos(u_time));
    float a = v_color.w;
    gl_FragColor = vec4(r, g, b, a);
}
