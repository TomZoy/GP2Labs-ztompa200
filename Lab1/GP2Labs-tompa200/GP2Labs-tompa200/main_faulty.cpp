﻿//headerfiles
#include <iostream> //include for input/output handling
#include <gl/glew.h>
#include <SDL.h> //header for SDL functionality
#include <SDL_opengl.h>
#include <gl\GLU.h>
#include <time.h>
#include "Vertex.h" //"" for includes from the same directory
#include "Shader.h"
#include <SDL_Image.h>
#include "Texture.h"

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;
using glm::vec4;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint texture = 0;


#ifdef _DEBUG && WIN32
const std::string ASSET_PATH = "assets";
#else
const std::string ASSET_PATH = "assets";
#endif
const std::string SHADER_PATH = "/shaders";



//global variables here
SDL_Window * window;
const int WINDOW_WIDTH = 640; //constant to control window creation
const int WINDOW_HEIGHT = 480;
bool running = true;
SDL_GLContext glcontext = NULL; ////SDL GL Context
double fallSpeed = 5.00;
double rotateSpeed = 1000.00;
bool rotate = false;

GLuint triangleVBO;
GLuint triangleEBO;
GLuint VAO;

/*
ColorStruct Color[] = 
{
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f }
};
*/

/* WHERE THE TRIANGLES ARE DEFINED*/

//3 value RGB colour + 3 value XYZ coordinates
float TR1[3][6] = {
	1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f
};

float TR2[3][6] = {
	1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f,
	0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.0f,
	0.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.0f
};


GLuint shaderProgram = 0;


//matrices
mat4 viewMatrix;
mat4 projMatrix;
mat4 worldMatrix;


//VERSION 1 - Triangle
/*

		float triangleData[]=
		{
			0.0f, 1.0f, 0.0f, //Top
			-1.0f, -1.0f, 0.0f, //bottom left
			1.0f, -1.0, 0.0f, //bottom right
			//2.0f,0.0f,0.0f
		};
*/


//VERSION 2 - Triangle
/* 

Vertex triangleData[]
{
	{
		0.0f,1.0f,0.0f,			 //x, y, z
		1.0f,0.0f,0.0f,1.0f		//r, g, b, a
	},

	{
		-1.0f, -1.0f, 0.0f,			 //x, y, z
		0.0f, 1.0f, 0.0f, 1.0f		//r, g, b, a
	},

	{
		1.0f, -1.0f, 0.0f,			 //x, y, z
		0.0f, 0.0f, 1.0f, 1.0f		//r, g, b, a
	}

};
*/


//Version 3 - CUBE 

/*

Vertex triangleData[] =
{
	//Front face

	{ // 1. top left
		-0.5f, 0.5f, 0.5f,
		1.0f, 0.0f, 1.0f, 1.0f
		
	},
	
	{ // 2. bottom left
		-0.5f, -0.5f, 0.5f,
		1.0f, 1.0f, 0.0f, 1.0f
		
	},

	{ // 3. bottom right
		0.5f, -0.5f, 0.5f,
		0.0f, 1.0f, 1.0f, 1.0f
		
	},

	{ // 4. top right
		0.5f, 0.5f, 0.5f,
		1.0f, 0.0f, 1.0f, 1.0f
		
	},

	{ // 5. (1.) top left
		//-0.5f, 0.5f, 0.5f,
		//1.0f, 0.0f, 1.0f, 1.0f

		
		triangleData[0].x, triangleData[0].y, triangleData[0].z,
		triangleData[0].r, triangleData[0].g, triangleData[0].b, triangleData[0].a


	},

	{ // 6. (3.) bottom right
		//0.5f, -0.5f, 0.5f,
		//0.0f, 1.0f, 1.0f, 1.0f
		
		triangleData[2].x, triangleData[2].y, triangleData[2].z,
		triangleData[2].r, triangleData[2].g, triangleData[2].b, triangleData[2].a


	},


	//BACK face
	
	{ // 7. top left
		-0.5f, 0.5f, -0.5f,
		1.0f, 0.0f, 0.0f, 1.0f
	},

	{ // 8. bottom left
		-0.5f, -0.5f, -0.5f,
		1.0f, 1.0f, 1.0f, 1.0f
	},

	{ // 9. bottom right
		0.5f, -0.5f, -0.5f,
		0.0f, 1.0f, 1.0f, 1.0f
	},

	{ // 10. top right
		0.5f, 0.5f, -0.5f,
		0.0f, 0.0f, 1.0f, 1.0f
	},

	{ // 11. (1.) top left
		//-0.5f, 0.5f, 0.5f,
		//1.0f, 0.0f, 1.0f, 1.0f

		triangleData[6].x, triangleData[6].y, triangleData[6].z,
		triangleData[6].r, triangleData[6].g, triangleData[6].b, triangleData[6].a

	},

	{ // 12. (3.) bottom right
		//0.5f, -0.5f, 0.5f,
		//0.0f, 1.0f, 1.0f, 1.0f

		triangleData[8].x, triangleData[8].y, triangleData[8].z,
		triangleData[8].r, triangleData[8].g, triangleData[8].b, triangleData[8].a
	},

	{
		//&triangleData[0]
	
	}


	

};


void overwrite()

{
	triangleData[4] = triangleData[0];
	triangleData[5] = triangleData[2];
}

*/

