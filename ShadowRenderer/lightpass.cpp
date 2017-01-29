#include "stdafx.h"
#include "lightpass.h"

glt::Shader & CLightPass::GetShader( ) {
	static glt::Shader SHADER;
	static bool COMPILED = false;

	if (!COMPILED) {
		SHADER.compileFile( "shdr_light_d" );
		COMPILED = true;
	}

	return SHADER;
}

glt::Shader & CLightPass::GetFilterShader( ) {
	static glt::Shader SHADER;
	static bool COMPILED = false;

	if (!COMPILED) {
		SHADER.compileFile( "shdr_apply" );
		COMPILED = true;
	}

	return SHADER;
}

CLightPass::CLightPass( glm::ivec2 size ) :
	m_ShadowAlpha( 0.05f ),
	m_SSAOAlpha( 1.f ),
	m_BloomAlpha( 1.f ),
	m_Framebuffer( size ),
	m_Bloom( size ),
	m_OutputFramebuffer( size ) {

	m_ColorTex.init( GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, m_Framebuffer.getSize( ) );
	m_BloomTex.init( GL_RGB16F, GL_RGB, GL_FLOAT, m_Framebuffer.getSize( ) );

	m_Framebuffer.bindTexture( m_ColorTex, GL_COLOR_ATTACHMENT0 );
	m_Framebuffer.bindTexture( m_BloomTex, GL_COLOR_ATTACHMENT1 );

	m_OutputTex.init( GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, m_Framebuffer.getSize( ) );
	m_OutputFramebuffer.bindTexture( m_OutputTex, GL_COLOR_ATTACHMENT0 );

	if (!m_Framebuffer.isComplete( )) {
		std::cout << "LIGHT BUFFER ERROR (depth)!\n";
	}
}

void CLightPass::Pass( CGBuffer& gbuffer, CShadowBuffer& shadow, COcclusionPass& occlusion, CScene& scene ) {
	{
		glt::Shader& shdr = GetShader( );

		m_Framebuffer.bind( );
		glDisable( GL_DEPTH_TEST );
		glClear( GL_COLOR_BUFFER_BIT );

		// Upload light
		scene.m_Light.UploadTo( shdr );

		// Effects
		shdr.set( "u_effects.ssao_alpha", m_SSAOAlpha );
		shdr.set( "u_effects.bloom_alpha", m_BloomAlpha );

		// GBuffer
		shdr.set( "u_deferred.world", 0 );
		shdr.set( "u_deferred.normal", 1 );
		shdr.set( "u_deferred.color", 2 );
		shdr.set( "u_deferred.emissive", 3 );
		shdr.set( "u_deferred.shadow", 4 );
		shdr.set( "u_deferred.occlusion", 5 );

		gbuffer.GetWorld( ).bind( 0 );
		gbuffer.GetNormal( ).bind( 1 );
		gbuffer.GetColor( ).bind( 2 );
		gbuffer.GetEmissive( ).bind( 3 );
		shadow.GetShadowBuffer( ).bind( 4 );
		occlusion.GetBuffer( ).bind( 5 );

		shdr.set( "u_shadow.alpha", m_ShadowAlpha );
		glt::FrameBufferUtils::Quad( shdr );

		m_Framebuffer.release( );
	}

	// Bloom effect
	m_Bloom.ApplyBlur( m_BloomTex );

	// Apply to lighting
	{
		glt::Shader& shdr = GetFilterShader( );

		m_OutputFramebuffer.bind( );

		shdr.set( "u_first", 0 );
		shdr.set( "u_second", 1 );

		m_ColorTex.bind( 0 );
		m_BloomTex.bind( 1 );

		glt::FrameBufferUtils::Quad( shdr );

		m_OutputFramebuffer.release( );
	}
}