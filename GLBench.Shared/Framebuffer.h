#pragma once

#include <iomanip>
#include <iostream>
#include "shared.h"

struct Framebuffer {
	GLsizei maxSize;

	Framebuffer() {
		glGetIntegerv( GL_MAX_RENDERBUFFER_SIZE, &maxSize );
		std::cout << "Max FBO size: " << maxSize << '\n';
		GL::Check();
		glGenFramebuffers( 1, &name );
		glBindFramebuffer( GL_FRAMEBUFFER, name );
		GLuint rBuff;
		glGenRenderbuffers( 1, &rBuff );
		GL::Check();
		glBindRenderbuffer( GL_RENDERBUFFER, rBuff );
		GL::Check();
		glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA8, maxSize, maxSize );
		GL::Check();
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rBuff );
		GL::Check();
		auto status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
		if ( status != GL_FRAMEBUFFER_COMPLETE ) {
			std::cout << "FB status: " << std::hex << status << "\n";
		}
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}
	void Bind() {
		glBindFramebuffer( GL_FRAMEBUFFER, name );
		glViewport( 0, 0, maxSize, maxSize );
	}
	void Blit() {
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glBlitFramebuffer(0, 0, maxSize-1, maxSize-1, 0, 0, 511, 511, GL_COLOR_BUFFER_BIT, GL_NEAREST );
	}
private:
	GLuint name;
};