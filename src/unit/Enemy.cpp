
#include "Zombie.h"
#include "Enemy.h"

const struct EnemyDataStructure Enemy::EnemyData[] = {

{"Turret.mesh",
 100,
 50,
 100,
 2,
 1,
 1,
 0
},

{"Flyer.mesh",
 100,
 20,
 100,
 6,
 2,
 1,
 0
	},

{"robot.mesh",
 100,
 40,
 100,
 4,
 3,
 0.1,
 -3
	}
};

//-------------------------------------------------------------------------------------
Enemy::Enemy(int ty, int ind, Ogre::Real initX, Ogre::Real initZ)
{
	Ogre::SceneManager* mSceneMgr = Ogre::Root::getSingleton().getSceneManager("ingameManager");
	type = ty;	
	index = ind;

	// Enemy Entity
	char aux[20];
	sprintf(aux, "E.%d", index);
	entity = mSceneMgr->createEntity(Ogre::String(aux), EnemyData[type].model);
	entity->setQueryFlags(ENEMY_MASK);
	// bounding box
	box = entity->getBoundingBox();
	// Enemy Node
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	// Node Position (relative to bounding box Left-Bottom)
	//node->showBoundingBox(true);
	node->setPosition(initX, -box.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM).y + EnemyData[type].boundYcorrection , initZ);
	// attach to node
	node->attachObject(entity);
	node->scale(Ogre::Vector3(EnemyData[type].scaleFactor, EnemyData[type].scaleFactor, EnemyData[type].scaleFactor));

	// start ALIVE !
	alive = true;

	// ATributes
	shoot = false;
	patrol = false;
	seek = true;
	attack = true;

	range   = EnemyData[type].range;
	dps 	= EnemyData[type].dps;
	life 	= EnemyData[type].life;


	// Movement
	speed = EnemyData[type].speed;
	speedTurn = EnemyData[type].speedTurn;
	angleTurn = 0;
	translateVector = Ogre::Vector3(0, 0, 0);
	turning = false;

	shootingDelay = 0;

	//Animations
	anim_walk = entity->getAnimationState("walk");
	anim_shoot = entity->getAnimationState("shoot");
	anim_idle = entity->getAnimationState("idle");

	anim_walk->setEnabled(false);
	anim_walk->setLoop(true);
	anim_shoot->setEnabled(false);
	anim_shoot->setLoop(true);
	anim_idle->setEnabled(true);
	anim_idle->setLoop(true);

	char aux2[20];
	sprintf(aux2, "TurretSmoke-%d", index);
	smokeParticles = mSceneMgr->createParticleSystem(Ogre::String(aux2), "TurretSmoke");
	smokeParticles->setEmitting(false);
	smokeNode = node->createChildSceneNode();
	smokeNode->attachObject(smokeParticles);
	smokeNode->scale(0.05, 0.05, 0.05);
	smokeNode->setVisible(false);
}

//-------------------------------------------------------------------------------------
Enemy::~Enemy(void)
{
}

//-------------------------------------------------------------------------------------
void Enemy::move(Ogre::Real axisX, Ogre::Real axisZ)
{
	angleTurn = - Ogre::Math::ATan2(axisZ - node->getPosition().z, axisX - node->getPosition().x);
	turning = true;
	
	translateVector.x = speed;
}

//-------------------------------------------------------------------------------------
void Enemy::trace(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, ZombiePack** zombies)
{

	if (alive)
	{
		Ogre::Degree angle	  = node->getOrientation().getYaw() + Ogre::Degree(90);
		Ogre::Vector3 origin      = node->getPosition();
		origin.y = 4;
		Ogre::Vector3 destination = Ogre::Vector3(Ogre::Math::Sin(angle), 0,  Ogre::Math::Cos(angle));
	
		Ogre::Entity *tmpE = NULL;
		Ogre::Vector3 result;
		float distToColl;
	
		// enable shoot animation when a zombie is near to robotNode - Kill the zombie		
		if(mCollisionTools->raycastFromPoint(origin, destination, result, tmpE, distToColl, ZOMBIE_MASK)){

			if (distToColl <= range)
			{
				Ogre::String name = tmpE->getName();
				std::vector<Ogre::String, Ogre::STLAllocator<Ogre::String, Ogre::GeneralAllocPolicy> > nameGroups = Ogre::StringUtil::split(name, Ogre::String("-"));
				int group = Ogre::StringConverter::parseInt(nameGroups[1]);
				int individual = Ogre::StringConverter::parseInt(nameGroups[2]);

				if (zombies[group]->getZombie(individual)->isAlive())
				{
					fire();
					zombies[group]->getZombie(individual)->damage(dps, evt.timeSinceLastFrame);
				}
			}
		}
	}
}


//-------------------------------------------------------------------------------------
void Enemy::update(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, ZombiePack** zombies)
{
	if (attack)
		this->trace(mCollisionTools, evt, zombies);

	if(alive){
		
		if(shoot)
		{
			anim_shoot->addTime(evt.timeSinceLastFrame);
			shootingDelay -= evt.timeSinceLastFrame;
		
			if (shootingDelay <= 0) {
				shoot = false;
			}

	    	// If we are still turning we have to update the orientation:
	    	} else if (seek)
		{
			//robotAnimState_idle->addTime(2*evt.timeSinceLastFrame);
			node->yaw(Ogre::Radian(evt.timeSinceLastFrame*speedTurn));
			anim_idle->addTime(evt.timeSinceLastFrame);
		
		} 
		else if (patrol)
		{

			//--------------------------------------------------------------------------------------------
			// The movements... (ENCAPSULAR EST NO ESTARÍA MAL)

			// MOC collision
			// Get the old position movement
			Ogre::Vector3 oldPos = node->getPosition();

			Ogre::Radian actualBearing = node->getOrientation().getYaw();

			    // If we are still turning we have to update the orientation:
			    if (turning)
				{
					// Avoid to rotate using the longest path:
					if(Ogre::Math::Abs(actualBearing.valueRadians()+ - angleTurn.valueRadians()) > Ogre::Math::PI)
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
				if (mCollisionTools->collidesWithEntity(oldPos, node->getPosition(), 4.0f , 3.0f, STATIC_MASK))
				{
					// undo move
					node->setPosition(oldPos);
				}

				anim_walk->addTime(evt.timeSinceLastFrame);

		}

	} else{

		// The smoke is also on
	}
}

void Enemy::damage(int dps,  double deltaT){

	life = life - dps * deltaT;
	if (life <= 0)
	{
		// KILL KILL KILL
		alive = false;
		entity->setQueryFlags(OTHER_MASK);
		smokeParticles->setEmitting(true);
		smokeNode->setVisible(true);
	}
}

void Enemy::setSeek(){

	seek = true;
	patrol = false;

	anim_walk->setEnabled(false);
	anim_idle->setEnabled(true);
}

void Enemy::setPatrol(){

	seek = false;
	patrol = true;

	anim_walk->setEnabled(true);
	anim_idle->setEnabled(false);
}

void Enemy::fire(){
	shoot = true;
	shootingDelay = anim_shoot->getLength();

	anim_shoot->setEnabled(true);
}

void Enemy::setAttack(bool b){

	attack = b;
}

// Check if Enemy is live or dead
bool Enemy::isAlive(){
	return alive;
}

void Enemy::modifySpeed(double factor)
{
	speed *= factor;
	speedTurn *= factor;
}



