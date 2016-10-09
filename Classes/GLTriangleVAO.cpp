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
	//第二个例子
	//自定义shader
	//资源位置在resource目录下
	//注意最后要设置this->setGLProgram(program);

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
	//创建和绑定vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//创建和绑定vbo
	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);

	auto size = Director::getInstance()->getVisibleSize();
	//float vertercies[] = { 0, 0,   //第一个点的坐标
	//	size.width, 0,   //第二个点的坐标
	//	size.width / 2, size.height };  //第三个点的坐标
	float vertercies[] = { -1,-1,   //第一个点的坐标
		0, 0,   //第二个点的坐标
		-1,1 };  //第三个点的坐标

	float color[] = { 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1 };

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertercies), vertercies, GL_STATIC_DRAW);
	//获取vertex attribute "a_position"的入口点
	GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
	//打开入a_position入口点
	glEnableVertexAttribArray(positionLocation);
	//传递顶点数据给a_position，注意最后一个参数是数组的偏移了。
	//第二个参数2表示只处理shader中a_position前两位的值(xy被赋值,zw无视)
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	//set for color
	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
	glEnableVertexAttribArray(colorLocation);
	//第二个参数4表示只处理shader中a_color前4位的值(rgba)
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	//写入一个颜色 与顶点颜色相乘
	//颜色的取值是0~1
	//这里可以是红色保留 其余的颜色被清除
	//也可以使整体变暗
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


	//use vao，因为vao记录了每一个顶点属性和缓冲区的状态，所以只需要绑定就可以使用了
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
