
#include "Enemy.h"

//-------------------------------------------------------------------------------------
Enemy::Enemy(Ogre::String model, Ogre::Real initX, Ogre::Real initZ, Ogre::Real sp, Ogre::Real sptr)
{
	Ogre::SceneManager* mSceneMgr = Ogre::Root::getSingleton().getSceneManager("ingameManager");
	
	// Enemy Entity
	entity = mSceneMgr->createEntity(model);
	entity->setQueryFlags(ENEMY_MASK);
	// bounding box
	box = entity->getBoundingBox();
	// Enemy Node
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	// Node Position (relative to bounding box Left-Bottom)
	//node->showBoundingBox(true);
	node->setPosition(initX, -box.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM).y, initZ);
	// attach to node
	node->attachObject(entity);

	speed = sp;
	speedTurn = sptr;
	angleTurn = 0;
	translateVector = Ogre::Vector3(0, 0, 0);
	
	// start ALIVE !
	live = true;

	// Aux
	shoot = false;
	patrol = false;
	seek = true;

	//Animations
	//robotAnimState_idle = entity->getAnimationState("Idle");
	robotAnimState_shoot = entity->getAnimationState("shoot");

	//robotAnimState_idle->setEnabled(true);
	//robotAnimState_idle->setLoop(true);

	robotAnimState_shoot->setEnabled(true);
	robotAnimState_shoot->setLoop(true);
}

//-------------------------------------------------------------------------------------
Enemy::~Enemy(void)
{
}

//-------------------------------------------------------------------------------------
void Enemy::move(Ogre::Real axisX, Ogre::Real axisZ)
{
	// Just turn:
	//angleTurn = - Ogre::Math::ATan2(axisZ - node->getPosition().z, axisX - node->getPosition().x);
	//turning = true;

	//translateVector.x = speed;
	
	//headingTo.x = axisX;
	//headingTo.z = axisZ;
}

//-------------------------------------------------------------------------------------
void Enemy::update(const Ogre::FrameEvent& evt)
{
	if(live){
		
	    	// If we are still turning we have to update the orientation:
	    	if (seek)
		{
			//robotAnimState_idle->addTime(2*evt.timeSinceLastFrame);
			node->yaw(Ogre::Radian(evt.timeSinceLastFrame*speedTurn));
		} 
		else if(shoot)
		{

			robotAnimState_shoot->addTime(evt.timeSinceLastFrame);

			shoot = false;
			seek = true;

		}

	} else{
		node->yaw(Ogre::Degree(1)); // simulate the dead turning
	}
}

// Kill the Enemy
void Enemy::kill(){
	live = false;
}

void Enemy::fire(){
	seek = false;
	shoot = true;
}

// Check if Enemy is live or dead
bool Enemy::isLive(){
	return live;
}


