#include "stdafx.h"
#include "bloom.h"

glt::Shader& CBloom::GetShader( ) {
	static glt::Shader SHADER;
	static bool COMPILED = false;

	if (!COMPILED) {
		SHADER.compileFile( "shdr_bloom" );
		COMPILED = true;
	}

	return SHADER;
}

CBloom::CBloom( glm::ivec2 size ) :
	m_Framebuffer( size ) {

	m_Outputs[0].init( GL_RGB16F, GL_RGB, GL_FLOAT, size );
	m_Outputs[1].init( GL_RGB16F, GL_RGB, GL_FLOAT, size );
}

void CBloom::Blur( glt::Texture& input ) {
	glt::Shader& shdr = GetShader( );

	glDisable( GL_DEPTH_TEST );

	// Horizontal
	Blur( input, m_Outputs[0], vec2( 1.0, 0.0 ) );
	Blur( m_Outputs[0], m_Outputs[1], vec2( 0.0, 1.0 ) );

	for (size_t i = 0; i < 2; i++) {
		Blur( m_Outputs[1], m_Outputs[0], vec2( 1.0, 0.0 ) );
		Blur( m_Outputs[0], m_Outputs[1], vec2( 0.0, 1.0 ) );
	}

	m_Framebuffer.release( );
}

void CBloom::ApplyBlur( glt::Texture& input ) {
	Blur( input );

	m_Framebuffer.bindTexture( input, GL_COLOR_ATTACHMENT0 );
	m_Framebuffer.bind( );

	m_Outputs[1].bind( );
	glt::FrameBufferUtils::Quad( );

	m_Framebuffer.release( );
	m_Framebuffer.bindTexture( m_Outputs[0], GL_COLOR_ATTACHMENT0 );
}

void CBloom::Blur( glt::Texture & input, glt::Texture & output, glm::vec2 direction ) {
	glt::Shader& shdr = GetShader( );

	shdr.set( "u_blurdirection", direction );
	m_Framebuffer.bindTexture( output, GL_COLOR_ATTACHMENT0 );

	m_Framebuffer.bind( );

	glClearColor( 1.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );

	input.bind( );
	glt::FrameBufferUtils::Quad( shdr );

	m_Framebuffer.release( );
}