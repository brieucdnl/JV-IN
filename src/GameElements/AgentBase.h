#ifndef _GameElements_AgentBase_H
#define _GameElements_AgentBase_H

#include <GameElements/Agent.h>
#include <OgreFramework/GlobalConfiguration.h>
#include <OIS/OIS.h>
namespace GameElements
{
	class AgentBase : public Agent
	{
	protected:
		Ogre::ManualObject *circle;
		int m_numAgent;
		Math::Vector2<Config::Real> m_velocity ;
		Math::Vector3<Config::Real> m_targetPos;

	public:
		AgentBase(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, int numAgent, bool computeCollisionMesh=true);
		~AgentBase();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void NullAgent::update(const Config::Real & dt);
		///
		/// \brief	Updates the given dt.
		///
		/// \author	Fabrice Lamarche, university of Rennes 1
		/// \param	dt	The dt.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void update(const Config::Real & dt) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void NullAgent::onCollision (const CollisionMessage & message);
		///
		/// \brief	Executes the collision action.
		///
		/// \author	Fabrice Lamarche, university of Rennes 1
		/// \param	message	The message.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void onCollision (const CollisionMessage & message);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual Math::Vector2<Config::Real> RandomAgent::getVelocity() const
		///
		/// \brief	Gets the velocity.
		///
		/// \author	Fabrice Lamarche, university of Rennes 1
		/// \return	The velocity.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Math::Vector3<Config::Real> getTargetPosition() const;
		Math::Vector2<Config::Real> getVelocity() const ;
		Math::Vector2<Config::Real> randomVelocity() const ;
		Math::Vector2<Config::Real> computeVelocity(Math::Vector3<Config::Real> position) const ; 
		virtual void goToPosition(const Math::Vector3<Config::Real> position);
		virtual void followTarget(Agent& agent);
		void setTargetPosition(Math::Vector3<Config::Real> pos);
		void agentSelection () const;
		void agentUnSelection () const;
	};
}

#endif