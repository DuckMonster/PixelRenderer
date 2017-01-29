#include "stdafx.h"
#include "dirlight.h"

#include "colorutils.h"

CDirLight::CDirLight( ) :
	m_Position( 0.f ), m_Direction( 1.f, 0.f, 0.f ) {
}

void CDirLight::UploadTo( glt::Shader & program ) {
	program.set( "u_light.ambient_color", vec3( StringToColor( "204c56" ) ) );
	program.set( "u_light.diffuse_direction", m_Direction );
}

glm::mat4 CDirLight::GetMatrix( ) {
	glm::mat4 matrix;
	matrix = glm::ortho( -25.f, 25.f, -25.f, 25.f, -25.f, 40.f ) * glm::lookAt( m_Position, m_Position + m_Direction, glm::vec3( 0.f, 1.f, 0.f ) );

	return matrix;
}