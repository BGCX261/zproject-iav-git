
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
	lifebarNode->setVisible(false);


	// get animation:
	anim_walk = entity->getAnimationState("Andar");
	anim_walk->setEnabled(true);
	anim_walk->setLoop(true);
	anim_walk->setTimePosition((float)rand()/(float)RAND_MAX*anim_walk->getLength());
	
	anim_death = entity->getAnimationState("Muerte");
	anim_death->setEnabled(false);
	anim_death->setLoop(false);

	anim_attack = entity->getAnimationState("Atacar");
	anim_attack->setEnabled(false);
	anim_attack->setLoop(true);

	// Atributes
	speed = sp;
	speedTurn = sptr;
	angleTurn = 0;
	translateVector = Ogre::Vector3(0, 0, 0);
	

	// start ALIVE !
	alive = true;

	// State:
	attacking = false;
	life = 100.0;
	hunger = 0.6;
	dps = 5;
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
void Zombie::attack(const Ogre::FrameEvent& evt, MOC::CollisionTools *mCollisionTools, Ogre::String* &nombre)
{
	Ogre::Entity *tmpE = NULL;

	if (alive) {
	bool aux = mCollisionTools->collidesWithEntityCustom(node->getPosition()+translateVector, node->getPosition(), tmpE, 8.0f , 4.0f, ENEMY_MASK);
		// Check if we are colliding with an enemy and start to attack him:
		if (aux)
		{
			attacking = true;
			anim_walk->setEnabled(false);
			anim_attack->setEnabled(true);
			nombre = new Ogre::String(tmpE->getName());			
			return;
		} else {

			attacking = false;
			anim_walk->setEnabled(true);
			anim_attack->setEnabled(false);
		}
	}

	nombre = new Ogre::String("X.-1");

	
}

//-------------------------------------------------------------------------------------
void Zombie::update(const Ogre::FrameEvent& evt, MOC::CollisionTools *mCollisionTools)
{
	if(alive){

		if (attacking)
		{
			// Update Animation
			anim_attack->addTime(evt.timeSinceLastFrame);
		} else
		{
			// MOC collision
			// Get the old position movement
			Ogre::Vector3 oldPos = node->getPosition();

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
		
				// Commit the movement
				node->translate(translateVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);


				// Check if we are colliding with anything with a collision radius of 4.0 ogre units and we 
				// set the ray origin 10.0 for the bunker collision
				if (mCollisionTools->collidesWithEntity(oldPos, node->getPosition(), 4.0f , 10.0f, STATIC_MASK))
				{
					// undo move
					node->setPosition(oldPos);
				}


				lifebarNode->setPosition(node->getPosition().x, 8, node->getPosition().z);


				anim_walk->addTime(evt.timeSinceLastFrame * (speed*0.3));
		}
	} else {
			double aux = anim_death->getTimePosition() + evt.timeSinceLastFrame*1.5;
			if (aux >= anim_death->getLength()-0.05)
			{
				anim_death->setTimePosition(anim_death->getLength()-0.05);
			} 
			else {
				anim_death->setTimePosition(aux);
			}
	}
}

void Zombie::damage(int dps, double deltaT){

	life = life - dps * deltaT;
	lifebarNode->setScale(life/100, 1, 1);
	if (life <= 0)
	{
		// KILL KILL KILL
		alive = false;
		entity->setQueryFlags(OTHER_MASK);

		anim_walk->setEnabled(false);
		anim_attack->setEnabled(false);
		anim_death->setEnabled(true);
	}
}

// Check if zombie is live or dead
bool Zombie::isAlive(){
	return alive;
}

void Zombie::select()
{
	lifebarNode->setVisible(true);
}

void Zombie::deselect()
{
	lifebarNode->setVisible(false);
}


