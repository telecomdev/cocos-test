#include "GLTriangle.h"
#include "../cocos/ui/shaders/UIShaders.h"
USING_NS_CC;

Scene* GLTriangle::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GLTriangle::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GLTriangle::GLTriangle()
	:m_pTitle(nullptr),
	m_pGround(nullptr),
	m_pTest(nullptr),
	m_pButton(nullptr)
{}

// on "init" you need to initialize your instance
bool GLTriangle::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	//��һ������
	//����Ĭ��shader������������
	this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));

    
    
    return true;
}

void GLTriangle::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	Layer::visit(renderer, parentTransform, parentFlags);

	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(GLTriangle::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void GLTriangle::onDraw()
{
	//��õ�ǰHelloWorld��shader
	auto glProgram = getGLProgram();
	//ʹ�ô�shader
	glProgram->use();
	//���ø�shader��һЩ����uniform,��Ҫ��MVP����model-view-project����
	glProgram->setUniformsForBuiltins();

	auto size = Director::getInstance()->getWinSize();
	//ָ����Ҫ���Ƶ������ε��������㣬�ֱ�λ����Ļ���½ǣ����½Ǻ����м�Ķ���
	float vertercies[] = { 0, 0,   //��һ���������
		size.width, 0,   //�ڶ����������
		size.width / 2, size.height };  //�������������
	//ָ��ÿһ���������ɫ����ɫֵ��RGBA��ʽ�ģ�ȡֵ��Χ��0-1
	float color[] = { 0, 0, 0, 1,    //��һ�������ɫ����ɫ
		1, 0, 0, 1,  //�ڶ��������ɫ, ��ɫ
		0, 0, 1, 1 };  //�����������ɫ�� ��ɫ
	//��������Ϊposition��color��vertex attribute
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	
	/**
		���ﾭ�����Կ��Է���
		a_position �� a_color 
		loc�ֱ�Ϊ0��1
		��Ӧ��������ֵ
		GLProgram::VERTEX_ATTRIB_POSITION
		GLProgram::VERTEX_ATTRIB_COLOR
		��program����ʱ����glBindAttribLocationԤ���ƶ�λ��
		GLProgram::bindPredefinedVertexAttribs

		GL::enableVertexAttribs��װ��glEnableVertexAttribArray����
		Ҳ�ǰ���loc������Attrib����
		�����Ȱ�ĳ��attrib enable��֮��
		���ܽ��ŵ���glVertexAttribPointer
		��shader�е����Ը�ֵ
	*/
	GLint locPos = glGetAttribLocation(glProgram->getProgram(), "a_position");
	GLint locColor = glGetAttribLocation(glProgram->getProgram(), "a_color");
	CCLOG("%d , %d", locPos, locColor);
	
	//�ֱ��position��colorָ������Դ
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);
	//���������Σ���ν��draw call����ָ�����������
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//֪ͨcocos2d-x ��renderer�������ں��ʵ�ʱ�������ЩOpenGL����
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
	//��������ˣ�����ʹ�������������ȡ������Ϣ
	CHECK_GL_ERROR_DEBUG();
}


void GLTriangle::menuCloseCallback(Ref* pSender)
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
