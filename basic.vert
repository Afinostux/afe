#version 130

attribute vec3 v_color;
varying vec4 vcolor;
 
void main(void) {
	vcolor = vec4(v_color.rgb,1);
	gl_Position = ftransform();
}
