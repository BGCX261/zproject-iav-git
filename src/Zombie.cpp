
#include "Zombie.h"

//-------------------------------------------------------------------------------------
Zombie::Zombie(Ogre::String model, int gr, int ind, Ogre::Real initX, Ogre::Real initZ, Ogre::Real sp, Ogre::Real sptr)
{
	Ogre::SceneManager* mSceneMgr = Ogre::Root::getSingleton().getSceneManager("ingameManager");
	
	// Zombie Entity
	group = gr;
	individual = ind;
	char aux[20];
	sprintf(aux, "Z-%d-%d", group, individual);

	entity = mSceneMgr->createEntity(Ogre::String(aux), model);
	entity->setQueryFlags(ZOMBIE_MASK);			// add the mask to entity
	// bounding box
	Ogre::AxisAlignedBox box = entity->getBoundingBox();
	// Zombie Node
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	// Node Position (relative to bounding box Left-Bottom)
	//node->showBoundingBox(true);
	node->setPosition(initX, -box.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM).y, initZ);
	// attach to node
	node->attachObject(entity);

	// Set lifebar
	lifebar = mSceneMgr->createEntity("Vida.mesh");
	lifebar->setQueryFlags(OTHER_MASK);
	lifebarNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	lifebarNode->attachObject(lifebar);
	lifebarNode->setPosition(initX, 8, initZ);


	// get animation:
	anim_walk = entity->getAnimationState("Andar");
	anim_walk->setEnabled(true);
	anim_walk->setLoop(true);
	anim_walk->setTimePosition((float)rand()/(float)RAND_MAX*anim_walk->getLength());
	

	speed = sp;
	speedTurn = sptr;
	angleTurn = 0;
	translateVector = Ogre::Vector3(0, 0, 0);
	

	// start ALIVE !
	live = true;
	life = 100.0;
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

		Ogre::Radian actualBearing = node->getOrientation().getYaw();

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
			actualBearing = actualBearing + (angleTurn - actualBearing) * (speedTurn*evt.timeSinceLastFrame);
			    
			// Apply the turn on the node:
			node->setOrientation(Ogre::Quaternion(actualBearing, Ogre::Vector3(0, 1, 0)));
		}
		
		// Commint the movement
		node->translate(translateVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		lifebarNode->setPosition(node->getPosition().x, 8, node->getPosition().z);


		anim_walk->addTime(evt.timeSinceLastFrame * (speed*0.3));
	} else{
		node->yaw(Ogre::Degree(1)); // simulate the dead turning
	}
}

void Zombie::damage(double deltaT){
	life = life - 20*deltaT;
	lifebarNode->setScale(life/100, 1, 1);
	if (life <= 0)
		// KILL KILL KILL
		live = false;
}

// Check if zombie is live or dead
bool Zombie::isLive(){
	return live;
}




