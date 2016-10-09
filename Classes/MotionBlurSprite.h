#ifndef __MotionBlur_SPRITE_H__
#define __MotionBlur_SPRITE_H__

#include "cocos2d.h"
USING_NS_CC;

#define MOTION_FRAME_MAX 4

class MotionBlurSprite : public cocos2d::Sprite
{
public:
	static MotionBlurSprite* create(const std::string& filename);
	MotionBlurSprite();
	virtual ~MotionBlurSprite();
	//重写draw
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDraw(const Mat4 &transform, uint32_t flags);
	void setMotionBlur(bool bBlur, unsigned short frame = 4);

protected:
	CustomCommand _customCommand;
	//强度
	CC_SYNTHESIZE(float, _intensity, Intensity);
	//透明度
	CC_SYNTHESIZE(float, _uvOpacity, UvOpacity);
	GLuint _uvOpacityLoc;

	// motion blur
	cocos2d::Mat4 motions[MOTION_FRAME_MAX];
	int currentMotions;
	bool bMotionBlur;
	unsigned short uFrame;
};

#endif // __MotionBlur_SPRITE_H__
