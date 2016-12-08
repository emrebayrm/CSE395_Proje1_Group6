/*
 * Ball and Plate Group 6
 */

#include <irrlicht.h>
#include <iosfwd>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <unistd.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

enum {
    GUI_ID_DIALOG_ROOT_WINDOW = 0x10000,

    GUI_ID_X_SCALE,
    GUI_ID_Y_SCALE,
    GUI_ID_Z_SCALE,

    GUI_ID_OPEN_MODEL,
    GUI_ID_SET_MODEL_ARCHIVE,
    GUI_ID_LOAD_AS_OCTREE,

    GUI_ID_SKY_BOX_VISIBLE,
    GUI_ID_TOGGLE_DEBUG_INFO,

    GUI_ID_DEBUG_OFF,
    GUI_ID_DEBUG_BOUNDING_BOX,
    GUI_ID_DEBUG_NORMALS,
    GUI_ID_DEBUG_SKELETON,
    GUI_ID_DEBUG_WIRE_OVERLAY,
    GUI_ID_DEBUG_HALF_TRANSPARENT,
    GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES,
    GUI_ID_DEBUG_ALL,

    GUI_ID_MODEL_MATERIAL_SOLID,
    GUI_ID_MODEL_MATERIAL_TRANSPARENT,
    GUI_ID_MODEL_MATERIAL_REFLECTION,

    GUI_ID_CAMERA_MAYA,
    GUI_ID_CAMERA_FIRST_PERSON,

    GUI_ID_POSITION_TEXT,

    GUI_ID_ABOUT,
    GUI_ID_QUIT,

    GUI_ID_TEXTUREFILTER,
    GUI_ID_SKIN_TRANSPARENCY,
    GUI_ID_SKIN_ANIMATION_FPS,

    GUI_ID_BUTTON_SET_SCALE,
    GUI_ID_BUTTON_SCALE_MUL10,
    GUI_ID_BUTTON_SCALE_DIV10,
    GUI_ID_BUTTON_OPEN_MODEL,
    GUI_ID_BUTTON_SHOW_ABOUT,
    GUI_ID_BUTTON_SHOW_TOOLBOX,
    GUI_ID_BUTTON_SELECT_ARCHIVE,

    GUI_ID_ANIMATION_INFO,

    // And some magic numbers
    MAX_FRAMERATE = 80,
    DEFAULT_FRAMERATE = 30
};

enum {
    DOWN = 101,
    UP,
    LEFT,
    RIGHT,
    TEST,
    GUI_ID_TRANSPARENCY_SCROLL_BAR
};

struct SAppContext {
    IrrlichtDevice *device;
    s32 counter;
    IGUIListBox* listbox;
};

// Resolution
const int ResX = 1366;
const int ResY = 768;
// Full Screen
bool fullScreen = false;

IrrlichtDevice *device;
ICameraSceneNode *camera = 0;

// Main scene node
ISceneNode *sceneNode;

// Plate node control to collision
ISceneNode *emtyPlateNode;
// Plate node
ISceneNode *plateModel;
// Ball node
ISceneNode *ballModel;

ISceneNode *plateModelTwo;
ISceneNode *poleNode;
ISceneNode *jointNode;

int key = 1;

class Angles {
public:

    double ballx;
    double bally;
    double motorx;
    double motory;

    Angles(double ballx1, double bally1, double motorx1, double motory1) {
        ballx = ballx1;
        bally = bally1;
        motorx = motorx1;
        motory = motory1;
    };

};

class MyEventReceiver : public IEventReceiver {
public:

    MyEventReceiver(SAppContext & context) : Context(context) {
    }
public:

