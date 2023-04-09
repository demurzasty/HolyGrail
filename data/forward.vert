#version 460 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec2 iUV;
layout (location = 2) in vec3 iNormal;

struct GlobalData {
    mat4 projection;
    mat4 view;
};

layout (std140, binding = 0) uniform GlobalUniform {
    GlobalData data;
} uGlobal;

layout (location = 0) out vec2 vUV;
layout (location = 1) out vec3 vNormal;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    vUV = iUV / 16.0 + vec2(1.0 / 16.0, 0.0) * 2;
    vNormal = iNormal;
    gl_Position = uGlobal.data.projection * uGlobal.data.view * vec4(iPosition, 1.0);
}
