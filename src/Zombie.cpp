

#include "Zombie.h"

//-------------------------------------------------------------------------------------
Zombie::Zombie(Ogre::String model, Ogre::Real initX, Ogre::Real initZ, Ogre::Real sp)
{
	Ogre::SceneManager* mSceneMgr = Ogre::Root::getSingleton().getSceneManager("ingameManager");
	
	// Zombie Entity
	entity = mSceneMgr->createEntity(model);
	// bounding box
	Ogre::AxisAlignedBox box = entity->getBoundingBox();
	// Zombie Node
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	// Node Position (relative to bounding box Left-Bottom)
	node->setPosition(initX, -box.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM).y, initZ);
	// attach to node
	node->attachObject(entity);

	speed = sp;
	translateVector = Ogre::Vector3(0, 0, 0);

	// start ALIVE !
	live = true;
	hunger = 0.5;
}

//-------------------------------------------------------------------------------------
Zombie::~Zombie(void)
{
}

//-------------------------------------------------------------------------------------
void Zombie::move(Ogre::Real axisX, Ogre::Real axisZ)
{
	/*double Bx = axisX - node->getPosition().x;
	double Bz = axisZ - node->getPosition().z;

	double Vx = Ogre::Math::Cos(node->getOrientation().getYaw()); 
	double Vz = Ogre::Math::Sin(node->getOrientation().getYaw()); 

	Ogre::Radian angle = Ogre::Math::ACos(Ogre::Math::Abs(Bx*Vx + Bz*Vz) / (Ogre::Math::Sqrt(Bx*Bx + Bz*Bz) + (Ogre::Math::Sqrt(Vx*Vx + Vz*Vz))));*/

	Ogre::Radian angle = Ogre::Math::ATan2(axisZ - node->getPosition().z, axisX - node->getPosition().x);

	node->setOrientation(Ogre::Quaternion(-angle, Ogre::Vector3::UNIT_Y));
	translateVector.x = speed;
	translateVector.z = 0;
}

//-------------------------------------------------------------------------------------
void Zombie::update(const Ogre::FrameEvent& evt)
{
	if(live){
		node->translate(translateVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	} else{
		node->yaw(Ogre::Degree(1)); // simulate the dead turning
	}
}

// Kill the zombie
void Zombie::kill(){
	live = false;
}

// Check if zombie is live or dead
bool Zombie::isLive(){
	return live;
}


