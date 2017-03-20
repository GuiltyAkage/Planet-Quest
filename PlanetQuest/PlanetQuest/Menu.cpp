//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2016 Media Design School
//
// File Name	: Menu.h
// Description	: Creates a Menu that can be navigated and determines the programmes next execution task based on menu selection
// Author		: Cameron Peet
// Mail			: Cameron.Peet@mediadesign.school.nz
//


#include "Menu.h"
#include "SceneManager.h"

/**
*
* This function initialises the menu.
* (Task ID: Menu)
*
* @author  and Cameron Peet
* @param void
* @return Returns true if successful
*
*/
bool CMenu::Init()
{
	m_pAllText.push_back(&m_pMainMenu);

	/*MainMenu*/
	//Menu Text in order seen top to bottom.
	//Play button - Clicked to open the Play Menu
	m_pMainMenu.push_back(new TextLabel("Play", "Assets/Fonts/waltographUI.ttf"));
	m_pMainMenu[0]->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	m_pMainMenu.push_back(new TextLabel("Controls", "Assets/Fonts/waltographUI.ttf"));
	m_pMainMenu[1]->setColor(glm::vec3(1.0f, 1.0f, 1.0f));


	//Quit Application Button - Click to glutLeaveMainLoop() and end application
	m_pMainMenu.push_back(new TextLabel("Quit", "Assets/Fonts/waltographUI.ttf"));
	m_pMainMenu[2]->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	//m_pMainMenu.push_back(new TextLabel("Press WASD to move", "Assets/Fonts/waltographUI.ttf"));
	//m_pMainMenu[3]->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	//m_pMainMenu[3]->setActive(false);
	//m_pMainMenu[3]->setButton(false);

	//m_pMainMenu.push_back(new TextLabel("Press IJKL to shoot", "Assets/Fonts/waltographUI.ttf"));
	//m_pMainMenu[4]->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	//m_pMainMenu[4]->setActive(false);
	//m_pMainMenu[4]->setButton(false);

	
	/*********
	Models
	**********/
	//EditBox - To represent the text field in which the users character presses will appear.
	m_pEditBox = new Model(QUAD, "AsteroidBelt.jpg");
	VALIDATE(m_pEditBox->Initialise());
	m_pEditBox->m_Scale = glm::vec3(2, 1, 1);
	m_pEditBox->m_Rotation = glm::quat(1.0f, glm::radians(60.0f), 0.0f, 0.0f);
	m_pModels.push_back(m_pEditBox);

	//Current Menu State - Used to determine rendering and proccessing.
	m_strMenuName = "MainMenu";

	SetTextPositions();

	return true;
}

/**
*
* This function sets the text positions.
* (Task ID: Menu)
*
* @author Cameron Peet
* @param void
* @return void
*
*/
void CMenu::SetTextPositions()
{
	float centreX, centreY;
	centreX = Utils::WIDTH / 2;
	centreY = Utils::HEIGHT / 2;
	float maxItems = 8.0f;
	float spacing = Utils::HEIGHT / maxItems;
	float yTop = Utils::HEIGHT - spacing * 3;


	m_pMainMenu[0]->setPosition(glm::vec3(centreX  / 2, yTop, 10));
	m_pMainMenu[1]->setPosition(glm::vec3(centreX  / 2, yTop - spacing, 10));
	m_pMainMenu[2]->setPosition(glm::vec3(centreX  / 2, yTop - spacing * 2, 10));
	//m_pMainMenu[3]->setPosition(glm::vec3(centreX / 2 + spacing * 2, yTop - spacing * 3, 10));
	//m_pMainMenu[4]->setPosition(glm::vec3(centreX / 2 + spacing * 2, yTop - spacing * 4, 10));
	m_pEditBox->m_Position = glm::vec3(0, -1, 0);

}

/**
*
* This function renders the menu.
* (Task ID: Menu)
*
* @author Cameron Peet
* @param program This is the program.
* @param camera This is the camera.
* @return void
*
*/
void CMenu::Render(GLuint program, Camera& camera)
{
	for (auto itr : m_pModels)
	{
		itr->Render(program, camera);
	}
	for (auto itr : *m_pCurrentMenu)
	{
		itr->Render(camera);
	}
}

