/*
-----------------------------------------------------------------------------
Filename:    Zproyect.h
-----------------------------------------------------------------------------


This source file is generated by the
   ___                   _              __    __ _                  _ 
  /___\__ _ _ __ ___    /_\  _ __  _ __/ / /\ \ (_)______ _ _ __ __| |
 //  // _` | '__/ _ \  //_\\| '_ \| '_ \ \/  \/ / |_  / _` | '__/ _` |
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \  /\  /| |/ / (_| | | | (_| |
\___/ \__, |_|  \___| \_/ \_/ .__/| .__/ \/  \/ |_/___\__,_|_|  \__,_|
      |___/                 |_|   |_|                                 
      Ogre 1.7.x Application Wizard for VC10 (July 2011)
      http://code.google.com/p/ogreappwizards/
-----------------------------------------------------------------------------
*/
#ifndef __Zproyect_h_
#define __Zproyect_h_

#include "BaseApplication.h"

#include "Zombie.h"
#include "Enemy.h"
#include "CameraMan.h"
#include "UnitMovModel.h"

#include "time.h"


class Zproyect : public BaseApplication
{
public:
	Zproyect(void);
	virtual ~Zproyect(void);

protected:
	virtual void createScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	// OIS::KeyListener
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );

	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	
	// Camara:
	Ogre::SceneNode* cameraNode;
	CameraMan* cameraMan;

	// Nodo para la banderita de seleccion
	Ogre::SceneNode* banderaNode;
	// plano para acceder a el globalmente
	Ogre::Plane plane;

	// Zombies
	int nZombies;
	Zombie** zombies;
	UnitMovModel* zombiesMovementModel;

	// Enemies:
	Enemy* enemy;

	// *** MOC Setup *** Collision
	MOC::CollisionTools* mCollisionTools;
	// Pick an Entity
	void pickEntity(const OIS::MouseEvent &arg, const Ogre::uint32 queryMask = 0xFFFFFFFF);
	// *****************


};

#endif // #ifndef __Zproyect_h_
