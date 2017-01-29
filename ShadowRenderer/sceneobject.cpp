#include "stdafx.h"
#include "sceneobject.h"

#include "objloader.h"

CSceneObject::CSceneObject( const std::string filename, const glt::Transform transform, const TMaterial material ) :
	m_Mesh( ), m_Transform( transform ), m_Material( material ) {

	ObjLoader::Load( filename, m_Mesh );
	m_Mesh.bindAttributes( 0, 1, 2, 3 );
}