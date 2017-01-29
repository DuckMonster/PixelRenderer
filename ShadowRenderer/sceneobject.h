#pragma once
#include "material.h"

class CSceneObject {
public:
	CSceneObject( const std::string filename ) : CSceneObject( filename, glt::Transform( ), TMaterial( ) ) {}
	CSceneObject( const std::string filename, const glt::Transform transform ) : CSceneObject( filename, transform, TMaterial( ) ) {}
	CSceneObject( const std::string filename, const TMaterial material ) : CSceneObject( filename, glt::Transform( ), material ) {}
	CSceneObject( const std::string filename, const glt::Transform transform, const TMaterial material );

	glt::Transform& GetTransform( ) { return m_Transform; }
	glt::Mesh& GetMesh( ) { return m_Mesh; }
	TMaterial& GetMaterial( ) { return m_Material; }

private:
	glt::Transform		m_Transform;
	glt::Mesh			m_Mesh;
	TMaterial			m_Material;
};