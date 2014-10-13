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

    vec4 c31 = texture2DRect(u_sampler2d, v_texCoord0 + vec2( - 2.0, 0.0));
    vec4 c32 = texture2DRect(u_sampler2d, v_texCoord0 + vec2( - 1.0, 0.0));
    vec4 c33 = texture2DRect(u_sampler2d, v_texCoord0);
    vec3 rgbColor = c33.rgb - 0.6*c32.rgb - 0.4*c31.rgb;
    float len = clamp(length(rgbColor), 0.0, 1.0);
    gl_FragColor = vec4(1.0-len, 1.0-len, 1.0-len, 1.0);
}
