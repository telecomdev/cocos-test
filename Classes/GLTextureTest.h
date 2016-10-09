#ifndef __GLTexture_TEXT_H__
#define __GLTexture_TEXT_H__

#include "cocos2d.h"



class GLTextureTest : public cocos2d::Layer
{
	typedef struct {
		float Position[2];
		float Color[4];
		float TexCoord[2];
	} Vertex;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GLTextureTest);

	GLTextureTest();

	virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)override;
	void onDraw();

	void initVAO();
	//void initMVP();
private:
	cocos2d::CustomCommand _command;


	cocos2d::Mat4 _modelViewMV;

	cocos2d::GLProgram *mShaderProgram;
	// attribute locations 
	GLint _positionLoc;
	GLint _texCoordLoc;

	// sampler locations 
	GLuint _baseMapLoc;
	GLuint _lightMapLoc;
	// Texture handle 
	GLuint _baseMapTexId;
	GLuint _lightMapTexId;

	GLuint VAO;
	GLuint vertexBuffer;
	GLuint indexBuffer;


	GLuint vao;
	GLuint vertexVBO;
	GLuint colorVBO;
	GLuint textureId;
	GLuint textureId1;

	GLuint uvCoordLoc;
private:
	cocos2d::Label			*m_pTitle;
	cocos2d::Sprite				*m_pGround;
	cocos2d::Sprite				*m_pTest;
	cocos2d::MenuItemImage		*m_pButton;
};

#endif // __GLTexture_TEXT_H__
