#pragma once

#include <GameElements/AgentBase.h>
#include <OgreFramework/GlobalConfiguration.h>
//#include <System/IAgentEmitter.h>
//#include <System/IAgentListener.h>

namespace GameElements
{
	class IAgent : public AgentBase
	{
	protected:
		Math::Vector2<Config::Real> m_velocity ;


		Math::Vector2<Config::Real> randomVelocity()
		{
			Math::Vector2<Config::Real> velocity(rand()-RAND_MAX/2, rand()-RAND_MAX/2) ;
			velocity = velocity.normalized() * m_archetype->m_speed ;
			return velocity ;
		}

		//System::IAgentListener<short> listener;

		// Get attributes
		std::string getNameAgent(Agent::Pointer a);
		float getFrequency(Agent::Pointer a);
		int getStrenght(Agent::Pointer a);

		// Choisir un ennemi en fonction de son nom
		int choose_by_name(std::vector<Agent::Pointer> &liste_ennemis, int i1, int i2);

		// Dégats infligés par unités de temps
		int degat_par_t(Agent::Pointer a);

		// Degats recus par unités de temps pour plusieurs ennemis
		int degat_par_t_list(std::vector<Agent::Pointer> &liste_ennemis);

		// Temps de survie de l'unité en fonction des ennemis percus dans la zone
		float temps_de_survie(std::vector<Agent::Pointer> &liste_ennemis, int mylife = 0 );

		//Temps nécessaire pour tuer l'ennemi
		float temps_pour_kill(Agent::Pointer ennemi);

		// Renvoyer la liste des AgentBases que l'on peut tuer alentour
		std::vector<Agent::Pointer> who_kill(std::vector<Agent::Pointer> &liste_ennemis);

		//Temps pour aller d'un point A a un point B
		double time_to_join(Agent::Pointer a);

		//Tester si une situation est désespérée
		bool alone_in_the_dark(std::vector<Agent::Pointer> &liste_amis, std::vector<Agent::Pointer> &liste_ennemis);

		int choose_by_survival(std::vector<Agent::Pointer> &liste_ennemis);

		// Calcul de la vie qu'il nous reste si on tue l'ennemi i dans la liste
		int vie_apres_kill(std::vector<Agent::Pointer> &liste_ennemis, int i);

		void callFriends(std::vector<Agent::Pointer> &liste_amis);
		
		std::vector<Agent::Pointer> getFriends();
		
		//Choisir qui tuer
		// Renvoit l'indice de l'ennemi à attaquer dans la liste
		// Si renvoit -1, FUITE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		int choix(std::vector<Agent::Pointer> &liste_ennemis);


	public:
		IAgent(const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, int numAgent, bool computeCollisionMesh=true);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void NullAgentBase::update(const Config::Real & dt);
		///
		/// \brief	Updates the given dt.
		///
		/// \author	Fabrice Lamarche, university of Rennes 1
		/// \param	dt	The dt.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void update(const Config::Real & dt);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void NullAgentBase::onCollision (const CollisionMessage & message);
		///
		/// \brief	Executes the collision action.
		///
		/// \author	Fabrice Lamarche, university of Rennes 1
		/// \param	message	The message.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void onCollision (const CollisionMessage & message);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual Math::Vector2<Config::Real> IAgent::getVelocity() const
		///
		/// \brief	Gets the velocity.
		///
		/// \author	Fabrice Lamarche, university of Rennes 1
		/// \return	The velocity.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual Math::Vector2<Config::Real> getVelocity() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void addListener();
		///
		/// \brief	Add a first listener to the AgentBase.
		///
		/// \author	Paul LD & Fabrice Lamarche, university of Rennes 1
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//virtual void addListener(System::IAgentEmitter<short> * emitter);
	};
}