    virtual bool OnEvent(const SEvent& event) {
        if (event.EventType == EET_GUI_EVENT) {
            s32 id = event.GUIEvent.Caller->getID();
            IGUIEnvironment* env = Context.device->getGUIEnvironment();

            switch (event.GUIEvent.EventType) {
                case EGET_SCROLL_BAR_CHANGED:
                    if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR) {

                    }
                    break;

                case EGET_BUTTON_CLICKED:
                    switch (id) {
                        case DOWN:
                        {
                            //Serial Communication i�in down buttonu.
                            cout << "down pressed\n";
                        }
                            return true;

                        case UP:
                        {
                            cout << "up pressed\n";
                        }
                            return true;

                        case TEST:
                        {
                            key = 0;
                            cout << "test pressed\n";
                        }
                            return true;

                        case LEFT:
                        {
                            //Serial Communication i�in Left buttonu.
                            cout << "left pressed\n";
                        }
                            return true;
                        case RIGHT:
                        {
                            //Serial Communication i�in right buttonu.
                            cout << "right pressed\n";
                        }
                            return true;

                        default:
                            return false;
                    }
                    break;

                case EGET_FILE_SELECTED:
                {
                    // show the model filename, selected in the file dialog
                    IGUIFileOpenDialog* dialog =
                            (IGUIFileOpenDialog*) event.GUIEvent.Caller;
                    Context.listbox->addItem(dialog->getFileName());
                }
                    break;

                default:
                    break;
            }
        } else if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                event.KeyInput.Key == KEY_KEY_W && event.KeyInput.PressedDown) {

            vector3df pos = ballModel->getPosition();
            vector3df posRot = ballModel->getRotation();
            vector3df plateRot = emtyPlateNode->getRotation();
            ballModel->setRotation(vector3df(posRot.X - 5, posRot.Y, posRot.Z));
            ballModel->setPosition(vector3df(pos.X, pos.Y + 1, pos.Z));
            emtyPlateNode->setRotation(vector3df(plateRot.X + 1, plateRot.Y, plateRot.Z));

            return true;
        }//Key A enables/disables SplitScreen
        else if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                event.KeyInput.Key == KEY_KEY_A && event.KeyInput.PressedDown) {

            vector3df pos = ballModel->getPosition();
            vector3df posRot = ballModel->getRotation();
            vector3df plateRot = emtyPlateNode->getRotation();
            ballModel->setPosition(vector3df(pos.X - 1, pos.Y, pos.Z));
            ballModel->setRotation(vector3df(posRot.X, posRot.Y + 5, posRot.Z));
            emtyPlateNode->setRotation(vector3df(plateRot.X, plateRot.Y + 1, plateRot.Z));

            return true;
        }//Key S enables/disables SplitScreen
        else if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                event.KeyInput.Key == KEY_KEY_S && event.KeyInput.PressedDown) {

            vector3df pos = ballModel->getPosition();
            vector3df posRot = ballModel->getRotation();
            vector3df plateRot = emtyPlateNode->getRotation();
            ballModel->setPosition(vector3df(pos.X, pos.Y - 1, pos.Z));
            ballModel->setRotation(vector3df(posRot.X + 5, posRot.Y, posRot.Z));
            emtyPlateNode->setRotation(vector3df(plateRot.X - 1, plateRot.Y, plateRot.Z));

            return true;

        }//Key D enables/disables SplitScreen
        else if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                event.KeyInput.Key == KEY_KEY_D && event.KeyInput.PressedDown) {

            vector3df pos = ballModel->getPosition();
            vector3df posRot = ballModel->getRotation();
            vector3df plateRot = emtyPlateNode->getRotation();
            ballModel->setPosition(vector3df(pos.X + 1, pos.Y, pos.Z));
            ballModel->setRotation(vector3df(posRot.X, posRot.Y - 5, posRot.Z));
            emtyPlateNode->setRotation(vector3df(plateRot.X, plateRot.Y - 1, plateRot.Z));

            return true;

        } else {
            return false;
        }
    }
private:
    SAppContext & Context;
};


// Functions
vector<Angles> returnAngles();
void drawControlPanel();


// Main

