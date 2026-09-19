#pragma once

#include <chrono>
#include <type_traits>

#include <owlet/debug/log.h>
#include <owlet/os/core.h>

namespace owlet::engine {

template <class Impl>
struct App {
  public:
    App() : m_desktop() {
        debug::Assert(!s_instance);
        s_instance = this;
    }

    void CreateWindow(os::Window::Settings&& window_settings, gl::Settings&& gl_settings) {
        auto monitors = m_desktop.AvailableMonitors();

        auto err_callback = [](int err, const char* description) {
            static_cast<Impl*>(Instance())->OnError(static_cast<int32>(err), std::string(description));
        };

        auto input_callback = [](GLFWwindow* window, int key, int scan_code, int action, int mode) noexcept {
            auto val = Instance();
            for (auto& w : val->m_windows) {
                if (w.Raw() == window) {
                    static_cast<Impl*>(val)->OnInput(w, static_cast<int32>(key), static_cast<int32>(scan_code),
                                                     static_cast<int32>(action), static_cast<int32>(mode));
                }
            }
        };

        auto window = os::Window(std::move(window_settings), err_callback, input_callback);
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

            auto end_time = std::chrono::high_resolution_clock::now();

            std::chrono::duration<float, std::milli> passed = end_time - start_time;

            dt = passed.count() / 1000.0;
        }
    }

    static App<Impl>* Instance() noexcept { return s_instance; }

    os::Desktop& Desktop() noexcept { return m_desktop; }
    os::Window& MainWindow() noexcept { return m_windows.front(); }

  protected:
    static App<Impl>* s_instance;

    os::Desktop m_desktop;
    std::vector<os::Window> m_windows;
};

template <class Impl>
App<Impl>* App<Impl>::s_instance = nullptr;

}    // namespace owlet::engine
