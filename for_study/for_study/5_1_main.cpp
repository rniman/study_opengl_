#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/GLU.h>

#include "read_Obj.h"
#include <random>
#include "stb_image.h"

std::random_device rd;
std::mt19937 gen(rd());

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
void initBuffer();
GLvoid initTexture();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);

const GLint window_w = 1000, window_h = 1000;
GLfloat rColor = 0.4f, gColor = 0.4f, bColor = 0.4f;

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;
unsigned int lightPosLocation;
unsigned int lightColorLocation;
unsigned int objColorLocation;
unsigned int viewPosLocation;
unsigned int ambientLocation;

GLuint vao;
GLuint vbo[2];

glm::mat4 camera;
glm::vec3 camera_eye = {500.0f, 300.0f, 500.0f};
glm::vec3 camera_origin_eye = {500.0f, 300.0f, 500.0f};
GLfloat cameraAngle = 0.0f;
GLfloat cameraRotateAngle = 0.0f;

glm::mat4 projection;
glm::mat4 view;


glm::vec3 lightPos = {300.0f, 300.0f, 300.0f};
glm::vec3 lightDelta = { 0.0f,0.0f,0.0f };
glm::vec3 originLightPos = { 300.0f, 200.0f, 0.0f };
glm::vec3 lightColor =glm::vec3(1.0f,1.0f,1.0f);
glm::mat4 lightTrans = glm::mat4(1.0f);

GLuint vao_cuboid;
GLuint vbo_cuboid[3];
glm::mat4 cuboid_trans(1.0f);
std::vector<GLfloat> cuboid_vertex;
std::vector<GLfloat> cuboid_normal;
std::vector<GLfloat> cuboid_texture;
float yAngle = 0.0f;
float xAngle = 0.0f;

unsigned int floor_vao;
unsigned int floor_vbo[3];
glm::mat4 floor_trans(1.0f);
std::vector<GLfloat> floor_vertex
{
	//밑면
	-200.0f, 0.0f, -200.0f,
	200.0f, 0.0f, -200.0f,
	-200.0f, 0.0f, 200.0f,


	-200.0f, 0.0f, 200.0f,
	200.0f, 0.0f, -200.0f,
	200.0f, 0.0f, 200.0f,

};
std::vector<GLfloat> floor_normal
{
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f
};
std::vector<GLfloat> floor_texture
{
	0.0f,0.0f,
	0.0f,1.0f,
	1.0f,0.0f,
	0.0f,1.0f,
	1.0f,1.0f,
	1.0f,0.0f
};

unsigned int pyramid_vao;
unsigned int pyramid_vbo[3];
glm::mat4 pyramid_trans[4];
std::vector<glm::vec3> pyramid_vertex
{
	//앞면
	glm::vec3(0.0f, 282.8f, 0.0f),
	glm::vec3(-200.0f, 0.0f, 200.0f),
	glm::vec3(200.0f, 0.0f, 200.0f),
};
std::vector<glm::vec3> pyramid_normal{};
std::vector<glm::vec2> pyramid_texture
{
	glm::vec2(0.5f, 1.0f),
	glm::vec2(0.0, 0.0f),
	glm::vec2(1.0f, 0.0f)
};

struct Texture_data
{
	unsigned int texture;
	int img_width = 0, img_height = 0, img_channel = 0;
	unsigned char* data = nullptr;
};

Texture_data texture[6];


