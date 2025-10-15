#include "stdafx.h"
#include "Renderer.h"
#define pNum 10000

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}

Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_TestShader = CompileShaders("./Shaders/test.vs", "./Shaders/test.fs");
	m_ParticleShader = CompileShaders("./Shaders/particle.vs", "./Shaders/particle.fs");
	m_GridMeshShader = CompileShaders("./Shaders/GridMesh.vs", "./Shaders/GridMesh.fs");
	m_FullScreenShader = CompileShaders("./Shaders/Fullscreen.vs", "./Shaders/Fullscreen.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	//Create Particles
	CreateParticles(pNum);

	// create gridmesh
	CreateGridMesh(100, 100);

	// create full screen
	CreateFullScreenObjects();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

void Renderer::DeleteAllShaderPrograms()
{
	glDeleteShader(m_SolidRectShader);
	glDeleteShader(m_TestShader);
	glDeleteShader(m_ParticleShader);
	glDeleteShader(m_GridMeshShader);
}

void Renderer::CompileAllShaderPrograms()
{
	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_TestShader = CompileShaders("./Shaders/test.vs", "./Shaders/test.fs");
	m_ParticleShader = CompileShaders("./Shaders/particle.vs", "./Shaders/particle.fs");
	m_GridMeshShader = CompileShaders("./Shaders/GridMesh.vs", "./Shaders/GridMesh.fs");
	m_FullScreenShader = CompileShaders("./Shaders/Fullscreen.vs", "./Shaders/Fullscreen.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	//Create Particles
	CreateParticles(pNum);

	// create gridmesh
	CreateGridMesh(100, 100);

	// create full screen
	CreateFullScreenObjects();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::ReloadAllShaderPrograms()
{
	DeleteAllShaderPrograms();
	CompileAllShaderPrograms();
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,
		-1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 
		1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1

		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  
		1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f,
		1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float center = 0.5;
	float size = 0.1;
	float testPos[]
		=
	{
		(0.f - center) * size, (0.f - center) * size, 0.f, 1,		// x, y, z, value
		(1.f - center) * size, (0.f - center) * size, 0.f, 1,
		(1.f - center) * size, (1.f - center) * size, 0.f, 1,
		(0.f - center) * size, (0.f - center) * size, 0.f, 1,
		(1.f - center) * size, (1.f - center) * size, 0.f, 1,
		(0.f - center) * size, (1.f - center) * size, 0.f, 1,		//Quad1

		(0.f - center) * size, (0.f - center) * size, 0.f, 0.5,
		(1.f - center) * size, (0.f - center) * size, 0.f, 0.5,
		(1.f - center) * size, (1.f - center) * size, 0.f, 0.5,
		(0.f - center) * size, (0.f - center) * size, 0.f, 0.5,
		(1.f - center) * size, (1.f - center) * size, 0.f, 0.5,
		(0.f - center) * size, (1.f - center) * size, 0.f, 0.5		//Quad2
	};

	glGenBuffers(1, &m_VBOTestPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testPos), testPos, GL_STATIC_DRAW);

	float testColor[]
		=
	{
		1.f, 0.f, 0.f, 1.0f,
		0.f, 1.f, 0.f, 1.0f,
		0.f, 0.f, 1.f, 1.0f,
		1.f, 0.f, 0.f, 1.0f,
		0.f, 1.f, 0.f, 1.0f,
		0.f, 0.f, 1.f, 1.0f,	//Quad1

		1.f, 0.f, 0.f, 1.0f,
		0.f, 1.f, 0.f, 1.0f,
		0.f, 0.f, 1.f, 1.0f,
		1.f, 0.f, 0.f, 1.0f,
		0.f, 1.f, 0.f, 1.0f,
		0.f, 0.f, 1.f, 1.0f		//Quad2
	};

	glGenBuffers(1, &m_VBOTestColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testColor), testColor, GL_STATIC_DRAW);
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];

	size_t slen = strlen(pShaderText);
	if (slen > INT_MAX) {
		// Handle error
	}
	GLint len = (GLint)slen;

	Lengths[0] = len;
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawTest()
{
	// time ����
	m_Time += 0.0001;

	//Program select
	glUseProgram(m_TestShader);

	int u_TimeLoc = glGetUniformLocation(m_TestShader, "u_Time");
	glUniform1f(u_TimeLoc, m_Time);

	// Pos
	int aPosLoc = glGetAttribLocation(m_TestShader, "a_Position");
	glEnableVertexAttribArray(aPosLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);
	glVertexAttribPointer(aPosLoc,
		3, GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 4,
		0);
	
	//value
	int aValueLoc = glGetAttribLocation(m_TestShader, "a_Value");
	glEnableVertexAttribArray(aValueLoc);
	glVertexAttribPointer(aValueLoc,
		1, GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 4,
		(GLvoid*)(sizeof(float) * 3));

	// Color
	int aColorLoc = glGetAttribLocation(m_TestShader, "a_Color");
	glEnableVertexAttribArray(aColorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestColor);
	glVertexAttribPointer(aColorLoc,
		4, GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 4,
		0);

	glDrawArrays(GL_TRIANGLES, 0, 12);

	// ���������� Enable �� Disable.
	glDisableVertexAttribArray(aPosLoc);
	glDisableVertexAttribArray(aColorLoc);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawParticle()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// time ����
	m_Time += 0.0001;

	//Program select
	glUseProgram(m_ParticleShader);

	int u_Force = glGetUniformLocation(m_ParticleShader, "u_Force");
	glUniform3f(u_Force, 2, 0, 0);

	int u_TimeLoc = glGetUniformLocation(m_ParticleShader, "u_Time");
	glUniform1f(u_TimeLoc, m_Time);

	int stride = 15;

	int aPosLoc = glGetAttribLocation(m_ParticleShader, "a_Position");
	int aValueLoc = glGetAttribLocation(m_ParticleShader, "a_Value");
	int aColorLoc = glGetAttribLocation(m_ParticleShader, "a_Color");
	int aTimeLoc = glGetAttribLocation(m_ParticleShader, "a_sTime");
	int aVelLoc = glGetAttribLocation(m_ParticleShader, "a_Vel");
	int alTimeLoc = glGetAttribLocation(m_ParticleShader, "a_lifeTime");
	int aMassLoc = glGetAttribLocation(m_ParticleShader, "a_Mass");
	int aPeriodLoc = glGetAttribLocation(m_ParticleShader, "a_Period");

	glEnableVertexAttribArray(aPosLoc);
	glEnableVertexAttribArray(aValueLoc);
	glEnableVertexAttribArray(aColorLoc);
	glEnableVertexAttribArray(aTimeLoc);
	glEnableVertexAttribArray(aVelLoc);
	glEnableVertexAttribArray(alTimeLoc);
	glEnableVertexAttribArray(aMassLoc);
	glEnableVertexAttribArray(aPeriodLoc);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);
	glVertexAttribPointer(aPosLoc,
		3, GL_FLOAT,
		GL_FALSE,
		sizeof(float) * stride,
		0);

	glVertexAttribPointer(aValueLoc,
		1, GL_FLOAT,
		GL_FALSE,
		sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 3));

	glVertexAttribPointer(aColorLoc,
		4, GL_FLOAT,
		GL_FALSE,
		sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 4));

	glVertexAttribPointer(aTimeLoc,
		1, GL_FLOAT,
		GL_FALSE,
		sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 8));

	glVertexAttribPointer(aVelLoc,
		3, GL_FLOAT,
		GL_FALSE,
		sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 9));

	glVertexAttribPointer(alTimeLoc,
		1, GL_FLOAT,
		GL_FALSE,
		sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 12));

	glVertexAttribPointer(aMassLoc,
		1, GL_FLOAT,
		GL_FALSE,
		sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 13));

	glVertexAttribPointer(aPeriodLoc,
		1, GL_FLOAT,
		GL_FALSE,
		sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 14));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOParticlesVertexCount);

	// ���������� Enable �� Disable.
	glDisableVertexAttribArray(aPosLoc);
	glDisableVertexAttribArray(aColorLoc);
	glDisableVertexAttribArray(aValueLoc);
	glDisableVertexAttribArray(aTimeLoc);
	glDisableVertexAttribArray(aVelLoc);
	glDisableVertexAttribArray(alTimeLoc);
	glDisableVertexAttribArray(aMassLoc);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::CreateParticles(int count)
{
	int particleCounts = count;
	int verticesCounts = count * 6;
	int floatCountPerVertex = 3 + 1 + 4 + 1 + 3 + 1 + 1 + 1;			// x, y, z, value, rgba, sTime, vx, vy, vz, lifeTime, mass, period
	int totalfloatCounts = verticesCounts * floatCountPerVertex;
	int floatCountsPerParticle = floatCountPerVertex * 6;

	float* temp = NULL;
	temp = new float[totalfloatCounts];

	for (int i = 0; i < particleCounts; i++)
	{
		float size = 0.02 * (float)rand() / (float)RAND_MAX;
		float centerX = 0; // (float)rand() / (float)RAND_MAX * 2.f - 1.f;
		float centerY = 0; // (float)rand() / (float)RAND_MAX* 2.f - 1.f;
		float value = (float)rand() / (float)RAND_MAX;
		float r = (float)rand() / (float)RAND_MAX;
		float g = (float)rand() / (float)RAND_MAX;
		float b = (float)rand() / (float)RAND_MAX;
		float a = 1;
		float sTime = (float)rand() / (float)RAND_MAX * 2.f;
		float vx = ((float)rand() / (float)RAND_MAX * 2.f - 1.f)*2;
		float vy = (float)rand() / (float)RAND_MAX * 0.3f + 1.f;
		float vz = 0;
		float lifeTime = (float)rand() / (float)RAND_MAX;
		float mass = (float)rand() / (float)RAND_MAX + 0.1f;
		float period = (float)rand() / (float)RAND_MAX;
		int index = i * floatCountsPerParticle;

		temp[index] = centerX - size; index++; // x
		temp[index] = centerY - size; index++; // y
		temp[index] = 0; index++;	           // z
		temp[index] = value; index++;		   // value
		temp[index] = r; index++;			   // r
		temp[index] = g; index++;			   // g
		temp[index] = b; index++;			   // b
		temp[index] = a; index++;			   // a
		temp[index] = sTime; index++;		   // sTime
		temp[index] = vx; index++;			   // vx
		temp[index] = vy; index++;			   // vy
		temp[index] = vz; index++;			   // vz
		temp[index] = lifeTime; index++;	   // lifeTime
		temp[index] = mass; index++;		   // mass
		temp[index] = period; index++;		   // period

		temp[index] = centerX + size; index++;
		temp[index] = centerY + size; index++;
		temp[index] = 0; index++;			
		temp[index] = value; index++;
		temp[index] = r; index++;			  
		temp[index] = g; index++;			   
		temp[index] = b; index++;			   
		temp[index] = a; index++;
		temp[index] = sTime; index++;
		temp[index] = vx; index++;			   
		temp[index] = vy; index++;			   
		temp[index] = vz; index++;
		temp[index] = lifeTime; index++;
		temp[index] = mass; index++;
		temp[index] = period; index++;

		temp[index] = centerX - size; index++;
		temp[index] = centerY + size; index++;
		temp[index] = 0; index++;
		temp[index] = value; index++;
		temp[index] = r; index++;
		temp[index] = g; index++;
		temp[index] = b; index++;
		temp[index] = a; index++;
		temp[index] = sTime; index++;
		temp[index] = vx; index++;
		temp[index] = vy; index++;
		temp[index] = vz; index++;
		temp[index] = lifeTime; index++;
		temp[index] = mass; index++;
		temp[index] = period; index++;

		temp[index] = centerX - size; index++;
		temp[index] = centerY - size; index++;
		temp[index] = 0; index++;	
		temp[index] = value; index++;
		temp[index] = r; index++;
		temp[index] = g; index++;
		temp[index] = b; index++;
		temp[index] = a; index++;
		temp[index] = sTime; index++;
		temp[index] = vx; index++;
		temp[index] = vy; index++;
		temp[index] = vz; index++;
		temp[index] = lifeTime; index++;
		temp[index] = mass; index++;
		temp[index] = period; index++;

		temp[index] = centerX + size; index++;
		temp[index] = centerY - size; index++;
		temp[index] = 0; index++;	
		temp[index] = value; index++;
		temp[index] = r; index++;
		temp[index] = g; index++;
		temp[index] = b; index++;
		temp[index] = a; index++;
		temp[index] = sTime; index++;
		temp[index] = vx; index++;
		temp[index] = vy; index++;
		temp[index] = vz; index++;
		temp[index] = lifeTime; index++;
		temp[index] = mass; index++;
		temp[index] = period; index++;

		temp[index] = centerX + size; index++;
		temp[index] = centerY + size; index++;
		temp[index] = 0; index++;
		temp[index] = value; index++;
		temp[index] = r; index++;
		temp[index] = g; index++;
		temp[index] = b; index++;
		temp[index] = a; index++;
		temp[index] = sTime; index++;
		temp[index] = vx; index++;
		temp[index] = vy; index++;
		temp[index] = vz; index++;
		temp[index] = lifeTime; index++;
		temp[index] = mass; index++;
		temp[index] = period; index++;
	}

	glGenBuffers(1, &m_VBOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalfloatCounts, temp, GL_STATIC_DRAW);
	m_VBOParticlesVertexCount = verticesCounts;

	delete[] temp;
}

