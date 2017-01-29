#pragma once
#include "fbuffer.h"

class CDirLight {
public:
	CDirLight( );

	void UploadTo( glt::Shader& program );
	glm::mat4 GetMatrix( );

	glm::vec3	m_Position;
	glm::vec3	m_Direction;

private:
};