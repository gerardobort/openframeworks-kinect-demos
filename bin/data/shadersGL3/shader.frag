#version 120
 
//attribute vec4 outputColor;
//out vec4 outputColor; // version 150

uniform int worldWidth;
uniform int worldHeight;
uniform int nearThreshold;
uniform float time;
varying vec4 pcolor;
 
void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.
 
    //float windowWidth = 1024.0;
    //float windowHeight = 768.0;
 
    float r = pcolor.x * gl_FragCoord.x / worldWidth;
    float g = pcolor.y * gl_FragCoord.y / worldHeight;
    float b = pcolor.z * (0.5 + 0.5 *cos(time));
    float a = pcolor.w;
    gl_FragColor = vec4(r, g, b, a);
}
