#pragma once

class CBloom {
public:
	static glt::Shader& GetShader( );

public:
	CBloom( glm::ivec2 size );

	void Blur( glt::Texture& input );
	void ApplyBlur( glt::Texture& input );

public:
	void Blur( glt::Texture& input, glt::Texture& output, glm::vec2 direction );

	glt::FrameBuffer	m_Framebuffer;
	glt::Texture		m_Outputs[2];
};