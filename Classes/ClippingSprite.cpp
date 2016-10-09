#include "ClippingSprite.h"
USING_NS_CC;

ClippingSprite* ClippingSprite::create(const std::string& filename)
{
	ClippingSprite *sprite = new (std::nothrow) ClippingSprite();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
ClippingSprite::ClippingSprite()
	:_uvTexture(nullptr)
	, _uvVelocity(0.0f, 0.0f)
	, _uvPosition(0.0f, 0.0f)
	, _uvRepeat(false)
{}

ClippingSprite::~ClippingSprite()
{
	CC_SAFE_RELEASE(_uvTexture);
}

void ClippingSprite::__setUVTexture(Texture2D *texture)
{
	CC_SAFE_RETAIN(texture);
	CC_SAFE_RELEASE(_uvTexture);
	_uvTexture = texture;
}

void ClippingSprite::setUvRepeat(bool val)
{
	_uvRepeat = val;

	//GL_LINEAR ����Ŵ��ɫ��ʧ��
	//GL_REPEAT u/v����ʱ(0~1)repeat
	Texture2D::TexParams		tRepeatParams;
	tRepeatParams.magFilter = GL_LINEAR;
	tRepeatParams.minFilter = GL_LINEAR;
	tRepeatParams.wrapS = val ? GL_REPEAT : GL_CLAMP_TO_EDGE;
	tRepeatParams.wrapT = val ? GL_REPEAT : GL_CLAMP_TO_EDGE;
	_uvTexture->setTexParameters(tRepeatParams);
}
bool ClippingSprite::setContent(const std::string& filename)
{
	CCASSERT(filename.size()>0, "Invalid filename for sprite");
	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(filename);
	bool result = texture != nullptr;
	if (texture)
	{
		__setUVTexture(texture);

		//��¼��Sprite��uv����ĳߴ����
		_uvScale.x = getContentSize().width / texture->getContentSize().width;
		_uvScale.y = getContentSize().height / texture->getContentSize().height;

		//�����޸���shader�еĴ���
		//C++����Ҳ��Ҫ�޸ĺ����±���һ�β�����Ч
		/*
		auto program = new GLProgram();
		program->initWithFilenames("clippingSprite.vert", "clippingSprite.frag");
		program->link();
		//set uniform locations
		program->updateUniforms();
		this->setGLProgram(program); 
		*/
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_CLIPPING_SPRITE));
		auto program = getGLProgram();

		//��¼shader��uniform��loc
		_uvScaleLoc = glGetUniformLocation(program->getProgram(), "u_uvScale"); 
		_uvVelocityLoc = glGetUniformLocation(program->getProgram(), "u_vVelocity");
		_uvRepeatLoc = glGetUniformLocation(program->getProgram(), "u_iRepeat");
	}
	return result;
}
void ClippingSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if (_uvTexture)
	{
		//��дdraw���� ʹ��customCommand��Ⱦ����
		_customCommand.init(_globalZOrder);
		_customCommand.func = CC_CALLBACK_0(ClippingSprite::onDraw, this, transform, flags);
		renderer->addCommand(&_customCommand);
	}
	else
	{
		//do nothing
	}
}
void ClippingSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
	auto glProgramState = getGLProgramState();

	glProgramState->apply(transform);
	GL::blendFunc(_blendFunc.src, _blendFunc.dst);

	//�ֱ�ȡ���������uv�������Ĭ�ϵ�0,1����Ԫ��
	//����������Ԫ��shader�ж�ӦCC_Texture0/CC_Texture1
	GL::bindTexture2D(_texture->getName());
	GL::bindTexture2DN(1, _uvTexture->getName());
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

#define kQuadSize sizeof(_quad.bl)
	size_t offset = (size_t)&_quad;

	// ����vertex�����Ǳ任���Ķ�����Ϣ
	// ����texCoods��color��_quad�е���Ϣ
	// vertex
	int diff = offsetof(V3F_C4B_T2F, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// texCoods
	diff = offsetof(V3F_C4B_T2F, texCoords);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// color
	diff = offsetof(V3F_C4B_T2F, colors);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));

	// �������ǵ��Զ������
	// λ�úܹؼ� 
	// ��ǰֻ��ȡλ��loc
	// ��ֵ��Ҫ������
	// uvScale
	glUniform2f(_uvScaleLoc, _uvScale.x, _uvScale.y);
	glUniform1i(_uvRepeatLoc, (int)_uvRepeat);
	_uvPosition = _uvPosition + _uvVelocity;
	glUniform2f(_uvVelocityLoc, _uvPosition.x, _uvPosition.y);
	
	//����������
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 
	CHECK_GL_ERROR_DEBUG();
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
}