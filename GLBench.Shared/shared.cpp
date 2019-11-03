#include <cstdlib>
#include <cstdarg>
#include "shared.h"

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

GLuint LoadShader( GLenum type, const char* shaderSrc )
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
			esLogMessage( "Error compiling shader:\n%s\n", infoLog );

			free( infoLog );
		}

		glDeleteShader( shader );
		return 0;
	}

	return shader;

}

Bench::Bench() {
	//UserData* userData = esContext->userData;
	char vShaderStr[] =
		"#version 300 es                          \n"
		"layout(location = 0) in vec4 vPosition;  \n"
		"void main()                              \n"
		"{                                        \n"
		"   gl_Position = vPosition;              \n"
		"}                                        \n";

	char fShaderStr[] =
		"#version 300 es                              \n"
		"precision mediump float;                     \n"
		"out vec4 fragColor;                          \n"
		"void main()                                  \n"
		"{                                            \n"
		"   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
		"}                                            \n";

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = LoadShader( GL_VERTEX_SHADER, vShaderStr );
	fragmentShader = LoadShader( GL_FRAGMENT_SHADER, fShaderStr );

	// Create the program object
	programObject = glCreateProgram();

	if ( programObject == 0 )
	{
		return;
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
		return;
	}

	// Store the program object
	userData.programObject = programObject;

	GL::Check();
}

void Bench::Frame() {
	GLfloat vVertices[] = { 0.0f,  0.5f, 0.0f,
							 -0.5f, -0.5f, 0.0f,
							 0.5f, -0.5f, 0.0f
	};

	// Clear the color buffer
	GL::Check();
	frameBuffer.Bind();
	GL::Check();
	glClear( GL_COLOR_BUFFER_BIT );

	// Use the program object
	glUseProgram( userData.programObject );

	// Load the vertex data
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
	glEnableVertexAttribArray( 0 );

	glDrawArrays( GL_TRIANGLES, 0, 3 );

	GL::Check();
	frameBuffer.Blit();
	GL::Check();
}
