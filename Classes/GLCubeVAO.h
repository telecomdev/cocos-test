#ifndef __GLCube_VAO_H__
#define __GLCube_VAO_H__

#include "cocos2d.h"



class GLCubeVAO : public cocos2d::Layer
{
	typedef struct {
		float Position[3];
		float Color[4];
	} Vertex;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GLCubeVAO);

	GLCubeVAO();

	virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)override;
	void onDraw();

	void initVAO();
private:
	cocos2d::CustomCommand _command;

	GLuint vao;
	GLuint vertexVBO;
	GLuint colorVBO;
private:
	cocos2d::Label			*m_pTitle;
	cocos2d::Sprite				*m_pGround;
	cocos2d::Sprite				*m_pTest;
	cocos2d::MenuItemImage		*m_pButton;
};

#endif // __GLCube_VAO_H__