/**
*
* Handles mouse over.
* (Task ID: Menu)
*
* @author Cameron Peet
* @param x This is the mouse position.
* @param y This is the mouse position.
* @return void
*/
void CMenu::PassiveMotion(int x, int y)
{
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	for (auto itr : *m_pCurrentMenu)
	{
		if (itr->isActive() && itr->isButton())
		{
			if (x > itr->position.x && x < itr->position.x + itr->width && y > itr->position.y && y < itr->position.y + itr->height)
			{
				if (!itr->isHighlighted())
				{
					CResources::PlayAudio("mouseover.wav");
				}
				itr->setHighlighted(true);
				itr->setHighlight(glm::vec3(0, 0, 1));
			}
			else
			{
				if (itr->isHighlighted())
				{
					itr->setHighlighted(false);
				}
			}
		}
	}
}

/**
*
* Handles mouse clicks.
* (Task ID: Menu)
*
* @author Cameron Peet
* @param button This is the button.
* @param state This is the state.
* @param x This is the mouse position.
* @param y This is the mouse position.
* @return void
*/
void CMenu::Mouse(int button, int state, int x, int y)
{
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;
	//Dp Nothing
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			for (auto itr : *m_pCurrentMenu)
			{
				if (itr->isActive() && itr->isButton())
				{

					if (x > itr->position.x && x < itr->position.x + itr->width && y > itr->position.y && y < itr->position.y + itr->height)
					{
						std::string text = itr->getText();
						if (text == "Play")
						{
							CSceneManager& _rSceneManager = CSceneManager::GetInstance();
							_rSceneManager.SelectScene("Game");
						}

						else if (text == "Quit")
						{
							glutLeaveMainLoop();
							//Utils::g_Play = false;
						}
					
						itr->setHighlighted(false);
						break;
					}
				}
			}
		}
	}
}

void CMenu::KeyboardDown(unsigned char c, int x, int y)
{

}

/**
*
* Keyboard release.
* (Task ID: Game)
*
* @author Cameron Peet
* @param c This is the character.
* @param x This is the position.
* @param y This is the position.
* @return void
*
*/
void CMenu::KeyboardUp(unsigned char c, int x, int y)
{
	//if (m_strMenuName == "GetName")
	//{
	//	if (c == '\r')
	//	{
	//		if (m_pName->getText().length() >= 3)
	//		{
	//			m_pPlayerNames.push_back(m_pName);
	//			CNetwork& _rNetwork = CNetwork::GetInstance();
	//			_rNetwork.GetNetworkEntity()->SetName(m_pPlayMenu[3]->getText());
	//			if (_rNetwork.GetNetworkEntity()->IsHost())
	//			{
	//				m_strMenuName = "LobbyMenu";
	//				m_pCurrentMenu = &m_pLobbyMenu;
	//			}
	//			else
	//			{
	//				_rNetwork.GetNetworkEntity()->SetName(m_pName->getText()); 
	//				_rNetwork.GetNetworkEntity()->SelectServer(m_strServerName);		
	//			}

	//		}
	//	}
	//	else if (m_pName->getText().length() < 12)
	//	{
	//		m_pName->AddText(c);
	//	}
	//}
}

/**
*
* Reshapes the menu.
* (Task ID: Menu)
*
* @author Cameron Peet
* @param width This is the width.
* @param height This is the height.
* @return void
*
*/
void CMenu::Reshape(int width, int height)
{
	SetTextPositions();
}

/**
*
* Finds the games.
* (Task ID: Menu)
*
* @author Cameron Peet
* @param void
* @return Returns true if successful.
*
*/


/**
*
* Updates the menu.
* (Task ID: Menu)
*
* @author Cameron Peet
* @param fDeltaTime This is the delta time.
* @return void
*
*/
void CMenu::Update(float fDeltaTime)
{

}