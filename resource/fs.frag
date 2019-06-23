#version 330
layout(location=0) out vec4 color;

uniform sampler2D text;
uniform sampler2D flashshadowMap;
uniform sampler2D lampshadowMap;
uniform sampler2D text_normal;
uniform sampler2D text_disp;



uniform vec3 object_color; 
uniform vec3 light_pos; 
uniform vec3 lamp_pos; 
uniform vec3 lamp_center; 


uniform int normal_use;
uniform int parallax_use;


uniform vec3 eye_pos;
uniform mat4 vp;
uniform float cutoff;
uniform vec3 light_center;

vec2 SteepPallax(vec2 texCoords, vec3 view_dir);

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
    //bias = 0.0005;
    //float shadow = currentDepth-bias > closestDepth  ? 1.0 : 0.0; 
    return shadow;
}

vec3 LightCalculation(vec3 norm, vec3 light_dir)
{
    vec3 light_color = vec3(1.0f, 1.0f, 1.0f);    
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    return diffuse;
}

vec2 SteepPallax(vec2 texCoords, vec3 view_dir)
{
   
   // number of depth layers
    const float minLayers = 10;
    const float maxLayers = 20;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), view_dir)));
	float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = view_dir.xy / view_dir.z * -0.02; 
    vec2 deltaTexCoords = P / numLayers;
	
	// get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(text_disp, currentTexCoords).r;
    int count = 0;
	while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(text_disp, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;
		count ++;
		if(count>10)
			break;
    }
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(text_disp, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
    //return finalTexCoords;
	//return texCoords;
}

in mat3 g_TBN;
in vec3 g_color;
in vec2 g_uv;
in vec3 g_normal;
in vec3 g_fragpos;
in vec4 g_fragposlightspace;
in vec4 g_fragposlamplightspace;

void main()
{

    vec3 viewDir = normal_use==1 ? g_TBN * normalize(eye_pos-g_fragpos) : normalize(eye_pos-g_fragpos);
    vec2 parallax_texCoords = SteepPallax(g_uv,  viewDir);
    vec2 texCoords = parallax_use==1 ? parallax_texCoords : g_uv;

    vec3 n = texture(text_normal, texCoords).rgb;
	vec3 norm = normal_use==1 ? normalize(n * 2.0 - 1.0) : normalize(g_normal);


    //vec3 norm = normalize(g_normal);
    vec3 light_color = vec3(1.0f, 1.0f, 1.0f);    
    vec3 light_dir = normal_use == 1 ? g_TBN * normalize(light_pos - g_fragpos) : normalize(light_pos - g_fragpos);
    vec3 lamp_dir = normal_use == 1 ? g_TBN * normalize(lamp_pos - g_fragpos) : normalize(lamp_pos - g_fragpos);
    vec3 light_center_dir = normal_use == 1 ? g_TBN * normalize(light_pos - light_center) : normalize(light_pos - light_center);

    vec3 lamp_center_dir = normal_use == 1 ? g_TBN * normalize(lamp_pos - lamp_center) : normalize(lamp_pos - lamp_center);

    vec3 ambient = 0.0 * light_color;

    vec3 flashlight = LightCalculation(norm, light_dir);
    vec3 lamplight = LightCalculation(norm, lamp_dir);

    float theta = dot(light_dir, normalize(light_center_dir));
    float lamp_theta = dot(lamp_dir, lamp_center_dir);

    float flashshadow;
    float lampshadow;

    float bias = max(0.0001 * (1.0 - dot(norm, lamp_dir)), 0.0001);
    lampshadow = ShadowCalculation(g_fragposlamplightspace, bias, lampshadowMap);
    bias = max(0.05 * (1.0 - dot(norm, light_dir)), 0.005);
    flashshadow = ShadowCalculation(g_fragposlightspace, bias, flashshadowMap);

    float shadow;
    //spot light effect
    if(theta > cutoff ){
        flashlight = (1.0 - flashshadow) * flashlight;
    }else{
        flashlight = 0.0 * flashlight;
    }

    if(lamp_theta > cutoff){
        lamplight = (1.0 - lampshadow) * lamplight; 
    }else{
        lamplight = 0.0f * lamplight;
    }
    lamplight = lamplight * 0.5;
    flashlight = flashlight * 0.8;

    vec3 result = (ambient + flashlight + lamplight) * object_color * texture(text, texCoords).rgb;
    color = vec4(result, 1.0);
}



