#include "stdafx.h"
#include "scene.h"

#include "colorutils.h"

CScene::CScene( glm::ivec2 screenSize ) :
	m_Objects( ), m_Light( ),
	m_Camera( screenSize ) {
}

CScene::~CScene( ) {
	for (CSceneObject* obj : m_Objects)
		delete obj;

	m_Objects.clear( );
}

void CScene::Load( ) {
	m_Objects.push_back( new CSceneObject( "Assets/test0.obj", TMaterial( StringToColor( "e2d088" ) ) ) );
	m_Objects.push_back( new CSceneObject( "Assets/test1.obj", TMaterial( StringToColor( "e19863" ) ) ) );
	m_Objects.push_back( new CSceneObject( "Assets/cube.obj", glt::Transform( glm::vec3( 0.f, 5.f, 0.f ), vec3( 0.f ), vec3( 1.f ) ), TMaterial( vec4( 1.7f ), 1.f ) ) );
	m_Objects.push_back( new CSceneObject( "Assets/test2.obj", TMaterial( StringToColor( "00baff" ) * 1.9f, 1.f ) ) );
	m_Objects.push_back( new CSceneObject( "Assets/test3.obj", TMaterial( StringToColor( "ffd118" ) * 1.f, 1.f ) ) );
	//m_Objects.push_back( new CSceneObject( "Assets/test4.obj", TMaterial( StringToColor( "ff00fc" ) * 1.f, 1.f ) ) );

	m_Light.m_Position = vec3( 1, 1, 1 ) * 5.f;
	m_Light.m_Direction = glm::normalize( -m_Light.m_Position );

	m_Camera.position = vec3( sin( 8.5f ), 0.8f, cos( 8.5f ) ) * 10.f;
	m_Camera.direction = -m_Camera.position;

	m_Camera.farPlane = 150.f;
}