#include <owlet/engine/app.h>

#include <algorithm>

namespace owlet::engine {
SceneManager::SceneManager() : m_scenes() {}

void SceneManager::Add(std::unique_ptr<Scene> scene) {
    m_scenes.push_back(std::move(scene));
    if (m_scenes.size() == 1) {
        m_scenes.front()->Load();
    }
}

void SceneManager::Remove(Scene* that) {
    std::erase_if(m_scenes, [that](const std::unique_ptr<Scene>& val) { return val.get() == that; });
}

Scene* SceneManager::GetActive() {
    if (m_scenes.empty()) {
        return nullptr;
    }
    return m_scenes.front().get();
}

void SceneManager::SetActive(Scene* that) {
    auto element = std::find_if(m_scenes.begin(), m_scenes.end(),
                                [that](const std::unique_ptr<Scene>& val) { return val.get() == that; });
    if (element == m_scenes.begin()) {
        return;
    }

    m_scenes.front()->Unload();
    std::iter_swap(m_scenes.begin(), element);
    m_scenes.front()->Load();
}
}    // namespace owlet::engine
