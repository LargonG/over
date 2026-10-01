#include <owlet/engine/core.h>

#include "example_app.h"

namespace owlet {
void Run() {
    std::unique_ptr<example::App> app = std::make_unique<example::App>();

    app->Run();
}

WLT_DEF_EMPTY_CALLBACKS

}    // namespace owlet
