#include <irrlicht.h>

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

//Resolution
const int ResX=800;
const int ResY=600;
// Full Screen
bool fullScreen=false;

IrrlichtDevice *device;
IMeshSceneNode *plateModel;
IMeshSceneNode *ballModel;
ICameraSceneNode *camera = 0;

class MyEventReceiver : public IEventReceiver
{
    public:
        virtual bool OnEvent(const SEvent& event)
        {
            //Key W enables/disables SplitScreen
            if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                    event.KeyInput.Key == KEY_KEY_W && event.KeyInput.PressedDown)
            {       
                vector3df pos = ballModel->getPosition();
                ballModel->setPosition(vector3df(pos.X , pos.Y + 1,pos.Z));
                return true;
            }
            
            //Key A enables/disables SplitScreen
            else if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                    event.KeyInput.Key == KEY_KEY_A && event.KeyInput.PressedDown)
            {       
                vector3df pos = ballModel->getPosition();
                ballModel->setPosition(vector3df(pos.X - 1, pos.Y,pos.Z));
                return true;
            }
            //Key S enables/disables SplitScreen
            else if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                    event.KeyInput.Key == KEY_KEY_S && event.KeyInput.PressedDown)
            {       
                vector3df pos = ballModel->getPosition();
                ballModel->setPosition(vector3df(pos.X , pos.Y- 1,pos.Z));
                return true;
            }
            //Key D enables/disables SplitScreen
            else if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                    event.KeyInput.Key == KEY_KEY_D && event.KeyInput.PressedDown)
            {       
                vector3df pos = ballModel->getPosition();
                ballModel->setPosition(vector3df(pos.X + 1, pos.Y ,pos.Z));
                return true;
            }   
        }
};

int main()
{
    MyEventReceiver eventReceiver;
    device = createDevice( video::EDT_OPENGL, dimension2d<u32>(ResX,ResY), 16,
                    fullScreen, false, false,&eventReceiver);

    if (!device)
        return 1;
    
    device->setWindowCaption(L"Ball and Plate Simulation");

    ISceneManager *smgr  = device->getSceneManager();
    IVideoDriver *driver = device->getVideoDriver();
    IGUIEnvironment* env = device->getGUIEnvironment();
    
    env->addButton(rect<s32>(10,240,110,240 + 32), 0, 0,
                    L"Connect", L"Try to Connect Device");    
    
    //Load model
    plateModel = smgr->addCubeSceneNode();
    if (plateModel)
    {  
        plateModel->setMaterialTexture(0,driver->getTexture("../../media/faerie2.bmp"));
        plateModel->setMaterialFlag(EMF_LIGHTING,false);
    }
    ballModel = smgr->addSphereSceneNode();
    if(ballModel)
        ballModel->setMaterialFlag(EMF_LIGHTING,false);

    camera = smgr->addCameraSceneNodeMaya();
    if (camera)
        camera->setPosition(core::vector3df(0,0,-50));
    
    device->getCursorControl()->setVisible(true);

    
    while(device->run())
    {
        
        driver->setViewPort(rect<s32>(0,0,ResX/2,ResY/2));
        driver->beginScene(true, true, SColor(255,100,100,100));
        smgr->setActiveCamera(camera);
        env->drawAll();
        smgr->drawAll();
        
        if (fullScreen)
            driver->setViewPort(rect<s32>(ResX/2,ResY/2,ResX,ResY));
        
        //Activate camera
        smgr->setActiveCamera(camera);

        driver->endScene();
    }

    device->drop();

    return 0;
}

/*
That's it. Compile and run.
**/
