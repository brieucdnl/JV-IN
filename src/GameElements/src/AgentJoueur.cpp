#include <stdafx.h>
#include <GameElements/AgentJoueur.h>
#include <GameElements/BulletBase.h>

namespace GameElements
{
	AgentJoueur::AgentJoueur( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, int numAgent, bool computeCollisionMesh/*=true*/ ) : AgentBase(archetype, weaponArchetype, numAgent, computeCollisionMesh)
	{
		
	}

	
	void AgentJoueur::update( const Config::Real & dt )
	{
		
		
	}
}