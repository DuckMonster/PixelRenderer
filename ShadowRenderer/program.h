#pragma once
#include "framework.h"
#include "dirlight.h"
#include "fbuffer.h"
#include "gbuffer.h"
#include "lightpass.h"
#include "shadowbuffer.h"
#include "occlusionpass.h"

class CProgram : public IProgram {
public:
	CProgram( CFramework* framework );

	void Load( ) override;
	void Update( float delta ) override;
	void Render( float delta ) override;
	void KeyEvent( sf::Keyboard::Key key, int state ) override;

	CFramework* GetFramework( ) { return m_Framework; }

private:
	CFramework* const	m_Framework;

	CGBuffer			m_GBuffer;
	CShadowBuffer		m_ShadowBuffer;
	COcclusionPass		m_OcclusionPass;

	CLightPass			m_LightPass;

	CScene				m_Scene;
};