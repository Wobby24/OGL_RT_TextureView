#pragma once

#include <stdexcept>
#include "util/Vec.hpp"
#include <window/Interface/IRenderSurface.hpp>

// Forward declare GLFW types
struct GLFWwindow;

namespace Aero::GL::Window {

    class GLFWRenderSurface final : public IRenderSurface
    {
    public:
        explicit GLFWRenderSurface(GLFWwindow* window);

        Vec2<int> GetSize() const override;
        void Bind() override;
        void Present() override;

    private:
        GLFWwindow* window_ = nullptr; // non-owning
    };

} // namespace GLRenderer
