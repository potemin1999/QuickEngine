#include "Player.h"

#include <ModelLoader.h>
#include "Boot.h"
#include "Crate_01.h"
#include "Crate_02.h"
#include "FloorModel.h"

Player::Player(World *world) : GameObject(world) {
    // create portable world
    portableWorld = new World(1);
    engine->addWorld(portableWorld);
    // create floor in second world
    for (int x = -5; x <= 5; x++)
        for (int z = -5; z <= 5; z++) {
            auto n = new FloorModel(portableWorld);
            n->setPos(glm::vec3(x * 6, 0.0f, z * 6));
            portableWorld->addObject(n);
        }

    for (int y = 0; y < 50; y++) {
        auto n = new Crate_02(portableWorld);
        n->setPos(glm::vec3(0, 50 + y * 2, 0));
        portableWorld->addObject(n);
    }


    ModelLoader::load_object("", "crate_01.obj");

    glm::tquat rot = this->getRotation();
    btQuaternion rotation(rot.x, rot.y, rot.z, rot.w);

    glm::vec3 pos = this->getPos();
    btVector3 position = btVector3(pos.x, pos.y, pos.z);

    // init construction info here
    btDefaultMotionState *motionState = new btDefaultMotionState(btTransform(rotation, position));
    btCollisionShape *shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));

    btScalar bodyMass = 1;
    btVector3 bodyInertia;
    shape->calculateLocalInertia(bodyMass, bodyInertia);

    btRigidBody::btRigidBodyConstructionInfo info(bodyMass, motionState, shape, bodyInertia);

    info.m_restitution = 0.5f;
    info.m_friction = 0.5f;

    rigidBody = new btRigidBody(info);


    class MainInputReceiver : public InputReceiver {
        void onReceiverRegistered() override {
        }

        void onReceiverUnregistered() override {
        }

        InputProcessResult onKeyInputEvent(KeyInputEvent *event) override {
            float playerSpeed = 2;

            if (event->keyAction == ACTION_RELEASE) {
                switch (event->keyCode) {
                    case GLFW_KEY_ESCAPE: {
                        exit(0);
                    }
                    case GLFW_KEY_LEFT_ALT: {
                        engine->lockCursor = !engine->lockCursor;
                        // return mouse to center on cursor relock to prevent rotation jump
                        if (engine->lockCursor)
                            glfwSetCursorPos(window, engine->windowWidth * 0.5f, engine->windowHeight * 0.5f);
                        break;
                    }
                    case GLFW_KEY_TAB: {
                        auto player = (Player *) (engine->camera->getAttachedTo());
                        player->isInSecondWorld = !player->isInSecondWorld;
                        if (player->isInSecondWorld) {
                            player->moveToWorld(engine->getWorld(1));
                            engine->renderer->attachWorld(engine->getWorld(1));
                        } else {
                            player->moveToWorld(engine->getWorld(0));
                            engine->renderer->attachWorld(engine->getWorld(0));
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            if (event->keyAction == ACTION_PRESSED)
                switch (event->keyCode) {
                    case GLFW_KEY_W: {
                        glm::vec3 dir = glm::conjugate(engine->camera->getQOrientation()) *
                                        glm::vec3(0, 0, -playerSpeed * engine->deltaTime);
                        dir.y = 0;
                        dir = glm::normalize(dir) * playerSpeed * engine->deltaTime;
                        engine->camera->getAttachedTo()->move(dir);
                        break;
                    }
                    case GLFW_KEY_A: {
                        glm::vec3 dir = glm::conjugate(engine->camera->getQOrientation()) *
                                        glm::vec3(-playerSpeed * engine->deltaTime, 0, 0);
                        dir.y = 0;
                        dir = glm::normalize(dir) * playerSpeed * engine->deltaTime;
                        engine->camera->getAttachedTo()->move(dir);
                        break;
                    }
                    case GLFW_KEY_S: {
                        glm::vec3 dir = glm::conjugate(engine->camera->getQOrientation()) *
                                        glm::vec3(0, 0, playerSpeed * engine->deltaTime);
                        dir.y = 0;
                        dir = glm::normalize(dir) * playerSpeed * engine->deltaTime;
                        engine->camera->getAttachedTo()->move(dir);
                        break;
                    }
                    case GLFW_KEY_D: {
                        glm::vec3 dir = glm::conjugate(engine->camera->getQOrientation()) *
                                        glm::vec3(1, 0, 0);
                        dir.y = 0;
                        dir = glm::normalize(dir) * playerSpeed * engine->deltaTime;
                        engine->camera->getAttachedTo()->move(dir);
                        break;
                    }
                    case GLFW_KEY_SPACE: {
                        engine->camera->getAttachedTo()->rigidBody->applyForce(btVector3(0, 20, 0),
                                                                               btVector3(0, 0, 0));
                        break;
                    }
                    case GLFW_KEY_R: {
                        glm::vec3 startPos = engine->camera->getAttachedTo()->getPos() + engine->camera->getOffsetPos();
                        glm::vec3 forward = engine->camera->getForward();

                        World *world = engine->camera->getAttachedTo()->getWorld();
                        auto crate = new Crate_01(world);
                        crate->setPos(startPos + forward);
                        crate->rigidBody->applyForce(btVector3(forward.x, forward.y, forward.z) * 10000,
                                                     btVector3(0, 0, 0));
                        world->addObject(crate);

                        break;
                    }
                    default:
                        break;
                }

            return InputProcessResult{INPUT_PROCESSED};
        }

    public:
        InputProcessResult onCharInputEvent(CharInputEvent *event) override {
            return INPUT_NOT_PROCESSED;
        }

    private:

        InputProcessResult onMouseInputEvent(MouseInputEvent *event) override {
            return INPUT_NOT_PROCESSED;
        };
    };
    engine->inputManager->registerInputReceiver(new MainInputReceiver);

}

void Player::moveToWorld(World *world) {
    this->getWorld()->removeObject(this);
    world->addObject(this);
}
