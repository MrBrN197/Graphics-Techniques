
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(){

    if(!glfwInit()){
        std::cout <<"Failed To Initialize GLFW!" << std::endl;
        return 0;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    int STATUS = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if(!STATUS){
        std::cout << "Failed To Initialize Glad!" << std::endl;
        glfwTerminate();
        return 0;
    }

    while(!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();

}