#include <glad/glad.h>
#include <glfw3.h>
#define STB_IMAGE_IMPLEMENTATION 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>
#include <model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Teapot Universe", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

    // load models
    // -----------
    Model ourModel("teapot/Chaynik.obj");
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  -0.1f,  0.2f),
        glm::vec3(2.0f,  5.2f, 5.0f),
        glm::vec3(-3.8f, -1.0f, -2.3f),
        glm::vec3(2.4f, -4.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.5f,  -2.0f, -2.5f),
        glm::vec3(7.5f,  -4.2f, -1.5f),
        glm::vec3(-1.3f,  -1.0f, -0.5f),
        glm::vec3(-6.5f, -2.2f, 2.5f),
        glm::vec3(-2.3f,  1.2f, -1.8f),
        glm::vec3(-2.8f, -1.0f, -2.3f),
        glm::vec3(2.8f, 0.4f, -1.3f),
        glm::vec3(1.8f,  2.0f, 1.3f),
        glm::vec3(5.3f, -2.65f, -2.5f),
        glm::vec3(-3.0f,  2.4f, -7.0f),
        glm::vec3(3.7f,  -2.4f, -1.0f),
        glm::vec3(12.4f, 0.94f, -3.6f),
        glm::vec3(-11.7f,  -3.7f, -6.5f),
        glm::vec3(2.75f,  2.78f, 2.5f),
        glm::vec3(2.25f,  -3.2f, 4.5f),
        glm::vec3(6.0f,  7.0f,  5.0f),
        glm::vec3(2.9f,  -5.9f, -15.9f),
        glm::vec3(-15.8f, 12.0f, 2.3f),
        glm::vec3(12.4f, -10.4f, -16.5f),
        glm::vec3(-11.7f,  13.0f, -17.5f),
        glm::vec3(11.5f,  2.8f, -2.5f),
        glm::vec3(1.1f,  -1.2f,  6.7f),
        glm::vec3(2.4f,  5.4f, 5.0f),
        glm::vec3(-3.4f, -9.2f, -2.1f),
        glm::vec3(2.2f, -8.9f, -3.8f),
        glm::vec3(-1.2f,  3.7f, 7.5f),
        glm::vec3(0.77f,  -1.7f,  1.77f),
        glm::vec3(2.9f,  5.72f, 5.0f),
        glm::vec3(-3.8f, 4.78f, -2.3f),
        glm::vec3(6.45f, -4.1f, -3.95f),
        glm::vec3(-3.45f,  3.6f, -7.95f),
        glm::vec3(1.9f,  7.60f, -2.95f),
        glm::vec3(7.5f,  -4.92f, 1.95f),
    };

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.13f, 0.11f, 0.41f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        for (unsigned int i = 0; i < 38; i++)
        {
        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]); // translate it down so it's at the center of the scene
        float angle = 20.0f * i;
        if (i % 1 == 0)  // every 1ST iteration (including the first) we set the angle using GLFW's time function.
        angle = glfwGetTime() * 15.0f;
        if (i %4 == 0)  // every 4TH iteration (including the first) we set the angle using GLFW's time function.
        angle = glfwGetTime() * 45.0f;
        if (i % 5 == 0)  // every 5TH iteration (including the first) we set the angle using GLFW's time function.
        angle = glfwGetTime() * 65.0f;
        if (i % 7 == 0)  // every 7TH iteration (including the first) we set the angle using GLFW's time function.
        angle = glfwGetTime() * 35.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));	// it's a bit too small for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

        }
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}