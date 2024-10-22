#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <common/shader.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <ctime>
#include <random>
#include <common/controls.hpp>

#include "common/Textures.hpp"

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
    if(window == nullptr) {
        fprintf( stderr, "Failed to open GLFW window \n" );
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGL(glfwGetProcAddress)) {
        fprintf( stderr, "Failed to initialize GLAD\n" );
        return -1;
    }

    // set input mode for glfw to use
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // set unlimited fps
    //glfwSwapInterval(0);

    // give windows to input control
    controls inputControl(window);
    // setup fps counter
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // enable OpenGL Depth Test -> correct drawing order
    glEnable(GL_DEPTH_TEST);
    // accept fragment if it is closest to the camera
    glDepthFunc(GL_LESS);

    // set up vertex data
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // vertex data triangle
    static const GLfloat g_triangle_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    // color data triangle
    static const GLfloat g_triangle_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f
    };

    // identifier for buffer
    GLuint triangle_vertexbuffer;
    // generate buffer with identifier
    glGenBuffers(1, &triangle_vertexbuffer);
    // bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, triangle_vertexbuffer);
    // Give the defined vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_triangle_vertex_buffer_data), g_triangle_vertex_buffer_data, GL_STATIC_DRAW);

    // identifier for buffer
    GLuint triangle_colorbuffer;
    // generate buffer with identifier
    glGenBuffers(1, &triangle_colorbuffer);
    // bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, triangle_colorbuffer);
    // Give the defined vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_triangle_color_buffer_data), g_triangle_color_buffer_data, GL_STATIC_DRAW);

    GLuint programID = LoadShaders("src/shaders/TextureShader.vert", "src/shaders/TextureShader.frag");
    GLuint programID_triangle = LoadShaders("src/shaders/ColorShader.vert", "src/shaders/ColorShader.frag");

    // get the MVP ID to be used for the shader
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Create the projection Matrix: 45° FOV, Aspect Ratio: Width / Height, display range: 0.1 -> 100.0 units
    //mat4 Projection = glm::perspective(glm::radians(45.0f), ASPECT_RATIO, 0.1f, 100.0f);

    /*mat4 View = glm::lookAt(
        vec3(4,3,3),            // camera is at (4,3,3)
        vec3(0, 0, 0),          // looks ath the world origin
        vec3(0, 1, 0)           // and the head is up
    );*/

    // model matrix: scaling with a factor of (2, 1.5f, -1)
    mat4 Model_Triangle = glm::translate(glm::mat4(1.0f), vec3(1, 0, 0));
    mat4 Model_Cube = glm::translate(glm::mat4(1.0f), vec3(-2, 0, 0));




    // cube handling
    static const GLfloat g_cube_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };


    // identifier for buffer
    GLuint cube_vertexbuffer;
    // generate buffer with identifier
    glGenBuffers(1, &cube_vertexbuffer);
    // bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, cube_vertexbuffer);
    // Give the defined vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_vertex_buffer_data), g_cube_vertex_buffer_data, GL_STATIC_DRAW);

    // load texture to be used on cube
    //GLuint Texture = Textures::loadBMP("src/Textures/uvtemplate-2.bmp");
    GLuint Texture = Textures::loadDDS("src/Textures/uvtemplate.DDS");

    // get the location of texture Uniform
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

    // UV data for cube
    static GLfloat g_uv_buffer_data[] = {
        0.000059f, 1.0f-0.000004f,
        0.000103f, 1.0f-0.336048f,
        0.335973f, 1.0f-0.335903f,
        1.000023f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.336024f, 1.0f-0.671877f,
        0.667969f, 1.0f-0.671889f,
        1.000023f, 1.0f-0.000013f,
        0.668104f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.000059f, 1.0f-0.000004f,
        0.335973f, 1.0f-0.335903f,
        0.336098f, 1.0f-0.000071f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.336024f, 1.0f-0.671877f,
        1.000004f, 1.0f-0.671847f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.668104f, 1.0f-0.000013f,
        0.335973f, 1.0f-0.335903f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.668104f, 1.0f-0.000013f,
        0.336098f, 1.0f-0.000071f,
        0.000103f, 1.0f-0.336048f,
        0.000004f, 1.0f-0.671870f,
        0.336024f, 1.0f-0.671877f,
        0.000103f, 1.0f-0.336048f,
        0.336024f, 1.0f-0.671877f,
        0.335973f, 1.0f-0.335903f,
        0.667969f, 1.0f-0.671889f,
        1.000004f, 1.0f-0.671847f,
        0.667979f, 1.0f-0.335851f
    };

    // transform uv coordinates
    for(unsigned int i = 1; i < sizeof(g_uv_buffer_data); i += 2) {
        g_uv_buffer_data[i] = 1.0f - g_uv_buffer_data[i];
    }


    GLuint cube_uvbuffer;
    glGenBuffers(1, &cube_uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cube_uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);



    constexpr float TIMESTEP = 0.01f;

    // set background
    glClearColor(0.2f, 0.0f, 0.7f, 1.0f);

    // enable backface culling
    glEnable(GL_CULL_FACE);

    do {
        // track the frame time
        double currentTime = glfwGetTime();
        nbFrames++;
        if(currentTime - lastTime >= 1.0) { // more than a second has elapsed
            // print the current frame time over 1 second and reset the timer
            printf("Frame Time: %f ms [%i fps]\n", 1000.0/double(nbFrames), nbFrames);
            lastTime = currentTime;
            nbFrames = 0;
        }


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // recompute MVP matrix to enable camera movement
        inputControl.ComputeMatricesFromInputs();
        // recompute View Matrix
        glm::mat4 View = inputControl.GetViewMatrix();
        // recompute Projection
        glm::mat4 Projection = inputControl.GetProjectionMatrix();

        // recompute MVP based on control input
        // construct the project view model matrix
        mat4 MVP_Triangle = Projection * View * Model_Triangle;
        mat4 MVP_Cube = Projection * View * Model_Cube;

        // 1st Draw Call: Pass OpenGL the data for the triangle
        glEnableVertexAttribArray(0); // 1st attribute: Vertex data triangle
        glBindBuffer(GL_ARRAY_BUFFER, triangle_vertexbuffer);
        glVertexAttribPointer(
            0,          // attribute 0, must match layout in shader
            3,          //size
            GL_FLOAT,   //type
            GL_FALSE,   //normalized?
            0,          //stride
            static_cast<void *>(nullptr)    // array buffer offset
        );

        glEnableVertexAttribArray(1); // 2nd attribute: Color data triangle
        glBindBuffer(GL_ARRAY_BUFFER, triangle_colorbuffer);
        glVertexAttribPointer(
            1,          // attribute 0, must match layout in shader
            3,          //size
            GL_FLOAT,   //type
            GL_FALSE,   //normalized?
            0,          //stride
            static_cast<void *>(nullptr)    // array buffer offset
        );

        // use the shader
        glUseProgram(programID_triangle);

        // give the shader the triangle MVP matrix
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP_Triangle[0][0]);

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // 2nd Draw Call: Pass OpenGL the data for the cube
        glEnableVertexAttribArray(0); // 1st Attribute: Vertex data
        glBindBuffer(GL_ARRAY_BUFFER, cube_vertexbuffer);
        glVertexAttribPointer(
        0,          // attribute 0, must match layout in shader
        3,          //size
        GL_FLOAT,   //type
        GL_FALSE,   //normalized?
        0,          //stride
        static_cast<void *>(nullptr)    // array buffer offset
        );

        glEnableVertexAttribArray(1); // 2nd Attribute: Color data
        glBindBuffer(GL_ARRAY_BUFFER, cube_uvbuffer);
        glVertexAttribPointer(
        1,          // attribute 0, must match layout in shader
        2,          //size
        GL_FLOAT,   //type
        GL_FALSE,   //normalized?
        0,          //stride
        static_cast<void *>(nullptr)    // array buffer offset
        );

        // use the shader
        glUseProgram(programID);

        // give the shader the cube MVP matrix
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP_Cube[0][0]);

        // Bind the texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // set the Sampler2D in the shader to use this texture
        glUniform1i(TextureID, 0);

        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);


    // close window
    glfwTerminate();

    return 0;
}
