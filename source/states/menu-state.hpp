#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/game-manager.hpp>
#include <asset-loader.hpp>

#include <stdint.h>

// This state for the menu.
class MenuState : public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::GameMananger gameManager;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene0"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets")) {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world")) {
            world.deserialize(config["world"]);
        }
        gameManager.enter(getApp(), gameManager.menu_state);
    }

    void onDraw(double deltaTime) override {
        
        // And finally we use the renderer system to draw the scene
        auto size = getApp()->getFrameBufferSize();
        gameManager.update(deltaTime);
        renderer.render(&world, glm::ivec2(0, 0), size);
    }

    void onDestroy() override {
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }

    void onImmediateGui() {

        //ImGui::ShowDemoWindow();
        ImGui::Begin("Game Menu", NULL);

        ImGui::Text("Turbo Snail!");

        if (ImGui::Button("Start Game!")) {
            gameManager.startGame();
        }

        ImGui::End();
    }
};