#pragma once

#include <stdio.h>
#ifdef __ANDROID__
#include <GLES/gl.h>
#elif __APPLE__
#include <OpenGLES/ES1/gl.h>
#elif defined(_WIN32)
#include "glad/glad.h"
#endif

bool BenchInit();
void BenchFrame();