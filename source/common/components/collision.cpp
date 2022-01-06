#include "collision.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    void CollisionComponent::deserialize(const nlohmann::json& data)
    {
        if(!data.is_object()) return;
        radius = data.value("radius", radius);
        center = data.value("center". center);   
    }
} // namespace our
