#include "GLGray.h"
#include "../cocos/ui/shaders/UIShaders.h"
USING_NS_CC;

Scene* GLGray::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GLGray::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GLGray::GLGray()
	:m_pTitle(nullptr),
	m_pGround(nullptr),
	m_pTest(nullptr),
	m_pButton(nullptr)
{}

// on "init" you need to initialize your instance
bool GLGray::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GLGray::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));


	m_pButton = MenuItemImage::create(
		"normal.png",
		"selected.png",
		CC_CALLBACK_1(GLGray::clickCallback, this));
	m_pButton->setPosition(Vec2(origin.x + m_pButton->getContentSize().width / 2,
		origin.y + m_pButton->getContentSize().height / 2));

    // create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, m_pButton, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
	m_pTitle = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
	m_pTitle->setPosition(Vec2(origin.x + visibleSize.width / 2,
		visibleSize.height / 2 + origin.y));

    // add the label as a child to this layer
	this->addChild(m_pTitle);

    // add "GLGray" splash screen"
	m_pGround = Sprite::create("Test.jpg");

    // position the sprite on the center of the screen
	m_pGround->setPosition(Vec2(visibleSize.width / 3 + origin.x, visibleSize.height / 2 + origin.y));

    // add the sprite as a child to this layer
	this->addChild(m_pGround);


	m_pTest = Sprite::create("Test.jpg");
	m_pTest->setPosition(Vec2(visibleSize.width*2 / 3 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(m_pTest);
    
    return true;
}

void GLGray::clickCallback(Ref* pSender)
{
	static bool isGray = false;
	isGray = !isGray;

	auto programGray = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert,
		ccUIGrayScale_frag);
	auto programNormal = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
	m_pTest->setGLProgram(isGray ? programGray : programNormal);

	/*
	static bool isGray = false;
	isGray = !isGray;
	CCLOG("onclick%d", isGray);

	if (isGray)
	{
		auto programGray = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert,
			ccUIGrayScale_frag);
		m_pTest->setGLProgram(programGray);
	}
	else
	{
		GLProgramState *glState = nullptr;
		glState = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
		m_pTest->setGLProgramState(glState);
	}
	*/
}

void GLGray::menuCloseCallback(Ref* pSender)
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
