

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
	translateVector = Ogre::Vector3(0, 0, 0);

	// start ALIVE !
	live = true;
	hunger = 0.75;
}

//-------------------------------------------------------------------------------------
Zombie::~Zombie(void)
{
}

//-------------------------------------------------------------------------------------
void Zombie::move(Ogre::Real axisX, Ogre::Real axisZ)
{
	Ogre::Radian angle = Ogre::Math::ATan2(axisZ - node->getPosition().z, axisX - node->getPosition().x);

	node->setOrientation(Ogre::Quaternion(-angle, Ogre::Vector3::UNIT_Y));

	translateVector.x = speed;
	
	headingTo.x = axisX;
	headingTo.z = axisZ;
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


