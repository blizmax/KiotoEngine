//
// Copyright (C) Alexandr Vorontsov. 2017
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#pragma once

#include "Core/Core.h"
#include "Core/CoreTypes.h"

#include <vector>

namespace Kioto
{
class SceneSystem;
class Entity;
class TransformSystem;

class Scene
{
public:
    KIOTO_API Scene();
    Scene(const Scene& scene) = delete;
    Scene& operator=(Scene scene) = delete;
    KIOTO_API virtual ~Scene();

    ///
    /// Init scene.
    ///
    KIOTO_API virtual void Init();
    ///
    /// Update scene.
    ///
    KIOTO_API virtual void Update(float32 dt);
    ///
    /// Shutdown scene.
    ///
    KIOTO_API virtual void Shutdown();
    ///
    /// Add system to the end of systems list.
    ///
    KIOTO_API void AddSystem(SceneSystem* system);
    ///
    /// Insert system before the system of type T in systems list. Returns success status.
    ///
    template <typename T, typename = std::enable_if_t<std::is_convertible_v<T*, SceneSystem*>>>
    bool AddSystemBefore(SceneSystem* system);
    ///
    /// Insert system after the system of type T in systems list. Returns success status.
    ///
    template <typename T, typename = std::enable_if_t<std::is_convertible_v<T*, SceneSystem*>>>
    bool AddSystemAfter(SceneSystem* system);

    ///
    /// Remove system of type T from systems list.
    ///
    template <typename T, typename = std::enable_if_t<std::is_convertible_v<T*, SceneSystem*>>>
    void RemoveSystem();
    ///
    /// Remove system from systems list.
    ///
    KIOTO_API void RemoveSystem(SceneSystem* system);

    ///
    /// Finds system of type T in systems list. Returns success status.
    ///
    template <typename T, typename U, typename = 
        std::enable_if_t<
            std::is_convertible_v<T*, SceneSystem*> 
            && std::is_convertible_v<U*, SceneSystem*>
                        >
            >
    bool FindSystem(U* resultSystem) const;
    ///
    /// Get all scene systems.
    ///
    const std::vector<SceneSystem*>& GetSystems() const;

    void AddEntity(Entity* entity);
    void RemoveEntity(Entity* entity);

private:
    std::vector<SceneSystem*> m_systems; // [a_vorontsov] TODO: linked list in custom arena?
    std::vector<Entity*> m_entities; // [a_vorontsov] Same as above.
};

template <typename T, typename>
bool Scene::AddSystemBefore(SceneSystem* system)
{
    std::vector<SceneSystem*>::const_iterator* it = nullptr;
    bool success = FindSystem<T>(it);
    if (success)
        m_systems.insert(it, system);
    return success;
}

template <typename T, typename>
bool Scene::AddSystemAfter(SceneSystem* system)
{
    std::vector<SceneSystem*>::const_iterator* it = nullptr;
    bool success = FindSystem<T>(it);
    if (success)
        m_systems.insert(it + 1, system);
    return success;
}

template <typename T, typename>
void Scene::RemoveSystem()
{
    std::vector<SceneSystem*>::const_iterator* it = nullptr;
    bool success = FindSystem<T>(it);
    if (success)
    {
        delete &(*it);
        m_systems.erase(it);
    }
}

template <typename T, typename U, typename>
bool Scene::FindSystem(U* resultSystem) const
{
    auto it = std::find_if(m_systems.begin(), m_systems.end(),
        [](const SceneSystem* system)
        {
            return static_cast<T*>(system) != nullptr;
        });
    if (it != m_systems.end())
    {
        resultSystem = it;
        return true;
    }
    return false;
}

inline const std::vector<SceneSystem*>& Scene::GetSystems() const
{
    return m_systems;
}

}