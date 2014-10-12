#version 120
 
uniform mat4 modelViewProjectionMatrix;
attribute vec4 position;
attribute vec4 color;
varying vec4 pcolor;

uniform int farThreshold;
uniform float time;
 
void main(){
    vec4 pos = position;
    pos.z *= 1.0 + 0.2*(0.5 + 0.5*sin(time));

    vec4 col = color;
    col.w = clamp(1.0 - 0.2*(position.z/farThreshold)*(position.z/farThreshold), 0.0, 1.0);

    gl_Position = modelViewProjectionMatrix * pos;
    pcolor = col;
}
