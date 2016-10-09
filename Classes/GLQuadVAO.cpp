#include "GLQuadVAO.h"
#include "../cocos/ui/shaders/UIShaders.h"
USING_NS_CC;

Scene* GLQuadVAO::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GLQuadVAO::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GLQuadVAO::GLQuadVAO()
	:m_pTitle(nullptr),
	m_pGround(nullptr),
	m_pTest(nullptr),
	m_pButton(nullptr)
{}

bool GLQuadVAO::init()
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

void GLQuadVAO::initVAO()
{
	auto program = getGLProgram();
	//创建和绑定vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	Vertex data[] =
	{
		{ { -1, -1 }, { 0, 1, 0, 1 } },
		{ { 1, -1 }, { 0, 1, 0, 1 } },
		{ { -1, 1 }, { 0, 1, 0, 1 } },
		{ { 1, 1 }, { 0, 1, 0, 1 } }
	};

	//创建和绑定vbo
	glGenBuffers(1, &vertexVBO);
	//顶点信息 传入data与绑定类型相对应GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);


	//想要省去顶点的重复定义 
	//必须定义绘制顶点顺序索引
	//这样才能绘6个点(包括重复的)
	//绘出两个三角形,拼成一个quad
	//GL_ELEMENT_ARRAY_BUFFER
	//注意这样数据格式写入显存之后
	//绘制语法由glDrawArrays 改为 glDrawElements
	GLubyte indices[] = { 0, 1, 2,  //第一个三角形索引
		2, 3, 1 }; //第二个三角形索引
	GLuint indexVBO;
	glGenBuffers(1, &indexVBO);
	//顶点索引 传入data与绑定类型相对应GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//获取vertex attribute "a_position"的入口点
	GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
	//打开入a_position入口点
	glEnableVertexAttribArray(positionLocation);
	//传递顶点数据给a_position，注意最后一个参数是数组的偏移了。
	//第二个参数2表示只处理shader中a_position前两位的值(xy被赋值,zw无视)
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));

	//set for color
	//glGenBuffers(1, &colorVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
	glEnableVertexAttribArray(colorLocation);
	//第二个参数4表示只处理shader中a_color前4位的值(rgba)
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));

	//写入一个颜色 与顶点颜色相乘
	//颜色的取值是0~1
	//这里可以是红色保留 其余的颜色被清除
	//也可以使整体变暗
	GLuint uColorLocation = glGetUniformLocation(program->getProgram(), "u_color");
	//float uColor[] = { 0.5, 0.5, 0.5, 1.0 };
	float uColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glUniform4fv(uColorLocation, 1, uColor);

	//for safty
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLQuadVAO::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	Layer::visit(renderer, parentTransform, parentFlags);

	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(GLQuadVAO::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void GLQuadVAO::onDraw()
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

	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
	

	glBindVertexArray(0);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
	CHECK_GL_ERROR_DEBUG();


	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void GLQuadVAO::menuCloseCallback(Ref* pSender)
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
