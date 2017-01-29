#pragma once

class CFBuffer {
public:
	CFBuffer( glm::ivec2 size );
	~CFBuffer( );

	void Bind( ) { m_FrameBuffer.bind( ); }
	void Release( ) { m_FrameBuffer.release( ); }

	void AddColorTexture( );
	void AddDepthTexture( );

	glt::Texture* GetColor( ) { return GetColor( 0 ); }
	glt::Texture* GetColor( int n ) { return m_ColorBuffers[n]; }
	glt::Texture* GetDepth( ) { return m_DepthBuffer; }

private:
	glt::FrameBuffer m_FrameBuffer;

	std::vector<glt::Texture*> m_ColorBuffers;
	glt::Texture* m_DepthBuffer;
};