#include "Boot.h"
#include "Main.h"
#include "FloorModel.h"
#include "Crate_01.h"
#include "MaxModel.h"

void onInit() {
// init camera
    auto p = glm::vec3(0.0f, 2.0f, 0.0f);
    auto v = glm::vec3(0.0f, 0.0f, 1.0f);
    auto u = glm::vec3(0.0f, 1.0f, 0.0f);
    engine->camera->setCameraPosition(p, v, u);
    engine->camera->setTrackVerticalAxis(true);

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
                    default:
                        break;
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

            return InputProcessResult{INPUT_PROCESSED};
        };
    };

    engine->inputManager->registerInputReceiver(new MainInputReceiver);

    printf("Shit created successfully. Enjoy.\n");
}

void onTick(float deltaTime) {
    // update camera pos
    engine->camera->setPos(engine->camera->getAttachedTo()->getPos() + engine->camera->getOffsetPos());
    engine->tick(deltaTime);
}