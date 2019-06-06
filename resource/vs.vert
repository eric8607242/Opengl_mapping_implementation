#version 330
layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texcoord;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 vp;
uniform mat4 lightspacematrix;

out vec2 v_uv;
out vec3 v_color;
out vec3 v_normal;
out vec3 v_fragpos;
out vec4 v_fragposlightspace;

void main()
{

    v_fragpos = vec3(model * vec4(position, 1.0));
    v_normal = transpose(inverse(mat3(model))) * normal;
    v_fragposlightspace = lightspacematrix * vec4(v_fragpos, 1.0);
    v_uv = texcoord;
    gl_Position = vp*model*vec4(position, 1.0);
}
