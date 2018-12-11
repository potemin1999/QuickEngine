#include "Main.h"
#include "Boot.h"
#include "FloorModel.h"
#include "Crate_01.h"
#include "MaxModel.h"

using namespace QE;

int multisampling = 1;
int window_width = 1200;
int window_height = 800;

GLFWwindow *window;
QuickEngine *engine;
double xpos, ypos;
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

//void key_up(int key) {
//    printf("key up: %i\n", key);
//    auto event = new InputEvent();
//    event->keyAction = QE::ACTION_UP;
//    event->keyCode = key;
//    engine->inputManager->injectInputEvent(event);
//    switch (key) {
//        case KEY_ESC: {
//            exit(0xc105ed);
//        }
//        case KEY_BRACKET_RIGHT: {
//            engine->renderer->v_Brightness += 0.1f;
//            break;
//        }
//        case KEY_BRACKET_LEFT: {
//            engine->renderer->v_Brightness -= 0.1f;
//            break;
//        }
//        case KEY_SPACE: {
//            break;
//        }
//        case KEY_W: {
//            break;
//        }
//        case KEY_A: {
//            break;
//        }
//        case KEY_S: {
//            break;
//        }
//        case KEY_D: {
//            break;
//        }
//        default:
//            break;
//    }
//}

//void key_down(int key) {
//    float speed = 1;
//
//    auto event = new InputEvent();
//    event->keyAction = QE::ACTION_DOWN;
//    event->keyCode = key;
//    engine->inputManager->injectInputEvent(event);
//
//
//    switch (key) {
//        case GLFW_KEY_SPACE: {
//            break;
//        }
//        case GLFW_KEY_W: {
//            engine->camera->getAttachedTo()->move(glm::vec3(0, 0, -speed * engine->deltaTime));
//            break;
//        }
//        case KEY_A: {
//            engine->camera->getAttachedTo()->move(glm::vec3(-speed * engine->deltaTime, 0, 0));
//            break;
//        }
//        case KEY_S: {
//            engine->camera->getAttachedTo()->move(glm::vec3(0, 0, speed * engine->deltaTime));
//            break;
//        }
//        case KEY_D: {
//            engine->camera->getAttachedTo()->move(glm::vec3(speed * engine->deltaTime, 0, 0));
//            break;
//        }
//    }
//}

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


        // init camera
        auto p = glm::vec3(0.0f, 2.0f, 0.0f);
        auto v = glm::vec3(0.0f, 0.0f, 1.0f);
        auto u = glm::vec3(0.0f, 1.0f, 0.0f);
        engine->camera->setCameraPosition(p, v, u);
        engine->camera->setTrackVerticalAxis(true);


        engine->resize(window_width, window_height);
        check_gl_error("resize");

        engine->compileShaders();
        check_gl_error("compile shaders");

        // init world
        printf("Gonna create this shitty world\n");

        // add max
        auto m = new MaxModel();
        auto mm = new GameObject();
        mm->mesh_count = m->mesh_count;
        mm->meshes = m->meshes;
        mm->setPos(glm::vec3(0.0f, 2.0f, 0.0f));
        engine->addObject(mm);

        // add floor
        auto n = new FloorModel();
        n->setPos(glm::vec3(0.0f, -1.0f, 0.0f));
        engine->addObject(n);

        // drop some crates
