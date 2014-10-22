#version 120

uniform int u_worldWidth;
uniform int u_worldHeight;
uniform int u_farThreshold;
uniform float u_time;
uniform sampler2DRect u_sampler2d;
uniform int u_index;

varying vec4 v_color;
varying vec4 v_position;
varying vec2 v_texCoord0;
 
void main()
{
    vec4 c31 = texture2DRect(u_sampler2d, v_texCoord0 + vec2( - 2.0, 0.0));
    vec4 c32 = texture2DRect(u_sampler2d, v_texCoord0 + vec2( - 1.0, 0.0));
    vec4 c33 = texture2DRect(u_sampler2d, v_texCoord0);
    if (1 == u_index) {
        gl_FragColor = vec4(0.0, 0.0, c33.b + c33.g, 0.7);
    } else if (2 == u_index) {
        vec3 rgbColor = c33.rgb - 0.6*c32.rgb - 0.4*c31.rgb;
        float len = clamp(length(rgbColor), 0.0, 1.0);
        gl_FragColor = vec4(0, 1.0, 0, 4.5*len);
    }
}
