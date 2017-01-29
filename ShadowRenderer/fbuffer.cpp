#include "stdafx.h"
#include "fbuffer.h"

CFBuffer::CFBuffer( glm::ivec2 size ) :
	m_FrameBuffer( size ),
	m_ColorBuffers( ), m_DepthBuffer( NULL ) {
}

CFBuffer::~CFBuffer( ) {
	for (glt::Texture* tex : m_ColorBuffers)
		delete tex;

	if (m_DepthBuffer)
		delete m_DepthBuffer;
}

void CFBuffer::AddColorTexture( ) {
	glt::Texture* tex = new glt::Texture( );
	tex->init( GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, m_FrameBuffer.getSize( ) );
	m_FrameBuffer.bindTexture( *tex, GL_COLOR_ATTACHMENT0 );

	m_ColorBuffers.push_back( tex );
}

void CFBuffer::AddDepthTexture( ) {
	m_DepthBuffer = new glt::Texture( );
	m_DepthBuffer->init( GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, m_FrameBuffer.getSize( ) );
	m_FrameBuffer.bindTexture( *m_DepthBuffer, GL_DEPTH_ATTACHMENT );
}
