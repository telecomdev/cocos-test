#ifndef __ClippingSprite_H__
#define __ClippingSprite_H__

#include "cocos2d.h"
USING_NS_CC;


class ClippingSprite : public cocos2d::Sprite
{
public:
	static ClippingSprite* create(const std::string& filename);
	ClippingSprite();
	virtual ~ClippingSprite();
	//重写draw
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDraw(const Mat4 &transform, uint32_t flags);
	//设置显示元素
	bool setContent(const std::string& filename);

	void setUvRepeat(bool val);
protected:
	void __setUVTexture(cocos2d::Texture2D *texture);

	cocos2d::Texture2D*       _uvTexture;
	CustomCommand _customCommand;

	//uvTexture与Sprite自身纹理的宽高比例,uv纹理是否拉伸
	Vec2 _uvScale;
	GLuint _uvScaleLoc;
	//uv纹理移动速度,坐标偏移量
	CC_SYNTHESIZE(Vec2, _uvVelocity, UvVelocity);
	CC_SYNTHESIZE_READONLY(Vec2, _uvPosition, UvPosition);
	GLuint _uvVelocityLoc;

	//repeat标志
	GLuint _uvRepeatLoc;
	CC_SYNTHESIZE_READONLY(bool, _uvRepeat, UvRepeat);
};

#endif // __ClippingSprite_H__