// Version 4 - CUBE

Vertex triangleData[] = {


	/*
	//Front
		{ -0.5f, 0.5f, 0.5f,
		1.0f, 0.0f, 1.0f, 1.0f },// Top Left
		{ -0.5f, -0.5f, 0.5f,
		1.0f, 1.0f, 0.0f, 1.0f },// Bottom Left
		{ 0.5f, -0.5f, 0.5f,
		0.0f, 1.0f, 1.0f, 1.0f }, //Bottom Right
		{ 0.5f, 0.5f, 0.5f,
		1.0f, 0.0f, 1.0f, 1.0f },// Top Right

		//back
		{ -0.5f, 0.5f, -0.5f,
		1.0f, 0.0f, 1.0f, 1.0f },// Top Left
		{ -0.5f, -0.5f, -0.5f,
		1.0f, 1.0f, 0.0f, 1.0f },// Bottom Left
		{ 0.5f, -0.5f, -0.5f,
		0.0f, 1.0f, 1.0f, 1.0f }, //Bottom Right
		{ 0.5f, 0.5f, -0.5f,
		1.0f, 0.0f, 1.0f, 1.0f },// Top Right

*/

	//front
		
			{ vec3(-0.5f, 0.5f, 0.5f),vec2(0.0f,0.0f),glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },  //top left
			{ vec3(-0.5f, -0.5f, 0.5f), vec2(0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, //bottom left
			{ vec3(0.5f, -0.5f, 0.5f), vec2(1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }, //bottom right
			{ vec3(0.5f, 0.5f, 0.5f), vec2(1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) } //top right
		,
		//back
		
		{ vec3(-0.5f, 0.5f, -0.5f), vec2(0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ vec3(-0.5f, -0.5f, -0.5f), vec2(0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ vec3(0.5f, -0.5f, -0.5f), vec2(1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ vec3(0.5f, 0.5f, -0.5f), vec2(1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) }
		



};

/*note: these indices represent an index of a vertex in the VBO*/
GLuint indices[] = {
	//front
	0, 1, 2,
	0, 3, 2,
	//left
	4, 5, 1,
	4, 1, 0,
	//right
	3, 7, 2,
	7, 6, 2,
	//bottom
	1, 5, 2,
	6, 2, 1,
	//top
	5, 0, 7,
	5, 7, 3,
	//back
	4, 5, 6,
	4, 7, 6
};






void InitWindow(int width, int height, bool fullscreen)

{
	window = SDL_CreateWindow(

		"Lab 2", //window title
		SDL_WINDOWPOS_CENTERED, //w cetered
		SDL_WINDOWPOS_CENTERED, //h centered
		width, // width, in pixels
		height, // height, in pixels
		SDL_WINDOW_OPENGL //flags

		);

}

//function to clean up resources after the code closes
void CleanUp()
{
	glDeleteTextures(1, &texture);
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &triangleVBO);  // This will delete the number of buffers specified(1st parameter), with the actual buffers being passed in as the 2nd parametr
	glDeleteBuffers(1, &triangleEBO);
	glDeleteVertexArrays(1, &VAO);

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//function to initialise OpenGL
void initOpenGL()
{

	//Ask for version 3.2 of OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//create openGL context
	glcontext = SDL_GL_CreateContext(window);

	//if it's still empty, something went wrong
	if (!glcontext)
	{
		std::cout << "ERROR CREATING OPENGL CONTEXT" << SDL_GetError() <<
			std::endl;
	}

	//setting up some initial OpenGL states
	//smooth shading
	glShadeModel(GL_SMOOTH);
	//clear the background to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//clear the depht buffer to 1.0
	glClearDepth(1.0f);
	//enable depht testing
	glEnable(GL_DEPTH_TEST);
	//the type of depht test to use
	glDepthFunc(GL_LEQUAL);
	//set perspective correction to best
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//enable experimental for programmable pipeline
	glewExperimental = GL_TRUE;

	//init GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/*Problem: glewInt failed to, something is wrong*/
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}



}

//function to set/reset viewport
void setViewport(int width, int height)
{
	//sreen ratio
	GLfloat ratio;

	//make sure height is always above 0
	if (height <= 0)
	{
		height = 1;
	}

	//calculate screen ratio
	ratio = (GLfloat)width / (GLfloat)height;

	//set-up viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);


	

}

// OLD function to draw the triangle
/*

void DrawTriangle(float Tri[3][6])
{
	//switch to ModelView
	glMatrixMode(GL_MODELVIEW);
	//reset using the identity matrix
	glLoadIdentity();
	//translate to -5f on Z axis
	glTranslatef(0.0f, 0.0f, -5.0f);

	//begin drawing the triangles
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < 3; i++)
	{
		glColor3f(Tri[i][0], Tri[i][1], Tri[i][2]); //colour of the vertices
		glVertex3f(Tri[i][3], Tri[i][4], Tri[i][5]); //top/left/rigt
	}
	glEnd();

	

	glMatrixMode(GL_MODELVIEW);
	This switches the matrix mode state to the Model View mode, this mode is used to position our vertices in 3D space. We will often carry out a rotation, translation or scaling after this call.

	glLoadIdentity();
	This will push the identity matrix onto the current matrix(in this case ModelView , see above).  The identity matrix is like setting a value to 1(a gross simplification but will do for now).

	glTranslatef(0.0f,0.0f,-5.0f);
	Will push a translation matrix  onto the current matrix, in this case it will translate the current matrix -5.0f units.

	glBegin(GL_TRIANGLES);
	This begins the drawing process, we pass in the primitive type we are going to draw.

	glColor(1.0f,0.0f,0.0f);
	This sets the colour of the vertices where each component is a floating point number(0.0f � 1.0f) and represents red, green and blue.

	glVertex3f(1.0f,0.0f,0.0f);
	This specifies a Vertex using an x. y and z positional values.

	glEnd();
	This ends the drawing process

	

}
*/







//function to draw
void render()
{
	//set the clear colour
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//clear the colour and depth-buffer	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);

	//Make the new VBO active. Repeat here as sanity check (may have changed since inisialisation)
	glBindBuffer(GL_ARRAY_BUFFER,triangleVBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);


	glUseProgram(shaderProgram);

	GLint texture0Location = glGetUniformLocation(shaderProgram, "texture0");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture);
	glUniform1i(texture0Location,0);

	GLint  MVPLocation	=	glGetUniformLocation(shaderProgram,	"MVP");
	mat4 MVP = projMatrix*viewMatrix*worldMatrix;
	glUniformMatrix4fv(MVPLocation,	1,	GL_FALSE, glm::value_ptr(MVP));


	//Tell the shader	that 0	is	the	position element 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);



	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(window);

}

//custom animation
/*
void MoveTriangle() //the "animation"
{
	TR2[0][4] -= 0.1f;
	TR2[1][4] -= 0.1f;
	TR2[2][4] -= 0.1f;

	// initialize random seed: 
	srand(time(NULL));

	if (TR2[0][4] < -2.0f)
	{
		//reset to the top
		TR2[0][4] = 4.0f;
		TR2[1][4] = 4.0f;
		TR2[2][4] = 3.0f;

		//randomise the colour

		for (int h = 0; h < 3; h++)
		{
			for (int i = 0; i < 3; i++)
			{
				TR2[i][h] = ((rand() % 100 + 1) / 100.00);
				// std::cout << "colour randomised = " << TR2[i][h] << ", "; //DEBUG INFO
			}
		}
	}
}


*/



//function to update game state
void update()
{
	projMatrix 	=	glm::perspective(45.0f,	(float)WINDOW_WIDTH	/(float)WINDOW_HEIGHT,0.1f,	100.0f);
	viewMatrix	=	glm::lookAt(vec3(0.0f,	0.0f,	10.0f), vec3(0.0f,	0.0f,	0.0f),	vec3(0.0f,	1.0f,	0.0f));
	worldMatrix =   glm::translate(mat4(1.0f),vec3(0.0f, 0.0f, 0.0f));
}

void initGeometry()
{

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//create buffer
	glGenBuffers(1, &triangleVBO);
	//Make the new VBO active
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	//copy vertex data to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);

		/*
	
	glGenBuffers call	takes	in	an	integer	which	specifies the	number	of	buffers	you	
	are	going	to	generate	and	the	2nd parameter	is	a	pointer	to	one	or	an	array	of	
	integers.

	glBindBuffer call	binds	the	specified	buffer(2nd parameter)	to	the	pipeline(this	
	is	a	state!),	the	1st parameter	specifies what	type	of	buffer	we	are	binding(Array	
	Buffer)	will	hold	vertices.
	
	glBufferData copies	data	to	the	bound	buffer,	1st	parameter	is	what	type	of	
	buffer	we	are	copying	too,	2nd parameter	is	the	size	of	the	data	we	are	copying	
	into	the	buffer,	3rd parameter	is	the	actual	data	we	are	copying,	and the	last	
	parameter	is	a	hint	to	OpenGL	on	what	do	with	the	buffer	data,	in	this	case	the	
	data	in	the	buffer	will	not	be	updated.
		
		*/


	//create buffer
	glGenBuffers(1, &triangleEBO);
	//Make the EBO active
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
	//Copy Index data to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
}


//SHADERS !!!!
void createShader()
{

	shaderProgram = glCreateProgram();


	GLuint vertexShaderProgram = 0;
	std::string	vsPath = ASSET_PATH + SHADER_PATH + "/textureVS.glsl";
	vertexShaderProgram = loadShaderFromFile(vsPath, VERTEX_SHADER);

	GLuint fragmentShaderProgram = 0;
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/textureFS.glsl";
	fragmentShaderProgram = loadShaderFromFile(fsPath, FRAGMENT_SHADER);

	glBindAttribLocation(shaderProgram,	0, "vertexPosition");
	glBindAttribLocation(shaderProgram, 1, "vertexTexCoords");
	glBindAttribLocation(shaderProgram, 2, "vertexColour");


	glAttachShader(shaderProgram, vertexShaderProgram);
	glAttachShader(shaderProgram, fragmentShaderProgram);
	glLinkProgram(shaderProgram);
	bool SErr = checkForLinkErrors(shaderProgram);
	
	//now we can delete	the	VS	&	FS	Programs
	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);


}

void createTexture()
{
	//std::string texturePath = ASSET_PATH + TEXTURE_PATH + "/texture.png";
	std::string texturePath = ASSET_PATH + "/texture.png";
	texture = loadTextureFromFile(texturePath);


}


//Main Methood entry point
int main(int argc, char * arg[]){

	//init everything - SDLm if it is nonzero we have a problem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "ERROR SDL_Init" << SDL_GetError() << std::endl;
		return -1;
	}

	//init SDLImage for jpg and png support
	int  imageInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int  returnInitFlags = IMG_Init(imageInitFlags);
	if (((returnInitFlags)&  (imageInitFlags)) != imageInitFlags)  
	{
		std::cout << "ERROR  SDL_Image  Init  " << IMG_GetError() << std::endl;
		//  handle  error
	}



	//initialise the app window
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);

	//Call out InitOpenGL Function
	initOpenGL();
	initGeometry();
	//Set out Viewport
	setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);




	SDL_Event event;


	double tFall = 0.0; //timer for the game loop for falling
	double tRotate = 0.0; //timer for the game loop for rotation


	createShader();



	// --- GAME LOOP START --- //
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			//get event type
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
			{
				//set the running controller boolean to false
				running = false;
			}

			/*EXPERIMENTAL BEGGINS*/
			/*
			switch (event.type){
				// Look for a keypress 
			case SDL_KEYDOWN:
				// Check the SDLKey values and move change the coords
				switch (event.key.keysym.sym){
				case SDLK_LEFT:
					TR2[0][3] -= 0.1f;
					TR2[1][3] -= 0.1f;
					TR2[2][3] -= 0.1f;
					break;
				case SDLK_RIGHT:
					TR2[0][3] += 0.1f;
					TR2[1][3] += 0.1f;
					TR2[2][3] += 0.1f;
					break;
				case SDLK_UP:
					TR2[1][4] += 0.1f;
					break;
				case SDLK_DOWN:
					TR2[1][4] -= 0.1f;
					break;

				default:
					break;
				}
			}

*/
			/*EXPERIMENTAL ENDS*/

		} //event checking ends here

		//animation bit - DISABLED
		/*
		if (tFall > fallSpeed)
		{
			MoveTriangle();
			tFall = 0.0;
		}
		tFall++;

		if (tRotate > rotateSpeed)
		{
			rotate = 1;
			tRotate = 0.0;
		}
		tRotate++;
		*/

		update();
		render();


	}

	CleanUp();
	return 0;

}