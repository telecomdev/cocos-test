#include "GLCubeVAO.h"
#include "../cocos/ui/shaders/UIShaders.h"
USING_NS_CC;

Scene* GLCubeVAO::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GLCubeVAO::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GLCubeVAO::GLCubeVAO()
	:m_pTitle(nullptr),
	m_pGround(nullptr),
	m_pTest(nullptr),
	m_pButton(nullptr)
{}

bool GLCubeVAO::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	//�ڶ�������
	//�Զ���shader
	//��Դλ����resourceĿ¼��
	//ע�����Ҫ����this->setGLProgram(program);

	//create my own program
	auto program = new GLProgram();
	program->initWithFilenames("myVertShader3D.vert", "myFragShader.frag");
	program->link();
	//set uniform locations
	program->updateUniforms();
	this->setGLProgram(program);

	initVAO();

	return true;
}

void GLCubeVAO::initVAO()
{
	auto program = getGLProgram();
	//�����Ͱ�vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	Vertex data[] =
	{
		// Front
		{ { 1, -1, 0 }, { 1, 0, 0, 1 } },
		{ { 1, 1, 0 }, { 0, 1, 0, 1 } },
		{ { -1, 1, 0 }, { 0, 0, 1, 1 } },
		{ { -1, -1, 0 }, { 0, 0, 0, 1 } },
		// Back
		{ { 1, 1, -2 }, { 1, 0, 0, 1 } },
		{ { -1, -1, -2 }, { 0, 1, 0, 1 } },
		{ { 1, -1, -2 }, { 0, 0, 1, 1 } },
		{ { -1, 1, -2 }, { 0, 0, 0, 1 } },
		// Left
		{ { -1, -1, 0 }, { 1, 0, 0, 1 } },
		{ { -1, 1, 0 }, { 0, 1, 0, 1 } },
		{ { -1, 1, -2 }, { 0, 0, 1, 1 } },
		{ { -1, -1, -2 }, { 0, 0, 0, 1 } },
		// Right
		{ { 1, -1, -2 }, { 1, 0, 0, 1 } },
		{ { 1, 1, -2 }, { 0, 1, 0, 1 } },
		{ { 1, 1, 0 }, { 0, 0, 1, 1 } },
		{ { 1, -1, 0 }, { 0, 0, 0, 1 } },
		// Top
		{ { 1, 1, 0 }, { 1, 0, 0, 1 } },
		{ { 1, 1, -2 }, { 0, 1, 0, 1 } },
		{ { -1, 1, -2 }, { 0, 0, 1, 1 } },
		{ { -1, 1, 0 }, { 0, 0, 0, 1 } },
		// Bottom
		{ { 1, -1, -2 }, { 1, 0, 0, 1 } },
		{ { 1, -1, 0 }, { 0, 1, 0, 1 } },
		{ { -1, -1, 0 }, { 0, 0, 1, 1 } },
		{ { -1, -1, -2 }, { 0, 0, 0, 1 } }
	};
	//�����Ͱ�vbo
	glGenBuffers(1, &vertexVBO);
	//������Ϣ ����data����������ӦGL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);


	//��Ҫʡȥ������ظ����� 
	//���붨����ƶ���˳������
	//�������ܻ�6����(�����ظ���)
	//�������������,ƴ��һ��quad
	//GL_ELEMENT_ARRAY_BUFFER
	//ע���������ݸ�ʽд���Դ�֮��
	//�����﷨��glDrawArrays ��Ϊ glDrawElements
	GLubyte indices[] = {
		// Front
		0, 1, 2,
		2, 3, 0,
		// Left
		8, 9, 10,
		10, 11, 8,
		// Top
		16, 17, 18,
		18, 19, 16,
		// Back
		4, 5, 6,
		4, 5, 7,
		// Right
		12, 13, 14,
		14, 15, 12,
		// Bottom
		20, 21, 22,
		22, 23, 20
	};
	GLuint indexVBO;
	glGenBuffers(1, &indexVBO);
	//�������� ����data����������ӦGL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	
	//��ȡvertex attribute "a_position"����ڵ�
	GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
	//����a_position��ڵ�
	glEnableVertexAttribArray(positionLocation);
	//���ݶ������ݸ�a_position��ע�����һ�������������ƫ���ˡ�
	//�ڶ�������2��ʾֻ����shader��a_positionǰ��λ��ֵ(xy����ֵ,zw����)
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));

	GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
	glEnableVertexAttribArray(colorLocation);
	//�ڶ�������4��ʾֻ����shader��a_colorǰ4λ��ֵ(rgba)
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));

	//д��һ����ɫ �붥����ɫ���
	//��ɫ��ȡֵ��0~1
	//��������Ǻ�ɫ���� �������ɫ�����
	//Ҳ����ʹ����䰵
	GLuint uColorLocation = glGetUniformLocation(program->getProgram(), "u_color");
	//float uColor[] = { 0.5, 0.5, 0.5, 1.0 };
	float uColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glUniform4fv(uColorLocation, 1, uColor);

	//for safty
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLCubeVAO::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	Layer::visit(renderer, parentTransform, parentFlags);

	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(GLCubeVAO::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void GLCubeVAO::onDraw()
{
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);



	Mat4 projectionMatrix;
	Mat4::createPerspective(60, 480 / 320, 1.0, 42, &projectionMatrix);
	Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, projectionMatrix);

	Mat4 modelViewMatrix;
	Mat4::createLookAt(Vec3(0, 0, 1), Vec3(0, 0, 0), Vec3(0, 1, 0), &modelViewMatrix);
	modelViewMatrix.translate(0, 2, -10);

	static float rotation = 0;
	modelViewMatrix.rotate(Vec3(0, 1, 0), CC_DEGREES_TO_RADIANS(rotation));
	rotation++;
	if (rotation > 360) {
		rotation = 0;
	}
	Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, modelViewMatrix);





	auto glProgram = getGLProgram();

	glProgram->use();

	//set uniform values, the order of the line is very important
	glProgram->setUniformsForBuiltins();


	//use vao����Ϊvao��¼��ÿһ���������Ժͻ�������״̬������ֻ��Ҫ�󶨾Ϳ���ʹ����
	glBindVertexArray(vao);

	//glDrawArrays(GL_TRIANGLES, 0, 6);
	unsigned int vertexNum = 36;
	glDrawElements(GL_TRIANGLES, vertexNum, GL_UNSIGNED_BYTE, (GLvoid*)0);
	

	glBindVertexArray(0);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, vertexNum);
	CHECK_GL_ERROR_DEBUG();


	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void GLCubeVAO::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
