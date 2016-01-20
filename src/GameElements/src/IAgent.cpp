#include <stdafx.h>
#include <GameElements/IAgent.h>
#include <GameElements/BulletBase.h>

/*
	TODO :
		* Call friends
*/

namespace GameElements
{
	// Get name
	std::string IAgent::getNameAgent(Agent::Pointer a){ return a->getArchetype()->m_name;}

	//normal
	bool myfn(const Triggers::CollisionObject::Pointer i, const Triggers::CollisionObject::Pointer j) { 
		return boost::dynamic_pointer_cast<Agent>(i)->getLifePoints() < boost::dynamic_pointer_cast<Agent>(j)->getLifePoints();
	}

	//pourcentage
	bool myfnp(const Triggers::CollisionObject::Pointer i, const Triggers::CollisionObject::Pointer j) { 
		return boost::dynamic_pointer_cast<Agent>(i)->getLifePoints() / boost::dynamic_pointer_cast<Agent>(i)->getArchetype()->m_life 
			   < 
			   boost::dynamic_pointer_cast<Agent>(j)->getLifePoints() / boost::dynamic_pointer_cast<Agent>(j)->getArchetype()->m_life;
	}

	void IAgent::onCollision( const CollisionMessage & message )
	{

	}

	Math::Vector2<Config::Real> IAgent::getVelocity() const
	{
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;
		return m_velocity*(1.0-currentCell.m_speedReduction) ;
	}

	IAgent::IAgent( const UnitsArchetypes::Archetype * archetype, const WeaponsArchetypes::Archetype * weaponArchetype, int numAgent, bool computeCollisionMesh/*=true*/ ) : AgentBase(archetype, weaponArchetype, numAgent, computeCollisionMesh)
	{
		m_velocity = randomVelocity() ;
	}

	void IAgent::update( const Config::Real & dt )
	{
		//nom et equipe de l'AgentBase (equipe = R ou B)
		std::string my_name = this->getArchetype()->m_name;
		char my_team = my_name[my_name.size() -1];

		// Computes movements
		const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;
		Math::Vector2<Config::Real> newPosition = getPosition().projectZ()+m_velocity*dt*(1.0-currentCell.m_speedReduction) ;
		
		// If displacement is valid, the AgentBase moves, otherwise, a new random velocity is computed
		if(OgreFramework::GlobalConfiguration::getCurrentMap()->isValid(newPosition) && OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(newPosition).m_speedReduction!=1.0)
		{
			setOrientation(m_velocity) ;
			setPosition(newPosition.push(0.0)) ;
		}
		else
		{
			m_velocity = randomVelocity() ;
		}
		
		// Handles perception and fires on AgentBases
		if(canFire())
		{
			::std::vector<Triggers::CollisionObject::Pointer> objects = m_perception->perceivedAgents() ; //objects = liste des objets détéctés
			for(unsigned int cpt=0 ; cpt<objects.size() ; ++cpt)
			{
				//supprimer de la liste les objets qui en sont pas des AgentBases
				if(boost::dynamic_pointer_cast<Agent>(objects[cpt])==NULL)
				{
					::std::swap(objects[cpt], objects.back()) ;
					--cpt;
					objects.pop_back() ;
				}
				
			}
			
			if(objects.size()!=0) // If there is something to shoot, then open fire !!!! 
			{
				//int index = rand()%objects.size() ;
				
				std::vector<Agent::Pointer> ennemis;
				for(unsigned int cpt=0;cpt<objects.size();cpt++)
				{
					//on cherche si on est allie
					Agent::Pointer ptr0 = boost::dynamic_pointer_cast<Agent>(objects[cpt]) ;
					char agent_team = ptr0->getArchetype()->m_name[ptr0->getArchetype()->m_name.size()-1];
					bool allie =  (agent_team == my_team) ;
					if(!allie)
					{
						ennemis.push_back(ptr0);
					}
				}

				int decision = choix(ennemis);
				if(decision != -1)
				{
					Agent::Pointer ptr = boost::dynamic_pointer_cast<Agent>(objects[decision]) ;
				
					if(ptr!=NULL)
					{
						//si l'AgentBase est un ennemi
						Math::Vector2<Config::Real> otherPosition = ptr->getPosition().projectZ() ;
						Math::Vector2<Config::Real> otherVelocity = ptr->getVelocity() ;
						Config::Real bulletSpeed = m_weapon.getArchetype()->m_speed ;
						Config::Real distanceToTarget = (getPosition().projectZ()-otherPosition).norm() ;
						Config::Real timeToTarget = distanceToTarget/bulletSpeed ;
						fire(otherPosition+otherVelocity*timeToTarget) ;
					}
				}
				else{//fuite
					const Map::GroundCellDescription & currentCell = OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(getPosition().projectZ()) ;
					Math::Vector2<Config::Real> newPosition = getPosition().projectZ()+m_velocity*(1.0-currentCell.m_speedReduction) ;
		
					// If displacement is valid, the AgentBase moves, otherwise, a new random velocity is computed
					if(OgreFramework::GlobalConfiguration::getCurrentMap()->isValid(newPosition) && OgreFramework::GlobalConfiguration::getCurrentMap()->getCell(newPosition).m_speedReduction!=1.0)
					{
						setOrientation(m_velocity) ;
						setPosition(newPosition.push(0.0)) ;
					}
					else
					{
						m_velocity = randomVelocity() ;
					}
				}
			}
		}
		m_perception->reset() ;
	}

