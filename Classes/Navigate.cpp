#include "Navigate.h"
#include "UVSpriteTest.h"
#include "ClippingTest.h"
#include "TableTest.h"
USING_NS_CC;

Navigate::Navigate()
{}

static void callBack4(const char*c , ...)
{
	va_list list;
	va_start(list, c);
	int index = va_arg(list, int);
	CCLOG("%s%d" , c , index);

	va_end(list);
}

bool Navigate::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();

	MenuItemFont::setFontSize(60);
	auto menu = Menu::create(
		MenuItemFont::create("Menu1", CC_CALLBACK_1(Navigate::callBack1, this)),
		MenuItemFont::create("Menu2", CC_CALLBACK_1(Navigate::callBack2, this)),
		MenuItemFont::create("Menu3", CC_CALLBACK_1(Navigate::callBack3, this)),
		nullptr);
	menu->alignItemsVerticallyWithPadding(20);
	menu->setPosition(Vec2(150, visibleSize.height - 150));


	//this->addChild(menu);

	auto grid = NodeGrid::create();
	addChild(grid);
	grid->addChild(menu);

	//auto action = Shaky3D::create(0.25, Size(10, 10), 2, false);
	//grid->runAction(RepeatForever::create(
	//	Sequence::create(
	//	action,
	//	//action->reverse(),
	//	//MoveBy::create(1, Vec3(0, 10, 0)),
	//	CallFunc::create([grid](){ grid->setGrid(nullptr); }),
	//	DelayTime::create(2),
	//	nullptr
	//	)
	//));

	callBack4("test", 99);
	return true;
}

void Navigate::callBack1(Ref* sender)
{
	CCLOG("Navigate::callBack1");
	Director::getInstance()->replaceScene(ClippingTest::createScene());
}

void Navigate::callBack2(Ref* sender)
{
	CCLOG("Navigate::callBack2");
	Director::getInstance()->replaceScene(UVSpriteTest::createScene());
}

void Navigate::callBack3(Ref* sender)
{
	CCLOG("Navigate::callBack3");
	Director::getInstance()->replaceScene(TableTest::createScene());
}

