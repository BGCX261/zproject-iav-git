/*
-----------------------------------------------------------------------------
Filename:    Zproyect.cpp
-----------------------------------------------------------------------------


-----------------------------------------------------------------------------
*/

#include "Zproyect.h"

//-------------------------------------------------------------------------------------
Zproyect::Zproyect(void)
{
}
//-------------------------------------------------------------------------------------
Zproyect::~Zproyect(void)
{
}

//-------------------------------------------------------------------------------------
void Zproyect::createScene(void)
{
	srand(time(0));

	// Create the camera for map navigation:
	cameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cameraNode", Ogre::Vector3(0, 35, 50));
	cameraNode->attachObject(mCamera);
	cameraMan = new CameraMan(cameraNode, Ogre::Vector3::ZERO ,15);

	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
		
	// Create a plane for terrain (with texture)
	plane.redefine(Ogre::Vector3::UNIT_Y, Ogre::Vector3(0,0,0) );
		Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	plane, 300, 300, 1, 1, true, 1, 12, 12, Ogre::Vector3::UNIT_Z);

    	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
    	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);

    	entGround->setMaterialName("GroundMat");
    	entGround->setCastShadows(false);


	// SkyBox with skydom
	mSceneMgr->setSkyDome(true, "CloudySky", 5, 8);


	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);


	// Create the zombies
	nZombies = 20;
	zombies = new Zombie*[nZombies];
	for (int i = 0; i < nZombies; i++) {
		char aux[20];
		sprintf(aux, "Zombie%li.mesh", random()%2+1);
		zombies[i] = new Zombie(Ogre::String(aux), rand() % nZombies, rand() % nZombies, 2, 3);	
	}
	//zombiesMovementModel = new UnitMovModelRandom();
	zombiesMovementModel = new UnitMovModelRBSFlock(30, 5);

	enemy = new Enemy(Ogre::String("robot.mesh"), 10, -33, 2, 1);


	// --------------------- Pruebas --------------------------------

	// Banderita selection with Ray
	Ogre::Entity* banderaEntity = mSceneMgr->createEntity("Banderita", "banderita.mesh");
	banderaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BanderitaNode",Ogre::Vector3(0,-20,0));
	banderaNode->attachObject(banderaEntity);
	banderaNode->roll(Ogre::Degree(-90));		// Redefine Banderita position (fuck Blender-Ogre exporter)
	banderaNode->pitch(Ogre::Degree(90));

	// Bunker
	Ogre::Entity* bunkerEntity = mSceneMgr->createEntity("Bunker", "bunker.mesh");
	Ogre::AxisAlignedBox bunkerBox = bunkerEntity->getBoundingBox();	// bounding box
	Ogre::SceneNode* bunkerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	bunkerNode->scale(5,5,6);
	bunkerNode->yaw(Ogre::Degree(90) );

	// Node Position (relative to bunker bounding box Left-Bottom)
	bunkerNode->setPosition(10, -bunkerBox.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM).y*5, -40);
	bunkerNode->attachObject(bunkerEntity);


}

//-----------------------------------------------------------------------------------------------
bool Zproyect::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	zombiesMovementModel->preProcess(evt.timeSinceLastFrame);

	cameraMan->update(evt);

	// We now move all the units, according to the movement model:
	for (int i = 0; i < nZombies; i++) {

		double x, z;
		if (zombiesMovementModel->calculateMove(zombies, nZombies, i, banderaNode->getPosition(), &x, &z))
			zombies[i]->move(x, z);

		zombies[i]->update(evt);


		// enable shoot animation when a zombie is near to robotNode - Kill the zombie		
		if( (zombies[i]->node->getPosition().distance( enemy->node->getPosition() ) <= 25 )&&( zombies[i]->isLive() ) ){
			enemy->shoot = true;
			zombies[i]->kill();
		}
	}

	zombiesMovementModel->postProcess();

	enemy->update(evt);

	return ret;
}

bool Zproyect::keyPressed( const OIS::KeyEvent &arg )
{
	bool ret = BaseApplication::keyPressed(arg);

	cameraMan->keyPressed(arg);
	
	return ret;
}

bool Zproyect::keyReleased( const OIS::KeyEvent &arg )
{
	bool ret = BaseApplication::keyReleased(arg);

	cameraMan->keyReleased(arg);

	return ret;
}

bool Zproyect::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	bool ret = BaseApplication::mousePressed(arg,id);

	// si boton izquierdo mouse
	if (arg.state.buttonDown(OIS::MB_Left))
	{
		// Rayo atraves del viewport
		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay((float)arg.state.X.abs/arg.state.width, (float)arg.state.Y.abs/arg.state.height );
	
		// Calcular interseccion con plano y obtener el punto 3D
		std::pair<bool, Ogre::Real> resultRay =  mouseRay.intersects( plane );	
		Ogre::Vector3 pointRay = mouseRay.getPoint(resultRay.second);
		// Colocar banderita en el punto
		banderaNode->setPosition(pointRay.x,4,pointRay.z);
	}
	

	return ret;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		Zproyect app;

		try {
			app.go();
		} catch( Ogre::Exception& e ) {
					#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
						MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
					#else
						std::cerr << "An exception has occured: " <<
							e.getFullDescription().c_str() << std::endl;
					#endif
		}

		return 0;
    	}

#ifdef __cplusplus
}
#endif
