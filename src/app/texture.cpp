#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cmath>

Texture::Texture(std::string path, std::string type){
    stbi_set_flip_vertically_on_load(true);  

    unsigned char * data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    pixelData = new float[width*height*channels];

    if (data){
        if(type == "RGB"){
            for(int i = 0; i < width*height*channels; ++i){
                pixelData[i] = std::pow((float)data[i] * (1/255.0f), 2.2f);
            }
            //tileData(tileSize);
        }
        else if (type == "XYZ"){
            for(int i = 0; i < width*height*channels; ++i){
                pixelData[i] = (float)data[i] * (2/255.0f) - 1.0f;
            }
            //tileData(tileSize);
        }
        else if (type == "BW"){
            for(int i = 0; i < width*height*channels; ++i){
                pixelData[i] = (float)data[i] * (1/255.0f);
            }
            //tileData(tileSize);
        }
        else{
            printf("Error unrecognized texture format type.\n");
        }
    }
    else{
        printf("Failed to load texture at: %s\n",path.c_str());
    }
    stbi_image_free(data);
}

void Texture::tileData(){
    float *tiledPixelData = new float[width*height*channels];

    int tileNumW = width / tileW;
    int tileNumH = height / tileH;

    for(int i = 0; i < tileH; ++i){
        
        for(int j = 0; j < tileW; ++j){

            for(int pH = 0; pH < tileH; ++pH){

                for(int pW = 0; pW < tileW; ++pW){

                    

                }

            }

        }

    }

    // //Channel by channel every i 3 = 1 pixel
    // int currentPixel = 0;
    // for(int i = 0; i < width*height*channels; ++i){
    //     int currentChannel = i % channels;

    //     if(currentChannel == 0){
    //         ++currentPixel;
    //     }


    //     if(currentPixel %)


    //     tiledPixelData[i] = pixelData[i];


    // }


    delete [] pixelData;
    pixelData = tiledPixelData;
}

Texture::~Texture(){
    delete [] pixelData;
}

Vector3f Texture::getPixelVal(float u, float v){

    //Simple bilinear filtering
    //DISABLED FOR NOW since it trashes the FPS
    // float intU;
    // float tU = std::modf(u * (width-1), &intU);
    // int uIntLo = (int)intU; 
    // int uIntHi = std::ceil(u * (width-1)); 

    // float intV;
    // float tV = std::modf(v * (height-1), &intV);
    // int vIntLo = (int)intV; 
    // int vIntHi = std::ceil(v * (height-1));

    // int index00 = (vIntLo*width + uIntLo)*channels;
    // int index01 = (vIntLo*width + uIntHi)*channels;
    // int index02 = (vIntHi*width + uIntLo)*channels;
    // int index03 = (vIntHi*width + uIntHi)*channels;

    // float red   = (pixelData[index00]*(1 - tU) + pixelData[index01]*tU)*(1-tV) + (pixelData[index02]*(1-tU) + pixelData[index03]*tU)*tV;
    // float green = (pixelData[index00 + 1]*(1 - tU) + pixelData[index01 + 1]*tU)*(1-tV) + (pixelData[index02 + 1]*(1-tU) + pixelData[index03 + 1]*tU)*tV;
    // float blue  = (pixelData[index00 + 2]*(1 - tU) + pixelData[index01 + 2]*tU)*(1-tV) + (pixelData[index02 + 2]*(1-tU) + pixelData[index03 + 2]*tU)*tV;

    // return Vector3f{red, green, blue};

    int uInt = u * (width-1); 
    int vInt = v * (height-1);
    int index = (vInt*width + uInt)*channels;
    return Vector3f{pixelData[index], pixelData[index+1], pixelData[index+2]};

}

float Texture::getIntensityVal(float u, float v){
    //Simple bilinear filtering
    //DISABLED FOR NOW since it trashes the FPS
    // float intU;
    // float tU = std::modf(u * (width-1), &intU);
    // int uIntLo = (int)intU; 
    // int uIntHi = std::ceil(u * (width-1)); 

    // float intV;
    // float tV = std::modf(v * (height-1), &intV);
    // int vIntLo = (int)intV; 
    // int vIntHi = std::ceil(v * (height-1));

    // int index00 = (vIntLo*width + uIntLo)*channels;
    // int index01 = (vIntLo*width + uIntHi)*channels;
    // int index02 = (vIntHi*width + uIntLo)*channels;
    // int index03 = (vIntHi*width + uIntHi)*channels;

    // float val   = (pixelData[index00]*(1 - tU) + pixelData[index01]*tU)*(1-tV) + (pixelData[index02]*(1-tU) + pixelData[index03]*tU)*tV;

    // return val;

    int uInt = u * (width-1); 
    int vInt = v * (height-1);
    int index = (vInt*width + uInt);
    return pixelData[index];
}

