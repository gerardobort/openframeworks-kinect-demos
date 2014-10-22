#version 120
 
attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoord0;

uniform mat4 modelViewProjectionMatrix;
uniform int u_worldWidth;
uniform int u_worldHeight;
uniform int u_mapWidth;
uniform int u_mapHeight;
uniform int u_farThreshold;
uniform float u_time;
 
varying vec4 v_color;
varying vec4 v_position;

varying vec2 v_texCoord0;
varying vec2 VaryingTexCoord0;

void main(){
    vec4 position = vec4(a_position.xy, a_position.z*(1.0 + 0.1*(0.5 + 0.5*sin(u_time))), 1.0);
    v_color = vec4(a_color.rgb, clamp(1.0 - 0.2*(a_position.z/u_farThreshold)*(a_position.z/u_farThreshold), 0.0, 1.0));
    gl_Position = modelViewProjectionMatrix * position;
    v_position = gl_Position;
    v_texCoord0 = (a_position.xy + vec2(u_mapWidth, u_mapHeight))*0.5;
}
