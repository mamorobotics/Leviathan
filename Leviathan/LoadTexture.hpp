#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.hpp"
#include "glad/glad.h"

class LoadTexture {
public:
<<<<<<< HEAD:Leviathan/LoadTextureFromBuffer.hpp
    static bool LoadTexture(const char* image_data, int image_width, int image_height, GLuint *  out_texture)
    {
=======
    static bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
    {
        // Load from file
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
            return false;

>>>>>>> parent of 478f7c4 (Merge pull request #4 from mamorobotics/Networking):Leviathan/LoadTexture.hpp
        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

        // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
<<<<<<< HEAD:Leviathan/LoadTextureFromBuffer.hpp

        *out_texture = image_texture;
=======
        stbi_image_free(image_data);

        *out_texture = image_texture;
        *out_width = image_width;
        *out_height = image_height;
>>>>>>> parent of 478f7c4 (Merge pull request #4 from mamorobotics/Networking):Leviathan/LoadTexture.hpp

        return true;
    }

};