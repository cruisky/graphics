R"(
#version 430 core
in vec3 tx_Pos;
in vec3 tx_Normal;
out vec4 position;
out vec3 normal;
uniform mat4 tx_M, tx_V, tx_P;
uniform mat3 tx_M_3x3InvTr;

void main(){
	position = tx_M * vec4(tx_Pos, 1.0);
	normal = normalize(tx_M_3x3InvTr * tx_Normal);
	gl_Position = tx_P * tx_V * position;
}
)"