//        int startX = -2, startY = 20, startZ = -2;
//        for (int x = 0; x < 5; x++)
//            for (int y = 0; y < 5; y++)
//                for (int z = 0; z < 5; z++) {
//                    auto crate = new Crate_01();
//                    crate->setPos(glm::vec3(startX + x, startY + y, startZ + z));
//                    engine->addObject(crate);
//                }

        // add crate
        auto c1 = new Crate_01();
        c1->setPos(glm::vec3(0, 1, 0));
        engine->addObject(c1);

        // attach camera to crate
        engine->camera->attachTo(c1);
        engine->camera->setOffsetPos(glm::vec3(0, 2, 0));

        // init input
        class MainInputReceiver : public InputReceiver {
            void onReceiverRegistered() override {
                printf("Yay! InputReceiver registered!\n");
            }

            void onReceiverUnregistered() override {
                printf("Yay! InputReceiver unregistered!\n");
            }

            InputProcessResult onInputEvent(InputEvent *event) override {
                float speed = 2;

                if (event->keyAction == ACTION_UP) {
                    switch (event->keyCode) {
                        case GLFW_KEY_ESCAPE: {
                            exit(0);
                        }
                    }
                }

                if (event->keyAction == ACTION_PRESSED)
                    switch (event->keyCode) {
                        case GLFW_KEY_W: {
                            glm::vec3 dir = glm::conjugate(engine->camera->getQOrientation()) *
                                            glm::vec3(0, 0, -speed * engine->deltaTime);
                            dir.y = 0;
                            dir = glm::normalize(dir) * speed * engine->deltaTime;
                            engine->camera->getAttachedTo()->move(dir);
                            break;
                        }
                        case GLFW_KEY_A: {
                            glm::vec3 dir = glm::conjugate(engine->camera->getQOrientation()) *
                                            glm::vec3(-speed * engine->deltaTime, 0, 0);
                            dir.y = 0;
                            dir = glm::normalize(dir) * speed * engine->deltaTime;
                            engine->camera->getAttachedTo()->move(dir);
                            break;
                        }
                        case GLFW_KEY_S: {
                            glm::vec3 dir = glm::conjugate(engine->camera->getQOrientation()) *
                                            glm::vec3(0, 0, speed * engine->deltaTime);
                            dir.y = 0;
                            dir = glm::normalize(dir) * speed * engine->deltaTime;
                            engine->camera->getAttachedTo()->move(dir);
                            break;
                        }
                        case GLFW_KEY_D: {
                            glm::vec3 dir = glm::conjugate(engine->camera->getQOrientation()) *
                                            glm::vec3(1, 0, 0);
                            dir.y = 0;
                            dir = glm::normalize(dir) * speed * engine->deltaTime;
                            engine->camera->getAttachedTo()->move(dir);
                            break;
                        }
                        case GLFW_KEY_SPACE: {
                            engine->camera->getAttachedTo()->rigidBody->applyForce(btVector3(0, 20, 0),
                                                                                   btVector3(0, 0, 1));
                            break;
                        }
                        default:
                            break;
                    }
            };
        };

        engine->inputManager->registerInputReceiver(new MainInputReceiver);

        printf("Shit created successfully. Enjoy.\n");
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

            glfwGetCursorPos(window, &xpos, &ypos);
            mouseDx = (float) xpos - (window_width * 0.5f);
            mouseDy = (window_height * 0.5f) - (float) ypos;
            engine->camera->addCameraRotation(0.001f * mouseDy, 0.001f * mouseDx, 0);
            glfwSetCursorPos(window, window_width * 0.5f, window_height * 0.5f);
            glfwPollEvents();

            // update camera pos
            engine->camera->setPos(engine->camera->getAttachedTo()->getPos() + engine->camera->getOffsetPos());
            engine->tick(dT);

            glfwSwapBuffers(window);
        }
    } catch (exception &e) {
        printf("FATAL LOOP EXCEPTION : %s", e.what());
    }
}

int main_graphics() {
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
        window_width = vidmode->width;
        window_height = vidmode->height;
        window = glfwCreateWindow(window_width, window_height, "QuickEngine", monitor, NULL);
        //engine->resize(window_width, window_height);
        //camera->updateProjectionMatrix(float(vidmode->width) / float(vidmode->height));
    } else {
        window = glfwCreateWindow(window_width, window_height, "QuickEngine", NULL, NULL);
        //engine->resize(window_width, window_height);
        //camera->updateProjectionMatrix(float(window_width) / float(window_height));
    }
    printf("width: %i\nheight: %i\n", window_width, window_height);

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

int run_engine() {
    return main_graphics();
}
