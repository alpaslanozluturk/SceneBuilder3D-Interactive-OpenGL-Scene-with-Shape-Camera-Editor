#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/scene.hpp"
#include "../include/camera.hpp"
#include "../include/transform.hpp"
#include <iostream>

// Global değişkenler
float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;
graf::Scene* scene = nullptr;

// Mouse callback fonksiyonu
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    graf::Camera* camera = scene->getActiveCamera();
    if (camera)
    {
        camera->turn(xoffset);  // Yatay dönüş
        camera->look(yoffset);  // Dikey bakış
    }
}

// Klavye işleme fonksiyonu
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    graf::Camera* camera = scene->getActiveCamera();
    if (camera)
    {
        float moveSpeed = 0.05f;
        graf::Transform* transform = camera->getTransform();
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            glm::vec3 pos = transform->getPosition();
            glm::vec3 look = transform->getLook();
            transform->setPosition(pos + look * moveSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            glm::vec3 pos = transform->getPosition();
            glm::vec3 look = transform->getLook();
            transform->setPosition(pos - look * moveSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            glm::vec3 pos = transform->getPosition();
            glm::vec3 right = transform->getRight();
            transform->setPosition(pos - right * moveSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            glm::vec3 pos = transform->getPosition();
            glm::vec3 right = transform->getRight();
            transform->setPosition(pos + right * moveSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            glm::vec3 pos = transform->getPosition();
            glm::vec3 up = transform->getUp();
            transform->setPosition(pos + up * moveSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            glm::vec3 pos = transform->getPosition();
            glm::vec3 up = transform->getUp();
            transform->setPosition(pos - up * moveSpeed);
        }
    }
}

// Pencere boyutu değiştiğinde çağrılacak fonksiyon
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // GLFW başlatma
    if (!glfwInit())
    {
        std::cerr << "GLFW başlatılamadı!" << std::endl;
        return -1;
    }

    // OpenGL versiyonu ve profil ayarları
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Pencere oluşturma
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Oyun", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "GLFW penceresi oluşturulamadı!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD başlatma
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "GLAD başlatılamadı!" << std::endl;
        return -1;
    }

    // Viewport ayarı
    glViewport(0, 0, 800, 600);

    // Callback fonksiyonlarını ayarla
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR,  GLFW_CURSOR_NORMAL);

    // Scene oluştur
    try {
        scene = new graf::Scene();
        scene->createGameField(10, 10, 1.0f); // 10x10 oyun alanı
    }
    catch (const std::exception& e) {
        std::cerr << "Scene oluşturma hatası: " << e.what() << std::endl;
        return -1;
    }

    // OpenGL ayarları
    glEnable(GL_DEPTH_TEST);

    // Ana döngü
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Scene'i render et
        scene->render();

        // Buffer swap ve event check
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Temizlik
    delete scene;
    glfwTerminate();
    return 0;
}