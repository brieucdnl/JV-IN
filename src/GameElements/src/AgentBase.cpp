#include <stdafx.h>
#include <GameElements/AgentBase.h>
#include <GameElements/BulletBase.h>

namespace GameElements
{	

	AgentBase::AgentBase( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, int numAgent, bool computeCollisionMesh/*=true*/ ) 
		: Agent(archetype, weaponArchetype, computeCollisionMesh)
	{
		m_numAgent = numAgent;
		circle = OgreFramework::GlobalConfiguration::getSceneManager()->createManualObject(this->getArchetype()->m_name+"num"+(char)m_numAgent);
		circle->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

		float radius = 5;//this->getArchetype()->m_scale;.
		float accuracy = 35;
		unsigned point_index = 0;

		for(float theta = 0; theta <= 2*Math::pi; theta += Math::pi / accuracy)
		{
			circle->position(radius*cos(theta), 0, radius*sin(theta));
			circle->index(point_index++);
		}
		circle->index(0);
		circle->end();
		circle->setVisible(false);

		this->m_entity->attachObject(circle);
	}

	void AgentBase::agentSelection() const
	{
		circle->setVisible(true);
		std::cout<<"selected item : "<<this->getArchetype()->m_name+"num"+(char)m_numAgent
			
			
			
			<<std::endl;
	}

}