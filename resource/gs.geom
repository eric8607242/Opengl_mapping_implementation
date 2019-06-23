#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 v_uv[];
in vec3 v_color[];
in vec3 v_normal[];
in vec3 v_fragpos[];
in vec4 v_fragposlightspace[];
in vec4 v_fragposlamplightspace[];
in mat3 TBN[];

out vec2 g_uv;
out vec3 g_color;
out vec3 g_normal;
out vec3 g_fragpos;
out vec4 g_fragposlightspace;
out vec4 g_fragposlamplightspace;
out mat3 g_TBN;

void main() {
    gl_Position = gl_in[0].gl_Position;
    g_uv = v_uv[0];
    g_color = v_color[0];
    g_normal = v_normal[0];
    g_fragpos = v_fragpos[0];
    g_fragposlightspace = v_fragposlightspace[0];
    g_fragposlamplightspace = v_fragposlamplightspace[0];
    g_TBN = TBN[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    g_uv = v_uv[1];
    g_color = v_color[1];
    g_normal = v_normal[1];
    g_fragpos = v_fragpos[1];
    g_fragposlightspace = v_fragposlightspace[1];
    g_fragposlamplightspace = v_fragposlamplightspace[1];
    g_TBN = TBN[0];
    EmitVertex();
    
    gl_Position = gl_in[2].gl_Position;
    g_uv = v_uv[2];
    g_color = v_color[2];
    g_normal = v_normal[2];
    g_fragpos = v_fragpos[2];
    g_fragposlightspace = v_fragposlightspace[2];
    g_fragposlamplightspace = v_fragposlamplightspace[2];
    g_TBN = TBN[0];
    EmitVertex();

    EndPrimitive();
}  
