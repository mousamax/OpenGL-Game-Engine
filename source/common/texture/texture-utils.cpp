#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

glm::ivec2 our::texture_utils::loadImage(Texture2D &texture, const char *filename, bool generate_mipmap)
{
    glm::ivec2 size;
    int channels;
    //Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    //We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    //Load image data and retrieve width, height and number of channels in the image
    //The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    //Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char *data = stbi_load(filename, &size.x, &size.y, &channels, 4);
    if (data == nullptr)
    {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return {0, 0};
    }
    //Bind the texture such that we upload the image data to its storage
    //TODO: Finish this function
    //HINT: The steps should be as follows: bind the texture, send the pixel data to the GPU, then generate the mipmap (if requested).
    texture.bind();
    //values allowed are 1,2,4,8 (bytes at which rows start at)
    //use 4 in case the image has padding
    //unpacks data from memory to VRAM
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    //     texture type - miplevel - internal format - width - height - border ==> (vram)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //border of width must be 0
    //miplevel = 0 (highest definition)
    //RGBA8 8 bits for every pixel (8 bits red , 8 bits green .. etc)
    //x * y number of pixels
    //last 3 params are format - data type - data pointer of how the data is saved in RAM (4x4 data array)
    //unsigned byte because values are from 0 to 255
    //RGBA -> red,green,blue,alpha channels
    if (generate_mipmap)
        glGenerateMipmap(GL_TEXTURE_2D); //average of every 4 adjacent pixels of every level to get lower definition of images
    //from level 0 to level 1 .. etc (number of pixels decrease each level)
    stbi_image_free(data); //Free image data after uploading to GPU
    return size;
}