#version 120
 
attribute vec4 a_position;
attribute vec4 a_color;

uniform mat4 modelViewProjectionMatrix;
uniform int u_farThreshold;
uniform float u_time;
 
varying vec4 v_color;

void main(){
    vec4 pos = a_position;
    pos.z *= 1.0 + 0.2*(0.5 + 0.5*sin(u_time));

    vec4 col = a_color;
    col.w = clamp(1.0 - 0.2*(a_position.z/u_farThreshold)*(a_position.z/u_farThreshold), 0.0, 1.0);

    gl_Position = modelViewProjectionMatrix * pos;
    v_color = col;
}
