//
// Created by jonas on 16.10.24.
//

#include "Textures.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

GLuint Textures::loadBMP(const char *filename) {
    // Data from the header of the BMP file
    unsigned char header[54]; // each file has a 54 byte header
    unsigned int dataPosition; // Position of the actual data in the file
    unsigned int width, height;
    unsigned int imageSize; // should just be = width * height * 3
    unsigned char *data; // Actual RGB data from file

    // Open the file
    FILE *file = fopen(filename, "rb");
    if (!file) {printf("Image file could not be opened\n"); return 0;}

    // check if we can read the header section
    if( fread(header, 1, 54, file) != 54) {printf("Header could not be read. Not a correct BMP file\n"); return 0;}

    // check if the header identifier is correct
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Not a valid BMP file\n");
        return 0;
    }

    // read the header data from the buffer
    dataPosition = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    // some files are misformatted, so try to guess some values
    if (imageSize == 0) imageSize = width * height * 3; // one byte for each red, green and blue
    if (dataPosition == 0) dataPosition = 54;

    data = new unsigned char[imageSize];

    fread(data, 1, imageSize, file);

    fclose(file);

    // create an OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // Bind the new texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // give the image to opengl
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // delete data has it is no longer needed
    delete[] data;

    // Poor filtering ...
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Far Better Trilinear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // generate mipmaps automatically
    glGenerateMipmap(GL_TEXTURE_2D);

    // return the id for the texture
    return textureID;
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

/** Loads OpenGL Texture from .DDS File
 *
 *  @param[in] filename The absolute path to the file
 *  @returns OpenGL ID for the loaded texture
 *  @note The loaded texture is inverted (DXT compression), to correctly display texture invert all uv.v coordinates
 */
GLuint Textures::loadDDS(const char *filename) {
    unsigned char header[124];
    FILE *file = fopen(filename, "rb");
    if (!file) {printf("Image file could not be opened\n"); return 0;}

    char filecode[4];
    fread(filecode, 1, 4, file);
    if(strcmp(filecode, "DDS ") != 0) {
        fclose(file);
        return 0;
    }

    fread(&header, 124, 1, file);

    unsigned int height = *(unsigned int*)&(header[8]);
    unsigned int width = *(unsigned int*)&(header[12]);
    unsigned int linearSize = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC = *(unsigned int*)&(header[80]);

    unsigned char* buffer;
    unsigned int bufferSize;

    bufferSize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufferSize * sizeof(unsigned char));
    fread(buffer, 1, bufferSize, file);
    fclose(file);

    unsigned int components = (fourCC == FOURCC_DXT1) ? 4 : 3;
    unsigned int format;

    switch (fourCC) {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            free(buffer);
        return 0;
    }

    // Create OpenGL Texture
    GLuint textureID;
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    // load the mipmaps
    for(unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
        unsigned int size = ((width+3)/4) * ((height+3)/4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);
        offset += size;
        width /= 2;
        height /= 2;
    }
    free(buffer);

    return textureID;
}