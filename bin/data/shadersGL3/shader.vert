#version 120
 
uniform mat4 modelViewProjectionMatrix;
attribute vec4 position;
//in vec4 position; // version 150
 
void main(){
    gl_Position = modelViewProjectionMatrix * position;
}