int main() {

    device = createDevice(video::EDT_OPENGL, dimension2d<u32>(ResX, ResY));
    SAppContext context;
    context.device = device;
    context.counter = 0;

    // Then create the event receiver, giving it that context structure.
    MyEventReceiver eventReceiver(context);

    device = createDevice(video::EDT_OPENGL, dimension2d<u32>(ResX, ResY), 16,
            fullScreen, false, false, &eventReceiver);
    if (!device)
        return 1;

    device->setWindowCaption(L"Ball and Plate Simulation");

    ISceneManager *smgr = device->getSceneManager();
    IVideoDriver *driver = device->getVideoDriver();
    IGUIEnvironment* env = device->getGUIEnvironment();

    drawControlPanel();

    emtyPlateNode = smgr->addEmptySceneNode();
    sceneNode = smgr->addEmptySceneNode();

    sceneNode->addChild(emtyPlateNode);

    // Plate node
    plateModel = smgr-> addCubeSceneNode();
    if (plateModel) {
        plateModel->setMaterialTexture(0, driver->getTexture("./Textures/plate_texture.jpg"));
        plateModel->setMaterialFlag(EMF_LIGHTING, false);

    }
    vector3df plateScale = plateModel->getScale();
    plateModel->setScale(vector3df(plateScale.X * 8, plateScale.Y * 6, plateScale.Z / 16));

    emtyPlateNode->addChild(plateModel);

    // Ball node
    ballModel = smgr->addSphereSceneNode();
    if (ballModel) {
        ballModel->setMaterialTexture(0, driver->getTexture("./Textures/ball_texture.jpg"));
        ballModel->setMaterialFlag(EMF_LIGHTING, false);
    }
    vector3df ballScale = ballModel->getScale();
    ballModel->setScale(ballScale / 2);

    emtyPlateNode->addChild(ballModel);
    ballModel->setPosition(vector3df(ballModel->getPosition().X, ballModel->getPosition().Y, ballModel->getPosition().Z - 2 * ballScale.Z));


    plateModelTwo = smgr-> addCubeSceneNode();
    if (plateModelTwo) {
        plateModelTwo->setMaterialTexture(0, driver->getTexture("./Textures/red_texture.jpg"));
        plateModelTwo->setMaterialFlag(EMF_LIGHTING, false);

    }
    vector3df plateSca = plateModelTwo->getScale();
    plateModelTwo->setScale(vector3df(plateSca.X * 8, plateSca.Y * 6, plateSca.Z / 4));
    plateModelTwo->setScale(plateModelTwo->getScale()*1.2);

    plateModelTwo->setPosition(vector3df(plateModelTwo->getPosition().X, plateModelTwo->getPosition().Y, plateModelTwo->getPosition().Z + 50));

    sceneNode->addChild(plateModelTwo);


    poleNode = smgr-> addCubeSceneNode();
    if (poleNode) {
        poleNode->setMaterialTexture(0, driver->getTexture("./Textures/metal_texture.jpg"));
        poleNode->setMaterialFlag(EMF_LIGHTING, false);

    }
    vector3df plateS = poleNode->getScale();
    poleNode->setScale(vector3df(plateS.X / 2, plateS.Y / 2, plateS.Z * 4));

    poleNode->setPosition(vector3df(poleNode->getPosition().X, poleNode->getPosition().Y, poleNode->getPosition().Z + 25));

    sceneNode->addChild(poleNode);


    jointNode = smgr->addSphereSceneNode();
    if (jointNode) {
        jointNode->setMaterialTexture(0, driver->getTexture("./Textures/metal_texture.jpg"));
        jointNode->setMaterialFlag(EMF_LIGHTING, false);

    }
    vector3df plateS2 = jointNode->getScale();
    jointNode->setScale(vector3df(plateS2.X / 2, plateS2.Y / 2, plateS2.Z / 2));

    jointNode->setPosition(vector3df(jointNode->getPosition().X, jointNode->getPosition().Y, jointNode->getPosition().Z + 3));

    emtyPlateNode->addChild(jointNode);

    // Add camera
    camera = smgr->addCameraSceneNode();

    if (camera)
        camera->setPosition(core::vector3df(0, -100, -10));

    device->getCursorControl()->setVisible(true);

    // Main scene node
    sceneNode->setPosition(vector3df(sceneNode->getPosition().X - 20, sceneNode->getPosition().Y, sceneNode->getPosition().Z + 80));

    sceneNode->setScale(sceneNode->getScale()*1.5);

    // To testing
    vector<Angles> angles = returnAngles();
    int i = 0;

    while (device->run()) {

        driver->setViewPort(rect<s32>(0, 0, ResX, ResY));
        driver->beginScene(true, true, SColor(255, 100, 100, 100));
        smgr->setActiveCamera(camera);

        // To testing
        if (key == 0 && !angles.empty()) {
            if (i < angles.size()) {
                vector3df pos = ballModel->getPosition();
                vector3df posRot = ballModel->getRotation();
                ballModel->setPosition(vector3df(angles[i].ballx, angles[i].bally, pos.Z));
                for (int j = 0; j < 100000000; ++j);
                ballModel->setRotation(vector3df(posRot.X + 5, posRot.Y, posRot.Z));
                ++i;
            }
        }

        env->drawAll();
        smgr->drawAll();

        //Activate camera
        smgr->setActiveCamera(camera);

        driver->endScene();

    }

    device->drop();

    return 0;
}

