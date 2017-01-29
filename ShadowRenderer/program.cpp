#include "stdafx.h"
#include "program.h"

#include "objloader.h"
#include "colorutils.h"

const int SCALE = 3;

CProgram::CProgram( CFramework* framework ) :
	m_Framework( framework ),
	m_Scene( framework->WindowSize( ) / SCALE ),
	m_GBuffer( framework->WindowSize( ) / SCALE ),
	m_LightPass( framework->WindowSize( ) / SCALE ),
	m_OcclusionPass( framework->WindowSize( ) / SCALE ),
	m_ShadowBuffer( framework->WindowSize( ) / SCALE, glm::ivec2( 4096 ) ) {
}

void CProgram::Load( ) {
	/*m_Shader.compileFile( "shdr_default" );
	m_Lighting.compileFile( "shdr_light_d" );*/

	m_Scene.Load( );

	/*m_Camera.position = glm::vec3( 0.f, 0.f, 5.f );
	m_Camera.direction = -m_Camera.position;

	MeshData loadedMesh = ObjLoader::Load( "Assets/test.obj" );
	ObjLoader::Load( "Assets/test0.obj", m_Mesh );
	ObjLoader::Load( "Assets/test1.obj", m_Mesh2 );
	ObjLoader::Load( "Assets/cube.obj", m_CubeMesh );

	m_Mesh.bindAttributes( m_Shader, "a_vertex", "a_normal", "", "" );
	m_Mesh2.bindAttributes( m_Shader, "a_vertex", "a_normal", "", "" );
	m_CubeMesh.bindAttributes( m_Shader, "a_vertex", "a_normal", "", "" );

	{
		int gridSize = 50;
		std::vector<glm::vec3> gridVerts;

		for (int x = -gridSize; x <= gridSize; x++) {
			gridVerts.push_back( glm::vec3( x, 0, -gridSize ) );
			gridVerts.push_back( glm::vec3( x, 0, gridSize ) );
			gridVerts.push_back( glm::vec3( -gridSize, 0, x ) );
			gridVerts.push_back( glm::vec3( gridSize, 0, x ) );
		}

		m_GridMesh.drawMode = GL_LINES;
		m_GridMesh.setVertices( glm::value_ptr( gridVerts[0] ), sizeof( glm::vec3 ) * gridVerts.size( ) );
		m_GridMesh.bindAttributes( m_Shader, "a_vertex", "", "", "" );
	}

	{
		m_FBuffer.AddColorTexture( );
		m_FBuffer.AddDepthTexture( );
	}

	m_Light.m_Position = vec3( 1, 1, 1 ) * 5.f;
	m_Light.m_Direction = glm::normalize( -m_Light.m_Position );*/
}

void CProgram::Update( float delta ) {
	static float t = 0.f;
	t += delta;

	m_Scene.m_Camera.position = vec3( sin( t * 0.05f ), 0.8f, cos( t * 0.05f ) ) * 10.f;
	m_Scene.m_Camera.direction = -m_Scene.m_Camera.position;

	m_Scene.m_Objects[2]->GetTransform( ).position = vec3( 0.f, 5.f + sin( t*0.6f ), 0.f );
	m_Scene.m_Objects[2]->GetTransform( ).rotation = vec3(
		t * 50.f,
		t * -23.f,
		t * 92.f
		);

	m_Scene.m_Light.m_Position = vec3( sin( t * 0.1f ), 0.5f, cos( t*0.1f ) ) * 10.f;
	m_Scene.m_Light.m_Direction = normalize( -m_Scene.m_Light.m_Position );
}

void CProgram::Render( float delta ) {
	m_ShadowBuffer.Render( m_Scene );
	m_GBuffer.Render( m_Scene );

	m_OcclusionPass.Pass( m_GBuffer, m_Scene );

	m_LightPass.m_SSAOAlpha = sf::Keyboard::isKeyPressed( sf::Keyboard::S ) ? 0.f : 1.f;
	m_LightPass.m_BloomAlpha = sf::Keyboard::isKeyPressed( sf::Keyboard::A ) ? 0.f : 1.f;
	m_LightPass.Pass( m_GBuffer, m_ShadowBuffer, m_OcclusionPass, m_Scene );

	m_LightPass.GetOutput( ).bind( );
	glt::FrameBufferUtils::Quad( );

	if (sf::Keyboard::isKeyPressed( sf::Keyboard::Num1 )) {
		m_GBuffer.GetWorld( ).bind( );
		glt::FrameBufferUtils::Quad( glm::vec2( -1.f, 0.f ), glm::vec2( 0.f, 1.f ) );

		m_GBuffer.GetNormal( ).bind( );
		glt::FrameBufferUtils::Quad( glm::vec2( 0.f, 0.f ), glm::vec2( 1.f, 1.f ) );

		m_GBuffer.GetColor( ).bind( );
		glt::FrameBufferUtils::Quad( glm::vec2( -1.f, -0.5f ), glm::vec2( -0.5f, 0.f ) );
		m_GBuffer.GetEmissive( ).bind( );
		glt::FrameBufferUtils::Quad( glm::vec2( -0.5f, -0.5f ), glm::vec2( 0.f, 0.f ) );

		m_GBuffer.GetDepth( ).bind( );
		glt::FrameBufferUtils::Quad( glm::vec2( 0.f, -1.f ), glm::vec2( 1.f, 0.f ) );
	}
	if (sf::Keyboard::isKeyPressed( sf::Keyboard::Num2 )) {
		m_ShadowBuffer.GetDepthBuffer( ).bind( );
		glt::FrameBufferUtils::Quad( glm::vec2( -1.f, 0.f ), glm::vec2( 0.f, 1.f ) );

		m_ShadowBuffer.GetShadowBuffer( ).bind( );
		glt::FrameBufferUtils::Quad( glm::vec2( 0.f, 0.f ), glm::vec2( 1.f, 1.f ) );
	}

	if (sf::Keyboard::isKeyPressed( sf::Keyboard::Num3 )) {
		m_OcclusionPass.GetBuffer( ).bind( );
		glt::FrameBufferUtils::Quad( glm::vec2( -1.f, -1.f ), glm::vec2( 0.f, 0.f ) );

		m_OcclusionPass.GetNoise( ).bind( );
		glt::FrameBufferUtils::Quad( glm::vec2( 0.f, -1.f ), glm::vec2( 1.f, 0.f ) );
	}

	if (sf::Keyboard::isKeyPressed( sf::Keyboard::Num4 )) {
		m_LightPass.GetBloom( ).bind( );
		glt::FrameBufferUtils::Quad( glm::vec2( -1.f, 0.f ), glm::vec2( 0.f, 1.f ) );
	}

	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//m_Shader.set( "u_camera", m_Scene.m_Camera.getMatrix( ) );

	//for (CSceneObject* obj : m_Scene.m_Objects) {
	//	m_Shader.set( "u_model", obj->GetTransform( ).getMatrix( ) );
	//	m_Shader.set( "u_color", vec4( 1.f ) );
	//	//obj->GetMesh( ).bindAttributes( m_Shader, "a_vertex", "", "", "" );

	//	obj->GetMesh( ).draw( );
	//}
}

void CProgram::KeyEvent( sf::Keyboard::Key key, int state ) {
	if (state == 0)
		return;

	if (key == sf::Keyboard::Escape)
		GetFramework( )->Exit( );

	if (key == sf::Keyboard::Up)
		m_ShadowBuffer.m_Bias += 0.0001f;
	if (key == sf::Keyboard::Down)
		m_ShadowBuffer.m_Bias -= 0.0001f;
}
