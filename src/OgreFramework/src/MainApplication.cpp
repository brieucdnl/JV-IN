#include <stdafx.h>

#include <OgreFramework/MainApplication.h>

#include <OGRE/OgreRay.h>
#include <OGRE/OgreSceneQuery.h>
#include <OgreFramework/EntityAdapter.h>
#include <Ogre/DotSceneLoader.h>
#include <OgreFramework/PickingSelectionBuffer.h>
#include <OgreFramework/PickingBoundingBox.h>
#include <OgreFramework/GeometryLoader.h>
#include <OgreFramework/GlobalConfiguration.h>
#include <GameElements/ConfigurationLoader.h>
#include <GameElements/NullAgent.h>
#include <GameElements/Bullet.h>
#include <GameElements/StraightBullet.h>
#include <GameElements/BallisticBullet.h>
#include <Ext/Ogre/ComputeBoundingBox.h>
#include <Triggers/BasicCollisionDetector.h>
#include <GameElements/RandomAgent.h>
#include <GameElements/IAgent.h>
#include <sstream>
#include <OgreFramework/Dijkstra.h>

#define SSTR( x ) static_cast< std::ostringstream & >( \
    ( std::ostringstream() << std::dec << x ) ).str()

namespace OgreFramework
{
	int hippo_c = 1000, croco_c = 500, moustic_c = 250;

	float MainApplication::nbR = 0;
	float MainApplication::nbB = 0;
	float MainApplication::nbR_total = 0;
	float MainApplication::nbB_total = 0;

	OgreBites::Label * b_r ;
	OgreBites::Label * arme;
	std::vector<int> armee(6,0);
	//FLAGS pour savoir ou on est avant le debut de la partie./
	bool CINE = true;
	bool DEF_VIEW = false;
	bool START = false;
	bool PLAYING = false;

	// Data parties
	int budget = 0;
	int budget_restant = 0;
	int mode=0;

	//data carte
	std::vector<Ogre::Vector3> cibles;
	short current_cible = 0;

	MainApplication::MainApplication()
		: m_keyboardState(*KeyboardState::getInstance())
	{
	}

	MainApplication::~MainApplication()
	{
	}

	void MainApplication::createScene(void)
	{
		// Application  global configuration
		GlobalConfiguration::setup("../data/xml/hot_config.xml", m_sceneManager, m_trayManager) ;

		// Creates the instance of collision detector
		new Triggers::SweepAndPrune ;
		//new Triggers::BasicCollisionDetector ;

		GlobalConfiguration::setCurrentMap("map01") ;
		creerChemin("map01");

		// create your scene here :)
		::std::cout<<"Creating scene..."<<::std::flush ;
	
		// 0- Sets the camera style
		//m_cameraManager->setStyle(OgreBites::CS_ORBIT) ; //OgreBites::CS_MANUAL) ;

		// 1- Sets the shadows (shadow volumes)
		// ------------------------------------
		m_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE) ; // No shadows
		//m_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE) ; ////Ogre::SHADOWTYPE_STENCIL_ADDITIVE) ;//
		//////m_sceneManager->setShadowDirectionalLightExtrusionDistance(10000000) ;
		//m_sceneManager->setShadowIndexBufferSize(500000) ;
		//m_sceneManager->setShadowColour(::Ogre::ColourValue(.3,.3,.3)) ;
		//////m_sceneManager->setShowDebugShadows(true) ;

		m_camera->setFarClipDistance(Ogre::Real(3000)) ;
		m_camera->setNearClipDistance(Ogre::Real(0.01)) ;

