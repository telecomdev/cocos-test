#include "GLTextureVAO.h"
#include "../cocos/ui/shaders/UIShaders.h"
USING_NS_CC;

Scene* GLTextureVAO::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GLTextureVAO::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GLTextureVAO::GLTextureVAO()
	:m_pTitle(nullptr),
	m_pGround(nullptr),
	m_pTest(nullptr),
	m_pButton(nullptr)
{}

bool GLTextureVAO::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	//第二个例子
	//自定义shader
	//资源位置在resource目录下
	//注意最后要设置this->setGLProgram(program);

	//create my own program
	auto program = new GLProgram();
	program->initWithFilenames("uvVertShader.vert", "uvFragShader.frag");
	program->link();
	//set uniform locations
	program->updateUniforms();
	this->setGLProgram(program);

	initVAO();


	//this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
	//initMVP();
	return true;
}

void GLTextureVAO::initVAO()
{
	auto program = getGLProgram();
	mShaderProgram = program;
	_baseMapTexId = Director::getInstance()->getTextureCache()->addImage("HelloWorld.png")->getName();
	_lightMapTexId = Director::getInstance()->getTextureCache()->addImage("3D/caustics.png")->getName();
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
}

void GLTextureVAO::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	Layer::visit(renderer, parentTransform, parentFlags);

	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(GLTextureVAO::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void GLTextureVAO::onDraw()
{
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

	typedef struct {
		float Position[3];
		float TexCoord[2];
	} Vertex;
#define TEX_COORD_MAX   1

	Vertex Vertices[] = {

		{ { -0.5, 0.5, 0 }, { 0, 0 } },
		{ { -0.5, -0.5, 0 }, { 0, TEX_COORD_MAX } },
		{ { 0.5, -0.5, 0 }, { TEX_COORD_MAX, TEX_COORD_MAX } },
		{ { 0.5, 0.5, 0 }, { TEX_COORD_MAX, 0 } },

	};
	int vertexCount = sizeof(Vertices) / sizeof(Vertices[0]);

	GLubyte Indices[] = {
		0, 1, 2,
		2, 3, 0,
	};

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	_positionLoc = glGetAttribLocation(mShaderProgram->getProgram(), "a_position");
	_texCoordLoc = glGetAttribLocation(mShaderProgram->getProgram(), "a_texCoord");
	//_baseMapLoc = glGetUniformLocation(mShaderProgram->getProgram(), "s_baseMap");
	//_lightMapLoc = glGetUniformLocation(mShaderProgram->getProgram(), "s_lightMap");
	_baseMapLoc = glGetUniformLocation(mShaderProgram->getProgram(), "CC_Texture0");
	_lightMapLoc = glGetUniformLocation(mShaderProgram->getProgram(), "CC_Texture1");

	glEnableVertexAttribArray(_positionLoc);
	glEnableVertexAttribArray(_texCoordLoc);

	glVertexAttribPointer(_positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
	glVertexAttribPointer(_texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, TexCoord));

	mShaderProgram->use();
	mShaderProgram->setUniformsForBuiltins();

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(VAO);

	GL::bindTexture2DN(0, _lightMapTexId);
	glUniform1i(_lightMapLoc, 0); // unnecc in practice

	GL::bindTexture2DN(1, _baseMapTexId);
	glUniform1i(_baseMapLoc, 1); // unnecc in practice

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

	glBindVertexArray(0);
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);

	CHECK_GL_ERROR_DEBUG();
	glDisable(GL_DEPTH_TEST);

	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void GLTextureVAO::menuCloseCallback(Ref* pSender)
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
