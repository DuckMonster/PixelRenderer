#pragma once
#include "scene.h"

class CShadowBuffer {
public:
	glt::Shader& GetDepthShader( );
	glt::Shader& GetShadowShader( );

public:
	CShadowBuffer( glm::ivec2 size, glm::ivec2 depthSize );

	void Render( CScene& scene );
	glt::Texture& GetDepthBuffer( ) { return m_DepthTex; }
	glt::Texture& GetShadowBuffer( ) { return m_ShadowValueTex; }

	float				m_Bias;
	float				m_AngleBias;

private:
	glt::FrameBuffer	m_DepthBuffer;
	glt::Texture		m_DepthTex;

	glt::FrameBuffer	m_ShadowBuffer;
	glt::Texture		m_ShadowValueTex;
	glt::Texture		m_ShadowDepthTex;
};