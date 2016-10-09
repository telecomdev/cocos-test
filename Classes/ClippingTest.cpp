#include "ClippingTest.h"
#include "ClippingSprite.h"
#include "Navigate.h"
USING_NS_CC;

Scene* ClippingTest::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ClippingTest::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

ClippingTest::ClippingTest()
{}

bool ClippingTest::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();


	auto background = Sprite::create("test.jpg");
	background->setPosition(Vec2(visibleSize / 2));
	addChild(background);


	ClippingSprite * sprite = ClippingSprite::create("ball.png");
	sprite->setPosition(Vec2(visibleSize / 2));
	addChild(sprite);        
	//sprite->setContent("HelloWorld.png");   
	sprite->setContent("rain.png");  
	sprite->setUvRepeat(true);
	sprite->setUvVelocity(Vec2(0.0f, -0.01f));


	auto menu = Navigate::create();
	addChild(menu);

	return true;
}