#ifndef __MotionBlur_TEST_H__
#define __MotionBlur_TEST_H__

#include "cocos2d.h"
#include "MotionBlurSprite.h"
#include "cocostudio/CocoStudio.h"
#define SPRITE_MAX 10
using namespace cocostudio;
class MotionBlurTest : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MotionBlurTest);

	MotionBlurTest();
	void callBackMotionBlur(Ref* sender);

	Armature *armature;
	MotionBlurSprite * sprite;
	MotionBlurSprite * sprites[SPRITE_MAX];
};

#endif // __MotionBlur_TEST_H__
