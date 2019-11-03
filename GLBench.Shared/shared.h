#pragma once

#include <stdio.h>
#ifdef __ANDROID__
#include <GLES/gl.h>
#elif __APPLE__
#include <OpenGLES/ES1/gl.h>
#elif defined(_WIN32)
#include "glad/glad.h"
#endif

#include <iomanip>
#include <iostream>

struct GL {
	static void Check() {
		auto e = glGetError();
		if ( e != GL_NO_ERROR )
			std::cout << "GL error: " << std::hex << e << "\n";
	}
};

#include "Framebuffer.h"

struct Bench {
	static double time;
	Bench();
	void Frame();
private:
	Framebuffer frameBuffer;
};
