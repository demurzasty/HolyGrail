#version 460 core
#extension GL_ARB_bindless_texture : enable

layout (location = 0) in vec2 vUV;
layout (location = 1) in vec3 vNormal;

layout (std430, binding = 4) buffer uSampler {
    sampler2D uSamplers[];
};

layout (location = 0) out vec4 oColor;

void main() {
    vec3 lightDir = normalize(vec3(-1.0));
    float nDotL = dot(-lightDir, vNormal);
    oColor = vec4(texture(uSamplers[0], vUV).rgb, 1.0);
    oColor = vec4(vNormal * 0.5 + 0.5, 1.0);
}
