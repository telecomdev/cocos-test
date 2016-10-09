#include "GLTriangleVAO.h"
#include "../cocos/ui/shaders/UIShaders.h"
USING_NS_CC;

Scene* GLTriangleVAO::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GLTriangleVAO::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GLTriangleVAO::GLTriangleVAO()
	:m_pTitle(nullptr),
	m_pGround(nullptr),
	m_pTest(nullptr),
	m_pButton(nullptr)
{}

bool GLTriangleVAO::init()
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
	program->initWithFilenames("myVertShader.vert", "myFragShader.frag");
	program->link();
	//set uniform locations
	program->updateUniforms();
	this->setGLProgram(program);

	initVAO();

	return true;
}

void GLTriangleVAO::initVAO()
{
	auto program = getGLProgram();
	//�����Ͱ�vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//�����Ͱ�vbo
	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);

	auto size = Director::getInstance()->getVisibleSize();
	//float vertercies[] = { 0, 0,   //��һ���������
	//	size.width, 0,   //�ڶ����������
	//	size.width / 2, size.height };  //�������������
	float vertercies[] = { -1,-1,   //��һ���������
		0, 0,   //�ڶ����������
		-1,1 };  //�������������

	float color[] = { 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1 };

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertercies), vertercies, GL_STATIC_DRAW);
	//��ȡvertex attribute "a_position"����ڵ�
	GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
	//����a_position��ڵ�
	glEnableVertexAttribArray(positionLocation);
	//���ݶ������ݸ�a_position��ע�����һ�������������ƫ���ˡ�
	//�ڶ�������2��ʾֻ����shader��a_positionǰ��λ��ֵ(xy����ֵ,zw����)
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	//set for color
	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
	glEnableVertexAttribArray(colorLocation);
	//�ڶ�������4��ʾֻ����shader��a_colorǰ4λ��ֵ(rgba)
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	//д��һ����ɫ �붥����ɫ���
	//��ɫ��ȡֵ��0~1
	//��������Ǻ�ɫ���� �������ɫ�����
	//Ҳ����ʹ����䰵
	GLuint uColorLocation = glGetUniformLocation(program->getProgram(), "u_color");
	//float uColor[] = { 0.5, 0.5, 0.5, 1.0 };
	float uColor[] = { 1.0, 0.0, 0.0, 1.0 };
	glUniform4fv(uColorLocation, 1, uColor);

	//for safty
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLTriangleVAO::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	Layer::visit(renderer, parentTransform, parentFlags);

	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(GLTriangleVAO::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void GLTriangleVAO::onDraw()
{
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

	auto glProgram = getGLProgram();

	glProgram->use();

	//set uniform values, the order of the line is very important
	glProgram->setUniformsForBuiltins();


	//use vao����Ϊvao��¼��ÿһ���������Ժͻ�������״̬������ֻ��Ҫ�󶨾Ϳ���ʹ����
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
	CHECK_GL_ERROR_DEBUG();


	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void GLTriangleVAO::menuCloseCallback(Ref* pSender)
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