	void IAgent::addListener(System::IAgentEmitter<short> * emitter)
	{
		//listener(emitter);
	}

	// Choisir un ennemi en fonction de son nom
	int IAgent::choose_by_name(std::vector<Agent::Pointer> &liste_ennemis, int i1, int i2)
	{
	  char name1 = getNameAgent(liste_ennemis[i1])[0];
	  char name2 = getNameAgent(liste_ennemis[i2])[0];
	  if(name1 == name2)
	  {
		  // Cas Croco - Croco
		  // Cas Moustic - Moustic
		  // Cas Hippo - Hippo
		  if(liste_ennemis[i1]->getLifePoints() > liste_ennemis[i2]->getLifePoints()) {return i2 ;}
		  else {return i1;}
	  }
	  else
	  {
		if(name1 == 'C') {return i1;} // Cas Croco - Autre
		else if(name2 == 'C'){return i2;}  // Cas Autre - Croco

		else if(name1 = 'M'){return i1;} //Cas Moustic - Hippo
		else{return i2;} //Cas Hippo - Moustic
	  }
	}

	float IAgent::getFrequency(Agent::Pointer a)
	{
		//return a.getArchetype()->m_weapon.getArchetype()->m_frequency ;
		char t = a->getArchetype()->m_weapon[0];

		switch (t){
		case 'M':
			return 15;
			break;
		case 'C':
			return 7;
			break;
		case 'H':
			return 1;
			break;
		default:
			return -1;
			break;
		}
	}

	int IAgent::getStrenght(Agent::Pointer a)
	{
		//return a.getArchetype()->m_weapon.getArchetype()->m_strenght ;
		char t = a->getArchetype()->m_weapon[0];

		switch (t){
		case 'M':
			return 2;
			break;
		case 'C':
			return 5;
			break;
		case 'H':
			return 20;
			break;
		default:
			return -1;
			break;
		}
	}

	// Dégats infligés par unités de temps
	int IAgent::degat_par_t(Agent::Pointer a)
	{
	  return getFrequency(a) * getStrenght(a);
	}

	// Degats recus par unités de temps pour plusieurs ennemis
	int IAgent::degat_par_t_list(std::vector<Agent::Pointer> &liste_ennemis)
	{
	  int degat = 0 ;

	  for(unsigned int cpt=0;cpt<liste_ennemis.size();cpt++) degat += degat_par_t(liste_ennemis[cpt]);

	  return degat ;
	}

	// Temps de survie de l'unité en fonction des ennemis percus dans la zone
	float IAgent::temps_de_survie(std::vector<Agent::Pointer> & liste_ennemis, int mylife)
	{
		if(mylife == 0) mylife = this->getLifePoints();
		int degat = degat_par_t_list(liste_ennemis);
		if(degat > 0){return mylife/degat;}
		else{return -1;}
	}

	//Temps nécessaire pour tuer l'ennemi
	float IAgent::temps_pour_kill(Agent::Pointer ennemi)
	{
	  return ennemi->getLifePoints() / degat_par_t(this) ;
	}

	// Renvoyer la liste des AgentBases que l'on peut tuer alentour
	std::vector<Agent::Pointer> IAgent::who_kill(std::vector<Agent::Pointer> &liste_ennemis)
	{
	  std::vector<Agent::Pointer> to_kill; // liste des AgentBases que l'on peut kill
	  float ts = this->temps_de_survie(liste_ennemis);

	  for(unsigned int cpt=0;cpt<liste_ennemis.size();cpt++)
		  if(ts > temps_pour_kill(liste_ennemis[cpt]))
			to_kill.push_back(liste_ennemis[cpt]);
	  return to_kill ;
	}

	//Temps pour aller d'un point A a un point B
	double IAgent::time_to_join(Agent::Pointer a)
	{
	  Math::Vector3<Config::Real> myposition = this->getPosition();
	  Math::Vector3<Config::Real> aposition = a->getPosition();
	  Config::Real aspeed = a->getMaxSpeed();

	  float distance = (myposition - aposition).norm();

	  return aspeed/distance ;
	}

	//Tester si une situation est désespérée
	bool IAgent::alone_in_the_dark(std::vector<Agent::Pointer> &liste_amis, std::vector<Agent::Pointer> &liste_ennemis)
	{
	  if(liste_amis.size() == 0)
	  {return true;}

	  else
	  {
		for(unsigned int cpt=0;cpt<liste_amis.size();cpt++)
		{
		  if(time_to_join(liste_amis[cpt]) < temps_de_survie(liste_ennemis)) {return false;}
		}
	  }
	  return true ;
	}

