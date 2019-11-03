//#define GLFW_DLL
//#define GLFW_INCLUDE_NONE
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

GLFWwindow* window;

void show( HWND hwnd )
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
}

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, true );
}

int main( int argc, char* argv[] ) {

	/* Initialize the library */
	if ( !glfwInit() )
		return -1;

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
	glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow( 640, 480, "Hello World", NULL, NULL );
	if ( !window )
	{
		cout << "Failed to create a GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwSetKeyCallback( window, key_callback );

	HWND hwnd = GetConsoleWindow();
//	show( hwnd );

	/* Make the window's context current */
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 );

	for ( auto name : { GL_VENDOR, GL_RENDERER, GL_VERSION } ) {
		const GLubyte* s = glGetString( name);
		cout << s << '\n';
	}

	/* Loop until the user closes the window */
	while ( !glfwWindowShouldClose( window ) )
	{
		/* Render here */
		glClearColor( 0, .5, 0, 0 );
		glClear( GL_COLOR_BUFFER_BIT );

		/* Swap front and back buffers */
		glfwSwapBuffers( window );

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate(); 
	return 0;
}
