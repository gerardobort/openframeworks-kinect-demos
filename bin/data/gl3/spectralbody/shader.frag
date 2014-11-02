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

out vec4 outputColor;

#define M_PI 3.1415926535897932384626433832795
 
void main()
{
    vec4 color = texture(u_sampler2d, v_texCoord0);
    float alpha = clamp(0.9 + 0.5*cos(M_PI*u_time*1.8), 0.0, 1.0);
    outputColor = vec4(color.rgb, alpha);
}
