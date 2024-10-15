#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <common/shader.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;


int main() {
    if( !glfwInit() ) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    constexpr int WINDOW_WIDTH = 1024;
    constexpr int WINDOW_HEIGHT = 768;
    constexpr float ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); // make MaOs happy
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH , WINDOW_HEIGHT, "Low Level 3d Engine", NULL, NULL);
    if(window == NULL) {
        fprintf( stderr, "Failed to open GLFW window \n" );
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        fprintf( stderr, "Failed to initialize GLAD\n" );
        return -1;
    }

    // set up vertex data
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // vertex data
    static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    };

    // identifier for buffer
    GLuint vertexbuffer;
    // generate buffer with identifier
    glGenBuffers(1, &vertexbuffer);
    // bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give the defined vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint programID = LoadShaders("src/shaders/SimpleShader.vert", "src/shaders/SimpleShader.frag");

    // get the MVP ID to be used for the shader
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Create the projection Matrix: 45° FOV, Aspect Ratio: Width / Height, display range: 0.1 -> 100.0 units
    mat4 Projection = glm::perspective(glm::radians(45.0f), ASPECT_RATIO, 0.1f, 100.0f);

    mat4 View = glm::lookAt(
        vec3(4,3,3),            // camera is at (4,3,3)
        vec3(0, 0, 0),          // looks ath the world origin
        vec3(0, 1, 0)           // and the head is up
    );

    // model matrix: scaling with a factor of (2, 1.5f, -1)
    mat4 Model = glm::scale(glm::mat4(1), vec3(2,1.5f,-1));

    // construct the project view model matrix
    mat4 MVP = Projection * View * Model;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // set background
    glClearColor(0.2f, 0.0f, 0.7f, 1.0f);
    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw here
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,          // attribute 0, must match layout in shader
            3,          //size
            GL_FLOAT,   //type
            GL_FALSE,   //normalized?
            0,          //stride
            static_cast<void *>(nullptr)    // array buffer offset
        );

        // use our shader
        glUseProgram(programID);

        // give the shader the MVP matrix
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    return 0;
}
