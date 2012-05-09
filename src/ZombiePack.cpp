
#include "ZombiePack.h"


ZombiePack::ZombiePack(int id, int nz, Ogre::Real initX, Ogre::Real initZ, Ogre::Real sp, Ogre::Real sptr)
{
	identifier = id;
	
	nZombies = nz;

	zombies = new Zombie*[nZombies];
	for (int i = 0; i < nZombies; i++) {
		char aux[20];
		sprintf(aux, "Zombie%d.mesh", rand()%2+1);
		zombies[i] = new Zombie(Ogre::String(aux), id, i, initX + rand()%(nZombies),  initZ + rand()%(nZombies), sp, sptr);
	}

	
	// Banderita selection with Ray
	Ogre::SceneManager* mSceneMgr = Ogre::Root::getSingleton().getSceneManager("ingameManager");
	Ogre::Entity* banderaEntity = mSceneMgr->createEntity("banderita.mesh");
	banderaEntity->setQueryFlags(OTHER_MASK);
	banderaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(initX, 4, initZ));
	banderaNode->attachObject(banderaEntity);
	banderaNode->roll(Ogre::Degree(-90));
	banderaNode->pitch(Ogre::Degree(90));
	banderaNode->setVisible(false);
}


ZombiePack::~ZombiePack(void)
{

}

void ZombiePack::move(UnitMovModel *model)
{
	// We now move all the units, according to the movement model:
	for (int i = 0; i < nZombies; i++) {

		double x, z;
		if (model->calculateMove(zombies, nZombies, i, banderaNode->getPosition(), &x, &z))
			zombies[i]->move(x, z);
	}
}

void ZombiePack::update(const Ogre::FrameEvent& evt, MOC::CollisionTools *mCollisionTools)
{
	for (int i = 0; i < nZombies; i++)
		zombies[i]->update(evt, mCollisionTools);
}

Zombie* ZombiePack::getZombie(int i)
{
	return zombies[i];
}

void ZombiePack::setBandera(double x, double z)
{
	banderaNode->setPosition(x, 4, z);
}

void ZombiePack::select()
{
	banderaNode->setVisible(true);
	for(int i=0;i<nZombies;i++)
		zombies[i]->select();
}

void ZombiePack::deselect()
{
	banderaNode->setVisible(false);
	for(int i=0;i<nZombies;i++)
		zombies[i]->deselect();
}

void ZombiePack::modifySpeed(float factor)
{
	for(int i=0;i<nZombies;i++)
		zombies[i]->speed *= factor;
}


