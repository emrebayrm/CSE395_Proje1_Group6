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

class MySceneNode : public scene::ISceneNode
{

	/*
	First, we declare some member variables:
	The bounding box, 4 vertices, and the material of the tetraeder.
	*/
	core::aabbox3d<f32> Box;
	video::S3DVertex Vertices[4];
	video::SMaterial Material;

	/*
	The parameters of the constructor specify the parent of the scene node,
	a pointer to the scene manager, and an id of the scene node.
	In the constructor we call the parent class' constructor,
	set some properties of the material, and
	create the 4 vertices of the tetraeder we will draw later.
	*/

public:

	MySceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
		: scene::ISceneNode(parent, mgr, id)
	{
		Material.Wireframe = false;
		Material.Lighting = false;

		Vertices[0] = video::S3DVertex(0,0,10, 1,1,0,
				video::SColor(255,0,255,255), 0, 1);
		Vertices[1] = video::S3DVertex(10,0,-10, 1,0,0,
				video::SColor(255,255,0,255), 1, 1);
		Vertices[2] = video::S3DVertex(0,20,0, 0,1,1,
				video::SColor(255,255,255,0), 1, 0);
		Vertices[3] = video::S3DVertex(-10,0,-10, 0,0,1,
				video::SColor(255,0,255,0), 0, 0);

	/*
	The Irrlicht Engine needs to know the bounding box of a scene node.
	It will use it for automatic culling and other things. Hence, we
	need to create a bounding box from the 4 vertices we use.
	If you do not want the engine to use the box for automatic culling,
	and/or don't want to create the box, you could also call
	irr::scene::ISceneNode::setAutomaticCulling() with irr::scene::EAC_OFF.
	*/
		Box.reset(Vertices[0].Pos);
		for (s32 i=1; i<4; ++i)
			Box.addInternalPoint(Vertices[i].Pos);
	}

	/*
	Before it is drawn, the irr::scene::ISceneNode::OnRegisterSceneNode()
	method of every scene node in the scene is called by the scene manager.
	If the scene node wishes to draw itself, it may register itself in the
	scene manager to be drawn. This is necessary to tell the scene manager
	when it should call irr::scene::ISceneNode::render(). For
	example, normal scene nodes render their content one after another,
	while stencil buffer shadows would like to be drawn after all other
	scene nodes. And camera or light scene nodes need to be rendered before
	all other scene nodes (if at all). So here we simply register the
	scene node to render normally. If we would like to let it be rendered
	like cameras or light, we would have to call
	SceneManager->registerNodeForRendering(this, SNRT_LIGHT_AND_CAMERA);
	After this, we call the actual
	irr::scene::ISceneNode::OnRegisterSceneNode() method of the base class,
	which simply lets also all the child scene nodes of this node register
	themselves.
	*/
	virtual void OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}

	/*
	In the render() method most of the interesting stuff happens: The
	Scene node renders itself. We override this method and draw the
	tetraeder.
	*/
	virtual void render()
	{
		u16 indices[] = {	0,2,3, 2,1,3, 1,0,3, 2,0,1	};
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setMaterial(Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 4, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
	}

	/*
	And finally we create three small additional methods.
	irr::scene::ISceneNode::getBoundingBox() returns the bounding box of
	this scene node, irr::scene::ISceneNode::getMaterialCount() returns the
	amount of materials in this scene node (our tetraeder only has one
	material), and irr::scene::ISceneNode::getMaterial() returns the
	material at an index. Because we have only one material here, we can
	return the only one material, assuming that no one ever calls
	getMaterial() with an index greater than 0.
	*/
	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return Box;
	}

	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual video::SMaterial& getMaterial(u32 i)
	{
		return Material;
	}	
};
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
                    false, false, false,&eventReceiver);
    if (!device)
        return 1;
    
    device->setWindowCaption(L"Ball and Plate Simulation");

    ISceneManager *smgr  = device->getSceneManager();
    IVideoDriver *driver = device->getVideoDriver();
    IGUIEnvironment* env = device->getGUIEnvironment();
    
    ITexture* images = driver->getTexture("../../media/white.jpg");

    plateModel = smgr->addCubeSceneNode();
    if (plateModel)
    { 
        plateModel->setMaterialTexture(0,driver->getTexture("../../media/wall.jpg"));
        plateModel->setMaterialFlag(EMF_LIGHTING,false);
    }
    ballModel = smgr->addSphereSceneNode();
    if(ballModel){
        ballModel->setMaterialTexture(0,driver->getTexture("../../media/002shot.jpg"));
        ballModel->setMaterialFlag(EMF_LIGHTING,false);
    }
    // Maya 
    camera = smgr->addCameraSceneNodeMaya();
    if (camera)
        camera->setPosition(core::vector3df(20,20,-50));
    
    device->getCursorControl()->setVisible(true);

    while(device->run() && driver)
    {
        
        //driver->setMaterial(SMaterial());
        driver->beginScene(true, true, SColor(255,102,102,0));
        
        driver->setViewPort(rect<s32>(200,100,ResX/2 + 200,ResY/2 + 200));
        
        driver->draw2DImage(images, core::position2d<s32>(0,0),
				core::rect<s32>(0,0,800,800), 0,
				video::SColor(255,255,255,255), true);
        env->drawAll();
       
        
        //Activate camera
        smgr->setActiveCamera(camera);
        smgr->drawAll();

        driver->endScene();
    }

    device->drop();

    return 0;
}