#ifndef __NAVIGATE_H__
#define __NAVIGATE_H__

#include "cocos2d.h"


class Navigate : public cocos2d::Layer
{
public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Navigate);

	Navigate();

private:

	void callBack1(cocos2d::Ref* sender);
	void callBack2(cocos2d::Ref* sender);
	void callBack3(cocos2d::Ref* sender);
};

#endif // __NAVIGATE_H__
