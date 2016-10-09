#include "MotionBlurTest.h"
#include "../cocos/ui/shaders/UIShaders.h"
#include "Navigate.h"

#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace cocostudio;
Scene* MotionBlurTest::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MotionBlurTest::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

MotionBlurTest::MotionBlurTest()
	:sprite(nullptr)
	, armature(nullptr)
{
	for (int i = 0; i < SPRITE_MAX; i++)
	{
		sprites[i] = nullptr;
	}
}

bool MotionBlurTest::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto background = Sprite::create("HelloWorld.png");
	background->setColor(Color3B(255, 0, 0));
	background->setPosition(Vec2(visibleSize / 2));
	background->setScale(2);
	addChild(background);

	ArmatureDataManager::getInstance()->addArmatureFileInfo("Cowboy.ExportJson");
	armature = Armature::create("Cowboy"); 
	armature->getAnimation()->playWithIndex(0);
	armature->setScale(0.2f);

	armature->setPosition(Vec2(visibleSize / 2));
	addChild(armature);


	for (int i = 0; i < SPRITE_MAX; i++)
	{
		sprites[i] = MotionBlurSprite::create("man.png");

		sprites[i]->setPosition(Vec2(visibleSize * (i+1) / SPRITE_MAX));
		addChild(sprites[i]);
	}


	MenuItemFont::setFontSize(60);
	auto menu = Menu::create(
		MenuItemFont::create("MotionBlur", CC_CALLBACK_1(MotionBlurTest::callBackMotionBlur, this)),
		MenuItemFont::create("Reset", [=](cocos2d::Ref*){
			bool bBlur = armature->getMotionBlur();
			armature->setMotionBlur(!bBlur);

		}),
		nullptr);
	menu->alignItemsVerticallyWithPadding(20);
	menu->setPosition(Vec2(150, 150));
	addChild(menu);


	return true;
}
void MotionBlurTest::callBackMotionBlur(Ref* sender)
{
	CCLOG("MotionBlurTest::callBack1");
	//Director::getInstance()->getScheduler()->setTimeScale(0.2);
	static float fIntensity = 1.5f;
	static bool bBlur = false;
	//for (int i = 0; i < SPRITE_MAX; i++)
	//{
	//	sprites[i]->runAction(Sequence::create(
	//		MoveBy::create(1.0, Vec2(200, 200)),
	//		MoveBy::create(0.8, Vec2(0, -400)),
	//		MoveBy::create(0.6, Vec2(-400, 400)),
	//		MoveBy::create(0.4, Vec2(0, -400)),
	//		MoveBy::create(0.2, Vec2(200, 200)),
	//		CallFunc::create([this , i](){
	//			this->sprites[i]->setMotionBlur(false);
	//		}),
	//		nullptr
	//		));
	//	sprites[i]->setMotionBlur(i % 2 == 0);
	//	//sprites[i]->setIntensity(fIntensity);
	//}
	//CCLOG("MotionBlurTest::callBack1 %f", fIntensity);
	//fIntensity += 0.2f;

	armature->stopAllActions();
	bBlur = !bBlur;
	armature->setMotionBlur(bBlur);
	armature->runAction(Sequence::create(
		MoveBy::create(1.0, Vec2(300, 200)),
		MoveBy::create(0.8, Vec2(0, -200)),
		MoveBy::create(0.5, Vec2(-600, 0)),
		MoveBy::create(0.4, Vec2(0, -200)),
		MoveBy::create(0.2, Vec2(300, 200)),
		CallFunc::create([this](){
		armature->setMotionBlur(false);
	}),
		nullptr
		));
}