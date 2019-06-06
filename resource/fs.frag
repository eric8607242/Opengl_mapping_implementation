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
uniform float cutoff;
uniform vec3 light_center;

float ShadowCalculation(vec4 fragposlightspace, float bias)
{
    vec3 projcoords = fragposlightspace.xyz / fragposlightspace.w;
    projcoords = projcoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projcoords.xy).r;
    float currentDepth = projcoords.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            float pcfDepth = texture(shadowMap, projcoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    if(projcoords.z > 1.0)
    {
        shadow = 0.0;
    }


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
    vec3 light_center_dir = normalize(light_pos - light_center);
    
    //spot light effect
    float theta = dot(light_dir, normalize(light_center_dir));

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 view_dir = normalize(eye_pos-g_fragpos);

    vec3 reflect_dir = -reflect(light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_s * spec * light_color;

    float shadow;

    //spot light effect
    if(theta > cutoff){
        float bias = max(0.05 * (1.0 - dot(norm, light_dir)), 0.005);
        shadow = ShadowCalculation(g_fragposlightspace, bias);
    }else{
        shadow = 1.0f;
    }

        vec3 result = (ambient+ (1.0 - shadow) * (diffuse+specular)) * object_color; 
        color = vec4(result, 1.0);
}
