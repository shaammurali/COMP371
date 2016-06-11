#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include <locale>
#include <fstream>
#include <ctime>

#include "objloader.h"

// Lance's include files
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SOIL/SOIL.h"

using namespace std;

// Window dimensions
const GLuint WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 1000;
const GLfloat CAMERA_MOVEMENT_STEP = 0.60f;
const float ANGLE_ROTATION_STEP = 0.15f;

glm::vec3 camera_position;
glm::mat4 projection_matrix;

//ADDED FOR PROJECT
glm::vec3 camera_movement = glm::vec3(0.0f, 0.0f, 15.0f);
double xpos, ypos;
bool mouse_click = false;

float y_rotation_angle = 0.0f, x_rotation_angle = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	// Update the Projection matrix after a window resize event
	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		camera_position.z += CAMERA_MOVEMENT_STEP;

	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		camera_position.z -= CAMERA_MOVEMENT_STEP;

	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		camera_position.x -= CAMERA_MOVEMENT_STEP;

	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		camera_position.x += CAMERA_MOVEMENT_STEP;

	//rotate cube
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
		x_rotation_angle += ANGLE_ROTATION_STEP;

	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
		x_rotation_angle -= ANGLE_ROTATION_STEP;

	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		y_rotation_angle += ANGLE_ROTATION_STEP;

	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		y_rotation_angle -= ANGLE_ROTATION_STEP;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			mouse_click = true;
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			mouse_click = false;
		}

}