void Renderer::CreateGridMesh(int x, int y)
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;
	int pointCountX = x;
	int pointCountY = y;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	m_GridMeshVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));

			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));

		}

	}



	//Make triangles

	int vertIndex = 0;

	for (int x = 0; x < pointCountX - 1; x++)

	{

		for (int y = 0; y < pointCountY - 1; y++)

		{

			//Triangle part 1

			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];

			vertIndex++;

			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];

			vertIndex++;

			vertices[vertIndex] = 0.f;

			vertIndex++;

			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];

			vertIndex++;

			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];

			vertIndex++;

			vertices[vertIndex] = 0.f;

			vertIndex++;

			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];

			vertIndex++;

			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];

			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_GridMeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);

}

void Renderer::DrawGridMesh()
{
	// time ����
	m_Time += 0.001;

	//Program select
	int shader = m_GridMeshShader;
	glUseProgram(m_GridMeshShader);

	int u_TimeLoc = glGetUniformLocation(shader, "u_Time");
	glUniform1f(u_TimeLoc, m_Time);

	// Pos
	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);

	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_GridMeshVertexCount);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawFullScreenColor(float r, float g, float b, float a)
{
	//Program select
	int shader = m_FullScreenShader;
	glUseProgram(shader);

	glUniform4f(glGetUniformLocation(shader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);

	glBindBuffer(GL_ARRAY_BUFFER, m_FullScreenVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::CreateFullScreenObjects()
{
	float rect[]
		=
	{
		-1.f, -1.f, 0.f,
		1.f, -1.f,  0.f,
		-1.f, 1.f,  0.f,//Triangle1

		-1.f, 1.f, 0.f,
		1.f, -1.f, 0.f,
		1.f, 1.f, 0.f //Triangle2
	};

	glGenBuffers(1, &m_FullScreenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_FullScreenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
}

