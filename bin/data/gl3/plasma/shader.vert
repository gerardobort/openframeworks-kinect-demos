#version 150
 
in vec4 a_position;
in vec4 a_color;
in vec2 a_texCoord0;

uniform mat4 modelViewProjectionMatrix;
uniform int u_worldWidth;
uniform int u_worldHeight;
uniform int u_mapWidth;
uniform int u_mapHeight;
uniform int u_farThreshold;
uniform int u_nearThreshold;
uniform float u_time;

uniform float u_musicSpectrumBand0;
uniform float u_musicSpectrumBand1;
uniform float u_musicSpectrumBand2;
uniform float u_musicSpectrumBand3;
 
out vec4 v_color;
out vec4 v_position;
out vec2 v_texCoord0;

void main(){
    vec4 position = vec4(a_position.xy, a_position.z*(1.0 + 0.1*(0.5 + 0.5*sin(u_time))) + 10.0, 1.0);
    v_color = vec4(a_color.rgb, clamp(1.0 - 0.2*(a_position.z/u_farThreshold)*(a_position.z/u_farThreshold), 0.0, 1.0));
    // gl_Position = modelViewProjectionMatrix * position;
    gl_Position = modelViewProjectionMatrix * (position + vec4(u_musicSpectrumBand0, u_musicSpectrumBand1, - 20.0 * u_musicSpectrumBand2, 0));
    v_position = gl_Position;
    v_texCoord0 = (a_position.xy * 1.15*(u_farThreshold/a_position.z) + vec2(u_mapWidth, u_mapHeight))*0.5;
}
