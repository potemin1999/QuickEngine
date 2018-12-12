#include "Boot.h"
#include "Main.h"
#include "FloorModel.h"
#include "Crate_01.h"
#include "MaxModel.h"
#include "NowhereSign_01.h"
#include "Player.h"

//#include "nanogui/nanogui.h"

//nanogui::Screen *screen = nullptr;

std::string strval = "A string";
bool b = true;
World *world1;

void createObjects() {
    // create floor
    for (int x = -5; x <= 5; x++)
        for (int z = -5; z <= 5; z++) {
            auto n = new FloorModel(world1);
            n->setPos(glm::vec3(x * 6, 0.0f, z * 6));
            world1->addObject(n);
        }

    // drop some crates
    int startX = 0, startY = 20, startZ = 15;
    for (int x = 0; x < 1; x++)
        for (int y = 0; y < 20; y++)
            for (int z = 0; z < 1; z++) {
                auto crate = new Crate_01(world1);
                crate->setPos(glm::vec3(startX + x, startY + y, startZ + z));
                world1->addObject(crate);
            }


    // add nowhere sign
    auto nowhereSign = new NowhereSign_01(world1);
    nowhereSign->setPos(glm::vec3(3, 0, 0));
    world1->addObject(nowhereSign);


    auto player = new Player(world1);
    player->setPos(glm::vec3(0, 1, 0));
    world1->addObject(player);

    // attach camera to player
    engine->camera->attachTo(player);
    engine->camera->setOffsetPos(glm::vec3(0, 1.7, 0));
}

void initCamera() {
    auto pos = glm::vec3(0.0f, 2.0f, 0.0f);
    auto view = glm::vec3(0.0f, 0.0f, 1.0f);
    auto up = glm::vec3(0.0f, 1.0f, 0.0f);
    engine->camera->setCameraPosition(pos, view, up);
    engine->camera->setTrackVerticalAxis(true);
}

void initGUI() {
//    screen = new nanogui::Screen(window);
//    auto gui = new nanogui::FormHelper(screen);
//    nanogui::ref<nanogui::Window> window1 = gui->addWindow(Eigen::Vector2i(10, 10), "Form helper example");
//    gui->addGroup("Basic types");
//    gui->addVariable("String", strval, true);
//    gui->addVariable("Boolean", b);
//    gui->addGroup("Other widgets");
//    gui->addButton("A button", []() {
//        printf("Gonna output text\n");
//        printf("Button pressed. String value: %s\n", strval.c_str());
//    });
//
//    class GUIInputReceiver : public InputReceiver {
//    public:
//        void onReceiverRegistered() override {
//            printf("Registered GUI input receiver\n");
//        }
//
//        void onReceiverUnregistered() override {
//            printf("Unregistered GUI input receiver\n");
//        }
//
//        InputProcessResult onKeyInputEvent(KeyInputEvent *event) override {
//            screen->keyCallbackEvent(event->keyCode, event->keyCode, event->keyAction, event->mods);
//
//            return INPUT_PROCESSED;
//        }
//
//        InputProcessResult onCharInputEvent(CharInputEvent *event) override {
//            screen->charCallbackEvent(event->codepoint);
//
//            return INPUT_PROCESSED;
//        }
//
//        InputProcessResult onMouseInputEvent(MouseInputEvent *event) override {
//            if (event->action != ACTION_PRESSED)
//                screen->mouseButtonCallbackEvent(event->button, event->action, event->mods);
//
//            return INPUT_PROCESSED;
//        }
//    };
//
//    engine->inputManager->registerInputReceiver(new GUIInputReceiver);
//
//    screen->setVisible(true);
//    screen->performLayout();
//    window1->center();
}

void onInit() {
    initGUI();
    initCamera();

    // init world
    world1 = new World(0);
    engine->addWorld(world1);
    engine->renderer->attachWorld(world1);
    createObjects();

    printf("Game initialized.\n");
}

void onTick(float deltaTime) {
    float mouseDx = (float) engine->mouseX - (engine->windowWidth * 0.5f);
    float mouseDy = (engine->windowHeight * 0.5f) - (float) engine->mouseY;

    if (engine->lockCursor) {
        engine->camera->addCameraRotation(0.001f * mouseDy, 0.001f * mouseDx, 0);
        glfwSetCursorPos(window, engine->windowWidth * 0.5f, engine->windowHeight * 0.5f);
    }

    // update camera pos
    engine->camera->setPos(engine->camera->getAttachedTo()->getPos() + engine->camera->getOffsetPos());
    engine->tick(deltaTime);

//    screen->cursorPosCallbackEvent(engine->mouseX, engine->mouseY);
//    screen->drawAll();
}