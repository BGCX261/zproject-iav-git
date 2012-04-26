

#include "Zombie.h"

//-------------------------------------------------------------------------------------
Zombie::Zombie(Ogre::String model, Ogre::Real initX, Ogre::Real initZ, Ogre::Real sp, Ogre::Real sptr)
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

	/*// get animation:
	anim_walk = entity->getAnimationState("my_animation");
	anim_walk->setEnabled(true);
	anim_walk->setLoop(true);*/

	speed = sp;
	speedTurn = sptr;
	angleTurn = 0;
	translateVector = Ogre::Vector3(0, 0, 0);
	

	// start ALIVE !
	live = true;
	hunger = 0.6;
}

//-------------------------------------------------------------------------------------
Zombie::~Zombie(void)
{
}

//-------------------------------------------------------------------------------------
void Zombie::move(Ogre::Real axisX, Ogre::Real axisZ)
{
	angleTurn = - Ogre::Math::ATan2(axisZ - node->getPosition().z, axisX - node->getPosition().x);
	turning = true;

	translateVector.x = speed;
	
	headingTo.x = axisX;
	headingTo.z = axisZ;
}

//-------------------------------------------------------------------------------------
void Zombie::update(const Ogre::FrameEvent& evt)
{
	if(live){

		Ogre::Radian actualBearing = node->getOrientation().getYaw();;

	    	// If we are still turning we have to update the orientation:
	    	if (turning)
		{
			// Avoid to rotate using the longest path:
			if(Ogre::Math::Abs(actualBearing.valueRadians() - angleTurn.valueRadians()) > Ogre::Math::PI)
			{
		    		if(angleTurn > actualBearing)
				{
		        		actualBearing += Ogre::Radian(Ogre::Math::PI * 2);
		    		}else
				{
		       			actualBearing -= Ogre::Radian(Ogre::Math::PI * 2);
		    		}
			}
		
			// If we are about to complete the turning we force it to be sure it reaches th exact amount:
			if (Ogre::Math::Abs(actualBearing.valueRadians() - angleTurn.valueRadians()) < 0.08726) {
			    actualBearing = angleTurn;
			    turning = false;
			}
		
			// Update the actual orientation:
			actualBearing = actualBearing + (angleTurn - actualBearing) / (speedTurn / evt.timeSinceLastFrame);
			    
			// Apply the turn on the node:
			node->setOrientation(Ogre::Quaternion(actualBearing, Ogre::Vector3(0, 1, 0)));
		}

		node->translate(translateVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		//anim_walk->addTime(evt.timeSinceLastFrame);
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


