#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sphere.h"
#include "shader.h"
#include <iostream>
#include "cylinder.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1050;
float xlight = 1.2f, ylight = 1.0f, zlight = 2.0f;

// camera
Camera camera(glm::vec3(-1.0f, 3.0f, 12.0f));
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float cameraSpeed = 0.05f; //global variable to pass the value to scroll_callback
bool projectionType = false;  //starts the view in reqular perspective

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "7-1-project", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	//Shader objectShader("shader/objectVertexShader.vs", "objectFragmentShader.fs");
	Shader lightingShader("shaderfiles/6.multiple_lights.vs", "shaderfiles/6.multiple_lights.fs");
	Shader lightCubeShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {


		//Used for plane 0 - 5  
		//x		//y		//z	 |     //normals    |    //texture
		-10.0f, -5.0f, -10.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //back left   back triangle    | 0
		 10.0f, -5.0f, -10.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, //back right    back triangle  | 1
		 10.0f, -5.0f,  10.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, //front right   back triangle  | 2
		 10.0f, -5.0f,  10.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, //front right  front triangle  | 3
		-10.0f, -5.0f,  10.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, //front left front triangle    | 4
		-10.0f, -5.0f, -10.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //back left front triangle     | 5

		//used for binder
		//used for flat surface of binder
		//x		//y	   //z	|     //normals    |    //texture
		-0.5f, 0.5f, -4.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //back left back triangle 0   | 
		0.5f, -0.5f, -4.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, //back right back triangle 0  | 
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, //front right back triangle 0 | 8
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, //front right front triangle 1| 9
		-0.5f, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, //front left front triangle 1 | 10
		-0.5f, 0.5f, -4.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //back left front triangle 1  | 11

		//front binder triangle
		//x		//y		//z	|     //normals  |   //texture
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.7f,   // Bottom left Vertex 6   | 12
		 -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f,  0.0f,  // Top Vertex 7           | 13
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.7f, 0.6f,  // Bottom Right Vertex 8   | 14

		//back binder triangle
		//x		//y		//z	|     //normals    |   //texture
		-0.5f, -0.5f, -4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.7f,   // Bottom left Vertex 6   | 15
		 -0.5f, 0.5f, -4.0f, 1.0f, 0.0f, 0.0f,  0.0f,  0.0f,  // Top Vertex 7           | 16
		 0.5f, -0.5f, -4.0f, 1.0f, 0.0f, 0.0f,  0.7f, 0.6f,  // Bottom Right Vertex 8   | 17



		//left wall - place holder
		//x		//y		//z	|     //normals    |    //texture
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, -0.5f,  // Bottom left Vertex 12   | 18
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.5f,  // Top Vertex 13            | 19
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f,  0.5f, -0.5f,  // Bottom Right Vertex 14    | 20

		//right wall - place holder
		//x		//y	  //z  |    //normals    |    //texture
		0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f,   // Bottom left Vertex 15   | 21
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.5f,  // Top Vertex 16             | 22
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  0.5f, -0.5f, // Bottom Right Vertex 17      | 23

		//cube for laptop 18 - 59
		// 
		//x		//y		//z	       //normals         //texture
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, //bottom front left back triangle
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f, //bottom rear rigt, right triangle
		 0.5f,  -0.5f, 0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, //bottom front right, right triangle
		 0.5f,  -0.5f, 0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, //bottom front right, left triangle
		-0.5f,  -0.5f, 0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, //bottom front left, left triangle
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, //bottom rear left, left triangle

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,//front lower left, right triangle		| 30
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,//front lower right, right triangle		| 31
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,//front upper right, right triangle		| 32
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,//front upper right, left triangle		| 33
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,//front upper left, left triangle		| 34
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,//front lower left, left triangle		| 35

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,//left upper right, left triangle		| 36
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,//left upper left, left triangle	    	| 37
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,//left lower left, left triangle		    | 38
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,//left lower left, right triangle		| 39
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,//left lower right, right triangle		| 40
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,//left upper right, right triangle		| 41

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,//right upper left, right triangle		| 42
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,//right upper right, right triangle		| 43
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,//right lower right, right triangle		| 44
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,//right lower right, left triangle		| 45
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,//right lower left, left triangle		| 46
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,//right upper left, left triangle		| 47

		-0.5f, -0.5f,  -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,//back lower left, right triangle		| 48
		 0.5f, -0.5f,  -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,//back lower right, right triangle		| 49
		 0.5f,  0.5f,  -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,//back upper right, right triangle		| 50
		 0.5f,  0.5f,  -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,//back upper right, left triangle		| 51
		-0.5f,  0.5f,  -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,//back upper left, left triangle		| 52
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,//back lower left, left triangle		    | 53

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,//top upper left, right triangle		    | 54
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,//top upper right, right triangle		| 55
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,//top lower right, right triangle		| 56
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,//top lower right, left triangle		    | 57
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,//top lower left, left triangle		    | 58
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f//top upper left, left triangle		    | 59

	};

	glm::vec3 cubePositions[] = {
	glm::vec3(4.5f, 6.0f, -4.0f), // light 1
	glm::vec3(4.0f, 6.0f, -2.0f), // light 2
	glm::vec3(4.5f, 6.0f,  0.0f), //light 3
	glm::vec3(-4.5f, 10.0f, 6.0f) //directional light
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(4.5f, 6.0f, -4.0f), // light 1
		glm::vec3(4.0f, 6.0f, -2.0f), // light 2
		glm::vec3(4.5f, 6.0f,  0.0f), // light 3
		glm::vec3(0.0f, 8.0f,  0.0f) //other light - not used
	};

	//////////////////////////////////////////

	unsigned int planeVBO, planeVAO;
	unsigned int lidVBO, lidVAO;
	unsigned int bodyVBO, bodyVAO;
	unsigned int sphereVBO, sphereVAO;
	unsigned int rimVBO, rimVAO;
	unsigned int pyramidVBO, pyramidVAO;
	unsigned int cubeVBO, cubeVAO;
	unsigned int lightCubeVBO, lightCubeVAO;

	//////////////////////////////////////////

	//plane
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//////////////////////////////////////////

	//Pyramid
	glGenVertexArrays(1, &pyramidVAO);
	glGenBuffers(1, &pyramidVBO);
	glBindVertexArray(pyramidVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// // normal coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//////////////////////////////////////////

	//Cylinder1 -lid
	glGenVertexArrays(1, &lidVAO);
	glGenBuffers(1, &lidVBO);
	glBindVertexArray(lidVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lidVBO);

	//////////////////////////////////////////

	//Cylinder1 - body
	glGenVertexArrays(1, &bodyVAO);
	glGenBuffers(1, &bodyVBO);
	glBindVertexArray(bodyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bodyVBO);


	//////////////////////////////////////////

	//Shphere - top body
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//////////////////////////////////////////

	//Cylinder - rim
	glGenVertexArrays(1, &rimVAO);
	glGenBuffers(1, &rimVBO);
	glBindVertexArray(rimVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rimVBO);


	//////////////////////////////////////////

	//cube
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//////////////////////////////////////////

	//light
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//////////////////////////////////////////

	unsigned int texture1Diffuse = loadTexture("images/brick.jpg");
	unsigned int texture1Specular = loadTexture("images/brickBW.jpg");
	unsigned int texture2Diffuse = loadTexture("images/dark-steel.jpg");
	unsigned int texture2Specular = loadTexture("images/dark-steelBW.jpg");
	unsigned int texture3Diffuse = loadTexture("images/rubber.jpg");
	unsigned int texture3Specular = loadTexture("images/rubberBW.jpg");
	unsigned int texture4Diffuse = loadTexture("images/rim.jpg");
	unsigned int texture4Specular = loadTexture("images/rimBW.jpg");
	unsigned int texture5Diffuse = loadTexture("images/rug.jpg");
	unsigned int texture5Specular = loadTexture("images/rugBW.jpg");
	unsigned int binderDiffuse = loadTexture("images/binderO.jpg");
	unsigned int binderSpecular = loadTexture("images/binderOBW.jpg");
	unsigned int keyboardDiffuse = loadTexture("images/keyboard.jpg");
    unsigned int keyboardSpecular = loadTexture("images/keyboardBW.jpg");
	unsigned int screenDiffuse = loadTexture("images/screen.jpg");
	unsigned int screenSpecular = loadTexture("images/screenBW.jpg");
	unsigned int screenbackDiffuse = loadTexture("images/back.jpg");
	unsigned int screenbackSpecular = loadTexture("images/backBW.jpg");
	unsigned int binderSideDiffuse = loadTexture("images/binderside.jpg");
	unsigned int binderSideSpecular = loadTexture("images/bindersideBW.jpg");
	unsigned int sIronDiffuse = loadTexture("images/brass.jpeg");
	unsigned int sIronSpecular = loadTexture("images/brassBW.jpeg");
	unsigned int sBrassDiffuse = loadTexture("images/insideBass.jpg");
	unsigned int sBrassSpecular = loadTexture("images/insideBassBW.jpg");

	//////////////////////////////////////////


	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);



	glm::mat4 model;
	float angle;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 20.0f);
		//lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);

		// light properties
		//////////////////////////////////////////
		// directional light - used to look more realistic. Light sphere included to show location
		lightingShader.setVec3("dirLight.position", -4.5f, 10.0f, 6.0f);
		lightingShader.setVec3("dirLight.direction", -10.0f, -5.0f, 10.0f );
		lightingShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("dirLight.diffuse", 0.1f, 0.1f, 0.1f);
		lightingShader.setVec3("dirLight.specular", 0.3f, 0.3f, 0.3f);

		//////////////////////////////////////////
		//light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", 1.2f, 1.2f, 1.2f); //used 4500k light 
		lightingShader.setVec3("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("pointLights[0].specular", 0.7f, 0.7f, 0.7f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.005);
		lightingShader.setFloat("pointLights[0].quadratic", 0.1);

		//////////////////////////////////////////
		// light 2
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", 1.6f, 1.6f, 1.6f); //used 4500k light 
		lightingShader.setVec3("pointLights[1].diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("pointLights[1].specular", 0.7f, 0.7f, 0.7f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032);

		//////////////////////////////////////////
		// light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[2].ambient", 1.6f, 1.6f, 1.6f); //used 4500k light 
		lightingShader.setVec3("pointLights[2].diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("pointLights[2].specular", 0.7f, 0.7f, 0.7f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09f);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
		
		//////////////////////////////////////////
		//// point light 4
		//zero'd the values below to remove the light without breaking the shader
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[3].ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("pointLights[3].diffuse", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("pointLights[3].specular", 0.0f, 0.0f, 0.0f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09f);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032f);

		//////////////////////////////////////////
		// spotLight
		//removed the values below to remove the light without breaking the shader
		/*lightingShader.setVec3("spotLight.position", camera.Position);
		lightingShader.setVec3("spotLight.direction", camera.Front);*/
		//lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		//lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		//lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("spotLight.constant", 0.0f);
		lightingShader.setFloat("spotLight.linear", 0.09f);
		lightingShader.setFloat("spotLight.quadratic", 0.032f);
		//lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		//lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// pass projection matrix to shader (note that in this case it could change every frame)

		//glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		glm::mat4 projection;
		if (projectionType) {
			float scale = 100;
			projection = glm::ortho(-((float)SCR_WIDTH / scale), ((float)SCR_WIDTH / scale), -((float)SCR_HEIGHT / scale), ((float)SCR_HEIGHT / scale), -10.0f, 6.5f);

		}
		else {
			projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		}


		lightingShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("view", view);

		//////////////////////////////////////////
		//Plane - rug

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5Diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture5Specular);
		glBindVertexArray(planeVAO);
		
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 3.0f));
		lightingShader.setMat4("model", model);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);


		//////////////////////////////////////////
		//Cylinder1 - lid

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3Diffuse);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture3Specular);

		glBindVertexArray(lidVAO);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 1.2f, 0.0f));
		lightingShader.setMat4("model", model);

		static_meshes_3D::Cylinder C0(0.2, 26, 1, true, true, true);
		C0.render();


		//////////////////////////////////////////
		//Cylinder2 - body


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2Diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2Specular);
		glBindVertexArray(bodyVAO);
		
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		lightingShader.setMat4("model", model);

		static_meshes_3D::Cylinder C(0.8, 26, 4, true, true, true);
		C.render();


		//////////////////////////////////////////
		//Sphere for water bottle


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2Diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2Specular);
		/*glBindVertexArray(bodyVAO);*/

		glBindVertexArray(sphereVAO);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.0f));
		lightingShader.setMat4("model", model);

		Sphere S(0.791, 17, 14);
		S.Draw();


		//////////////////////////////////////////
		//Cylinder 3 - rim


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4Diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture4Specular);

		glBindVertexArray(rimVAO);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 1.1f, 0.0f));
		lightingShader.setMat4("model", model);

		static_meshes_3D::Cylinder C2(0.6, 26, 1, true, true, true);
		C2.render();


		//////////////////////////////////////////
		//Cylinder 3 - rim


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2Diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2Specular);
		glBindVertexArray(bodyVAO);

		glBindVertexArray(rimVAO);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 1.1012f, 0.0f));
		lightingShader.setMat4("model", model);

		static_meshes_3D::Cylinder C3(0.59, 26, 1, true, true, true);
		C3.render();


		//////////////////////////////////////////
		//cube for laptop screen
		
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenDiffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, screenSpecular);
		glBindVertexArray(cubeVAO);
		
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(4.0f, -1.5f, 0.8f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 3.0f, 0.1f)); // a smaller cube
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 24, 60);

		//////////////////////////////////////////
		//cube for back of laptop screen


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenbackDiffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, screenbackSpecular);
		glBindVertexArray(cubeVAO);

		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(4.0f, -1.5f, 0.747));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 3.0f, 0.0)); // a smaller cube
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 24, 60);


		//////////////////////////////////////////
		//cube for laptop base


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, keyboardDiffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, keyboardSpecular);
		glBindVertexArray(cubeVAO);
		
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(3.6f, -2.95f, 2.3f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 0.1f, 3.0f));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 24, 60);


		//////////////////////////////////////////
		//Binder body


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, binderDiffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, binderSpecular);
		glBindVertexArray(pyramidVAO);

		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-3.0f, -2.58f, 8.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 2.0f));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 6, 12);

		//////////////////////////////////////////
		//Binder body inside front


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, binderSideDiffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, binderSideSpecular);
		glBindVertexArray(pyramidVAO);

		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-3.0f, -2.58f, 8.05f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 2.01f));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 6, 12);

		//////////////////////////////////////////
		//Binder body inside back


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, binderSideDiffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, binderSideSpecular);
		glBindVertexArray(pyramidVAO);

		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-3.0f, -2.58f, 8.05f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 2.02));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 6, 12);


		//////////////////////////////////////////
		//Binder spine

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2Diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2Specular);
		glBindVertexArray(bodyVAO);

		static_meshes_3D::Cylinder spine(0.6, 26, 7, true, true, true);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first	
		model = glm::rotate(90.0f, glm::vec3(3.0f, 2.2f, 1.4f));
		model = glm::translate(model, glm::vec3(-5.45f, 2.2f, -6.0f)); //x is up and down, y is forward and back, z is left and right
		lightingShader.setMat4("model", model);

		spine.render();

		//////////////////////////////////////////
		//Sphere for soldier iron cleaner


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sIronDiffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, sIronSpecular);
		glBindVertexArray(bodyVAO);

		glBindVertexArray(sphereVAO);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, -2.3f, 3.0f));
		lightingShader.setMat4("model", model);

		Sphere scleaner(0.38, 17, 14);
		scleaner.Draw();


		//////////////////////////////////////////
		//Cylinder for soldier iron cleaner base


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sIronDiffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, sIronSpecular);

		glBindVertexArray(rimVAO);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, -2.85f, 3.0f));
		lightingShader.setMat4("model", model);

		static_meshes_3D::Cylinder cCleaner(0.4, 26, 0.3, true, true, true);
		cCleaner.render();

		//////////////////////////////////////////
		//Cylinder for soldier iron cleaner body


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sIronDiffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, sIronSpecular);

		glBindVertexArray(rimVAO);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, -2.6f, 3.0f));
		lightingShader.setMat4("model", model);

		static_meshes_3D::Cylinder cCleanerBody(0.39, 26, 0.6, true, true, true);
		cCleanerBody.render();


		//////////////////////////////////////////
		// Cylinder for soldier iron cleaner body hole

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sBrassDiffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, sBrassSpecular);
		glBindVertexArray(bodyVAO);

		static_meshes_3D::Cylinder cCleanerHole(0.3, 26, 0.5, true, true, true);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first	
		model = glm::rotate(90.0f, glm::vec3(3.0f, 2.2f, 1.4f));
		model = glm::translate(model, glm::vec3(-2.5f, 3.029f, 0.08f)); //x is up and down, y is forward and back, z is left and right
		lightingShader.setMat4("model", model);

		cCleanerHole.render();

		//////////////////////////////////////////
		//lights
		
		
		for (unsigned int i = 0; i < 4; i++)
		{
			lightCubeShader.use();
			lightCubeShader.setMat4("projection", projection);
			lightCubeShader.setMat4("view", view);
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
			lightCubeShader.setMat4("model", model);

			glBindVertexArray(lightCubeVAO);
			/*glDrawArrays(GL_TRIANGLES, 18, 59);*/
			Sphere light(0.791, 17, 14);
			light.Draw();
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);

	glDeleteVertexArrays(1, &pyramidVAO);
	glDeleteBuffers(1, &pyramidVBO);

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);

	// glfw: terminate, clearing all previously allocated GLFW resources.aw
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//float cameraSpeed = 2.5 * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) //Go up
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) //Go down
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		projectionType = !projectionType;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

	camera.ProcessMouseScroll(yoffset);


}

//load textures
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}