#include "TableTest.h"
#include "../cocos/ui/shaders/UIShaders.h"
#include "Navigate.h"
USING_NS_CC;

Scene* TableTest::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = TableTest::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

TableTest::TableTest()
	:sprite(nullptr)
{}

bool TableTest::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();


	sprite = UVSprite::create("frame.png");
	//sprite->setPosition(Vec2(visibleSize / 2));
	sprite->setAnchorPoint(Vec2::ZERO);
	//sprite->setPosition(Vec2(0, visibleSize.height / 2));
	//sprite->setScale(0.6f);
	//addChild(sprite);
	//sprite->setUVTexture("3D/caustics.png");
	//sprite->setUVTexture("snowflake2.png");
	sprite->setUVTexture("number.jpg");
	sprite->setUvStretch(false);
	//sprite->setUvOpacity(0.5f); 
	sprite->setUvVelocity(Vec2(0.001f , -0.02f));
	sprite->setUvAlphaFilter(true);
		

	auto grid = NodeGrid::create();
	addChild(grid);
	grid->addChild(sprite);
	//grid->addChild(Sprite::create("ball.png"));
	
	auto action = Shaky3D::create(0.25, Size(15,10), 5, false);
	grid->runAction(RepeatForever::create(
		Sequence::create(
							action,
							//action->reverse(),
							//MoveBy::create(1, Vec3(0, 10, 0)),
							CallFunc::create([grid](){ grid->setGrid(nullptr); }),
							DelayTime::create(4),
							nullptr
						)
					));



	TableView* tableView = TableView::create(this, Size(sprite->getContentSize()));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setPosition(Vec2(30,0));
	tableView->setDelegate(this);
	this->addChild(tableView);
	tableView->reloadData();





	auto menu = Navigate::create();
	addChild(menu);



	Device::setAccelerometerEnabled(true);
	auto accListener = EventListenerAcceleration::create(CC_CALLBACK_2(TableTest::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(accListener, this);


	return true;
}
void TableTest::onAcceleration(Acceleration* acc, Event* event)
{
	static float prevX = 0, prevY = 0;

#define kFilterFactor 0.05f
	Vec2 v = sprite->getUvVelocity();
	sprite->setUvVelocity(Vec2(acc->x / -100, v.y));
	//float accelX = (float)acc->x * kFilterFactor + (1 - kFilterFactor)*prevX;
	//float accelY = (float)acc->y * kFilterFactor + (1 - kFilterFactor)*prevY;

	//prevX = accelX;
	//prevY = accelY;

	//auto v = Vec2(accelX, accelY);
	//v = v * 200;

	//if (_scene != nullptr)
	//{
	//	_scene->getPhysicsWorld()->setGravity(v);
	//}
}

void TableTest::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("cell touched at index: %ld", cell->getIdx());

	NodeGrid* grid = (NodeGrid*)cell->getChildByTag(10086);

	auto action = Shaky3D::create(3.25, Size(15, 15), 2, false);
	grid->runAction(action);
	//grid->runAction(RepeatForever::create(
	//	Sequence::create(
	//	action,
	//	//action->reverse(),
	//	//MoveBy::create(1, Vec3(0, 10, 0)),
	//	CallFunc::create([grid](){ grid->setGrid(nullptr); }),
	//	DelayTime::create(0.25),
	//	nullptr
	//	)
	//	));
}

Size TableTest::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(500, 60);
}

TableViewCell* TableTest::tableCellAtIndex(TableView *table, ssize_t idx)
{
	auto string = String::createWithFormat(" this table cell's location is at %ld", idx);
	TableViewCell *cell = table->dequeueCell();
	if (!cell) {
		cell = new (std::nothrow) TableViewCell();
		cell->autorelease();
		/*auto sprite = Sprite::create("rain.png");
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(Vec2(0, 0));
		cell->addChild(sprite);*/

		auto label = Label::createWithSystemFont(string->getCString(), "Helvetica", 20.0);
		label->setPosition(Vec2::ZERO);
		label->setAnchorPoint(Vec2::ZERO);
		label->setTag(123);
		//cell->addChild(label);

		auto grid = NodeGrid::create();
		grid->setTag(10086);
		cell->addChild(grid);
		grid->addChild(label);
	}
	else
	{
		auto label = (Label*)cell->getChildByTag(10086)->getChildByTag(123);
		label->setString(string->getCString());
	}


	return cell;
}

ssize_t TableTest::numberOfCellsInTableView(TableView *table)
{
	return 20;
}