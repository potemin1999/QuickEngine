#include "Boot.h"
#include "Main.h"

using namespace QE;

int multisampling = 1;

GLFWwindow *window;
QuickEngine *engine;
bool fullscreen = false;
bool lock_mouse = false;


void check_gl_error(const char *tag) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("!!! ERROR %s : %i \n", tag, error);
    }
}

void error(int error, const char *description) {
    printf("error : %i \n %s \n", error, description);
}

void resize(GLFWwindow *w, int width, int height) {
    engine->resize(width, height);
}

void key(GLFWwindow *window, int key, int scancode, int action, int mods) {
    switch (action) {
        case 1: {
            engine->inputManager->injectInputEvent(new InputEvent{key, ACTION_DOWN});
            break;
        }
        case 0: {
            engine->inputManager->injectInputEvent(new InputEvent{key, ACTION_UP});
            break;
        }
        default:
            break;
    }
}

void init() {
    try {
        printf("initializing engine...\n");
        engine->init();
        check_gl_error("init");

        engine->resize(engine->windowWidth, engine->windowHeight);
        check_gl_error("resize");

        engine->compileShaders();
        check_gl_error("compile shaders");

        // init game itself
        onInit();
    } catch (exception &e) {
        printf("FATAL INIT EXCEPTION : %s\n", e.what());
    }
}

void loop() {
    try {
        float lastTime = 0;
        float currentTime = 0;
        float dT = 0;
        float mouseDx = 0;
        float mouseDy = 0;
        while (!glfwWindowShouldClose(window)) {
            dT = (currentTime = float(glfwGetTime())) - lastTime;
            lastTime = currentTime;

            glfwGetCursorPos(window, &engine->mouseX, &engine->mouseY);
            mouseDx = (float) engine->mouseX - (engine->windowWidth * 0.5f);
            mouseDy = (engine->windowHeight * 0.5f) - (float) engine->mouseY;

            engine->camera->addCameraRotation(0.001f * mouseDy, 0.001f * mouseDx, 0);
            glfwSetCursorPos(window, engine->windowWidth * 0.5f, engine->windowHeight * 0.5f);
            glfwPollEvents();

            // loop game itself
            onTick(dT);

            glfwSwapBuffers(window);
        }
    } catch (exception &e) {
        printf("FATAL LOOP EXCEPTION : %s", e.what());
    }
}

int mainGraphics() {
    if (!glfwInit()) {
        return -1;
    }
    engine = QuickEngine::getInstance();

    glfwDefaultWindowHints();

    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    //glfwWindowHint(GLFW_RED_BITS, 16);
    //glfwWindowHint(GLFW_GREEN_BITS, 16);
    //glfwWindowHint(GLFW_BLUE_BITS, 16);
    //glfwWindowHint(GLFW_REFRESH_RATE, 200);

    ilInit();
    //iluInit();
    //ilSetInteger(IL_KEEP_DXTC_DATA, IL_TRUE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);

    if (fullscreen) {
        engine->windowWidth = vidmode->width;
        engine->windowHeight = vidmode->height;
        window = glfwCreateWindow(engine->windowWidth, engine->windowHeight, "QuickEngine", monitor, NULL);
        //engine->resize(window_width, window_height);
        //camera->updateProjectionMatrix(float(vidmode->width) / float(vidmode->height));
    } else {
        window = glfwCreateWindow(engine->windowWidth, engine->windowHeight, "QuickEngine", NULL, NULL);
        //engine->resize(window_width, window_height);
        //camera->updateProjectionMatrix(float(window_width) / float(window_height));
    }
    printf("width: %i\nheight: %i\n", engine->windowWidth, engine->windowHeight);

    if (!window) {
        printf("Couldn't create window");
        glfwTerminate();

        return WINDOW_CREATING_ERROR;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("glad is not loaded!\n");
        return -1;
    } else
        printf("OpenGL %d.%d loaded\n", GLVersion.major, GLVersion.minor);

    glfwSetWindowSizeCallback(window, resize);
    glfwSetKeyCallback(window, key);
    glfwSetErrorCallback(error);
    glfwShowWindow(window);
    if (lock_mouse)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    try {
        init();
        loop();
    } catch (exception &e) {
        printf("%s\n", e.what());
    }

    delete engine;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

int runEngine() {
    return mainGraphics();
}

int main(int argc, char **args) {
    return runEngine();
}