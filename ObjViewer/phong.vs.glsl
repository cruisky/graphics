#version 430 core
in vec3 v_coord;
in vec3 v_normal;
out vec4 position;
out vec3 normal;
uniform mat4 m, v, p;
uniform mat3 m_3x3_inv_transp;

void main(){
	vec4 v_coord4 = vec4(v_coord, 1.0);
	position = m * v_coord4;
	normal = normalize(m_3x3_inv_transp * v_normal);

	gl_Position = p * v * m * v_coord4;
}