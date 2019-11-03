#include <cstdlib>
#include <cstdarg>
#include <iostream>
#include "shared.h"

#define NameOf(x) #x

void esLogMessage( const char* formatStr, ... )
{
	va_list params;
	char buf[BUFSIZ];

	va_start( params, formatStr );
	vsprintf_s( buf, formatStr, params );

#ifdef ANDROID
	__android_log_print( ANDROID_LOG_INFO, "esUtil", "%s", buf );
#else
	printf( "%s", buf );
#endif

	va_end( params );
}

struct {
	GLuint programObject;
} userData;

GLuint LoadShader( GLenum type, const char* shaderSrc, const char* typeName )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader( type );

	if ( shader == 0 )
	{
		return 0;
	}

	// Load the shader source
	glShaderSource( shader, 1, &shaderSrc, NULL );

	// Compile the shader
	glCompileShader( shader );

	// Check the compile status
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled )
	{
		GLint infoLen = 0;

		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = (char*)malloc( sizeof( char ) * infoLen );

			glGetShaderInfoLog( shader, infoLen, NULL, infoLog );
			esLogMessage( "Error compiling %s:\n%s\n", typeName, infoLog );

			free( infoLog );
		}

		glDeleteShader( shader );
		return 0;
	}

	return shader;

}

bool Bench::Init() {
	//UserData* userData = esContext->userData;
	char vShaderStr[] = "#version 320 es\n			\
layout(location = 0) in vec4 vPosition;				\
layout(location = 0) uniform float colorShift;		\
out vec3 color;										\
void main()											\
{													\
	vec3 x = vec3(0,1,3) + colorShift + float(gl_VertexID);							\
	color = sin(x) * .5 + .5;						\
	gl_Position = vPosition;						\
}													\
";

	char fShaderStr[] = "#version 320 es\n			\
precision mediump float;							\
in vec3 color;										\
out vec4 fragColor;									\
void main()											\
{													\
   fragColor.rgb = color;							\
}													\
";

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = LoadShader( GL_VERTEX_SHADER, vShaderStr, NameOf( GL_VERTEX_SHADER ) );
	fragmentShader = LoadShader( GL_FRAGMENT_SHADER, fShaderStr, NameOf( GL_FRAGMENT_SHADER ) );

	// Create the program object
	programObject = glCreateProgram();

	if ( programObject == 0 )
	{
		return false;
	}

	glAttachShader( programObject, vertexShader );
	glAttachShader( programObject, fragmentShader );

	// Link the program
	glLinkProgram( programObject );

	// Check the link status
	glGetProgramiv( programObject, GL_LINK_STATUS, &linked );

	if ( !linked )
	{
		GLint infoLen = 0;

		glGetProgramiv( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = (char*)malloc( sizeof( char ) * infoLen );

			glGetProgramInfoLog( programObject, infoLen, NULL, infoLog );
			esLogMessage( "Error linking program:\n%s\n", infoLog );

			free( infoLog );
		}

		glDeleteProgram( programObject );
		return false;
	}

	// Store the program object
	userData.programObject = programObject;
	frameBuffer.Init();

	GL::Check();
	return true;
}

void Bench::Frame() {
	if ( !userData.programObject || !frameBuffer.ok )
		return;

	GLfloat vVertices[] = {
		-1, -1, 
		+1, -1,
		-1, +1,
		+1, +1,
	};

	// Clear the color buffer
	GL::Check();
	frameBuffer.Bind();
	GL::Check();

	// Use the program object
	glUseProgram( userData.programObject );

	// Load the vertex data
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, vVertices );
	glEnableVertexAttribArray( 0 );

	for ( int i = 0; i < passes; i++ ) {
		glUniform1f( 0, (float)Bench::time + i );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	}

	GL::Check();
	frameBuffer.Blit();
	GL::Check();
}