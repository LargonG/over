#pragma once

#include <chrono>
#include <span>
#include <type_traits>
#include <vector>

#include <owlet/debug/log.h>
#include <owlet/engine/scene.h>
#include <owlet/os/core.h>

namespace owlet::engine {

struct SceneManager {
  public:
    SceneManager();

    void Add(std::unique_ptr<Scene> scene);
    void Remove(Scene*);

    [[nodiscard]] Scene* GetActive();
    void SetActive(Scene*);

    std::span<std::unique_ptr<Scene>> All() noexcept { return m_scenes; }

  protected:
    std::vector<std::unique_ptr<Scene>> m_scenes;
};

template <class Impl>
struct App {
  private:
    struct CursorPositionMonitor {
      public:
        bool Refresh(float x, float y) {
            auto [lx, ly] = Last();
            if (lx == x && ly == y) {
                return false;
            }
            Update(x, y);
            return true;
        }

        void Update(float x, float y) {
            m_last_x = x;
            m_last_y = y;
        }

        std::tuple<float, float> Last() { return {m_last_x, m_last_y}; }

      private:
        float m_last_x = 0.0f;
        float m_last_y = 0.0f;
    };

  public:
    App() : m_desktop(), m_windows(), m_scenes_manager() {
        debug::Assert(!s_instance);
        s_instance = this;
    }

    void CreateWindow(os::Window::Settings&& window_settings, gl::Settings&& gl_settings) {
        auto monitors = m_desktop.AvailableMonitors();

        auto err_callback = [](int err, const char* description) {
            Instance()->This()->OnError(static_cast<int32>(err), std::string(description));
        };
        window_settings.error_callback = err_callback;

        auto key_input_callback = [](GLFWwindow* window, int key, int scan_code, int action, int mode) noexcept {
            auto val = Instance();
            os::Window& w = val->FindWindow(window);

            int32 tkey = static_cast<int32>(key);
            int32 tscan_code = static_cast<int32>(scan_code);
            int32 taction = static_cast<int32>(action);
            int32 tmode = static_cast<int32>(mode);

            val->This()->OnInput(w, tkey, tscan_code, taction, tmode);
            val->Scenes().GetActive()->OnInput(tkey, tscan_code, taction, tmode);
        };
        window_settings.key_input_callback = key_input_callback;

        auto resize_callback = [](GLFWwindow* window, int sizex, int sizey) {
            auto val = Instance();
            os::Window& w = val->FindWindow(window);
            w.GL()->Viewport(0, 0, static_cast<GLsizei>(sizex), static_cast<GLsizei>(sizey));
        };
        window_settings.window_resize_callback = resize_callback;

        auto mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods) {
            auto val = Instance();
            os::Window& w = val->FindWindow(window);

            int32 tbutton = static_cast<int32>(button);
            int32 taction = static_cast<int32>(action);
            int32 tmods = static_cast<int32>(mods);

            val->This()->OnMouseButton(w, tbutton, taction, tmods);
            val->Scenes().GetActive()->OnMouseButton(tbutton, taction, tmods);
        };
        window_settings.mouse_button_callback = mouse_button_callback;

        auto cursor_pos_callback = [](GLFWwindow* window, double xpos, double ypos) {
            auto val = Instance();

            os::Window& w = val->FindWindow(window);
            float pos_x = static_cast<float>(xpos);
            float pos_y = static_cast<float>(ypos);

            val->This()->OnCursorPosition(w, pos_x, pos_y);
            val->Scenes().GetActive()->OnCursorPosition(pos_x, pos_y);
            // bug: more complex logic needed for mutiple windows
            auto [last_x, last_y] = val->This()->m_cursor_position.Last();
            if (val->This()->m_cursor_position.Refresh(xpos, ypos)) {
                float delta_x = static_cast<float>(xpos - last_x);
                float delta_y = static_cast<float>(ypos - last_y);

                val->This()->OnCursorPositionChanged(w, delta_x, delta_y);
                val->Scenes().GetActive()->OnCursorPositionChanged(delta_x, delta_y);
            }
        };
        window_settings.cursor_pos_callback = cursor_pos_callback;

        auto window = os::Window(std::move(window_settings));
        m_windows.push_back(std::move(window));

        MainWindow().SetupGL(std::move(gl_settings));
    }

    void Run() {
        float dt = 1.0f / 60.0f;

        while (!MainWindow().ShouldClose()) {
            auto start_time = std::chrono::high_resolution_clock::now();
            Desktop().PollEvents();

            for (const auto& window : m_windows) {
                static_cast<Impl*>(this)->Update(window.GL(), dt);
            }
            for (auto& window : m_windows) {
                window.SwapBuffers();
            }

            auto scene = m_scenes_manager.GetActive();
            if (scene) {
                scene->Update(dt);
            }

            auto end_time = std::chrono::high_resolution_clock::now();

            std::chrono::duration<float, std::milli> passed = end_time - start_time;

            dt = passed.count() / 1000.0;
        }
    }

    static App<Impl>* Instance() noexcept { return s_instance; }

    os::Desktop& Desktop() noexcept { return m_desktop; }
    os::Window& MainWindow() noexcept { return m_windows.front(); }

    SceneManager& Scenes() noexcept { return m_scenes_manager; }

    Impl* This() noexcept { return static_cast<Impl*>(this); }
    const Impl* This() const noexcept { return static_cast<const Impl*>(this); }

  protected:
    os::Window& FindWindow(GLFWwindow* window) {
        for (auto& w : m_windows) {
            if (w.Raw() == window) {
                return w;
            }
        }
        debug::Unreachable("Could not find window, incorrect implementation");
    }

    static App<Impl>* s_instance;

    os::Desktop m_desktop;
    std::vector<os::Window> m_windows;
    SceneManager m_scenes_manager;

  private:
    CursorPositionMonitor m_cursor_position;
};

template <class Impl>
App<Impl>* App<Impl>::s_instance = nullptr;

}    // namespace owlet::engine
