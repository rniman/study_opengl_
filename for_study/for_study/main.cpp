#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

#include "make_Shader.h"
#include "read_Obj.h"
#include <random>
//#include "stb_image.h"

#include "Game.h"
#include "test_state.h"

GLvoid idle();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);

void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);

const GLint window_w = 600, window_h = 600;
GLfloat rColor = 0.4f, gColor = 0.4f, bColor = 0.4f;

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;
unsigned int lightPosLocation;
unsigned int lightColorLocation;
unsigned int objColorLocation;
unsigned int viewPosLocation;
unsigned int ambientLocation;


Game* a;

int main(int argc, char** argv)
{
	Test2State temp;
	a = &temp;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_5_1(26번)");

	glewExperimental = GL_TRUE;
	glewInit();	

	//세이더 읽어와서 세이더 프로그램 만들기
	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutIdleFunc(idle);
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

	glutMainLoop();
}

GLvoid idle()
{
	glutPostRedisplay();
}

GLvoid drawScene()
{
	a->draw();
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
	{
		glutExit();
	}
}


void initBuffer()
{

}


