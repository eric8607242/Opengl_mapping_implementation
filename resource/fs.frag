#version 330
layout(location=0) out vec4 color;

uniform sampler2D text;
uniform sampler2D shadowMap;
uniform float specular_s;
uniform float ambient_s;
uniform vec3 object_color;
uniform vec3 light_pos;
uniform vec3 eye_pos;
uniform mat4 vp;

float ShadowCalculation(vec4 fragposlightspace)
{
    float bias = 0.005;
    vec3 projcoords = fragposlightspace.xyz / fragposlightspace.w;
    projcoords = projcoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projcoords.xy).r;
    float currentDepth = projcoords.z;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

in vec3 g_color;
in vec2 g_uv;
in vec3 g_normal;
in vec3 g_fragpos;
in vec4 g_fragposlightspace;

void main()
{
    vec3 norm = normalize(g_normal);
    vec3 light_color = vec3(1.0f, 1.0f, 1.0f);    

    vec3 ambient = ambient_s * light_color;

    vec3 light_dir = normalize(light_pos - g_fragpos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 view_dir = normalize(eye_pos-g_fragpos);

    vec3 reflect_dir = reflect(light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_s * spec * light_color;

    float shadow = ShadowCalculation(g_fragposlightspace);

    vec3 result = (ambient+ (1.0 - shadow) * (diffuse+specular)) * object_color; 
    color = vec4(result, 1.0);
}
