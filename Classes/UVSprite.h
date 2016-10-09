#ifndef __UVSprite_H__
#define __UVSprite_H__

#include "cocos2d.h"
USING_NS_CC;


class UVSprite : public cocos2d::Sprite
{
public:
	static UVSprite* create(const std::string& filename);
	UVSprite();
	virtual ~UVSprite();
	//��дdraw
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDraw(const Mat4 &transform, uint32_t flags);
	//���ò�����uv����(��������,����ͨSprite��Ϊһ��)
	bool setUVTexture(const std::string& filename);
protected:
	void __setUVTexture(cocos2d::Texture2D *texture);
	cocos2d::Texture2D*       _uvTexture;
	CustomCommand _customCommand;

	//uvTexture��Sprite��������Ŀ�߱���,uv�����Ƿ�����
	Vec2 _uvScale;
	GLuint _uvScaleLoc;
	CC_SYNTHESIZE(bool, _uvStretch, UvStretch);

	//uv�����Ƿ���ԭͼalphaֵӰ��
	CC_SYNTHESIZE(bool, _uvAlphaFilter, UvAlphaFilter);
	GLuint _uvAlphaFilterLoc;

	//uv����͸����
	CC_SYNTHESIZE(float, _uvOpacity, UvOpacity);
	GLuint _uvOpacityLoc;

	//uv�����ƶ��ٶ�,����ƫ����
	CC_SYNTHESIZE(Vec2, _uvVelocity, UvVelocity);
	CC_SYNTHESIZE_READONLY(Vec2, _uvPosition, UvPosition);
	GLuint _uvVelocityLoc;
};

#endif // __UVSprite_H__
