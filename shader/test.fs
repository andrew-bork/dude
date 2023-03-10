#ifdef GL_ES
    precision mediump float;
#endif
varying vec4 fragmentColor;
void main(){
    //gl_FragColor=vec4(0,0,1,1);
    gl_FragColor=fragmentColor;
}