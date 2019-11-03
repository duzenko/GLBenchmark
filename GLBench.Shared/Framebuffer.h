#pragma once

#include <iomanip>
#include <iostream>
#include "shared.h"
#include <algorithm>

struct Framebuffer {
	GLsizei maxSize;
	bool ok;

	bool Init() {
		glGetIntegerv( GL_MAX_RENDERBUFFER_SIZE, &maxSize );
		std::cout << "Max FBO size: " << maxSize << '\n';
		maxSize = std::min<GLsizei>( 8192, maxSize );
		GL::Check();
		glGenFramebuffers( 1, &name );
		glBindFramebuffer( GL_FRAMEBUFFER, name );
		GLuint rBuff;
		glGenRenderbuffers( 1, &rBuff );
		GL::Check();
		glBindRenderbuffer( GL_RENDERBUFFER, rBuff );
		GL::Check();
		do {
			glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA32F, maxSize, maxSize );
			GL::Check();
			glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rBuff );
			GL::Check();
			auto status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
			if ( !( ok = status == GL_FRAMEBUFFER_COMPLETE ) ) {
				std::cout << "FB status: " << std::hex << status << std::dec << "\n";
			}
			if ( status == GL_FRAMEBUFFER_UNSUPPORTED && maxSize > 0 ) {
				maxSize >>= 1;
				std::cout << "Retrying with size: " << maxSize << '\n';
			} else
				break;
		} while ( 1 );
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		return true;
	}
	void Bind() {
		glBindFramebuffer( GL_FRAMEBUFFER, name );
		glViewport( 0, 0, maxSize, maxSize );
	}
	void Blit() {
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		if ( fullBlit )
			glBlitFramebuffer( 0, 0, maxSize - 1, maxSize - 1, 0, 0, 511, 511, GL_COLOR_BUFFER_BIT, GL_NEAREST );
		else
			glBlitFramebuffer( 0, 0, 511, 511, 0, 0, 511, 511, GL_COLOR_BUFFER_BIT, GL_NEAREST );
	}
private:
	GLuint name;
	bool fullBlit = false;
};