	// Calcul de la vie qu'il nous reste si on tue l'ennemi i dans la liste
	int IAgent::vie_apres_kill(std::vector<Agent::Pointer> &liste_ennemis, int i)
	{
	  int tk = temps_pour_kill(liste_ennemis[i]);
	  int mylife = this->getLifePoints() ;
	  int degat = degat_par_t_list(liste_ennemis) ;
	  return mylife - tk*degat ;
	}

	// appeler les amis a la rescousse
	void IAgent::callFriends(std::vector<Agent::Pointer> &liste_amis) {
		for(unsigned int cpt = 0; cpt < liste_amis.size(); cpt++){
			std::cout<<"appeler la fonction des tocards"<<std::endl;
		}
	}

	// Voir la liste des amis alentour
	std::vector<Agent::Pointer> IAgent::getFriends()
	{
		::std::vector<Triggers::CollisionObject::Pointer> objects = m_perception->perceivedAgents() ; //objects = liste des objets détéctés
		std::vector<Agent::Pointer> liste_amis;

		char my_team = getNameAgent(this)[getNameAgent(this).size()-1] ;
		//chercher les amis dans la liste
		for(unsigned int cpt=0;cpt<objects.size();cpt++)
		{
			if(boost::dynamic_pointer_cast<Agent>(objects[cpt])!=NULL)
			{
				//on cherche si on est allie
				Agent::Pointer ptr = boost::dynamic_pointer_cast<Agent>(objects[cpt]) ;
				char agent_team = ptr->getArchetype()->m_name[ptr->getArchetype()->m_name.size()-1];
				bool allie =  (agent_team == my_team) ;
			
				if(allie)
				{
					liste_amis.push_back( ptr );
				}
			}
			//supprimer de la liste les objets qui en sont pas des AgentBases
		}
		return liste_amis;
	}

	int IAgent::choose_by_survival(std::vector<Agent::Pointer> &liste_ennemis){
		int max_survie = 0;
		int ind_ennemi = 0 ;

		for(unsigned int cpt=0;cpt<liste_ennemis.size();cpt++)
		{
			std::vector<Agent::Pointer> liste_ennemis_copy(liste_ennemis);
			::std::swap(liste_ennemis_copy[cpt], liste_ennemis_copy.back());
			liste_ennemis_copy.pop_back();

			int ts = temps_de_survie(liste_ennemis_copy, vie_apres_kill( liste_ennemis, cpt )) ; // temps de survie si on tue l'ennemi cpt

			if(ts==-1){ind_ennemi = -1; break;}
			else
			{
				if(max_survie == 0){
					max_survie = ts;
				}
				else if(ts > max_survie) {
					ind_ennemi = cpt;
					max_survie = ts ;
				}
				else {
					if(ind_ennemi != -1) {
						ind_ennemi = choose_by_name(liste_ennemis, ind_ennemi, cpt);
					}
					else {
						ind_ennemi = cpt;
						max_survie = ts ;
					}
				}
			}
		}

		return ind_ennemi;
	}

	// Choisir qui tuer
	// Renvoit l'indice de l'ennemi à attaquer dans la liste
	// Si renvoit -1 : fuite
	int IAgent::choix(std::vector<Agent::Pointer> &liste_ennemis)
	{
		int ind_ennemi = -1 ;
		  if(liste_ennemis.size() !=0)
		  {
			  std::vector<Agent::Pointer> to_kill = who_kill(liste_ennemis);
			  
			  if(to_kill.size() == 0) // On ne peut tuer personne
			  {

				std::vector<Agent::Pointer> liste_amis = getFriends();

				if(!alone_in_the_dark(liste_amis,liste_ennemis)) //Support possible
				{
				  callFriends(liste_amis);
				  for(unsigned int cpt=0;cpt<liste_ennemis.size();cpt++)
				  {
					if(cpt==0) ind_ennemi = 0;
					else
					{
						ind_ennemi = choose_by_name(liste_ennemis, ind_ennemi, cpt); // On tire en priorité sur Croco puis Moustic puis Hippo
					}
				  }
				}
				
				else //Support impossible
				{
				  if(this->getLifePoints() < 0.4 * this->getArchetype()->m_life)
				  {
					  for(unsigned int cpt=0;cpt<liste_ennemis.size();cpt++)
					  {
						if(cpt==0) ind_ennemi = 0;
						else
						{
							ind_ennemi = choose_by_name(liste_ennemis, ind_ennemi, cpt); // On tire en priorité sur Croco puis Moustic puis Hippo
						}
					  }
				  }
				}
			  }
			  else if(to_kill.size()==1)
			  {
				ind_ennemi = 0;
			  }
			  else // On peut tuer au moins un ennemi
			  {
				//Trouver l'ennemi à tuer pour lequel mon temps de survie est le plus grand (pour deux temps equivalents, on choisit dans l'ordre Croco,Moustic,Hippo)
				ind_ennemi = choose_by_survival(liste_ennemis);
			  }
			}
	  		else
			{
				ind_ennemi = -1 ;//FUITE ou HELP FRIENDS
			}
	
		 return ind_ennemi;
		}

}