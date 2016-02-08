#include <stdafx.h>
#include <GameElements/AgentBase.h>
#include <GameElements/BulletBase.h>
#include <OgreFramework/MainApplication.h>

namespace GameElements
{	

	AgentBase::AgentBase( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, int numAgent, bool computeCollisionMesh/*=true*/ ) 
		: Agent(archetype, weaponArchetype, computeCollisionMesh)
	{
		m_velocity = randomVelocity() ;


		m_numAgent = numAgent;
		circle = OgreFramework::GlobalConfiguration::getSceneManager()->createManualObject(this->getArchetype()->m_name+"num"+(char)m_numAgent);
		circle->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

		float radius = 2;
		float accuracy = 35;
		unsigned point_index = 0;

		for(float theta = 0; theta <= 2*Math::pi; theta += Math::pi / accuracy)
		{
			circle->position(radius*cos(theta), radius*sin(theta), 1);
			circle->index(point_index++);
		}
		circle->index(0);
		circle->setBoundingBox(Ogre::AxisAlignedBox(0,0,0,0,0,0));
		circle->end();
		circle->setVisible(false);

		this->m_entity->attachObject(circle);
	}
	
	AgentBase::~AgentBase(){
		if(getArchetype()->m_name[getArchetype()->m_name.size()-1] == 'R') OgreFramework::MainApplication::nbR--;
		else  OgreFramework::MainApplication::nbB--;
	}

	void AgentBase::onCollision( const CollisionMessage & message )
	{

	}

	Math::Vector2<Config::Real> AgentBase::getVelocity() const
	{
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;
		return m_velocity*(1.0-currentCell.m_speedReduction) ;
	}

	void AgentBase::agentSelection() const
	{
		circle->setVisible(true);
		//std::cout << "selected item : " << this->getArchetype()->m_name+"num"+(char)m_numAgent << std::endl;
	}
	
	void AgentBase::agentUnSelection() const
	{
		circle->setVisible(false);
		//std::cout << "Unselected item : " << this->getArchetype()->m_name+"num"+(char)m_numAgent << std::endl;
	}


}