		// 2- Sets fog
		// -----------
		m_sceneManager->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(Ogre::Real(0.0), Ogre::Real(0.0), Ogre::Real(0.0)), Ogre::Real(0.03), Ogre::Real(2000.0), Ogre::Real(3000.0)) ;

		// 3- Adds lights
		Ogre::Light * light = m_sceneManager->createLight("light1siga") ;
		light->setType(Ogre::Light::LT_DIRECTIONAL) ;
		Ogre::Vector3 direction(-10.0, -10.0, -1.0) ;
		direction.normalise() ;
		light->setDirection(direction) ;
		light->setDiffuseColour(0.5,0.5,0.5) ;
		light->setSpecularColour(0.5, 0.5, 0.5) ;
		m_sceneManager->setAmbientLight(::Ogre::ColourValue(0.5,0.5,0.5)) ;
		m_sceneManager->getRootSceneNode()->attachObject(light) ;

		
		// Gets the root scene node
		Ogre::SceneNode * rootSceneNode = getRootSceneNode() ;
		
		// Setups the picking
		//m_picking = new PickingBoundingBox(m_sceneManager, m_camera, OIS::MB_Left) ;
		m_picking = new PickingSelectionBuffer(m_window, m_sceneManager, m_camera, OIS::MB_Left) ;

		// Setups the camera control system
		m_cameraManager = new RTSCameraManager(m_sceneManager, m_camera, &m_keyboardState) ;

		
		//TEST de DIJKSTRA
		/*Math::Vector2<Config::Real> my_position(0,0);
		Math::Vector2<Config::Real> target_position(10,15);
		std::vector<Math::Vector2<Config::Real>> chemin;
		if(GlobalConfiguration::getCurrentMap()->isValid(target_position))
		{
			calc_chemin(GlobalConfiguration::getCurrentMap(),my_position,target_position,chemin);
		}
		else{std::cout << "position non valide" <<std::endl;}
		std::cout << "/////////////////////////////////////////////////////////////////" << std::endl;
		std::cout << "Chemin : " << std::endl;
		for(int i=0;i<chemin.size();i++)
		{
			std::cout << chemin[i]  << std::endl;
		}
		std::cout << "/////////////////////////////////////////////////////////////////" << std::endl;*/


	}

	void MainApplication::createW()
	{
		{
			std::string caption = "Budget restant = 0 ";
			b_r =  m_trayManager->createLabel(OgreBites::TL_CENTER,"Budget_restant",caption,250);	
			OgreBites::Label * info = m_trayManager->createLabel(OgreBites::TL_CENTER,"Info","Vous etes les bleus ",250);
			b_r->hide();
			info->hide();
			OgreBites::Button * valid_armee =  m_trayManager->createButton(OgreBites::TL_CENTER, "Val_armee", "Valider",100) ;
			OgreBites::Button * annul_armee =  m_trayManager->createButton(OgreBites::TL_CENTER, "Annul_armee", "Annuler",100) ;
			valid_armee->hide();
			annul_armee->hide();
		}
		{
			Ogre::StringVector tmp ;
			tmp.push_back("1 VERSUS 1") ;
			tmp.push_back("MASSIVE") ;
			OgreBites::SelectMenu * menu1 = m_trayManager->createThickSelectMenu(OgreBites::TL_CENTER, "Mode", "Choisissez le MODE de jeu", 200, 10, tmp) ;		
			menu1->hide();
		}

		{
			Ogre::StringVector tmp ;
			tmp.push_back("Map 01") ;
			tmp.push_back("Msap 02") ;
			OgreBites::SelectMenu * menu2 = m_trayManager->createThickSelectMenu(OgreBites::TL_CENTER, "Map", "Choisissez la Map", 200, 10, tmp) ;	
			menu2->hide();
		}

		{
			Ogre::StringVector tmp ;
			tmp.push_back("250") ;
			tmp.push_back("500") ;
			tmp.push_back("750") ;
			tmp.push_back("1000") ;
			tmp.push_back("1250") ;
			tmp.push_back("1500") ;
			tmp.push_back("1750") ;
			tmp.push_back("2000") ;
			tmp.push_back("2250") ;
			tmp.push_back("2500") ;
			tmp.push_back("2750") ;
			tmp.push_back("3000") ;

			OgreBites::SelectMenu * menu3 = m_trayManager->createThickSelectMenu(OgreBites::TL_CENTER, "Budget", "Choisissez le budget", 200, 10, tmp) ;		
			menu3->hide();
		}

		{
			OgreBites::Button * val = m_trayManager->createButton(OgreBites::TL_CENTER, "Val", "READY",100) ;	
			val->hide();
		}

		{
			OgreBites::Button * b_H = m_trayManager->createButton(OgreBites::TL_CENTER, "Hippo", "Hippo",100) ;	
			OgreBites::Button * b_C = m_trayManager->createButton(OgreBites::TL_CENTER, "Croco", "Croco",100) ;	
			OgreBites::Button * b_M = m_trayManager->createButton(OgreBites::TL_CENTER, "Moustic", "Moustic",100) ;	
			b_H->hide();
			b_C->hide();
			b_M->hide();
		}
		{
			arme =  m_trayManager->createLabel(OgreBites::TL_CENTER,"Arme","Votre armee est vide",500);		
			arme->hide();
		}
	}
	
	void MainApplication::choixMode(std::string m,OgreBites::SelectMenu * menu){

		std::cout << "////////////////////////////////////////////////////////////////////////////////";
		std::cout << "Vous avez choisi le mode " << m << std::endl;
		std::cout << "////////////////////////////////////////////////////////////////////////////////";
		
		if(m=="MASSIVE"){
			mode = 2;
			m_trayManager->getWidget("Budget")->show();
		}
		else{
			mode =1;
			m_trayManager->getWidget("Budget")->hide();
		}

	}

	void MainApplication::choixMap(std::string m,OgreBites::SelectMenu * menu){

		std::cout << "////////////////////////////////////////////////////////////////////////////////";
		std::cout << "Vous avez choisi la map " << m << std::endl;
		std::cout << "////////////////////////////////////////////////////////////////////////////////";
		
		if(m=="Map 01"){
			GlobalConfiguration::setCurrentMap("map01") ;
			creerChemin("map01");
		}
		else{
			GlobalConfiguration::setCurrentMap("map02") ;
			creerChemin("map02");
		}
	}

	void MainApplication::choixBudget(std::string m,OgreBites::SelectMenu * menu){
		budget = stoi(m);
		budget_restant = budget;
	}

	void MainApplication::choixVal()
	{
		m_trayManager->destroyWidget("Mode");
		m_trayManager->destroyWidget("Map");
		m_trayManager->destroyWidget("Budget");

		m_trayManager->getWidget("Hippo")->show();
		m_trayManager->getWidget("Croco")->show();
		m_trayManager->getWidget("Moustic")->show();
		m_trayManager->getWidget("Val")->hide();
		m_trayManager->getWidget("Info")->show();

		if(mode==2)	{
			maj_caption();
			b_r->show();
		}

		else{
			std::string caption = "Choisissez une unite";
			arme->setCaption(caption);
		}
		arme->show();
	}

	void MainApplication::choixUnites(OgreBites::Button *b){

		if(mode==2)
		{
			if(b->getName() == "Hippo")	{
				budget_restant -= hippo_c;
				armee[2]++;
			}
			else if(b->getName() == "Croco"){
				budget_restant -= croco_c;
				armee[1]++;
			}
			else {
				budget_restant -= moustic_c;
				armee[0]++;
			}
		}
		else{
			if(b->getName() == "Hippo")	{armee[2]++; armee[5]++;}
			else if(b->getName() == "Croco"){armee[1]++; armee[4]++;}
			else {armee[0]++; armee[3]++;}
		}
		
		maj_caption();

	}

	void MainApplication::maj_caption()
	{
		if(mode==2)
		{
			std::string caption = "Budget restant = ";
			caption.append(std::to_string((_ULonglong)budget_restant));
			b_r->setCaption(caption);
		
			std::string caption2("Votre armee contient : \n");
			caption2.append(std::to_string((_ULonglong)armee[0]));
			caption2.append(" Moustic(s), ");
			caption2.append(std::to_string((_ULonglong)armee[1]));
			caption2.append(" Croco(s) et ");
			caption2.append(std::to_string((_ULonglong)armee[2]));
			caption2.append(" Hippo(s).");
			arme->setCaption(caption2);
		}
		if(budget_restant < moustic_c || mode==1)
		{
			m_trayManager->getWidget("Val_armee")->show();
			m_trayManager->getWidget("Annul_armee")->show();
			m_trayManager->getWidget("Hippo")->hide();
			m_trayManager->getWidget("Croco")->hide();
			m_trayManager->getWidget("Moustic")->hide();
		}
		else if(budget_restant < croco_c){
			m_trayManager->getWidget("Hippo")->hide();
			m_trayManager->getWidget("Croco")->hide();
		}
		else if(budget_restant < hippo_c){
			m_trayManager->getWidget("Hippo")->hide();
		}
	}

	void MainApplication::Annul_armee(){
		budget_restant = budget;
		armee[0] = 0;
		armee[1] = 0;
		armee[2] = 0;
		armee[3] = 0;
		armee[4] = 0;
		armee[5] = 0;
		maj_caption();
		m_trayManager->getWidget("Val_armee")->hide();
		m_trayManager->getWidget("Annul_armee")->hide();
		m_trayManager->getWidget("Hippo")->show();
		m_trayManager->getWidget("Croco")->show();
		m_trayManager->getWidget("Moustic")->show();

	}

	void MainApplication::Val_armee()
	{

		::std::vector<::std::string> types ;
		types.push_back("MousticB") ;
		types.push_back("CrocoB") ;
		types.push_back("HippoB") ;
		types.push_back("MousticR") ;
		types.push_back("CrocoR") ;
		types.push_back("HippoR") ;

		int budget_r_ia = budget;
		if(mode==2)
		{
			while(budget_r_ia >= moustic_c){
		
					if(budget_r_ia >= croco_c){	
						budget_r_ia -= croco_c;
						armee[4]++;
					}

					if(budget_r_ia >= croco_c){	
						budget_r_ia -= croco_c;
						armee[4]++;
					}

					if(budget_r_ia >= hippo_c){	
						budget_r_ia -= hippo_c;
						armee[5]++;
					}				
					if(budget_r_ia >= moustic_c){	
						budget_r_ia -= moustic_c;
						armee[3]++;
					}	
				}
		}
		creerArmee(types, armee);

		m_trayManager->getWidget("Val_armee")->hide();
		m_trayManager->destroyWidget("Annul_armee");
		m_trayManager->destroyWidget("Hippo");
		m_trayManager->destroyWidget("Croco");
		m_trayManager->destroyWidget("Moustic");
		m_trayManager->destroyWidget("Budget_restant");
		m_trayManager->destroyWidget("Arme");
	}

	void MainApplication::Quit()
	{exit(0);}

	void MainApplication::creerArmee(::std::vector<::std::string> types, std::vector<int> armee){
		
		int nbMoustic = armee[0], nbCroco = armee[1], nbHippo = armee[2];
		int nbMousticIA = armee[3], nbCrocoIA = armee[4], nbHippoIA = armee[5];

		/*
		 * JOUEUR
		 */
		for(int cpt=0 ; cpt<nbMoustic ; cpt++){
			const GameElements::UnitsArchetypes::Archetype * unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get(types[0]) ;
			const GameElements::WeaponsArchetypes::Archetype * weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;
			GameElements::IAgent::Pointer m_entityAdapter = new GameElements::IAgent(unit, weapon, cpt) ;//a changer
			m_entityAdapter->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()).push(0.0)) ;
			MainApplication::nbB++;

		}
		for(int cpt=0 ; cpt<nbCroco ; cpt++){
			const GameElements::UnitsArchetypes::Archetype * unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get(types[1]) ;
			const GameElements::WeaponsArchetypes::Archetype * weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;
			GameElements::IAgent::Pointer m_entityAdapter = new GameElements::IAgent(unit, weapon, cpt) ;//a changer
			m_entityAdapter->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()).push(0.0)) ;
			MainApplication::nbB++;
		}
		for(int cpt=0 ; cpt<nbHippo ; cpt++){
			const GameElements::UnitsArchetypes::Archetype * unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get(types[2]) ;
			const GameElements::WeaponsArchetypes::Archetype * weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;
			GameElements::IAgent::Pointer m_entityAdapter = new GameElements::IAgent(unit, weapon, cpt) ; //a changer
			m_entityAdapter->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()).push(0.0)) ;
			MainApplication::nbB++;
		}

		/*
		 * IA
		 */
		for(int cpt=0 ; cpt<nbMousticIA ; cpt++){
			const GameElements::UnitsArchetypes::Archetype * unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get(types[3]) ;
			const GameElements::WeaponsArchetypes::Archetype * weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;
			GameElements::IAgent::Pointer m_entityAdapter = new GameElements::IAgent(unit, weapon, cpt) ;
			m_entityAdapter->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()).push(0.0)) ;
			MainApplication::nbR++;
		}
		for(int cpt=0 ; cpt<nbCrocoIA ; cpt++){
			const GameElements::UnitsArchetypes::Archetype * unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get(types[4]) ;
			const GameElements::WeaponsArchetypes::Archetype * weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;
			GameElements::IAgent::Pointer m_entityAdapter = new GameElements::IAgent(unit, weapon, cpt) ;
			m_entityAdapter->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()).push(0.0)) ;
			MainApplication::nbR++;
		}
		for(int cpt=0 ; cpt<nbHippoIA ; cpt++){
			const GameElements::UnitsArchetypes::Archetype * unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get(types[5]) ;
			const GameElements::WeaponsArchetypes::Archetype * weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;
			GameElements::IAgent::Pointer m_entityAdapter = new GameElements::IAgent(unit, weapon, cpt) ;
			m_entityAdapter->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()).push(0.0)) ;
			MainApplication::nbR++;
		}
		
		MainApplication::nbB_total = MainApplication::nbB;
		MainApplication::nbR_total = MainApplication::nbR;
		START = true;
	}

	void MainApplication::creerChemin(std::string map)
	{

		const char * c = &map[map.size() - 1];
		cibles.clear();
		switch(atoi(c))
		{
		case 1:
			cibles.push_back(Ogre::Vector3(0,0,70));
			cibles.push_back(Ogre::Vector3(-30,30,75));
			cibles.push_back(Ogre::Vector3(-15,-20,75));
			cibles.push_back(Ogre::Vector3(-15,-20,30));
			cibles.push_back(Ogre::Vector3(-15,-5,30));
			cibles.push_back(Ogre::Vector3(15,-5,30));
			cibles.push_back(Ogre::Vector3(30,-30,30));
			cibles.push_back(Ogre::Vector3(30,-30,75));
			cibles.push_back(Ogre::Vector3(0,0,70));
			break;
		case 2:
			cibles.push_back(Ogre::Vector3(0,0,70));
			cibles.push_back(Ogre::Vector3(-30,30,75));
			cibles.push_back(Ogre::Vector3(-15,-20,75));
			cibles.push_back(Ogre::Vector3(-15,-20,30));
			cibles.push_back(Ogre::Vector3(-15,-5,30));
			cibles.push_back(Ogre::Vector3(15,-5,30));
			cibles.push_back(Ogre::Vector3(30,-30,30));
			cibles.push_back(Ogre::Vector3(30,-30,75));
			cibles.push_back(Ogre::Vector3(0,0,70));
			break;
		default:
			CINE = false;
			std::cout << "map inexistante" << std::endl;
			break;
		}
	}

	void MainApplication::update(Ogre::Real dt)
	{

		// Necessary for GUI...
		m_trayManager->adjustTrays() ;
		
		//dt = ::std::min(dt,0.01f) ;
		static double time = 0 ;
		time += dt ;
		//if(time>4)
		//{
		//	for(float cpt=-3 ; cpt<3 ; cpt+=0.6)
		//	{
		//		//new GameElements::Bullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 2, 0, Math::Vector2<Config::Real>(-6,cpt), Math::Vector2<Config::Real>(0.5+double(rand())/RAND_MAX,0), 12) ;
		//		//new GameElements::Bullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 2, 0, Math::Vector2<Config::Real>(6,cpt), Math::Vector2<Config::Real>(-0.5-+double(rand())/RAND_MAX,0), 12) ;
		//		new GameElements::StraightBullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 2, 0, Math::Vector2<Config::Real>(-6,cpt), Math::Vector2<Config::Real>(0.5+double(rand())/RAND_MAX,0), 12) ;
		//		//new GameElements::StraightBullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 2, 0, Math::Vector2<Config::Real>(6,cpt), Math::Vector2<Config::Real>(-0.5-+double(rand())/RAND_MAX,0), 12) ;
		//		//new GameElements::BallisticBullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 2, 0, Math::Vector2<Config::Real>(-6,cpt), 2+double(rand())/RAND_MAX, Math::Vector2<Config::Real>(4-8*double(rand())/RAND_MAX,4-8*double(rand())/RAND_MAX), 2) ;
		//		new GameElements::BallisticBullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 2, 0, Math::Vector2<Config::Real>(6,cpt), 2+double(rand())/RAND_MAX, Math::Vector2<Config::Real>(4-8*double(rand())/RAND_MAX,4-8*double(rand())/RAND_MAX), 2) ;
		//	}
		//	//new GameElements::Bullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 20, 0, Math::Vector2<Config::Real>(-2,1)*3.0, Math::Vector2<Config::Real>(0.5,0), 4*3.0) ;
		//	//new GameElements::Bullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 20, 0, Math::Vector2<Config::Real>(-2,0)*3.0, Math::Vector2<Config::Real>(0.5,0), 4*3.0) ;
		//	//new GameElements::Bullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 20, 0, Math::Vector2<Config::Real>(2,0)*3.0, Math::Vector2<Config::Real>(-0.5,0), 4*3.0) ;
		//	time = 0 ;
		//}

		//m_trayManager->showAll();
		
		//Ogre::AxisAlignedBox box = Ext::Ogre::computeBoundingBox(m_shipShape) ;
		//::std::cout<<box.getMinimum()<<" / "<<box.getMaximum()<<::std::endl ;

		static Ogre::Real absoluteTime  = 0.0 ;

		absoluteTime += dt ;
		//m_entityAdapter->setPosition(Math::Vector3<Config::Real>((int(absoluteTime*1000)%10000)/2000.0, (int(absoluteTime*1000)%10000)/2000.0, 0.0)) ;
		//m_entityAdapter->setOrientation(0.0,0.0,absoluteTime) ;

		// Updates camera manager
		m_cameraManager->update(dt) ;

		if(CINE){
			Ogre::Real dist = cibles[current_cible].squaredDistance(m_camera->getPosition()) ; 
			
			if( dist < 50 ) current_cible++; // TO DO !! 

			if(current_cible >= cibles.size()) {CINE = false ;DEF_VIEW = true;}
			else{
				Ogre::Vector3 v = cibles[current_cible] - m_camera->getPosition();
				if(m_camera->getPosition()[2] < 50) m_camera->move((v/v.normalise())* 400*dt);
				else m_camera->move((v/v.normalise())* 800*dt);
			}
		}
		
		if(DEF_VIEW){
			//std::cout << "DEF_VIEW" << std::endl;
			m_camera->setPosition(Ogre::Vector3(0,0,70));
			createW();
			m_trayManager->getWidget("Mode")->show();
			m_trayManager->getWidget("Map")->show();
			m_trayManager->getWidget("Val")->show();
			DEF_VIEW = false;
		}
		if(START)
		{
			m_trayManager->destroyWidget("Val");
			m_trayManager->destroyWidget("Val_armee");
			m_trayManager->destroyWidget("Info");
			START = false;
			PLAYING = true;
		}

		// Updates (animation, behavoir & son on) are called here :)
		GlobalConfiguration::getController()->update(dt) ;

		//static bool explosionFired = false ;
		//if(absoluteTime>10.0 && !explosionFired)
		//{
		//	explosionFired = true ;
		//	// Trying explosion
		//	// create a particle system named explosions using the explosionTemplate
		//	Ogre::ParticleSystem* particleSystem = m_sceneManager->createParticleSystem("explosions2", "explosionTemplate");
		//	particleSystem->setParameter("particle_width", "1") ;
		//	particleSystem->setParameter("particle_height", "1") ;
		//	// fast forward 1 second  to the point where the particle has been emitted
		//	particleSystem->fastForward(1.0);
		//	// attach the particle system to a scene node
		//	getRootSceneNode()->attachObject(particleSystem);
		//}



		if(PLAYING && ( (MainApplication::nbB == 0) || (MainApplication::nbR == 0))){
			std::string caption =" ";
			if(MainApplication::nbR == 0) {caption = "You win !" ;}
			else {caption = "You lose :(";}
			m_trayManager->createLabel(OgreBites::TL_CENTER,"Win",caption,250);
			m_trayManager->createButton(OgreBites::TL_CENTER, "Quit", "Quit",100) ;
			PLAYING = false;
		}
		 

	}

	bool MainApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if(CINE){
			CINE = false;
			DEF_VIEW = true;
		}
		bool result = BaseApplication::mousePressed(arg, id) ;
		m_picking->update(arg, id) ;
		return result ;
	}

	void MainApplication::itemSelected( OgreBites::SelectMenu* menu )
	{
		if(menu->getName() == "Mode")
		{
			std::string m = menu->getSelectedItem();
			choixMode(m,menu);
		}
		else if(menu->getName() == "Map")
		{
			std::string m = menu->getSelectedItem();
			choixMap(m,menu);
		}
		else if(menu->getName() == "Budget")
		{
			std::string m = menu->getSelectedItem();
			choixBudget(m,menu);
		}
	}

	void MainApplication::buttonHit( OgreBites::Button* button )
	{
		if(button->getName() == "Hippo" || button->getName() == "Croco" || button->getName() == "Moustic")
		{
			choixUnites(button);
		}
		else if(button->getName() == "Val_armee"){Val_armee();}
		else if(button->getName() == "Annul_armee"){Annul_armee();}
		else if(button->getName() == "Val"){choixVal();}
		else if(button->getName() == "Quit"){Quit();}
	}

	bool MainApplication::keyPressed( const OIS::KeyEvent &arg )
	{
		if(CINE){
			CINE = false;
			DEF_VIEW = true;
		}
		// Keeps the keyboard state up to date
		m_keyboardState.notifyKeyPressed(arg.key) ;
		// Sends notification to super class
		BaseApplication::keyPressed(arg) ;
		return true ;
	}

	bool MainApplication::keyReleased( const OIS::KeyEvent &arg )
	{
		// Keeps the keyboard state up to date
		m_keyboardState.notifyKeyReleased(arg.key) ;
		// Sends notification to super class
		BaseApplication::keyReleased(arg) ;
		return true ;
	}
}


