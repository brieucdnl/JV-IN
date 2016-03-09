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

		this->m_entity->showBoundingBox(false);
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

	Math::Vector2<Config::Real> AgentBase::randomVelocity() const
	{
		Math::Vector2<Config::Real> velocity(rand()-RAND_MAX/2, rand()-RAND_MAX/2) ;
		velocity = velocity.normalized() * m_archetype->m_speed ;
		return velocity ;
	}

	Math::Vector2<Config::Real> AgentBase::computeVelocity(Math::Vector3<Config::Real> position) const
	{
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;
		Math::Vector2<Config::Real> velocity(position.x()-getPosition().x(), position.y()-getPosition().y());
		velocity = velocity.normalized() * m_archetype->m_speed;
		return velocity;
	}

	void AgentBase::goToPosition(const Math::Vector3<Config::Real> position)
	{
		m_velocity = computeVelocity(position);
		setOrientation(m_velocity);
	}

	void AgentBase::followTarget(Agent& agent)
	{
		goToPosition(agent.getPosition(  ));
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

	void AgentBase::setTargetPosition(Math::Vector3<Config::Real> pos)
	{
		m_targetPos = pos;
	}

	Math::Vector3<Config::Real> AgentBase::getTargetPosition() const
	{
		return m_targetPos;
	}
}