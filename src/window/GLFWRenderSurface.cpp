#include <glad/glad.h>
#include <window/GLFWRenderSurface.hpp>

#include <GLFW/glfw3.h>

namespace Aero::GL::Window {

    GLFWRenderSurface::GLFWRenderSurface(GLFWwindow* window)
        : window_(window)
    {
        if (!window_) {
            throw std::runtime_error("GLFWRenderSurface: GLFWwindow is null");
        }
    }

    Vec2<int> GLFWRenderSurface::GetSize() const
    {
        int w = 0, h = 0;
        glfwGetFramebufferSize(window_, &w, &h);
        return { w, h };
    }

    void GLFWRenderSurface::Bind()
    {
        //glfwMakeContextCurrent(window_);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFWRenderSurface::Present()
    {
        glfwSwapBuffers(window_);
    }

} // namespace GLRenderer
