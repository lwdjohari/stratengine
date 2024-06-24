$input a_position, a_normal, a_texcoord0
#include "common.sh"

uniform mat4 u_modelViewProj;
uniform mat4 u_model;
uniform vec4 u_lightPos;

void main() {
    vec4 pos = mul(u_model, vec4(a_position, 1.0));
    gl_Position = mul(u_modelViewProj, pos);

    v_texcoord0 = a_texcoord0;
    v_normal = mul(u_model, vec4(a_normal, 0.0)).xyz;
    vec3 lightDir = normalize(u_lightPos.xyz - pos.xyz);
    v_lightDir = lightDir;
}
