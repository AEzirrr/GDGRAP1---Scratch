#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <cmath>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define pi 3.141592653589793238462643383279502884197

float moveSpeed = 0.001f;
float camSpeed = 0.001f;

std::unordered_map<int, bool> keyStates;

float x_mod = 0;
float y_mod = 0;
float z_mod = 0;

float x_cam = 0;
float y_cam = 0;
float z_cam = 0;

float scaleVal = 1;

float thetaX = 0;
float thetaY = 0;
float thetaZ = 0;

float axisX = 1;
float axisY = 0;

glm::mat4 identity_matrix(1.0);

bool isPressedD, isPressedA, isPressedW, isPressedS, 
isPressedQ, isPressedE, isPressedZ, isPressedX,
isPressedF, isPressedH, isPressedT, isPressedG, isPressedC, isPressedV,
isPressedLEFT, isPressedRIGHT, isPressedUP, isPressedDOWN;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        keyStates[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keyStates[key] = false;
    }
}

// Function to process input
void ProcessInput() {
    // Object movement
    if (keyStates[GLFW_KEY_D]) x_mod += moveSpeed;
    if (keyStates[GLFW_KEY_A]) x_mod -= moveSpeed;
    if (keyStates[GLFW_KEY_W]) y_mod += moveSpeed;
    if (keyStates[GLFW_KEY_S]) y_mod -= moveSpeed;
    if (keyStates[GLFW_KEY_Z]) z_mod -= moveSpeed;
    if (keyStates[GLFW_KEY_X]) z_mod += moveSpeed;

    // Camera movement
    if (keyStates[GLFW_KEY_H]) x_cam += camSpeed;
    if (keyStates[GLFW_KEY_F]) x_cam -= camSpeed;
    if (keyStates[GLFW_KEY_T]) y_cam += camSpeed;
    if (keyStates[GLFW_KEY_G]) y_cam -= camSpeed;
    if (keyStates[GLFW_KEY_C]) z_cam -= camSpeed;
    if (keyStates[GLFW_KEY_V]) z_cam += camSpeed;

    // Scaling
    if (keyStates[GLFW_KEY_Q]) scaleVal -= 0.0001f;
    if (keyStates[GLFW_KEY_E]) scaleVal += 0.0001f;

    // Rotation
    if (keyStates[GLFW_KEY_LEFT]) thetaY -= 0.1f;
    if (keyStates[GLFW_KEY_RIGHT]) thetaY += 0.1f;
    if (keyStates[GLFW_KEY_UP]) thetaX -= 0.1f;
    if (keyStates[GLFW_KEY_DOWN]) thetaX += 0.1f;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float windowWidth = 640;
    float windowHeight = 640;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "Marlou Vincent Ruiz", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    gladLoadGL();

    //          MinX MinY Width Height
    //glViewport(320, 0, 640/2, 480);

    glfwSetKeyCallback(window, Key_Callback);


    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    //Add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();
    ///////////////////////////////////////////
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    //Add the file stream to the string stream
    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();


    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);


    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);


    GLuint shaderProg = glCreateProgram();

    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);


    glLinkProgram(shaderProg);


    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    GLfloat vertices[]{
        //x    y    z
        0.f, 0.5f, 0.f,
        -0.5f, 0.f, 0.f,
        0.5f, 0.f, 0.f
    };

    GLuint indices[]{
        0,1,2
    };

    // ID of VAO, VBO
    GLuint VAO, VBO, EBO;

    //generate VAO and assign the ID to variable
    glGenVertexArrays(1, &VAO);
    //generate VBO and assign the ID to variable
    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);

    //current VAO = null

    glBindVertexArray(VAO); // current VAO = VAO

    //current VBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //current VBO = VBO
    // currVAO.VBO.append(VBO)

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * attributes.vertices.size(), &attributes.vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); //enable attrib index 0

    //current VBO = VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0); //current VBO = null

    //current VAO = VAO
    glBindVertexArray(0); //current VAO = null

    float radius = 0.5f;
    float prevAngle = 0;
    float yOffset = 0.54;

    //projection matrix
    /*glm::mat4 projectionMatrix = glm::ortho(
        -2.0f, //Left most point
        2.0f,  //Right most point
        -2.0f, //Bottom most point
        2.0f,  //Top most point
        -1.0f, //Z near
        1.0f   //Z Far
    );*/


    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.0f), //FOV
        windowHeight / windowWidth, //Aspect Ratio
        0.1f, //Z near should never be <= 0
        100.f //Z far
    );


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        ProcessInput();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //Position Matrix
        glm::vec3 cameraPos = glm::vec3(0 + x_cam, 0 + y_cam, 5.0f + z_cam); //eye
        glm::mat4 cameraPosMatrix = glm::translate(glm::mat4(1.0f), cameraPos * -1.0f);

        //Orientation
        glm::vec3 worldUp = glm::normalize(glm::vec3(0.0f, 1.0f , 0.0f )); //Pointing upwards
        glm::vec3 cameraCenter = glm::vec3(0.0f + x_cam, 0.0f + y_cam, 0 + z_cam);

        //Forward
        glm::vec3 F = cameraCenter - cameraPos;
        F = glm::normalize(F);

        //R = F X WorldUp
        glm::vec3 R = glm::cross(F, worldUp);
        //U = R X F
        glm::vec3 U = glm::cross(R, F);

        glm::mat4 cameraOrientation = glm::mat4(1.);
        //Matrix [Column][Row]
        //Row 1 = R
        cameraOrientation[0][0] = R.x;
        cameraOrientation[1][0] = R.y;
        cameraOrientation[2][0] = R.z;

        //Row 2 = U
        cameraOrientation[0][1] = U.x;
        cameraOrientation[1][1] = U.y;
        cameraOrientation[2][1] = U.z;

        //Row 3 = -F
        cameraOrientation[0][2] = -F.x;
        cameraOrientation[1][2] = -F.y;
        cameraOrientation[2][2] = -F.z;
        //end of Orientation Matrix


        //glm::mat4 viewMatrix = (cameraOrientation * cameraPosMatrix);

        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraCenter, worldUp);


        /*// simple animation
        unsigned int xLoc = glGetUniformLocation(shaderProg, "x");
        glUniform1f(xLoc, x_mod);

        unsigned int yLoc = glGetUniformLocation(shaderProg, "y");
        glUniform1f(yLoc, y_mod);

        unsigned int zLoc = glGetUniformLocation(shaderProg, "z");
        glUniform1f(zLoc, z_mod);
        */

        //translation
        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(x_mod, y_mod, z_mod));

        //scale
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scaleVal, scaleVal, scaleVal));

        //rotation
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(thetaX), glm::vec3(1, 0, 0)); 
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(thetaY), glm::vec3(0, 1, 0)); 
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(thetaZ), glm::vec3(0, 0, 1)); 

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));



        glUseProgram(shaderProg);
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        /*for (int i = 0; i < 8; i++) {

            float angle = 0;

            if (i == 0) {
                angle = (pi / 8.0);
            }
            else {
                angle = prevAngle + (pi / 4.0);
            }
            prevAngle = angle;
            glVertex2f(radius * cos(angle), radius * sin(angle) + yOffset);
        }
        glEnd();*/


        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}