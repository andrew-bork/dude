//fragmentColor=gl_Position*0.5+0.5;
attribute vec4 position;
uniform mat4 viewPort;
uniform mat4 cameraRot;
attribute vec4 colorBuffer;
varying vec4 fragmentColor;
void main(){
    gl_Position=viewPort*cameraRot*position;

    fragmentColor=colorBuffer;
}