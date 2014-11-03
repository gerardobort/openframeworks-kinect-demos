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
    vec4 c31 = texture(u_sampler2d, v_texCoord0 + vec2( - 2.0, 0.0));
    vec4 c32 = texture(u_sampler2d, v_texCoord0 + vec2( - 1.0, 0.0));
    vec4 c33 = texture(u_sampler2d, v_texCoord0);
    vec3 rgbColor = c33.rgb - 0.6*c32.rgb - 0.4*c31.rgb;
    float len = clamp(length(rgbColor), 0.0, 1.0);
    //float alpha = clamp(0.5 + 0.5*sin(M_PI*u_time*1.8), 0.0, 1.0);
    float alpha = clamp(0.2 + u_musicSpectrumBand3, 0.0, 1.0);
    outputColor = vec4(6.0*u_musicSpectrumBand0, 6.0*u_musicSpectrumBand1, 6.0*u_musicSpectrumBand2, 5.5 * len * alpha);
}
