#pragma once
#include "scene.h"

class CGBuffer {
public:
	static glt::Shader& GetShader( );

public:
	CGBuffer( glm::ivec2 screen );

	void Bind( );
	void Release( );

	void Render( CScene& scene );

	glt::Texture& GetColor( ) { return m_ColorTex; }
	glt::Texture& GetEmissive( ) { return m_EmissiveTex; }
	glt::Texture& GetWorld( ) { return m_WorldTex; }
	glt::Texture& GetNormal( ) { return m_NormalTex; }
	glt::Texture& GetDepth( ) { return m_DepthTex; }

private:
	glt::FrameBuffer	m_Framebuffer;

	glt::Texture		m_ColorTex;
	glt::Texture		m_EmissiveTex;
	glt::Texture		m_WorldTex;
	glt::Texture		m_NormalTex;
	glt::Texture		m_DepthTex;
};