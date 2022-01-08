#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/game-manager.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/collision.hpp>
#include <systems/movement.hpp>
#include <systems/player-movement.hpp>
#include <asset-loader.hpp>

#include <stdint.h>

// This state shows how to use the ECS framework and deserialization.
class Playstate: public our::State {

    our::World world;
    our::GameMananger gameManager;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::CollisionSystem collisionSystem;
    our::PlayerMovementSystem playerMovementSystem;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene1"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        gameManager.enter(getApp(),gameManager.play_state);
        playerMovementSystem.enter(getApp());
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        collisionSystem.update(&world);
        gameManager.update(deltaTime);
        playerMovementSystem.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        world.deleteMarkedEntities();
        auto size = getApp()->getFrameBufferSize();
        renderer.render(&world, glm::ivec2(0, 0), size);
    }

    // void onDestroy() override {
    //     // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
    //     cameraController.exit();
    //     // and we delete all the loaded assets to free memory on the RAM and the VRAM
    //     our::clearAllAssets();
    // }

    void onImmediateGui() {

        //ImGui::ShowDemoWindow();
        ImGui::Begin("Game Menu", false);

        ImGui::Text("Turbo Snail!");
        ImGui::Text("Score: %d", our::GameMananger::gm.score);
        ImGui::Text("Time: %d", 0);

        ImGui::End();
    }
};
