// OGL_RT_TextureView.cpp

#include "glad/glad.h"
#include "OGL_RT_TextureView.hpp"

#include "window/GLFW_Window.hpp"
#include "util/Texture.hpp"
#include "util/Shader.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

namespace {

	const char* vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

out vec2 uv;

void main()
{
    uv = aUV;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

	const char* fragmentShaderSource = R"(
#version 330 core

in vec2 uv;
out vec4 FragColor;

uniform sampler2D uTexture;

void main()
{
    vec2 flippedUV = vec2(uv.x, 1.0 - uv.y);
    FragColor = texture(uTexture, flippedUV);
}
)";

	float vertices[] = {
		// pos      // uv
		-0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
}

namespace Aero::GL {

	OGL_RT_TextureViewer::OGL_RT_TextureViewer() {

		window = std::make_unique<Aero::GL::Window::GLFW_Window>();

		output = std::make_unique<Texture>();

		outputShader = std::make_unique<Shader>(
			vertexShaderSource,
			fragmentShaderSource
		);

		initOGL();
		initWindow();
		createQuad();
	}

	OGL_RT_TextureViewer::~OGL_RT_TextureViewer() = default;

	bool OGL_RT_TextureViewer::shouldWindowClose() const {
		return window->ShouldClose();
	}

	void OGL_RT_TextureViewer::initOGL() {

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD\n";
			throw std::runtime_error("Failed to initialize GLAD");
		}

		outputShader->use();
	}

	void OGL_RT_TextureViewer::initWindow() {

		window->SetTitle("Aero Raytracer Output Viewer");

		// window->ToggleVSync();
	}

	void OGL_RT_TextureViewer::createQuad() {

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		// VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(vertices),
			vertices,
			GL_STATIC_DRAW
		);

		// EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(indices),
			indices,
			GL_STATIC_DRAW
		);

		// Position
		glVertexAttribPointer(
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			4 * sizeof(float),
			(void*)0
		);

		glEnableVertexAttribArray(0);

		// UV
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			4 * sizeof(float),
			(void*)(2 * sizeof(float))
		);

		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void OGL_RT_TextureViewer::renderToTexture(
		const std::vector<uint32_t>& buffer,
		int w,
		int h
	) {

		static int currentWidth = 0;
		static int currentHeight = 0;

		if (w != currentWidth || h != currentHeight) {

			currentWidth = w;
			currentHeight = h;

			window->SetSize(w, h);

			glViewport(0, 0, w, h);

			output->loadTexture(
				w,
				h,
				buffer,
				*outputShader.get(),
				"uTexture",
				0,
				Texture::TextureWrap::CLAMP_TO_EDGE,
				Texture::TextureWrap::CLAMP_TO_EDGE,
				Texture::TextureFilter::LINEAR,
				Texture::TextureFilter::LINEAR
			);
		}
		else {

			output->updateTexture(
				w,
				h,
				buffer
			);
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		outputShader->use();

		glActiveTexture(GL_TEXTURE0);

		output->bind(
			*outputShader,
			"uTexture",
			0
		);

		outputShader->setInt("uTexture", 0);

		glBindVertexArray(VAO);

		glDrawElements(
			GL_TRIANGLES,
			6,
			GL_UNSIGNED_INT,
			0
		);

		glBindVertexArray(0);

		window->SwapBuffers();

		window->PollEvents();
	}
}