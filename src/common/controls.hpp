//
// Created by jonas on 22.10.24.
//

#ifndef CONTROLS_H
#define CONTROLS_H
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class controls {
public:
        explicit controls(GLFWwindow* window);
        void ComputeMatricesFromInputs();
        static glm::mat4 GetViewMatrix();
        static glm::mat4 GetProjectionMatrix();
};



#endif //CONTROLS_H
