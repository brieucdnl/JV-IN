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

namespace OgreFramework
{
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
		
		// --------------------------------------
		// Setups the GUI (it's a test / example)
		// --------------------------------------
		
		{
			Ogre::StringVector tmp ;
			tmp.push_back("item 1") ;
			tmp.push_back("item 2") ;
			tmp.push_back("item 3") ;
			OgreBites::SelectMenu * menu1 = m_trayManager->createThickSelectMenu(OgreBites::TL_TOPLEFT, "Menu 1", "Foo", 200, 10, tmp) ;
		}
		{
			Ogre::StringVector tmp ;
			tmp.push_back("item 1") ;
			tmp.push_back("item 2") ;
			tmp.push_back("item 3") ;
			OgreBites::SelectMenu * menu1 = m_trayManager->createThickSelectMenu(OgreBites::TL_TOPLEFT, "Menu 2", "Foo 2", 200, 10, tmp) ;
		}

		// Setups the picking
		//m_picking = new PickingBoundingBox(m_sceneManager, m_camera, OIS::MB_Left) ;
		m_picking = new PickingSelectionBuffer(m_window, m_sceneManager, m_camera, OIS::MB_Left) ;

		// Setups the camera control system
		m_cameraManager = new RTSCameraManager(m_sceneManager, m_camera, &m_keyboardState) ;

		// ----------------------------------------
		// Creates two entities for testing purpose
		// ----------------------------------------
		int budget = 2000;
		choixUnites(budget);
	}

	void MainApplication::choixUnites(const int budget){
		assert( budget > 0 );

		int budget_r = budget;
		int budget_r_ia = budget;
		int hippo_c = 1000, croco_c = 500, moustic_c = 250;
		int choix;

		//0 = moustic ; 1 = croco; 2 = hippo;
		//3 = mousticIA ; 4 = crocoIA; 5 = hippoIA;
		std::vector<int> armee(6,0);
		
		/*
		 * Choix de l'IA
		 * 2C -> 1H -> 1M
		 */
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

		std::cout << "////////////////////////////////////////////////////////////////////////////////";
		std::cout << "////////////////////////////// Selection des unites de l'armee /////////////////";
		std::cout << "////////////////////////////////////////////////////////////////////////////////";	
		while(budget_r >= moustic_c){
			std::cout << "Montant restant de la caisse noire : $" << budget_r << "/$" << budget << std::endl;
			if(budget_r >= hippo_c){
				std::cout << "Veuillez choisir entre :\n (1) _Moustic ($250).\n (2) _Croco ($500).\n (3) _Hippo ($1000)." << std::endl;
				std::cin >> choix;
			}
			else if(budget_r >= croco_c){
				std::cout << "Veuillez choisir entre :\n (1) _Moustic ($250).\n (2) _Croco ($500)." << std::endl;
				std::cin >> choix;
			}
			else{
				std::cout << "Veuillez choisir entre :\n (1) _Moustic ($250)." << std::endl;
				std::cin >> choix;
			}
			switch(choix){
				case 1:
					budget_r -= moustic_c;
					armee[0]++;
					break;
				case 2:
					budget_r -= croco_c;
					armee[1]++;
					break;
				case 3:
					budget_r -= hippo_c;
					armee[2]++;
					break;
				default:
					std::cout << "Mauvais choix :(" << std::endl;
					break;
			}
		}
		
		std::cout << "////////////////////////////////////////////////////////////////////////////////";
		std::cout << "////////////////////////////////////////////////////////////////////////////////";
		std::cout << "Votre armee contient : " << armee[2] << " Hippo(s), " << armee[1] << " Croco(s) et " << armee[0] << " Moustic(s) " << std::endl;
		std::cout << "L' armee IA contient : " << armee[5] << " Hippo(s), " << armee[4] << " Croco(s) et " << armee[3] << " Moustic(s) " << std::endl;
		::std::vector<::std::string> types ;
		types.push_back("MousticB") ;
		types.push_back("CrocoB") ;
		types.push_back("HippoB") ;
		types.push_back("MousticR") ;
		types.push_back("CrocoR") ;
		types.push_back("HippoR") ;
		std::cout << "////////////////////////////////////////////////////////////////////////////////";
		std::cout << "////////////////////////////////////////////////////////////////////////////////" << std::endl;
		creerArmee(types, armee);
	}

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
		}
		for(int cpt=0 ; cpt<nbCroco ; cpt++){
			const GameElements::UnitsArchetypes::Archetype * unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get(types[1]) ;
			const GameElements::WeaponsArchetypes::Archetype * weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;
			GameElements::IAgent::Pointer m_entityAdapter = new GameElements::IAgent(unit, weapon, cpt) ;//a changer
			m_entityAdapter->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()).push(0.0)) ;
		}
		for(int cpt=0 ; cpt<nbHippo ; cpt++){
			const GameElements::UnitsArchetypes::Archetype * unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get(types[2]) ;
			const GameElements::WeaponsArchetypes::Archetype * weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;
			GameElements::IAgent::Pointer m_entityAdapter = new GameElements::IAgent(unit, weapon, cpt) ; //a changer
			m_entityAdapter->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()).push(0.0)) ;
		}

		/*
		 * IA
		 */
		for(int cpt=0 ; cpt<nbMousticIA ; cpt++){
			const GameElements::UnitsArchetypes::Archetype * unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get(types[3]) ;
			const GameElements::WeaponsArchetypes::Archetype * weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;
			GameElements::IAgent::Pointer m_entityAdapter = new GameElements::IAgent(unit, weapon, cpt) ;
			m_entityAdapter->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()).push(0.0)) ;
		}
		for(int cpt=0 ; cpt<nbCrocoIA ; cpt++){
			const GameElements::UnitsArchetypes::Archetype * unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get(types[4]) ;
			const GameElements::WeaponsArchetypes::Archetype * weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;
			GameElements::IAgent::Pointer m_entityAdapter = new GameElements::IAgent(unit, weapon, cpt) ;
			m_entityAdapter->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()).push(0.0)) ;
		}
		for(int cpt=0 ; cpt<nbHippoIA ; cpt++){
			const GameElements::UnitsArchetypes::Archetype * unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get(types[5]) ;
			const GameElements::WeaponsArchetypes::Archetype * weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;
			GameElements::IAgent::Pointer m_entityAdapter = new GameElements::IAgent(unit, weapon, cpt) ;
			m_entityAdapter->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()).push(0.0)) ;
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
	}

	bool MainApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		bool result = BaseApplication::mousePressed(arg, id) ;
		m_picking->update(arg, id) ;
		return result ;
	}

	void MainApplication::itemSelected( OgreBites::SelectMenu* menu )
	{
		::std::cout<<"Selection in menu "<<menu->getCaption()<<", item: "<<menu->getSelectedItem()<<::std::endl ;
	}

	bool MainApplication::keyPressed( const OIS::KeyEvent &arg )
	{
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