GLuint bg_vao;
GLuint bg_vbo[3];
Texture_data backGround;
std::vector<GLfloat> bg_vertex =
{
	-1000.0f, 1000.0f, -1000.f,
	-1000.0f, -1000.0f, -1000.0f,
	1000.0f, 1000.0f, -1000.0f,

	1000.0f, 1000.0f, -1000.0f,
	-1000.0f, -1000.0f, -1000.0f,
	1000.0f, -1000.0f, -1000.0f
};
std::vector<GLfloat> bg_normal =
{
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};
std::vector<GLfloat> bg_texture =
{
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,

	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
};

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_5_1(26번)");

	glewExperimental = GL_TRUE;
	glewInit();	

	makeCuboid(cuboid_vertex, 200, 200);
	setNormal(cuboid_normal);
	setTexture(cuboid_texture);


	pyramid_trans[0] = glm::mat4(1.0f);
	pyramid_trans[1] = glm::mat4(1.0f);
	pyramid_trans[2] = glm::mat4(1.0f);
	pyramid_trans[3] = glm::mat4(1.0f);
	{

		glm::vec3 p1 = pyramid_vertex[1] - pyramid_vertex[0];

		glm::vec3 p2 = pyramid_vertex[2] - pyramid_vertex[0];

		glm::vec3 temp;
		temp = glm::cross(p1, p2);

		for (int i = 0; i < pyramid_vertex.size() / 3; ++i)
		{
			pyramid_normal.push_back(temp);
			pyramid_normal.push_back(temp);
			pyramid_normal.push_back(temp);
		}
	}
	pyramid_trans[1] = glm::rotate(pyramid_trans[1], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	pyramid_trans[2] = glm::rotate(pyramid_trans[2], glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	pyramid_trans[3] = glm::rotate(pyramid_trans[3], glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	//세이더 읽어와서 세이더 프로그램 만들기
	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();
	initTexture();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	projLocation = glGetUniformLocation(shaderID, "projectionTransform");
	lightPosLocation = glGetUniformLocation(shaderID, "lightPos"); //조명 위치
	lightColorLocation = glGetUniformLocation(shaderID, "lightColor"); //조명 색 설정
	objColorLocation = glGetUniformLocation(shaderID, "objectColor");  //주황색 반사
	viewPosLocation = glGetUniformLocation(shaderID, "cameraEye");     //--- viewPos 값 전달: 카메라 위치 
	ambientLocation = glGetUniformLocation(shaderID, "ambientLight");

	camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	projection = glm::mat4(1.0f);
	//근평면은 포함이고 원평면은 포함X
	projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 2000.0f);

	glutMainLoop();
}


GLvoid drawScene()
{
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);

	glUniform1f(ambientLocation, 0.5f);

	glUniform3f(viewPosLocation, camera_eye.x, camera_eye.y, camera_eye.z);
	glUniform3f(objColorLocation, 1.0f, 1.0f, 1.0f);

	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glViewport(0, 0, window_w, window_h);

	//랜더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderID);

	glm::mat4 temp(1.0f);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(temp));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(temp));
	//glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(temp));
	glBindVertexArray(bg_vao);
	glBindTexture(GL_TEXTURE_2D, backGround.texture);
	glDrawArrays(GL_TRIANGLES, 0, bg_vertex.size() / 3);


	//카메라 변환 적용
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));

	//투영 변환 적용
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
	
	//좌표축 그리기
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, 6);


	//glUniform3f(objColorLocation, 0.6f, 0.0f, 0.0f);
	if (!keyState::c)
	{
		modelLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cuboid_trans));
		glBindVertexArray(vao_cuboid);
		for (int i = 0; i < 6; ++i)
		{
			glBindTexture(GL_TEXTURE_2D, texture[i].texture);
			glDrawArrays(GL_TRIANGLES, 6 * i, cuboid_vertex.size() / 18);
		}
	}
	else
	{
		modelLocation = glGetUniformLocation(shaderID, "modelTransform");

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(floor_trans));
		glBindVertexArray(floor_vao);

		glBindTexture(GL_TEXTURE_2D, texture[4].texture);
		glDrawArrays(GL_TRIANGLES, 0, floor_vertex.size() / 3);

		for (int j = 0; j < 4; ++j)
		{
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(pyramid_trans[j]));
			glBindVertexArray(pyramid_vao);
	
			glBindTexture(GL_TEXTURE_2D, texture[j].texture);
			glDrawArrays(GL_TRIANGLES, 0, pyramid_vertex.size());
		}
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{
	cuboid_trans = glm::mat4(1.0f);
	cuboid_trans = glm::rotate(cuboid_trans, glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	cuboid_trans = glm::rotate(cuboid_trans, glm::radians(xAngle), glm::vec3(1.0f, 0.0f, 0.0f));

	pyramid_trans[0] = glm::mat4(1.0f);
	pyramid_trans[1] = glm::mat4(1.0f);
	pyramid_trans[2] = glm::mat4(1.0f);
	pyramid_trans[3] = glm::mat4(1.0f);

	pyramid_trans[0] = glm::rotate(pyramid_trans[0], glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	pyramid_trans[1] = glm::rotate(pyramid_trans[1], glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	pyramid_trans[2] = glm::rotate(pyramid_trans[2], glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	pyramid_trans[3] = glm::rotate(pyramid_trans[3], glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	pyramid_trans[0] = glm::rotate(pyramid_trans[0], glm::radians(xAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	pyramid_trans[1] = glm::rotate(pyramid_trans[1], glm::radians(xAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	pyramid_trans[2] = glm::rotate(pyramid_trans[2], glm::radians(xAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	pyramid_trans[3] = glm::rotate(pyramid_trans[3], glm::radians(xAngle), glm::vec3(1.0f, 0.0f, 0.0f));

	pyramid_trans[1] = glm::rotate(pyramid_trans[1], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	pyramid_trans[2] = glm::rotate(pyramid_trans[2], glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	pyramid_trans[3] = glm::rotate(pyramid_trans[3], glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    floor_trans = glm::mat4(1.0f);
	floor_trans = glm::rotate(floor_trans, glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	floor_trans = glm::rotate(floor_trans, glm::radians(xAngle), glm::vec3(1.0f, 0.0f, 0.0f));

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 0);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
	{
		stbi_image_free(texture[0].data);
		glutExit();
	}
	else if (key == 's')
	{
		yAngle = 0.0f;
		xAngle = 0.0f;
	}
	else if (key == 'y')
	{
		yAngle += 10.f;
	}
	else if (key == 'Y')
	{
		yAngle -= 10.f;
	}
	else if (key == 'x')
	{
		xAngle += 10.f;
	}
	else if (key == 'X')
	{
		xAngle -= 10.f;
	}
	else if (key == 'c')
	{
		yAngle = 0.0f;
		xAngle = 0.0f;
		keyState::c = keyState::c ? false : true;
	}
}


void initBuffer()
{
	glGenVertexArrays(1, &bg_vao);
	glGenBuffers(3, bg_vbo);

	glBindVertexArray(bg_vao);

	glBindBuffer(GL_ARRAY_BUFFER, bg_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, bg_texture.size() * sizeof(GLfloat), bg_texture.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, bg_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, bg_normal.size() * sizeof(GLfloat), bg_normal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, bg_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, bg_vertex.size() * sizeof(GLfloat), bg_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, axes.axes_color.size() * sizeof(GLfloat), axes.axes_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, axes.axes_vertex.size() * sizeof(GLfloat), axes.axes_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glGenVertexArrays(1, &vao_cuboid);
	glGenBuffers(3, vbo_cuboid);

	glBindVertexArray(vao_cuboid);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cuboid[2]);
	glBufferData(GL_ARRAY_BUFFER, cuboid_texture.size() * sizeof(GLfloat), cuboid_texture.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cuboid[1]);
	glBufferData(GL_ARRAY_BUFFER, cuboid_normal.size() * sizeof(GLfloat), cuboid_normal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cuboid[0]);
	glBufferData(GL_ARRAY_BUFFER, cuboid_vertex.size() * sizeof(GLfloat), cuboid_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &floor_vao);
	glGenBuffers(3, floor_vbo);

	glBindVertexArray(floor_vao);

	glBindBuffer(GL_ARRAY_BUFFER, floor_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, floor_texture.size() * sizeof(GLfloat), floor_texture.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, floor_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, floor_normal.size() * sizeof(GLfloat), floor_normal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, floor_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, floor_vertex.size() * sizeof(GLfloat), floor_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glGenVertexArrays(1, &pyramid_vao);
	glGenBuffers(3, pyramid_vbo);

	glBindVertexArray(pyramid_vao);

	glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, pyramid_texture.size() * sizeof(glm::vec2), pyramid_texture.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, pyramid_normal.size() * sizeof(glm::vec3), pyramid_normal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pyramid_vertex.size() * sizeof(glm::vec3), pyramid_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}



GLvoid initTexture()
{
	glGenTextures(1, &backGround.texture);
	glBindTexture(GL_TEXTURE_2D, backGround.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	backGround.data = stbi_load("bg.jpg", &backGround.img_width, &backGround.img_height, &backGround.img_channel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, backGround.img_width, backGround.img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, backGround.data);

	//stbi_set_flip_vertically_on_load(false);
	glGenTextures(1, &texture[0].texture);
	glBindTexture(GL_TEXTURE_2D, texture[0].texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture[0].data = stbi_load("texture_00.jpg", &texture[0].img_width, &texture[0].img_height, &texture[0].img_channel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture[0].img_width, texture[0].img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture[0].data);

	glGenTextures(1, &texture[1].texture);
	glBindTexture(GL_TEXTURE_2D, texture[1].texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture[1].data = stbi_load("texture_01.jpg", &texture[1].img_width, &texture[1].img_height, &texture[1].img_channel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture[1].img_width, texture[1].img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture[1].data);

	glGenTextures(1, &texture[2].texture);
	glBindTexture(GL_TEXTURE_2D, texture[2].texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture[2].data = stbi_load("texture_02.jpg", &texture[2].img_width, &texture[2].img_height, &texture[2].img_channel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture[2].img_width, texture[2].img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture[2].data);

	glGenTextures(1, &texture[3].texture);
	glBindTexture(GL_TEXTURE_2D, texture[3].texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture[3].data = stbi_load("texture_03.jpg", &texture[3].img_width, &texture[3].img_height, &texture[3].img_channel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture[3].img_width, texture[3].img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture[3].data);

	glGenTextures(1, &texture[4].texture);
	glBindTexture(GL_TEXTURE_2D, texture[4].texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture[4].data = stbi_load("texture_04.jpg", &texture[4].img_width, &texture[4].img_height, &texture[4].img_channel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture[4].img_width, texture[4].img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture[4].data);

	glGenTextures(1, &texture[5].texture);
	glBindTexture(GL_TEXTURE_2D, texture[5].texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture[5].data = stbi_load("texture_05.jpg", &texture[5].img_width, &texture[5].img_height, &texture[5].img_channel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture[5].img_width, texture[5].img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture[5].data);
}