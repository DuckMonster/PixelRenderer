#pragma once
struct TMaterial {
public:
	glm::vec4 color;
	float emissive;

	TMaterial( ) : color( 1.f ), emissive( 0.f ) {}
	TMaterial( glm::vec4 color ) : color( color ), emissive( 0.f ) {}
	TMaterial( glm::vec4 color, float emissive ) : color( color ), emissive( emissive ) {}
};