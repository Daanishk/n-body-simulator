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

const double G = 1.0;

class object {
public:
    float radius = 0.5f;
    float PI = 3.141592f;
    float cx = 0.0f;
    float cy = 0.0f;
    float mass;

    std::vector<float> vertices;
    glm::vec3 position = { cx, cy, 0.0f };
    glm::vec3 color = { 1.0f, 0.0f, 0.0f };
    glm::vec3 velocity = glm::vec3(0, 0, 0);

    // Trail Data
    std::vector<glm::vec3> trail;
    //static constexpr int MAX_TRAIL_POINTS = 2000;

    // Each object owns its VAO & VBO
    VAO vao;
    VBO vbo;  

    object()
        : object(0.5f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0) {
    }

    object(float radius_, const glm::vec3& pos_, const glm::vec3& vel_, const glm::vec3& color_, float mass_)
        : radius(radius_), position(pos_), velocity(vel_), color(color_), mass(mass_)
    {
        // 1) build the vertex data on the CPU
        buildSphere();

        // 2) create VBO with our vertex data
        vbo.Init(vertices.data(), vertices.size() * sizeof(float));

        // 3) set up VAO attribute pointers
        vao.Bind();
        vao.linkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);                     // position :contentReference[oaicite:2]{index=2}
        vao.linkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));   // color
        vao.Unbind();
        vbo.Unbind();
    }

    ~object()
    {
        // clean up GPU resources
        vao.Delete();
        vbo.Delete();

    }

    void Render() 
    {
        vao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLint>(vertices.size() / 6));
    }

    void UpdatePos() {
        this->position[0] += this->velocity[0] / 94;
        this->position[1] += this->velocity[1] / 94;
        this->position[2] += this->velocity[2] / 94;
    }

    void accelerate(float x, float y, float z) {
        this->velocity[0] += x / 96;
        this->velocity[1] += y / 96;
        this->velocity[2] += z / 96;
    }

    float collision(object& other) {
        float dx = this->position[0] - other.position[0];
        float dy = this->position[1] - other.position[1];
        float dz = this->position[2] - other.position[2];
        float distance = sqrt(dx * dx + dy * dy + dz * dz);

        if (distance < this->radius + other.radius) {
            return -0.2f;
        }
        return 1.0f;
    }

    void addTrailPoint()
    {
        trail.push_back(position);
        //if (trail.size() > MAX_TRAIL_POINTS) {
        //    trail.erase(trail.begin());
        //}
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
        vertices.insert(vertices.end(), { color.x, color.y, color.z });
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
    GLint lightPosLoc = glGetUniformLocation(shaderProgram.ID, "lightPos");
    GLint lightColorLoc = glGetUniformLocation(shaderProgram.ID, "lightColor");
    GLint emissiveLoc = glGetUniformLocation(shaderProgram.ID, "isEmissive");

    std::vector<object> objects;
    objects.reserve(4);

    //SUN 1
    objects.emplace_back(20.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.95f, 0.8f), 3e10);
    //////SUN 2
    //objects.emplace_back(20.0f, glm::vec3(300, 300.0f, 0.0f), glm::vec3(-20.0f, -5.0f, 0.0f), glm::vec3(1.0f, 0.95f, 0.8f), 10e10);

    //Planet 1 & 2
    objects.emplace_back(8.0f, glm::vec3( 100.0, 0.0, 0.0f), glm::vec3(0.0f, 17.32f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2e9);
    objects.emplace_back(8.0f, glm::vec3( 200.7f, 0.0f, 0.0f), glm::vec3(0.0f, 12.25f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 2e9);
    objects.emplace_back(8.0f, glm::vec3( 300.f, 0.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 2e9);
    //objects.emplace_back(8.0f, glm::vec3( 400.7f, 0.0f, 0.0f), glm::vec3(9.0f, 9.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f), 2e9); 

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 100.0f)); 

    // -------- Trail VAO / VBO setup --------
    GLuint trailVAO, trailVBO;
    glGenVertexArrays(1, &trailVAO);
    glGenBuffers(1, &trailVBO);

    glBindVertexArray(trailVAO);
    glBindBuffer(GL_ARRAY_BUFFER, trailVBO);

    // layout: vec3 position, vec3 color = 6 floats
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
    // ---------------------------------------

        while (!glfwWindowShouldClose(window))
        {
            // Clear the framebuffer and depth buffer 
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shaderProgram.Activate();
            glm::vec3 sunColor = objects[0].color; 
            glUniform3fv(lightColorLoc, 1, glm::value_ptr(sunColor));

            //Update camera input, read keyboard and mouse state to recompute view matrix
            camera.Inputs(window);
            camera.Matrix(45.0f, 0.1f, 10000.f, shaderProgram, "camMatrix");

            glm::vec3 lightPos = objects[0].position;
            glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

            //Compute pairwise gravitational force against every other body
            for (size_t i = 0; i < objects.size(); ++i)
            {
                auto& obj1 = objects[i];
                for (auto& obj2 : objects) {
                    if (&obj2 == &obj1) continue;
                    float dx = obj2.position[0] - obj1.position[0];
                    float dy = obj2.position[1] - obj1.position[1];
                    float dz = obj2.position[2] - obj1.position[2];
                    float distance = sqrt(dx * dx + dy * dy + dz * dz);

                    if (distance > 0) {
                        std::vector<float> direction = { dx / distance, dy / distance, dz / distance };
                        distance *= 1000;
                        double Gforce = (G * obj1.mass * obj2.mass) / (distance * distance);


                        float acc1 = Gforce / obj1.mass;
                        std::vector<float> acc = { direction[0] * acc1, direction[1] * acc1, direction[2] * acc1 };
                        obj1.accelerate(acc[0], acc[1], acc[2]);
                    }

                    obj1.velocity *= obj1.collision(obj2);

                }

                obj1.UpdatePos();
                obj1.addTrailPoint();

                //Build a model matrix for each body to translate sphere into world position
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, obj1.position);  // each object gets its own model matrix
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                if ((i == 0) /*|| (i == 1)*/)
                    glUniform1i(emissiveLoc, GL_TRUE);
                else
                    glUniform1i(emissiveLoc, GL_FALSE);

                obj1.Render();
            }

            // -------- Render trails (for planets only) --------
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindVertexArray(trailVAO);

            for (size_t i = 0; i < objects.size(); ++i)
            {
                auto& obj = objects[i];
                if (obj.trail.size() < 2) continue;

                std::vector<float> trailVerts;
                trailVerts.reserve(obj.trail.size() * 6);

                for (const auto& p : obj.trail)
                {
                    // position
                    trailVerts.push_back(p.x);
                    trailVerts.push_back(p.y);
                    trailVerts.push_back(p.z);
                    // white color
                    trailVerts.push_back(1.0f);
                    trailVerts.push_back(1.0f);
                    trailVerts.push_back(1.0f);
                }

                glBindBuffer(GL_ARRAY_BUFFER, trailVBO);
                glBufferData(GL_ARRAY_BUFFER,
                    trailVerts.size() * sizeof(float),
                    trailVerts.data(),
                    GL_DYNAMIC_DRAW);

                // Identity model for trails, positions already in world space
                glm::mat4 trailModel = glm::mat4(1.0f);
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trailModel));

                glUniform1i(emissiveLoc, GL_TRUE);

                glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLint>(obj.trail.size()));
            }

            glBindVertexArray(0);
            glDisable(GL_BLEND);
            // --------------------------------------------------

            //Finally swapBuffers to push the rendered frame to the screen.
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    // object destructor will delete its VAO/VBO
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
