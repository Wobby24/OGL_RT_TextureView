#include <glad/glad.h>
#include <util/Texture.hpp>
#include <cstdint>
#include <vector>

namespace Aero::GL {
    Texture::Texture() : textureID(0), isBound(false) {
    }

    Texture::~Texture() {
        glDeleteTextures(1, &textureID);
    }

    void Texture::loadTexture(int w, int h, std::vector<uint32_t> data, Shader& shader, const std::string& uniformName, unsigned int unit,
        TextureWrap wrapS, TextureWrap wrapT, TextureFilter minFilter, TextureFilter magFilter) {

        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set the texture wrapping parameters
        setTextureWrapping(wrapS, wrapT);

        // Set the texture filtering parameters
        setTextureFiltering(minFilter, magFilter);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            w,
            h,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data.data()
        );
    }

    void Texture::updateTexture(int w, int h, std::vector<uint32_t> data) {
        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            0,
            0,
            w,
            h,
            GL_RGBA,
            GL_UNSIGNED_INT_8_8_8_8,
            data.data()
        );
    }

    void Texture::bind(Shader& shader, const std::string& uniformName, unsigned int unit) {
        if (!isBound) {
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, textureID);
            shader.use();
            shader.setInt(uniformName, unit);
            isBound = true; // Mark the texture as bound
        }
    }

    unsigned int Texture::getTextureID() const {
        return textureID;
    }

    void Texture::setTextureWrapping(TextureWrap wrapS, TextureWrap wrapT) {
        // Set the texture wrapping parameters
        switch (wrapS) {
        case TextureWrap::REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            break;
        case TextureWrap::MIRRORED_REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            break;
        case TextureWrap::CLAMP_TO_EDGE:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            break;
        case TextureWrap::CLAMP_TO_BORDER:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            break;
        }

        switch (wrapT) {
        case TextureWrap::REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case TextureWrap::MIRRORED_REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;
        case TextureWrap::CLAMP_TO_EDGE:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        case TextureWrap::CLAMP_TO_BORDER:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            break;
        }
    }

    void Texture::setTextureFiltering(TextureFilter minFilter, TextureFilter magFilter) {
        // Set the texture filtering parameters
        switch (minFilter) {
        case TextureFilter::NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
        case TextureFilter::LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        }

        switch (magFilter) {
        case TextureFilter::NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case TextureFilter::LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        }
    }

}
