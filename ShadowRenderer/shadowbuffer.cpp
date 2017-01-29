#include "stdafx.h"
#include "shadowbuffer.h"

glt::Shader& CShadowBuffer::GetDepthShader( ) {
	static glt::Shader SHADER;
	static bool COMPILED = false;

	if (!COMPILED) {
		SHADER.compileFile( "shdr_depth" );
		COMPILED = true;
	}

	return SHADER;
}
glt::Shader& CShadowBuffer::GetShadowShader( ) {
	static glt::Shader SHADER;
	static bool COMPILED = false;

	if (!COMPILED) {
		SHADER.compileFile( "shdr_shadow" );
		COMPILED = true;
	}

	return SHADER;
}

CShadowBuffer::CShadowBuffer( glm::ivec2 size, glm::ivec2 depthSize ) :
	m_Bias( 0.0008f ),
	m_AngleBias( 0.16 ),
	m_DepthBuffer( depthSize ), m_DepthTex( ),
	m_ShadowBuffer( size ), m_ShadowDepthTex( ), m_ShadowValueTex( ) {

	// DEPTH BUFFER
	m_DepthTex.init( GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, depthSize );
	m_DepthBuffer.bindTexture( m_DepthTex, GL_DEPTH_ATTACHMENT );

	if (!m_DepthBuffer.isComplete( )) {
		std::cout << "SHADOW BUFFER ERROR (depth)!\n";
	}

	// SHADOW BUFFER
	m_ShadowValueTex.init( GL_R16F, GL_RED, GL_FLOAT, m_ShadowBuffer.getSize( ) );
	m_ShadowDepthTex.init( GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, m_ShadowBuffer.getSize( ) );
	m_ShadowBuffer.bindTexture( m_ShadowValueTex, GL_COLOR_ATTACHMENT0 );
	m_ShadowBuffer.bindTexture( m_ShadowDepthTex, GL_DEPTH_ATTACHMENT );

	if (!m_ShadowBuffer.isComplete( ))
		std::cout << "SHADOW BUFFER ERROR (shadow)!\n";
}

void CShadowBuffer::Render( CScene& scene ) {
	{
		glt::Shader& shdr = GetDepthShader( );

		// Depth buffer
		m_DepthBuffer.bind( );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glEnable( GL_DEPTH_TEST );

		shdr.set( "u_camera", scene.m_Light.GetMatrix( ) );

		// Render objects
		for (CSceneObject* obj : scene.m_Objects) {
			shdr.set( "u_model", obj->GetTransform( ).getMatrix( ) );
			obj->GetMesh( ).draw( );
		}

		m_DepthBuffer.release( );
	}

	// Shadow buffer
	{
		glt::Shader& shdr = GetShadowShader( );

		m_ShadowBuffer.bind( );
		glClearColor( 0.0, 0.0, 0.0, 1.0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		shdr.set( "u_camera", scene.m_Camera.getMatrix( ) );
		shdr.set( "u_light", scene.m_Light.GetMatrix( ) );
		shdr.set( "u_lightdirection", scene.m_Light.m_Direction );

		shdr.set( "u_bias", m_Bias );
		shdr.set( "u_anglebias", m_AngleBias );

		m_DepthTex.bind( );

		// Objects
		for (CSceneObject* obj : scene.m_Objects) {
			shdr.set( "u_model", obj->GetTransform( ).getMatrix( ) );
			obj->GetMesh( ).draw( );
		}

		m_ShadowBuffer.release( );
	}

	glDisable( GL_DEPTH_TEST );
}