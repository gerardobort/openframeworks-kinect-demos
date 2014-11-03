#version 150
 
in vec4 v_color;
in vec4 v_position;
in vec2 v_texCoord0;

uniform int u_worldWidth;
uniform int u_worldHeight;
uniform int u_farThreshold;
uniform float u_time;
uniform sampler2DRect u_sampler2d;
uniform int u_index;

uniform float u_musicSpectrumBand0;
uniform float u_musicSpectrumBand1;
uniform float u_musicSpectrumBand2;
uniform float u_musicSpectrumBand3;

out vec4 outputColor;

#define M_PI 3.1415926535897932384626433832795
 
void main()
{
    outputColor = vec4(0, 0, 0, 0.8);
}