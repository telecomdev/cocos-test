#include "UVSpriteTest.h"
#include "../cocos/ui/shaders/UIShaders.h"
#include "Navigate.h"
USING_NS_CC;

Scene* UVSpriteTest::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = UVSpriteTest::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

UVSpriteTest::UVSpriteTest()
{}

bool UVSpriteTest::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();

	/*auto background = Sprite::create("HelloWorld.png");
	background->setColor(Color3B(255, 0, 0));
	background->setPosition(Vec2(visibleSize / 2));
	addChild(background);*/


	UVSprite * sprite = UVSprite::create("ball.png");
	//UVSprite * sprite = UVSprite::create("frame.png");
	sprite->setPosition(Vec2(visibleSize / 2));
	//sprite->setPosition(Vec2(0, visibleSize.height / 2));
	//sprite->setScale(0.6f);
	addChild(sprite);
	//sprite->setUVTexture("3D/caustics.png");
	sprite->setUVTexture("snowflake2.png");
	//sprite->setUVTexture("number.jpg");
	sprite->setUvStretch(false);
	//sprite->setUvOpacity(0.5f); 
	sprite->setUvVelocity(Vec2(0.001f , -0.02f));
	sprite->setUvAlphaFilter(true);
		



	auto menu = Navigate::create();
	addChild(menu);

	const GLubyte* OpenGLVersion = glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
	//const GLubyte* gluVersion = gluGetString(GLU_VERSION); //返回当前GLU工具库版本

	return true;
}