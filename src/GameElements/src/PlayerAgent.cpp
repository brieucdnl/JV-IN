#include <stdafx.h>
#include <GameElements/PlayerAgent.h>
#include <GameElements/BulletBase.h>

namespace GameElements
{
	PlayerAgent::PlayerAgent( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, int numAgent, bool computeCollisionMesh/*=true*/ ) : AgentBase(archetype, weaponArchetype, numAgent, computeCollisionMesh)
	{
		
	}

	void PlayerAgent::update( const Config::Real & dt )
	{
			
	}
}