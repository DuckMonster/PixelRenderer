#pragma once
#include "gbuffer.h"
#include "shadowbuffer.h"
#include "occlusionpass.h"
#include "bloom.h"

class CLightPass {
public:
	static glt::Shader& GetShader( );
	static glt::Shader& GetFilterShader( );

public:
	CLightPass( glm::ivec2 size );
	void Pass( CGBuffer& gbuffer, CShadowBuffer& shadow, COcclusionPass& occlusion, CScene& scenea );

	glt::Texture& GetOutput( ) { return m_OutputTex; }
	glt::Texture& GetBloom( ) { return m_BloomTex; }

	float	m_ShadowAlpha;

	float	m_SSAOAlpha;
	float	m_BloomAlpha;

private:
	glt::FrameBuffer	m_Framebuffer;
	glt::Texture		m_ColorTex;
	glt::Texture		m_BloomTex;

	glt::FrameBuffer	m_OutputFramebuffer;
	glt::Texture		m_OutputTex;

	CBloom				m_Bloom;
};