#pragma once

#include "../ecs/world.hpp"
#include "../components/collision.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/movement.hpp"
    class CollisionSystem {
    public:

        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
            // For each entity in the world
            // get the current player entity
            
            glm::vec3 playerPosition = vec3(0, 0, 0);
            float playerRadius = 0;

            for(auto entity : world->getEntities()){
                // get the movement component if it exists
                if (entity->name == "player")
                {
                    CollisionComponent * collision = entity->getComponent<CollisionComponent>();

                    playerPosition = collision->center + glm::vec3(entity->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1));;
                    playerRadius = collision->radius + glm::length(entity->localTransform.scale);
                    break;
                }
            }
            // get player collision component
            
            // get the new radius and new position

            for(auto entity : world->getEntities()){
                // Get the movement component if it exists
                CollisionComponent* collision = entity->getComponent<CollisionComponent>();
                // If the movement component exists
                if(collision){
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    glm::vec3 newPosition = collision->center + glm::vec3(entity->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1));
                    float newRadius = collision->radius + glm::length(entity->localTransform.scale);

                    if (glm::length(newPosition - playerPosition) < playerRadius + newRadius)
                    {
                        // collision occurred
                        if (entity->name == "coin")
                        {
                            
                            world->markForRemoval(entity);
                        }
                    }
                }
            }
        }

    };

}
