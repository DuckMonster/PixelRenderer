#include "stdafx.h"
#include "objloader.h"

using namespace glm;
using namespace std;

namespace {
	class ObjManifest {
	public:
		vector<vec3>		temp_vertices;
		vector<vec2>		temp_uvs;
		vector<vec3>		temp_normals;

		vector<vec3>		vertices;
		vector<vec2>		uvs;
		vector<vec3>		normals;
	};
}

vector<string> SplitString( const string& input, char delim ) {
	istringstream stream( input );
	vector<string> tokens;
	string token;

	while (getline( stream, token, delim )) {
		tokens.push_back( token );
	}

	return tokens;
}

void AddFace( const vector<string>& params, const vector<int> indices, ObjManifest& manifest ) {
	vector<vec3>& pos = manifest.vertices;
	vector<vec2>& uvs = manifest.uvs;
	vector<vec3>& normals = manifest.normals;

	for (const int index : indices) {
		istringstream data_stream( params[index] );
		int data[] = { 1, 1, 1 };
		char ignoreChar;

		if (data_stream.peek( ) != '/')
			data_stream >> data[0];

		data_stream.ignore( );

		if (data_stream.peek( ) != '/')
			data_stream >> data[1];

		data_stream.ignore( );

		if (data_stream.peek( ) != '/')
			data_stream >> data[2];

		pos.push_back( manifest.temp_vertices[data[0] - 1] );

		if (!manifest.temp_uvs.empty( ))
			uvs.push_back( manifest.temp_uvs[data[1] - 1] );

		if (!manifest.temp_normals.empty( ))
			normals.push_back( manifest.temp_normals[data[2] - 1] );
	}
}

void ParseLine( const string& line, ObjManifest& manifest ) {
	string data_type;
	istringstream stream( line );
	getline( stream, data_type, ' ' );

	// Vertex position
	if (data_type == "v") {
		float x, y, z;
		stream >> x >> y >> z;

		manifest.temp_vertices.push_back( vec3( x, y, z ) );
	}

	// Texture Coordinate
	else if (data_type == "vt") {
		float u, v;
		stream >> u >> v;

		manifest.temp_uvs.push_back( vec2( u, v ) );
	}

	// Normals
	else if (data_type == "vn") {
		float x, y, z;
		stream >> x >> y >> z;

		manifest.temp_normals.push_back( vec3( x, y, z ) );
	}

	// Face
	else if (data_type == "f") {
		vector<string> data_split = SplitString( line, ' ' );

		if (data_split.size( ) == 4)
			AddFace( data_split, { 1, 2, 3 }, manifest );
		else {
			AddFace( data_split, { 1, 2, 3 }, manifest );
			AddFace( data_split, { 1, 3, 4 }, manifest );
		}
	}
}

MeshData ObjLoader::Load( string file ) {
	ObjManifest manifest;
	ifstream fileStream( file.c_str( ) );

	while (!fileStream.eof( )) {
		string line;
		getline( fileStream, line );

		ParseLine( line, manifest );
	}

	return{ manifest.vertices, manifest.uvs, manifest.normals };
}

void ObjLoader::Load( std::string file, glt::Mesh & mesh ) {
	MeshData data = Load( file );

	mesh.setVertices( value_ptr( data.vertices[0] ), data.vertices.size( ) * sizeof( vec3 ) );

	if (!data.normals.empty( ))
		mesh.setNormals( value_ptr( data.normals[0] ), data.normals.size( ) * sizeof( vec3 ) );

	if (!data.uvs.empty( ))
		mesh.setUVS( value_ptr( data.uvs[0] ), data.uvs.size( ) * sizeof( vec2 ) );
}