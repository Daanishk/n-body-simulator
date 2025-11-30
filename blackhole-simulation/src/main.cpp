#include<filesystem>
namespace fs = std::filesystem;

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>

#include"../Header Files/Texture.h"
#include"../Header Files/shaderClass.h"
#include"../Header Files/VAO.h"
#include"../Header Files/VBO.h"
#include"../Header Files/EBO.h"
#include"../Header Files/Camera.h"


const unsigned int width = 800;
const unsigned int height = 800;

class object {
public:
    float radius = 0.5f;
    float PI = 3.141592f;
    float cx = 0.0f;
    float cy = 0.0f;

    std::vector<float> vertices;
    glm::vec3 position = { cx, cy, 0.0f };
    std::vector<float> color = { 1.0f, 0.0f, 0.0f };

    // Each object owns its VAO & VBO
    VAO vao;
    VBO* vbo;   // pointer so we can construct after vertices are generated

    object()
    {
        // 1) build the vertex data on the CPU
        buildSphere();

        // 2) create VBO with our vertex data
        vbo = new VBO(vertices.data(), vertices.size() * sizeof(float));  // :contentReference[oaicite:1]{index=1}

        // 3) set up VAO attribute pointers
        vao.Bind();
        vao.linkAttrib(*vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);                     // position :contentReference[oaicite:2]{index=2}
        vao.linkAttrib(*vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));   // color
        vao.Unbind();
        vbo->Unbind();
    }

    ~object()
    {
        // clean up GPU resources
        vao.Delete();
        vbo->Delete();
        delete vbo;
    }

    void Render() 
    {
        vao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLint>(vertices.size() / 6));
    }

private:
    void buildSphere()
    {
        int stacks = 10;
        int sectors = 10;

        for (int i = 0; i <= stacks; ++i) {
            float theta1 = (float(i) / stacks) * glm::pi<float>();
            float theta2 = float(i + 1) / stacks * glm::pi<float>();

            for (int j = 0; j < sectors; ++j) {
                float phi1 = float(j) / sectors * 2 * glm::pi<float>();
                float phi2 = float(j + 1) / sectors * 2 * glm::pi<float>();

                std::vector<float> v1 = getCoord(theta1, phi1);
                std::vector<float> v2 = getCoord(theta1, phi2);
                std::vector<float> v3 = getCoord(theta2, phi1);
                std::vector<float> v4 = getCoord(theta2, phi2);

                // Triangle 1: v1-v2-v3
                appendVertex(v1);
                appendVertex(v2);
                appendVertex(v3);

                // Triangle 2: v2-v4-v3
                appendVertex(v2);
                appendVertex(v4);
                appendVertex(v3);
            }
        }
    }

    std::vector<float> getCoord(float theta, float phi)
    {
        float y = radius * cos(theta);
        float r = radius * sin(theta);
        float x = r * cos(phi);
        float z = r * sin(phi);
        return { x, y, z };
    }

    void appendVertex(const std::vector<float>& pos)
    {
        // pos = x,y,z then color = r,g,b
        vertices.insert(vertices.end(), pos.begin(), pos.end());
        vertices.insert(vertices.end(), color.begin(), color.end());
    }
};


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "grav", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, width, height);

    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");
    GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");

    // *** HERE: OpenGL is ready, so we can safely create our object ***
    object o1;

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f)); 

        while (!glfwWindowShouldClose(window))
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shaderProgram.Activate();

            camera.Inputs(window);
            camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, o1.position);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            // Render our object (binds its VAO and draws)
            o1.Render();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    // object destructor will delete its VAO/VBO
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}