vector<Angles> returnAngles() {

    vector<Angles> V;
    double a, b, c, d;
    FILE *inp;

    inp = fopen("writeFile.txt", "r");

    if (inp == NULL) {
        return V;
    }

    int status;
    do {
        status = fscanf(inp, "%lf%lf%lf%lf", &a, &b, &c, &d);
        V.push_back(Angles(a, b, c, d));
    } while (EOF != status);

    fclose(inp);

    return V;
}

void drawControlPanel() {

    // remove tool box if already there
    IGUIEnvironment* env = device->getGUIEnvironment();
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
    if (e)
        e->remove();

    // create the toolbox window
    IGUIWindow* wnd = env->addWindow(core::rect<s32>(1000, 300, 1300, 700),
            false, L"Control Panel", 0, GUI_ID_DIALOG_ROOT_WINDOW);

    // create tab control and tabs
    IGUITabControl* tab = env->addTabControl(
            core::rect<s32>(0, 20, 400, 480), wnd, true, true);

    IGUITab* t1 = tab->addTab(L"Connection");
    IGUITab* t2 = tab->addTab(L"Rotating");
    IGUITab* t3 = tab->addTab(L"Special Movements");

    // add some edit boxes and a button to tab one
    env->addButton(core::rect<s32>(110, 90, 185, 134), t1, 0, L"Connection");
    env->addButton(core::rect<s32>(110, 144, 185, 188), t1, 0, L"Disconnection");
    env->addButton(core::rect<s32>(120, 50, 164, 94), t2, UP, L"UP");
    env->addButton(core::rect<s32>(120, 150, 164, 194), t2, DOWN, L"DOWN");
    env->addButton(core::rect<s32>(180, 100, 224, 144), t2, RIGHT, L"RIGHT");
    env->addButton(core::rect<s32>(65, 100, 109, 144), t2, LEFT, L"LEFT");
    env->addButton(core::rect<s32>(100, 20, 175, 54), t3, 0, L"Rectangle");
    env->addButton(core::rect<s32>(100, 64, 175, 98), t3, 0, L"Infinity");
    env->addButton(core::rect<s32>(100, 108, 175, 142), t3, 0, L"Star");
    env->addButton(core::rect<s32>(100, 152, 175, 186), t3, 0, L"Triangle");
    env->addButton(core::rect<s32>(100, 196, 175, 230), t3, 0, L"Square");
    env->addButton(core::rect<s32>(100, 235, 175, 269), t3, TEST, L"Test");
    env->addStaticText(L"Enter New Center:", core::rect<s32>(22, 275, 110, 289), false, false, t3);
    env->addEditBox(L"", core::rect<s32>(105, 275, 195, 289), true, t3, GUI_ID_Y_SCALE);
}