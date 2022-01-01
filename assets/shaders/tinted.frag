#version 330 core

in Varyings {
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;

struct Material {//vec3 not sampler as it's constant for whole object
    vec3 albedo;
    vec3 specular;
    vec3 roughness;
    vec3 emission;
};//no ambient occlusion as it's tint sold color

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

struct Light {
    int type;
    vec3 color;
    vec3 attenuation;
    vec2 cone_angles;
    vec3 position;//from the owing entity transform
    vec3 direction;//from the owing entity transform
};

struct SkyLight {
    vec3 sky, horizon, ground;
};
//Deffuse
float lambert(vec3 n, vec3 l){//depends on the angle between normal and light direction on the object
    return max(0, dot(n, l));
}
//specular
float phong(vec3 n, vec3 l, vec3 v, float shininess){
    vec3 r = reflect(-l, n);
    return pow(max(0, dot(v, r)), shininess);
}

vec3 compute_sky_light(vec3 normal, SkyLight sky_light) {
    float y = normal.y;
    float sky_factor = max(0, y);
    float ground_factor = max(0, -y);
    sky_factor *= sky_factor;
    ground_factor *= ground_factor;
    float horizon_factor = 1 - sky_factor - ground_factor;
    return sky_light.sky * sky_factor + sky_light.horizon * horizon_factor + sky_light.ground * ground_factor;
}

#define MAX_LIGHTS 8

uniform Material material;
uniform int light_count;
uniform Light lights[MAX_LIGHTS];
uniform SkyLight sky_light;

void main(){

    vec3 normal = normalize(fs_in.normal);
    vec3 view = normalize(fs_in.view);

    vec3 material_albedo = material.albedo;
    vec3 material_specular = material.specular;
    float roughness = material.roughness.r;//take only r channel as it is grayscale
    float shininess = 2.0f/pow(clamp(roughness, 0.001f, 0.999f), 4.0f) - 2.0f;
    vec3 material_emission = material.emission;
    
    frag_color = vec4(material_emission + material_albedo * compute_sky_light(normal, sky_light), 1);

    for(int i = 0; i < min(MAX_LIGHTS, light_count); i++){
        Light light = lights[i];

        vec3 light_vec = - light.direction;//as lambart assumelight direction from point to light
        if(light.type != DIRECTIONAL){//as DIRECTIONAL has no position
            light_vec = normalize(light.position - fs_in.world);
        }

        vec3 diffuse = material_albedo * light.color * lambert(normal, light_vec);
        vec3 specular = material_specular * light.color * phong(normal, light_vec, view, shininess);
        //vec3 ambient = material_ao * material_albedo * 0.1;

        float attenuation = 1;
        if(light.type != DIRECTIONAL){
            float d = distance(light.position, fs_in.world);
            attenuation *= 1.0/dot(light.attenuation, vec3(1, d, d*d));
            if(light.type == SPOT){
                float angle = acos(-dot(light.direction, light_vec));//outer cone - inner cone 
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle);
            }
        }

        frag_color.rgb += (diffuse + specular) * attenuation; // + ambient;
        frag_color *= tint;
    }
}