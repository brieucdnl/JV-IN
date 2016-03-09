#include <stdafx.h>
#include <OgreFramework/PickingSelectionBufferAgent.h>
#include <OgreFramework/PickableObject.h>

namespace OgreFramework
{


	PickingSelectionBufferAgent::PickingSelectionBufferAgent( Ogre::RenderWindow *renderWindow, Ogre::SceneManager * sceneManager, Ogre::Camera * camera, OIS::MouseButtonID buttonId ) 
		: Picking(sceneManager, camera, buttonId){
		mSelectionBuffer = new Ogre::SelectionBuffer(sceneManager, camera, renderWindow);
	}

	void PickingSelectionBufferAgent::update( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		//if(mSelectionBuffer)
		//	mSelectionBuffer->ShowOverlay(true);

		Ogre::Entity *selectedEntity ;
		
		if(m_isActive)
		{
			selectedEntity = mSelectionBuffer->OnSelectionClick(arg.state.X.abs, arg.state.Y.abs) ;
			if(id==m_button)
			{
			
				if(static_cast<Ogre::MovableObject*>(selectedEntity)!=m_lastSelected)
				{
					notifyUnselected(getLastSelected()) ;
				}
				if(selectedEntity!=0)
				{
					m_lastSelected = selectedEntity ;
					notifySelected(selectedEntity) ;
					::std::cout<<"Picking on object : "<<selectedEntity->getName()<<::std::endl ;
				}
			}
			if(id == OIS::MB_Right)
			{
				::std::cout<<"Going on object: "<<selectedEntity->getName()<<std::endl ;
			}
		}
	}
}