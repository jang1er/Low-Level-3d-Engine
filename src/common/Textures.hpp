//
// Created by jonas on 16.10.24.
//

#ifndef TEXTURES_H
#define TEXTURES_H
#include<glad/gl.h>


class Textures {
public:
    static GLuint loadBMP(const char * filename);
    static GLuint loadDDS(const char * filename);
};



#endif //TEXTURES_H
