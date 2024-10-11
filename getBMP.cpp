#include "getBMP.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

imageFile *getBMP(std::string fileName)
{
    int offset, w, h;
    
    imageFile *tempStore = new imageFile;
    imageFile *outRGB = new imageFile;
    imageFile *outRGBA = new imageFile;

    std::ifstream inFile(fileName.c_str(), std::ios::binary);
    if (!inFile) {
        throw std::runtime_error("Failed to open BMP file: " + fileName);
    }

    inFile.seekg(10);
    inFile.read((char *)&offset, 4); 

    inFile.seekg(18);
    inFile.read((char *)&w, 4);
    inFile.read((char *)&h, 4);

    int padding = (3 * w) % 4 ? 4 - (3 * w) % 4 : 0;

    tempStore->data = new unsigned char[(3 * w + padding) * h];
    if (!tempStore->data) {
        throw std::bad_alloc();
    }
    inFile.seekg(offset);
    inFile.read((char *)tempStore->data, (3 * w + padding) * h);
    inFile.close();

    outRGB->width = w;
    outRGB->height = h;
    outRGB->data = new unsigned char[3 * w * h];
    if (!outRGB->data) {
        delete[] tempStore->data;
        delete tempStore;
        throw std::bad_alloc();
    }

    int tempStorePos = 0;
    int outRGBpos = 0;
    for (int j = 0; j < h; j++)
        for (int i = 0; i < 3 * w; i += 3)
        {
            tempStorePos = (3 * w + padding) * j + i;
            outRGBpos = 3 * w * j + i;
            outRGB->data[outRGBpos] = tempStore->data[tempStorePos + 2];
            outRGB->data[outRGBpos + 1] = tempStore->data[tempStorePos + 1];
            outRGB->data[outRGBpos + 2] = tempStore->data[tempStorePos];
        }

    outRGBA->width = w;
    outRGBA->height = h;
    outRGBA->data = new unsigned char[4 * w * h];
    if (!outRGBA->data) {
        delete[] tempStore->data;
        delete tempStore;
        delete[] outRGB->data;
        delete outRGB;
        throw std::bad_alloc();
    }

    for (int j = 0; j < 4 * w * h; j += 4)
    {
        outRGBA->data[j] = outRGB->data[(j / 4) * 3];
        outRGBA->data[j + 1] = outRGB->data[(j / 4) * 3 + 1];
        outRGBA->data[j + 2] = outRGB->data[(j / 4) * 3 + 2];
        
        // Set alpha to 0 if the pixel is white, otherwise set it to 255
        if (outRGBA->data[j] == 255 && outRGBA->data[j + 1] == 255 && outRGBA->data[j + 2] == 255) {
            outRGBA->data[j + 3] = 0; // Fully transparent
        } else {
            outRGBA->data[j + 3] = 0xFF; // Fully opaque
        }
    }

    delete[] tempStore->data;
    delete tempStore;
    delete[] outRGB->data;
    delete outRGB;
    return outRGBA;
}