#include "GLTextureTest.h"
#include "../cocos/ui/shaders/UIShaders.h"
USING_NS_CC;

Scene* GLTextureTest::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GLTextureTest::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

GLTextureTest::GLTextureTest()
	:m_pTitle(nullptr),
	m_pGround(nullptr),
	m_pTest(nullptr),
	m_pButton(nullptr)
{}

bool GLTextureTest::init()
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
	program->initWithFilenames("uvTest.vert", "uvTest.frag");
	program->link();
	//set uniform locations
	program->updateUniforms();
	this->setGLProgram(program);

	initVAO();

	int global = 100;
	[=/*������ⲿ����(=��ʾȫ��)*/](/*������*/){
		CCLOG("GLTextureTest::init%d", global);
	}(/*ʵ��*/);//����

	std::function<void(int)> testFunc = [](int a){
		CCLOG("GLTextureTest::init%d" , a);
	};
	testFunc(99);

	return true;
}

void GLTextureTest::initVAO()
{
	auto program = getGLProgram();
	//�����Ͱ�vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//�����Ͱ�vbo
	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);

	//��Ҫʡȥ������ظ����� 
	//���붨����ƶ���˳������
	//�������ܻ�6����(�����ظ���)
	//�������������,ƴ��һ��quad
	//GL_ELEMENT_ARRAY_BUFFER
	//ע���������ݸ�ʽд���Դ�֮��
	//�����﷨��glDrawArrays ��Ϊ glDrawElements
	GLubyte indices[] = { 0, 1, 2,  //��һ������������
		2, 3, 1 }; //�ڶ�������������
	GLuint indexVBO;
	glGenBuffers(1, &indexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	Vertex data[] =
	{
		{ { -1, -1 }, { 0, 1, 0, 1 }, { 0, 1 } },
		{ { 1, -1 }, { 0, 1, 0, 1 }, { 1, 1 } },
		{ { -1, 1 }, { 0, 1, 0, 1 }, { 0, 0 } },
		{ { 1, 1 }, { 0, 1, 0, 1 }, { 1, 0 } }
	};


	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	//��ȡvertex attribute "a_position"����ڵ�
	GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
	//����a_position��ڵ�
	glEnableVertexAttribArray(positionLocation);
	//���ݶ������ݸ�a_position��ע�����һ�������������ƫ���ˡ�
	//�ڶ�������2��ʾֻ����shader��a_positionǰ��λ��ֵ(xy����ֵ,zw����)
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));

	//set for color
	//glGenBuffers(1, &colorVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
	glEnableVertexAttribArray(colorLocation);
	//�ڶ�������4��ʾֻ����shader��a_colorǰ4λ��ֵ(rgba)
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));


	GLuint uvLocation = glGetAttribLocation(program->getProgram(), "a_coord");
	glEnableVertexAttribArray(uvLocation);
	//�ڶ�������4��ʾֻ����shader��a_colorǰ4λ��ֵ(rgba)
	glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoord));

	//д��һ����ɫ �붥����ɫ���
	//��ɫ��ȡֵ��0~1
	//��������Ǻ�ɫ���� �������ɫ�����
	//Ҳ����ʹ����䰵
	GLuint uColorLocation = glGetUniformLocation(program->getProgram(), "u_color");
	//float uColor[] = { 0.5, 0.5, 0.5, 1.0 };
	float uColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glUniform4fv(uColorLocation, 1, uColor);


	textureId = Director::getInstance()->getTextureCache()->addImage("HelloWorld.png")->getName();
	
	auto uvTexture = Director::getInstance()->getTextureCache()->addImage("3D/caustics.png");
	//ʹ����repeat
	//���ǻ���ִ���OpenGL error 0x0500
	Texture2D::TexParams		tRepeatParams;
	tRepeatParams.magFilter = GL_LINEAR;
	tRepeatParams.minFilter = GL_LINEAR;
	tRepeatParams.wrapS = GL_REPEAT;
	tRepeatParams.wrapT = GL_REPEAT;
	uvTexture->setTexParameters(tRepeatParams);
	
	textureId1 = uvTexture->getName();
	
	//GLuint location0 = glGetUniformLocation(program->getProgram(), "CC_Texture0");
	//GLuint location1 = glGetUniformLocation(program->getProgram(), "CC_Texture1");
	
	uvCoordLoc = glGetUniformLocation(program->getProgram(), "v_animLight");
	
	//for safty
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLTextureTest::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	Layer::visit(renderer, parentTransform, parentFlags);

	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(GLTextureTest::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void GLTextureTest::onDraw()
{
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

	auto glProgram = getGLProgram();

	glProgram->use();

	//set uniform values, the order of the line is very important
	glProgram->setUniformsForBuiltins();

	GL::bindTexture2D(textureId);
	GL::bindTexture2DN(1 , textureId1);

	static float u = 0.0f;
	static float v = 0.0f;
	u += 0.001f;
	//v += 0.001f;
	if (u > 1) u = 0.0f;
	if (v > 1) v = 0.0f;
	float uv[] = { u , v };
	glUniform2fv(uvCoordLoc, 1, uv);

	//use vao����Ϊvao��¼��ÿһ���������Ժͻ�������״̬������ֻ��Ҫ�󶨾Ϳ���ʹ����
	glBindVertexArray(vao);

	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);


	glBindVertexArray(0);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
	CHECK_GL_ERROR_DEBUG();


	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void GLTextureTest::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
