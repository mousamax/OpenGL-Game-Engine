#pragma once
#define COIN_SCORE 1

#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include <systems/game-manager.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <iostream>
namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class CollisionSystem
    {
    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world)
        {

            glm::vec3 playerPosition = glm::vec3(0, 0, 0);
            float playerRadius = 0;

            for (auto entity : world->getEntities())
            {
                // get the current player entity and get its position and radius
                if (entity->name == "player")
                {
                    CollisionComponent *collision = entity->getComponent<CollisionComponent>();

                    playerPosition = collision->center + glm::vec3(entity->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1));
                    playerRadius = collision->radius * glm::length(entity->localTransform.scale) / 1.732f;
                    std::cout << "player position: " << playerPosition.x << " " << playerPosition.y << " " << playerPosition.z << std::endl;
                    break;
                }
            }

            for (auto entity : world->getEntities())
            {

                // get all entities that have collision component
                CollisionComponent *collision = entity->getComponent<CollisionComponent>();
                if (entity->name != "player")
                {

                    if (collision)
                    {
                        // get the new radius and position of the entity
                        glm::vec3 newPosition = collision->center + glm::vec3(entity->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1));
                        float newRadius = collision->radius * glm::length(entity->localTransform.scale) / 1.732f;

                        // compare with player position to check if it collides or not
                        if (glm::length(newPosition - playerPosition) < playerRadius + newRadius)
                        {
                            std::cout << "collision" << std::endl;
                            // collision occurred
                            if (entity->name == "coin")
                            {
                                our::GameMananger::gm.changeScore(COIN_SCORE);
                                world->markForRemoval(entity);
                            }
                        }
                    }
                }
            }
        }
    };

}
