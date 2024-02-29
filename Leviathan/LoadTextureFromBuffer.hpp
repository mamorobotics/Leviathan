#define STB_IMAGE_IMPLEMENTATION

#include "UI.hpp"
#include "glad/glad.h"
#include <opencv2/opencv.hpp>

class LoadTextureFromBuffer {
public:
    static bool LoadTexture(std::vector<char> data_buffer, int image_width, int image_height, GLuint *  out_texture)
    {
        cv::Mat mat = cv::imdecode(data_buffer, cv::IMREAD_COLOR);

        if(mat.empty()){
            std::cerr << "Error: unable to decode the JPEG image." << std::endl;
            return false;
        }
        std::cout << "decoded";

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.data);

        //*out_texture = image_texture;
        

        return true;
    }

};