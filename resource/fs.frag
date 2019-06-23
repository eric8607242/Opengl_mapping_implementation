#version 330
layout(location=0) out vec4 color;

uniform sampler2D text;
uniform sampler2D flashshadowMap;
uniform sampler2D lampshadowMap;
uniform vec3 object_color; 
uniform vec3 light_pos; 
uniform vec3 lamp_pos; 

uniform vec3 eye_pos;
uniform mat4 vp;
uniform float cutoff;
uniform vec3 light_center;

float ShadowCalculation(vec4 fragposlightspace, float bias, sampler2D shadowMap)
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

vec3 LightCalculation(vec3 norm, vec3 light_dir)
{
    vec3 light_color = vec3(1.0f, 1.0f, 1.0f);    
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    return diffuse;
}

in vec3 g_color;
in vec2 g_uv;
in vec3 g_normal;
in vec3 g_fragpos;
in vec4 g_fragposlightspace;
in vec4 g_fragposlamplightspace;

void main()
{
    vec3 norm = normalize(g_normal);
    vec3 light_color = vec3(1.0f, 1.0f, 1.0f);    
    vec3 light_dir = normalize(light_pos - g_fragpos);
    vec3 lamp_dir = normalize(lamp_pos - g_fragpos);
    vec3 light_center_dir = normalize(light_pos - light_center);

    vec3 ambient = 0.2 * light_color;

    vec3 flashlight = LightCalculation(norm, light_dir);
    vec3 lamplight = LightCalculation(norm, lamp_dir);

    vec3 light_diff = flashlight;

    float theta = dot(light_dir, normalize(light_center_dir));
    float lamp_theta = dot(lamp_dir, normalize(lamp_pos - vec3(0.0f, 0.0f, 0.0f)));


    float flashshadow;
    float lampshadow;

    float bias = max(0.05 * (1.0 - dot(norm, light_dir)), 0.005);
    lampshadow = ShadowCalculation(g_fragposlamplightspace, bias, lampshadowMap);

    float shadow;
    //spot light effect
    if(theta > cutoff ){
        flashshadow = ShadowCalculation(g_fragposlightspace, bias, flashshadowMap);
        shadow = flashshadow;
    }else{
        //if(lamp_theta > cutoff){
        //    shadow = lampshadow;
        //}else{
            shadow = 1.0f;
        //}
    }

    vec3 result = (ambient+ (1.0 - shadow) * light_diff) * object_color; 
    color = vec4(result, 1.0) * texture(text, g_uv);
}
