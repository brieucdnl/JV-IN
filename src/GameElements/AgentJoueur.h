#ifndef _GameElements_AgentJoueur_H
#define _GameElements_AgentJoueur_H

#include <GameElements/AgentBase.h>
#include <OgreFramework/GlobalConfiguration.h>


namespace GameElements
{
	class AgentJoueur : public AgentBase
	{
	protected:


	public:
		AgentJoueur(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, int numAgent, bool computeCollisionMesh=true);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void NullAgent::update(const Config::Real & dt);
		///
		/// \brief	Updates the given dt.
		///
		/// \author	Fabrice Lamarche, university of Rennes 1
		/// \param	dt	The dt.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void update(const Config::Real & dt);
	};
}

#endif