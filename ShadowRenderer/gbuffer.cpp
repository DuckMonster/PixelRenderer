#include "stdafx.h"
#include "gbuffer.h"

#include "colorutils.h"

glt::Shader& CGBuffer::GetShader( ) {
	static glt::Shader SHADER;
	static bool COMPILED = false;

	if (!COMPILED) {
		SHADER.compileFile( "shdr_deferred" );
		COMPILED = true;
	}

	return SHADER;
}

CGBuffer::CGBuffer( glm::ivec2 screen ) :
	m_Framebuffer( screen ),
	m_WorldTex( ), m_NormalTex( ), m_ColorTex( ), m_DepthTex( ) {

	m_ColorTex.init( GL_RGBA16F, GL_RGBA, GL_FLOAT, screen );
	m_EmissiveTex.init( GL_R16F, GL_RED, GL_FLOAT, screen );
	m_WorldTex.init( GL_RGB16F, GL_RGB, GL_FLOAT, screen );
	m_NormalTex.init( GL_RGB16F, GL_RGB, GL_FLOAT, screen );
	m_DepthTex.init( GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, screen );

	m_Framebuffer.bindTexture( m_ColorTex, GL_COLOR_ATTACHMENT0 );
	m_Framebuffer.bindTexture( m_EmissiveTex, GL_COLOR_ATTACHMENT1 );
	m_Framebuffer.bindTexture( m_WorldTex, GL_COLOR_ATTACHMENT2 );
	m_Framebuffer.bindTexture( m_NormalTex, GL_COLOR_ATTACHMENT3 );
	m_Framebuffer.bindTexture( m_DepthTex, GL_DEPTH_ATTACHMENT );

	if (!m_Framebuffer.isComplete( ))
		std::cout << "DEFERRED ERROR!\n";
}

void CGBuffer::Bind( ) {
	m_Framebuffer.bind( );
	glEnable( GL_DEPTH_TEST );
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
}

void CGBuffer::Release( ) {
	m_Framebuffer.release( );
}

void CGBuffer::Render( CScene & scene ) {
	glt::Shader& shdr = GetShader( );

	vec4 skyColor = StringToColor( "517174" );

	m_Framebuffer.bind( );
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

	shdr.set( "u_camera", scene.m_Camera.getMatrix( ) );

	// Render objects
	for (CSceneObject* obj : scene.m_Objects) {
		shdr.set( "u_model", obj->GetTransform( ).getMatrix( ) );
		shdr.set( "u_material.color", obj->GetMaterial( ).color );
		shdr.set( "u_material.emissive", obj->GetMaterial( ).emissive );

		obj->GetMesh( ).draw( );
	}

	m_Framebuffer.release( );

	glDisable( GL_DEPTH_TEST );
}