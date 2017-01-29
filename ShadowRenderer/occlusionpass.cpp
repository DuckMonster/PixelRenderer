#include "stdafx.h"
#include "occlusionpass.h"

#include <random>

glt::Shader & COcclusionPass::GetShader( ) {
	static glt::Shader SHADER;
	static bool COMPILED = false;

	if (!COMPILED) {
		SHADER.compileFile( "shdr_ssao" );
		COMPILED = true;
	}

	return SHADER;
}

COcclusionPass::COcclusionPass( glm::ivec2 size ) :
	m_Framebuffer( size ),
	m_Blur( size ) {

	m_BufferTex.init( GL_R16F, GL_RED, GL_FLOAT, m_Framebuffer.getSize( ) );
	m_Framebuffer.bindTexture( m_BufferTex, GL_COLOR_ATTACHMENT0 );

	if (!m_Framebuffer.isComplete( )) {
		std::cout << "SSAO BUFFER ERROR (depth)!\n";
	}

	GenVectors( );
	GenNoise( );
}

void COcclusionPass::Pass( CGBuffer& gbuffer, CScene& scene ) {
	glt::Shader& shdr = GetShader( );

	m_Framebuffer.bind( );
	glDisable( GL_DEPTH_TEST );
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT );

	// GBuffer
	shdr.set( "u_deferred.world", 0 );
	shdr.set( "u_deferred.normal", 1 );
	shdr.set( "u_deferred.depth", 2 );
	shdr.set( "u_noise", 3 );

	gbuffer.GetWorld( ).bind( 0 );
	gbuffer.GetNormal( ).bind( 1 );
	gbuffer.GetDepth( ).bind( 2 );
	m_NoiseTexture.bind( 3 );

	shdr.set( "u_projection", scene.m_Camera.getMatrix( ) );
	shdr.set( "u_bias", 0.00015f );
	shdr.set( "u_radius", 0.4f );
	shdr.set( "u_noiseScale", vec2( m_Framebuffer.getSize( ) ) / vec2( 256, 256 ) );

	glt::FrameBufferUtils::Quad( shdr );

	m_Framebuffer.release( );

	m_Blur.ApplyBlur( m_BufferTex );
}

void COcclusionPass::GenVectors( ) {
	std::uniform_real_distribution<float> randomFloats( 0.f, 1.0f ); // random floats between 0.0 - 1.0
	std::default_random_engine generator;

	std::vector<glm::vec3> ssaoKernel;
	for (size_t i = 0; i < 64; i++) {
		vec3 v(
			randomFloats( generator ) * 2.f - 1.f,
			randomFloats( generator ) * 2.f - 1.f,
			randomFloats( generator )
			);

		v = normalize( v );
		v *= randomFloats( generator );
		ssaoKernel.push_back( v );
	}

	GetShader( ).set( "u_kernels", &ssaoKernel[0], sizeof( vec3 ) * 64 );
}

void COcclusionPass::GenNoise( ) {
	std::uniform_real_distribution<float> randomFloats( -1.f, 1.0f ); // random floats between 0.0 - 1.0
	std::default_random_engine generator;

	std::vector<glm::vec3> ssaoNoise;
	for (size_t i = 0; i < 256 * 256; i++) {
		vec3 v(
			randomFloats( generator ),
			randomFloats( generator ),
			randomFloats( generator )
			);

		v = normalize( v );
		ssaoNoise.push_back( v );
	}

	GLuint texHandle = m_NoiseTexture.getHandle( );

	glBindTexture( GL_TEXTURE_2D, texHandle );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, 256, 256, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0] );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}
