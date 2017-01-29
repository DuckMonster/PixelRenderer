#pragma once

class CBlur {
public:
	static glt::Shader& GetShader( );

public:
	CBlur( glm::ivec2 size );

	void Blur( glt::Texture& input );
	void ApplyBlur( glt::Texture& input );

public:
	void Blur( glt::Texture& input, glt::Texture& output, glm::vec2 direction );

	glt::FrameBuffer	m_Framebuffer;
	glt::Texture		m_Outputs[2];
};