#include <GLFW/glfw3.h>
//#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <cmath>
void makeCircle(float cx, float cy, float r, float res);

double G = 6.6743e-11;
double c = 299792458.0;

struct Engine {
    GLFWwindow* window;
    float WIDTH = 800.0;
    float HEIGHT = 600.0;
    float width = 1e11;
    float height = 7.5e10;

    Engine() {
        if (!glfwInit())
            std::cerr << "glfw init failed" << std::endl;

        window = glfwCreateWindow(800, 600, "Blackhole", NULL, NULL);
        if (!window) {
            glfwTerminate();
            std::cerr << "window creation failed" << std::endl;
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        glViewport(0, 0, WIDTH, HEIGHT);
    }

    void run() {
        float left = -width;
        float right = width;
        float bottom = -height;
        float top = height;

        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(left, right, bottom, top, -1.0, 1.0);  // pixel coords

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    }
};
Engine engine;

struct object {
    std::vector<float> postion;
    std::vector<float> velocity;
    float radius = 50.0;
    float mass;

    object(std::vector<float> &pos, std::vector<float> &vel, float r, float m): postion(pos), velocity(vel), radius(r), mass(m) {

    }
    void makeCircle() {
        glBegin(GL_TRIANGLE_FAN);
        glColor3b()
        glVertex2d(position[0], cy);

        for (int i = 0; i <= res; ++i) {
            float angle = 2.0 * 3.141592 * (i / res);
            float x = cx + cos(angle) * r;
            float y = cy + sin(angle) * r;
            glVertex2d(x, y);
        }

        glEnd();
    }
};

int main(void)
{
    
    while (!glfwWindowShouldClose(engine.window)) {
        //makeCircle(center[0], center[1], radius, resolution);

        //center[0] += velocity[0];
        //center[1] += velocity[1];

        //velocity[1] -= 9.8 / 20.0f;  
        ////velocity[0] -= 9.8 / 20.0f;

        //if ((center[1] < radius) || (center[1] > engine.screenHeight - radius)) {
        //    velocity[1] *= -0.95;
        //}

        //if ((center[0] < radius) || (center[0] > engine.screenWidth -radius)) {
        //    velocity[0] *= -0.95;
        //}
        engine.run();

        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }

    glfwDestroyWindow(engine.window);
    glfwTerminate();
    return 0;
}
