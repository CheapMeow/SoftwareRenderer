#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string path) { 
    pixelData = stbi_load(path.c_str(), &width, &height, &channels, 0);
}

Vector3f Texture::getPixelVal(float u, float v)
{
    float intpart;
    int   uInt  = std::modf(u, &intpart) * (width - 1);
    int   vInt  = (1 - std::modf(v, &intpart)) * (height - 1);
    int   index = (vInt * width + uInt) * 3;
    // printf("%d, %d, %d, %x, %x, %x\n",vInt, uInt, index ,pixelData[index],pixelData[index+1],pixelData[index+2]);
    float val1 = pixelData[index];
    float val2 = pixelData[index + 1];
    float val3 = pixelData[index + 2];
    return Vector3f {val1, val2, val3};
}