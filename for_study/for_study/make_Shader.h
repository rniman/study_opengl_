#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/GLU.h>

#include "stb_image.h"
#include <vector>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());


GLuint shaderID;		//���̴� ���α׷� �̸�
GLuint vertexShader;	//���ؽ� ���̴� ��ü
GLuint fragmentShader;	//�����׸�Ʈ ���̴� ��ü


char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	
	fopen_s(&fptr, file, "rb"); /* Open file for reading */
	if (!fptr) /* Return NULL on failure */
		return NULL;
	fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
	length = ftell(fptr); /* Find out how many bytes into the file we are */
	buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file and a null terminator */
	fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
	fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
	fclose(fptr); /* Close the file */
	buf[length] = 0; /* Null terminator */

	return buf; /* Return the buffer */
}

GLboolean make_vertexShader()
{
	GLchar* vertexSource;

	//���ؽ� ���̴� �ڵ带 �о�ͼ� �����ϰ� �������� �ؾ���
	//filetobuf:��������� �Լ��� �ؽ�Ʈ�� �а� ���ڿ��� �����ϴ� �Լ�

	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return false;
	}
}

GLboolean make_fragementShader()
{
	GLchar* fragmentSource;

	fragmentSource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return false;
	}
}

GLint make_shaderProgram()
{
	make_vertexShader();	//���ؽ� ���̴� �����
	make_fragementShader();	//�����׸�Ʈ ���̴� �����

	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //���̴� ���α׷� �����

	glAttachShader(ShaderProgramID, vertexShader);
	glAttachShader(ShaderProgramID, fragmentShader);
	glLinkProgram(ShaderProgramID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		return false;
	}

	glUseProgram(ShaderProgramID);

	return ShaderProgramID;
}


unsigned int texture[6];
int widthImage, heightImage, numberOfChannel;

void InitTexture()
{
	glGenTextures(1, &texture[0]); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture[0]); //--- �ؽ�ó ���ε�

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	unsigned char* data = stbi_load("image/wall.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����
	stbi_image_free(data);

	glGenTextures(1, &texture[1]); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture[1]); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	data = stbi_load("image/floor.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����
	stbi_image_free(data);


	glGenTextures(1, &texture[2]); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture[2]); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load("image/s_pme_a0_cmp4.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����
	stbi_image_free(data);

	glGenTextures(1, &texture[3]); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture[3]); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load("image/s_pac_a0_cmp4.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����
	stbi_image_free(data);

	glGenTextures(1, &texture[4]); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture[4]); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load("image/s_pma_a0_cmp4.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����
	stbi_image_free(data);
}
