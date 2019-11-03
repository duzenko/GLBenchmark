//#define GLFW_DLL
//#define GLFW_INCLUDE_NONE
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <windows.h>
#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Cube.h"

using namespace std;

GLFWwindow* window;

/*void show( HWND hwnd )
{
	WINDOWPLACEMENT place = { sizeof( WINDOWPLACEMENT ) };
	GetWindowPlacement( hwnd, &place );
	switch ( place.showCmd )
	{
	case SW_SHOWMAXIMIZED:
		ShowWindow( hwnd, SW_SHOWMAXIMIZED );
		break;
	case SW_SHOWMINIMIZED:
		ShowWindow( hwnd, SW_RESTORE );
		break;
	default:
		ShowWindow( hwnd, SW_NORMAL );
		break;
	}
	SetWindowPos( 0, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );
	SetForegroundWindow( hwnd );
}*/

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, true );
}

void* LoadGLProc( const char* name ) {
	void* p = (void*)wglGetProcAddress( name );
	if ( p == 0 ||
		( p == (void*)0x1 ) || ( p == (void*)0x2 ) || ( p == (void*)0x3 ) ||
		( p == (void*)-1 ) )
	{
		HMODULE module = LoadLibraryA( "opengl32.dll" );
		p = (void*)GetProcAddress( module, name );
	}

	return p;
}

int main( int argc, char* argv[] ) {

	/* Initialize the library */
	if ( !glfwInit() )
		return -1;

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
	glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow( 512, 512, "OpenGL Window", NULL, NULL );
	if ( !window )
	{
		cout << "Failed to create a GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwSetKeyCallback( window, key_callback );

	//HWND hwnd = GetConsoleWindow();
//	show( hwnd );

	/* Make the window's context current */
	glfwMakeContextCurrent( window );
	gladLoadGLES2Loader( LoadGLProc );
	glfwSwapInterval( 1 );

	for ( auto name : { GL_VENDOR, GL_RENDERER, GL_VERSION } ) {
		const GLubyte* s = glGetString( name);
		cout << s << '\n';
	}

	BenchInit();

	/* Loop until the user closes the window */
	while ( !glfwWindowShouldClose( window ) )
	{
		/* Render here */
		glClearColor( 0, .5, 0, 0 );
		glClear( GL_COLOR_BUFFER_BIT );

		BenchFrame();

		/* Swap front and back buffers */
		glfwSwapBuffers( window );

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate(); 
	return 0;
}
