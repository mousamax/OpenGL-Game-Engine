#pragma once

#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The Game Manager system is responsible for controling the game logic.
    // keep track of the score, time etc... 
    class GameMananger {
    private:
        int score = 0;

    public:
        //this static object can be used by different logic components 
        static GameMananger gm;

        //used to add/subtract score ex:coin collection or hit obstacle
        void changeScore(int value)
        {
            score += value;
        }
        // This should be called every frame to update all entities containing a MovementComponent. 
        /*
        void update(World* world, float deltaTime) {
            // For each entity in the world
            for (auto entity : world->getEntities()) {
                // Get the movement component if it exists
                MovementComponent* movement = entity->getComponent<MovementComponent>();
                // If the movement component exists
                if (movement) {
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * movement->linearVelocity;
                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                }
            }
        }
        */
    };

}
