$input v_normal, v_texcoord0, v_lightDir
#include "common.sh"

uniform vec4 u_diffuseColor;
uniform vec4 u_specularColor;
uniform vec4 u_ambientColor;
uniform vec4 u_emissiveColor;
uniform vec4 u_transparency;
uniform float u_shininess;

uniform bool u_enableDiffuse;
uniform bool u_enableSpecular;
uniform bool u_enableNormalMap;
uniform bool u_enableEmissive;
uniform bool u_enableAmbient;
uniform bool u_enableOcclusion;
uniform bool u_enableRoughness;
uniform bool u_enableMetallic;
uniform bool u_enableHeight;
uniform bool u_enableOpacity;
uniform bool u_enableLightmap;

SAMPLER2D(s_diffuse, 0);
SAMPLER2D(s_specular, 1);
SAMPLER2D(s_normal, 2);
SAMPLER2D(s_emissive, 3);
SAMPLER2D(s_occlusion, 4);
SAMPLER2D(s_roughness, 5);
SAMPLER2D(s_metallic, 6);
SAMPLER2D(s_height, 7);
SAMPLER2D(s_opacity, 8);
SAMPLER2D(s_lightmap, 9);

void main() {
    vec3 normal = v_normal;
    if (u_enableNormalMap) {
        normal = texture2D(s_normal, v_texcoord0).xyz * 2.0 - 1.0;
    }

    vec3 lightDir = normalize(v_lightDir);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    vec3 ambient = vec3(0.0);
    vec3 emissive = vec3(0.0);
    vec3 occlusion = vec3(0.0);
    vec3 roughness = vec3(0.0);
    vec3 metallic = vec3(0.0);
    vec3 height = vec3(0.0);
    vec3 opacity = vec3(1.0);
    vec3 lightmap = vec3(0.0);

    if (u_enableDiffuse) {
        diffuse = texture2D(s_diffuse, v_texcoord0).rgb * u_diffuseColor.rgb;
    }

    if (u_enableSpecular) {
        vec3 viewDir = normalize(-gl_FragCoord.xyz);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_shininess);
        specular = spec * texture2D(s_specular, v_texcoord0).rgb * u_specularColor.rgb;
    }

    if (u_enableAmbient) {
        ambient = texture2D(s_occlusion, v_texcoord0).rgb * u_ambientColor.rgb;
    }

    if (u_enableEmissive) {
        emissive = texture2D(s_emissive, v_texcoord0).rgb * u_emissiveColor.rgb;
    }

    if (u_enableOcclusion) {
        occlusion = texture2D(s_occlusion, v_texcoord0).rgb;
    }

    if (u_enableRoughness) {
        roughness = texture2D(s_roughness, v_texcoord0).rgb;
    }

    if (u_enableMetallic) {
        metallic = texture2D(s_metallic, v_texcoord0).rgb;
    }

    if (u_enableHeight) {
        height = texture2D(s_height, v_texcoord0).rgb;
    }

    if (u_enableOpacity) {
        opacity = texture2D(s_opacity, v_texcoord0).rgb;
    }

    if (u_enableLightmap) {
        lightmap = texture2D(s_lightmap, v_texcoord0).rgb;
    }

    vec3 finalColor = ambient + diffuse + specular + emissive + occlusion + roughness + metallic + height + lightmap;
    gl_FragColor = vec4(finalColor, opacity.r) * u_transparency;
}
