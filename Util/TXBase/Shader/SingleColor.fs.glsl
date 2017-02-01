R"(
#version 430 core
in vec4 position;
in vec3 normal;
uniform vec4 color;
void main(){
	gl_FragColor = color;
}
)"
