#pragma once
#include "gbuffer.h"
#include "blur.h"

class COcclusionPass {
public:
	glt::Shader& GetShader( );

public:
	COcclusionPass( glm::ivec2 size );

	void Pass( CGBuffer& gbuffer, CScene& scene );
	glt::Texture& GetBuffer( ) { return m_BufferTex; }
	glt::Texture& GetNoise( ) { return m_NoiseTexture; }

private:
	void GenVectors( );
	void GenNoise( );

	glt::FrameBuffer	m_Framebuffer;
	glt::Texture		m_BufferTex;
	glt::Texture		m_NoiseTexture;

	CBlur				m_Blur;
};