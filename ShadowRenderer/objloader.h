#pragma once

struct MeshData {
public:
	const std::vector<vec3>	vertices;
	const std::vector<vec2>	uvs;
	const std::vector<vec3>	normals;
};

class ObjLoader {
public:


public:
	static MeshData Load( std::string file );
	static void Load( std::string file, glt::Mesh& mesh );
};