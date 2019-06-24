#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 model;
uniform mat4 vp;
uniform mat4 lightspacematrix;
uniform mat4 lamplightspacematrix;
uniform vec3 light_pos;


out mat3 TBN;
out vec2 v_uv;
out vec3 v_color;
out vec3 v_normal;
out vec3 v_fragpos;
out vec4 v_fragposlightspace;
out vec4 v_fragposlamplightspace;

out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;


void main()
{


    v_fragpos = vec3(model * vec4(position, 1.0));
    v_normal = transpose(inverse(mat3(model))) * normal;
    v_fragposlightspace = lightspacematrix * vec4(v_fragpos, 1.0);
    v_fragposlamplightspace = lamplightspacematrix * vec4(v_fragpos, 1.0);
    //v_uv = texcoord;

    v_uv = vec2(texcoord.x,texcoord.y);

    vec3 v_tangent = ((transpose(inverse(model))) * vec4(tangent,0.0)).xyz;
	vec3 v_bitangent = ((transpose(inverse(model))) * vec4(bitangent,0.0)).xyz;

    vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
	//T = normalize(T - dot(T, N) * N);
	//B = cross(T, N);
	TBN = transpose(mat3(T, B, N));


    //TangentLightPos = v_lightpos;
    
	//TangentViewPos  = TBN * eye_pos;
    //TangentFragPos  = T;
	//TangentFragPos  = v_position;


    gl_Position = vp*model*vec4(position, 1.0);
}
