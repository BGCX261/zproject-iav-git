

#include "Zombie.h"

//-------------------------------------------------------------------------------------
Zombie::Zombie(Ogre::String model, Ogre::Real initX, Ogre::Real initZ, Ogre::Real sp)
{
	Ogre::SceneManager* mSceneMgr = Ogre::Root::getSingleton().getSceneManager("ingameManager");

	entity = mSceneMgr->createEntity(model);
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(initX, 0, initZ));
	node->attachObject(entity);

	speed = sp;
	translateVector = Ogre::Vector3(0, 0, 0);
}

//-------------------------------------------------------------------------------------
Zombie::~Zombie(void)
{
}

//-------------------------------------------------------------------------------------
void Zombie::move(Ogre::Real axisX, Ogre::Real axisZ)
{
	translateVector.x = axisX * speed;
	translateVector.z = axisZ * speed;
}

//-------------------------------------------------------------------------------------
void Zombie::update(const Ogre::FrameEvent& evt)
{
	node->translate(translateVector * evt.timeSinceLastFrame, Ogre::Node::TS_WORLD);
}


