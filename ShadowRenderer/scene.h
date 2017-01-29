#pragma once
#include "sceneobject.h"
#include "dirlight.h"
#include "framework.h"

class CScene {
public:
	CScene( glm::ivec2 screenSize );
	~CScene( );

	void Load( );

	std::vector<CSceneObject*>	m_Objects;
	CDirLight					m_Light;
	glt::Camera					m_Camera;
};