#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"
#include <iostream>

namespace our
{

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const
    {
        // DONE#TODO: Write this function
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        if (data.contains("pipelineState"))
        {
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
    void TintedMaterial::setup() const
    {
        // DONE#TODO: Write this function
        Material::setup();
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
    void TexturedMaterial::setup() const
    {
        // DONE#TODO: Write this function
        TintedMaterial::setup();
        shader->set("alphaThreshold", alphaThreshold);
        // First, we set the active texture unit. 
        // Here, we pick unit 0 which is actually the active unit by default but we still wrote this line for demonstration.
        glActiveTexture(GL_TEXTURE0);
        // When we bind the texture, we also bind it to the active unit. So this texture is now bound to unit 0.
        texture->bind();
        // To tell OpenGL which sampler object we will use for this unit,
        // we bind the sampler to unit 0 (which is specified by the 1st parameter of the following function).
        sampler->bind(0);
        // send the unit number to the uniform variable "tex"
        shader->set("tex", 0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json &data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }
    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind each texture and sampler to a texture unit and send the unit number to 
    // corresponding uniform variable e.g"Material.albedo"
    void LitMaterial::setup() const
    {
        Material::setup();
        shader->set("alphaThreshold", alphaThreshold);
        // First, we set the active texture unit. 
        // Here, we pick unit 0 which is actually the active unit by default but we still wrote this line for demonstration.
        glActiveTexture(GL_TEXTURE0);
        // When we bind the texture, we also bind it to the active unit. So this texture is now bound to unit 0.
        albedo_tex->bind();
        // To tell OpenGL which sampler object we will use for this unit,
        // we bind the sampler to unit 0 (which is specified by the 1st parameter of the following function).
        sampler->bind(0);
        // send the unit number to the uniform variable "material.albedo"
        shader->set("material.albedo", 0);
        // ---------- specular tex ------------ //
        // Here, we pick unit 1.
        glActiveTexture(GL_TEXTURE1);
        // When we bind the texture, we also bind it to the active unit. So this texture is now bound to unit 1.
        specular_tex->bind();
        // we bind the sampler to unit 1 (which is specified by the 1st parameter of the following function).
        sampler->bind(1);
        // send the unit number to the uniform variable "tex"
        shader->set("material.specular", 1);
        // ---------- roughness tex -----------  //
        glActiveTexture(GL_TEXTURE2);
        roughness_tex->bind();
        sampler->bind(2);
        shader->set("material.roughness",2);
        // ---------- ambient_occlusion tex -----------  //
        glActiveTexture(GL_TEXTURE3);
        ao_tex->bind();
        sampler->bind(3);
        shader->set("material.ambient_occlusion", 3);
        // ---------- emission tex -----------  //
        glActiveTexture(GL_TEXTURE4);
        emission_tex->bind();
        sampler->bind(4);
        shader->set("material.emission", 4);
    }

    // This function read the material data from a json object
    void LitMaterial::deserialize(const nlohmann::json& data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        albedo_tex = AssetLoader<Texture2D>::get(data.value("albedo-texture", ""));
        specular_tex = AssetLoader<Texture2D>::get(data.value("specular-texture", ""));
        roughness_tex = AssetLoader<Texture2D>::get(data.value("roughness-texture", ""));
        ao_tex = AssetLoader<Texture2D>::get(data.value("ambient-occlusion-texture", ""));
        emission_tex = AssetLoader<Texture2D>::get(data.value("emission-texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

}
