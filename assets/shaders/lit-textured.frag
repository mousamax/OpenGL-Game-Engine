#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;
//sampler for each type
struct Material {
    sampler2D albedo;
    sampler2D specular;
    sampler2D roughness;
    sampler2D ambient_occlusion;
    sampler2D emission;
};

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

struct Light {
    int type;
    vec3 position; //from the owing entity transform
    vec3 direction;//from the owing entity transform
    vec3 color;
    vec3 attenuation;
    vec2 cone_angles;
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
    return pow(max(0, dot(v, r)), shininess);//depends on the angle between reflect and view direction
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

    vec3 normal = normalize(fs_in.normal);//normal on point
    vec3 view = normalize(fs_in.view);//from camera

    //we read from all texures simultanously as we created different texure units
    vec3 material_albedo = texture(material.albedo, fs_in.tex_coord).rgb;
    vec3 material_specular = texture(material.specular, fs_in.tex_coord).rgb;
    float roughness = texture(material.roughness, fs_in.tex_coord).r;//take only r channel as it is grayscale
    float shininess = 2.0f/pow(clamp(roughness, 0.001f, 0.999f), 4.0f) - 2.0f;
    vec3 material_emission = texture(material.emission, fs_in.tex_coord).rgb;
    float material_ao = texture(material.ambient_occlusion, fs_in.tex_coord).r;
    
    //material constants before applying lights
    frag_color = vec4(material_emission + material_albedo * material_ao * compute_sky_light(normal, sky_light), 1);

    for(int i = 0; i < min(MAX_LIGHTS, light_count); i++){
        Light light = lights[i];

        vec3 light_vec = - light.direction;//as lambart assume light direction from point to light
        if(light.type != DIRECTIONAL){//as DIRECTIONAL has no position and has constant direction
            light_vec = normalize(light.position - fs_in.world);
        }

        vec3 diffuse = material_albedo * light.color * lambert(normal, light_vec);
        vec3 specular = material_specular * light.color * phong(normal, light_vec, view, shininess);
        //vec3 ambient = material_ao * material_albedo * 0.1;

        float attenuation = 1;
        if(light.type != DIRECTIONAL){//as DIRECTIONAL has constant intensity anywhere in the scene
            float d = distance(light.position, fs_in.world);
            attenuation *= 1.0/dot(light.attenuation, vec3(1, d, d*d));
            if(light.type == SPOT){//light.direction in the middle of the inner cone
                float angle = acos(-dot(light.direction, light_vec));//outer cone - inner cone
                //inner cone => full intensity 
                //between inner and outer cone => gradually decrease intensity 
                //outer cone => 0 intensity 
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle);
            }
        }

        frag_color.rgb += (diffuse + specular) * attenuation; // + ambient;
    }
    //TODO: Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color and with the texture color
    // ---- frag_color = texture(tex,fs_in.tex_coord) * fs_in.color * tint;
}