GLuint loadCubemap(std::vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);

		SOIL_free_image_data(image); //free resources
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glEnable(GL_MULTISAMPLE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Open Space GLadiators", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback); //ADDED FOR PROJECT
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	// Build and compile our shader program
	// Vertex shader

	// Read the Vertex Shader code from the file
	string vertex_shader_path = "vertex.shader";
	string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

	if (VertexShaderStream.is_open()) {
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	string fragment_shader_path = "fragment.shader";
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
		getchar();
		exit(-1);
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader); //free up memory
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;

	loadOBJ("Falcon-Only.obj", vertices, normals, UVs);

	/* FOR PROJECT
	std::vector<GLfloat> vertices;
	std::vector<GLuint> vertexIndices;
	loadOBJ("Falcon-Only", vertices, vertexIndices);
	*/

	
	GLuint VAO, vertices_VBO, normals_VBO, UVs_VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertices_VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &UVs_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, UVs_VBO);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	
	/*FOR PROJECT
	// Array object configuration
	GLuint VAOmillenium, VBOVertexMillenium, EBOVertexMillenium;
	glGenVertexArrays(1, &VAOmillenium);
	glGenBuffers(1, &VBOVertexMillenium);
	glGenBuffers(1, &EBOVertexMillenium);

	glBindVertexArray(VAOmillenium);
	glBindBuffer(GL_ARRAY_BUFFER, VBOVertexMillenium);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOVertexMillenium);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(GLuint), &vertexIndices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/

	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
	GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view_matrix");
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model_matrix");

	GLuint drawing_skybox_id = glGetUniformLocation(shaderProgram, "drawingSkybox");

	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//
	//glActiveTexture(GL_TEXTURE0); //select texture unit 0
//
	//GLuint cube_texture;
	//glGenTextures(1, &cube_texture);
	//glBindTexture(GL_TEXTURE_2D, cube_texture); //bind this texture to the currently bound texture unit

												// Set the texture wrapping parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//
	//// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
	//// Set texture filtering parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
	//// Load image, create texture and generate mipmaps
	//int cube_texture_width, cube_texture_height;
	//unsigned char* cube_image = SOIL_load_image("brick.jpg", &cube_texture_width, &cube_texture_height, 0, SOIL_LOAD_RGB);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cube_texture_width, cube_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, cube_image);
//
	//SOIL_free_image_data(cube_image); //free resources

									  //Setup our cubemap

	vector<const GLchar*> faces;
	//srand(time(0));
	//int skybox = (int)(std::rand() % 3 + 1);
	//cout << skybox;

	int skybox = 3;
	if (skybox == 1)
	{
		faces.push_back("skybox/stars_right.jpg");
		faces.push_back("skybox/stars_left.jpg");
		faces.push_back("skybox/stars_up.jpg");
		faces.push_back("skybox/stars_down.jpg");
		faces.push_back("skybox/stars_back.jpg");
		faces.push_back("skybox/stars_front.jpg");
	}
	else if (skybox == 2)
	{
		faces.push_back("skybox/galaxy_right.jpg");
		faces.push_back("skybox/galaxy_left.jpg");
		faces.push_back("skybox/galaxy_up.jpg");
		faces.push_back("skybox/galaxy_down.jpg");
		faces.push_back("skybox/galaxy_back.jpg");
		faces.push_back("skybox/galaxy_front.jpg");
	}
	else
	{
		faces.push_back("skybox/galaxy2_right.jpg");
		faces.push_back("skybox/galaxy2_left.jpg");
		faces.push_back("skybox/galaxy2_up.jpg");
		faces.push_back("skybox/galaxy2_down.jpg");
		faces.push_back("skybox/galaxy2_back.jpg");
		faces.push_back("skybox/galaxy2_front.jpg");
	}

	glActiveTexture(GL_TEXTURE1);
	GLuint cubemapTexture = loadCubemap(faces);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	//glUniform1i(glGetUniformLocation(shaderProgram, "cubeTexture"), 0); //cubeTexture should read from texture unit 0
	glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 1); //sky box should read from texture unit 1

	//FOR PROJECT
	glfwGetCursorPos(window, &xpos, &ypos);
	double xpos_old, ypos_old;


        // Initial scaling and placement of the millenium falcon.
	glm::mat4 model_matrix = glm::mat4(1.0f);
        model_matrix = glm::scale(model_matrix, glm::vec3(0.15f));
        model_matrix = glm::translate(model_matrix, glm::vec3(0.f, 0.0f, 10.0f));
        model_matrix = glm::rotate(model_matrix, glm::radians(360.f - 50.0f), glm::vec3(0,1,0));

        //model_matrix = glm::rotate(model_matrix, glm::radians(90.0f), glm::vec3(0,0,0));


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (mouse_click)
		{
			xpos_old = xpos;
			ypos_old = ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			camera_movement += glm::vec3(0.0f, 0.0f, 1.0f) * ((GLfloat)(ypos - ypos_old)*0.025f);

			if ((ypos - ypos_old) > 0)
				x_rotation_angle += 0.01f;
			else if ((ypos - ypos_old) < 0)
				x_rotation_angle -= 0.01f;
			if ((xpos - xpos_old) > 0)
				y_rotation_angle += 0.01f;
			else if ((xpos - xpos_old) < 0)
				y_rotation_angle -= 0.01f;
		}

		//model_matrix = glm::rotate(model_matrix, y_rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f));
		//model_matrix = glm::rotate(model_matrix, x_rotation_angle, glm::vec3(1.0f, 0.0f, 0.0f));
	

		glm::mat4 view_matrix;
		view_matrix = translate(view_matrix, camera_position);
		view_matrix = glm::rotate(view_matrix, y_rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f));
		view_matrix = glm::rotate(view_matrix, x_rotation_angle, glm::vec3(1.0f, 0.0f, 0.0f));

		//view_matrix = glm::lookAt(glm::vec3(4.0f, 2.0f, 4.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		


		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		//Draw the skybox

		//glBindVertexArray(VAOmillenium); //FOR PROJECT
		glBindVertexArray(VAO);

		glm::mat4 skybox_view = glm::mat4(glm::mat3(view_matrix)); //remove the translation data
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(skybox_view));

		glDepthMask(GL_FALSE);

		glUniform1i(drawing_skybox_id, 1); //set the uniform boolean
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDepthMask(GL_TRUE);

		//Draw the textured cube
		model_matrix = glm::translate(model_matrix, camera_position);
		
		//view_matrix = glm::translate(view_matrix, camera_position);
		view_matrix = glm::lookAt(camera_position+glm::vec3(0.0f, 3.0f, -5.0f), camera_position + glm::vec3(0.0f,0.0f,10.0f), glm::vec3(0.0f,1.0f,0.0f));
		view_matrix = glm::translate(view_matrix, camera_position);
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniform1i(drawing_skybox_id, 0); //set the boolean
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
