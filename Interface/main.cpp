#include <irrlicht.h>
#include <iosfwd>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

void movePlate();


//Resolution
const int ResX = 1366;
const int ResY = 768;
// Full Screen
bool fullScreen = false;

IrrlichtDevice *device;
IMeshSceneNode *plateModel;
IMeshSceneNode *ballModel;
ICameraSceneNode *camera = 0;

class MyEventReceiver : public IEventReceiver {
public:

    virtual bool OnEvent(const SEvent& event) {
        //Key W enables/disables SplitScreen
        if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                event.KeyInput.Key == KEY_KEY_W && event.KeyInput.PressedDown) {
            vector3df pos = ballModel->getPosition();
            vector3df posRot = ballModel->getRotation();
            vector3df plateRot = plateModel->getRotation();
            ballModel->setPosition(vector3df(pos.X, pos.Y + 0.5, pos.Z));
            ballModel->setRotation(vector3df(posRot.X+5, posRot.Y, posRot.Z));
            plateModel->setRotation(vector3df(plateRot.X+1, plateRot.Y, plateRot.Z));
            return true;
        }
            //Key A enables/disables SplitScreen
        else if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                event.KeyInput.Key == KEY_KEY_A && event.KeyInput.PressedDown) {
            vector3df pos = ballModel->getPosition();
            vector3df posRot = ballModel->getRotation();
            vector3df plateRot = plateModel->getRotation();
            ballModel->setPosition(vector3df(pos.X - 0.5, pos.Y, pos.Z));
            ballModel->setRotation(vector3df(posRot.X, posRot.Y+5, posRot.Z));
            plateModel->setRotation(vector3df(plateRot.X, plateRot.Y+1, plateRot.Z));
            return true;
        }            //Key S enables/disables SplitScreen
        else if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                event.KeyInput.Key == KEY_KEY_S && event.KeyInput.PressedDown) {
            vector3df pos = ballModel->getPosition();
            vector3df posRot = ballModel->getRotation();
            vector3df plateRot = plateModel->getRotation();
            ballModel->setPosition(vector3df(pos.X, pos.Y - 0.5, pos.Z));
            ballModel->setRotation(vector3df(posRot.X-5, posRot.Y, posRot.Z));
            plateModel->setRotation(vector3df(plateRot.X-1, plateRot.Y, plateRot.Z));
            return true;
        }            //Key D enables/disables SplitScreen
        else if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                event.KeyInput.Key == KEY_KEY_D && event.KeyInput.PressedDown) {
            vector3df pos = ballModel->getPosition();
            vector3df posRot = ballModel->getRotation();
            vector3df plateRot = plateModel->getRotation();
            ballModel->setPosition(vector3df(pos.X + 0.5, pos.Y, pos.Z));
            ballModel->setRotation(vector3df(posRot.X, posRot.Y-5, posRot.Z));
            plateModel->setRotation(vector3df(plateRot.X, plateRot.Y-1, plateRot.Z));
            return true;
        }
    }
};

int main() {
    MyEventReceiver eventReceiver;
    device = createDevice(video::EDT_OPENGL, dimension2d<u32>(ResX, ResY), 16,
            fullScreen, false, false, &eventReceiver);

    if (!device)
        return 1;

    device->setWindowCaption(L"Ball and Plate Simulation");

    ISceneManager *smgr = device->getSceneManager();
    IVideoDriver *driver = device->getVideoDriver();
    IGUIEnvironment* env = device->getGUIEnvironment();

    env->addButton(rect<s32>(10, 240, 110, 240 + 32), 0, 0,
            L"Connect", L"Try to Connect Device");

    //Load model
    ballModel = smgr->addSphereSceneNode();
    
    ballModel->setPosition(vector3df(ballModel->getPosition().X, ballModel->getPosition().Y, ballModel->getPosition().Z-2));
    
    plateModel = smgr->addCubeSceneNode();
    
    vector3df plateScale= plateModel->getScale();
    //std::cout<<plateModel->getPosition().X << plateModel->getPosition().Y << plateModel->getPosition().Z;
    
    
  //  ballModel->addChild(plateModel);
    
   // plateModel->setVisible(false);

    
    plateModel->setScale(vector3df(plateScale.X*8,plateScale.Y*6, plateScale.Z/4));
    
     ballModel->setScale(ballModel->getScale()/4);
  

    if (plateModel) {
        plateModel->setMaterialTexture(0,driver->getTexture("../../media/rockwall.jpg"));
        plateModel->setMaterialFlag(EMF_LIGHTING, false);
    }

    if (ballModel){
        ballModel->setMaterialTexture(0,driver->getTexture("../../media/terrain-texture.jpg"));
        ballModel->setMaterialFlag(EMF_LIGHTING, false);
    }
        

    camera = smgr->addCameraSceneNode();
    camera->setFOV(64);
    if (camera)
        camera->setPosition(core::vector3df(0, -60, -80));

    device->getCursorControl()->setVisible(true);


    while (device->run()) {

        driver->setViewPort(rect<s32>(0, 0, ResX, ResY));
        driver->beginScene(true, true, SColor(255, 100, 100, 100));
        smgr->setActiveCamera(camera);
        
        
        env->drawAll();
        smgr->drawAll();

        if (fullScreen)
            driver->setViewPort(rect<s32>(ResX / 2, ResY / 2, ResX, ResY));

        //Activate camera
        smgr->setActiveCamera(camera);
        
        
       // movePlate();
        

        // plateModel->setScale(plateModel->getScale() + 0.001);

        // ballModel->setPosition(core::vector3df(ballModel->getPosition().X+0.001, ballModel->getPosition().Y, ballModel->getPosition().Z));
        driver->endScene();
    }

    device->drop();

    return 0;
}

void movePlate(){
    
    vector3df rote = plateModel->getRotation();
    
    if(rote.X <360){
        plateModel->setRotation(vector3df(rote.X+0.5, rote.Y+0.5, rote.Z));
      //  std::cout << rote.X << std::endl;
    }else{
        
    }
    
    

        
    
}

/*
That's it. Compile and run.
 **/