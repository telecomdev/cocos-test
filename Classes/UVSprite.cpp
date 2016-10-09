#include "UVSprite.h"
USING_NS_CC;

UVSprite* UVSprite::create(const std::string& filename)
{
	UVSprite *sprite = new (std::nothrow) UVSprite();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
UVSprite::UVSprite()
	:_uvTexture(nullptr)
	,_uvOpacity(1.0f)
	,_uvStretch(false)
	,_uvAlphaFilter(false)
	,_uvVelocity(0.0f, 0.0f)
	,_uvPosition(0.0f, 0.0f)
{}

UVSprite::~UVSprite()
{
	CC_SAFE_RELEASE(_uvTexture);
}

void UVSprite::__setUVTexture(Texture2D *texture)
{
	CC_SAFE_RETAIN(texture);
	CC_SAFE_RELEASE(_uvTexture);
	_uvTexture = texture;

	//GL_LINEAR ����Ŵ��ɫ��ʧ��
	//GL_REPEAT u/v����ʱ(0~1)repeat
	Texture2D::TexParams		tRepeatParams;
	tRepeatParams.magFilter = GL_LINEAR;
	tRepeatParams.minFilter = GL_LINEAR;
	tRepeatParams.wrapS = GL_REPEAT;
	tRepeatParams.wrapT = GL_REPEAT;
	_uvTexture->setTexParameters(tRepeatParams);
}


bool UVSprite::setUVTexture(const std::string& filename)
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
		//auto program = new GLProgram();
		//program->initWithFilenames("uvSprite.vert", "uvSprite.frag");
		//program->link();
		////set uniform locations
		//program->updateUniforms();
		//this->setGLProgram(program); 
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_UV_SPRITE));
		auto program = getGLProgram();

		//��¼shader��uniform��loc
		_uvScaleLoc = glGetUniformLocation(program->getProgram(), "u_uvScale"); 
		_uvOpacityLoc = glGetUniformLocation(program->getProgram(), "u_vOpacity");
		_uvVelocityLoc = glGetUniformLocation(program->getProgram(), "u_vVelocity");
		_uvAlphaFilterLoc = glGetUniformLocation(program->getProgram(), "u_vAlphaFilter");
	}
	return result;
}
void UVSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if (_uvTexture)
	{
		//��дdraw���� ʹ��customCommand��Ⱦ����
		_customCommand.init(_globalZOrder);
		_customCommand.func = CC_CALLBACK_0(UVSprite::onDraw, this, transform, flags);
		renderer->addCommand(&_customCommand);
	}
	else
	{
		Sprite::draw(renderer, transform, flags);
	}
}
void UVSprite::onDraw(const Mat4 &transform, uint32_t flags)
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
	glUniform2f(_uvScaleLoc, _uvStretch ? 1.0f : _uvScale.x, _uvStretch ? 1.0f : _uvScale.y);
	glUniform1f(_uvOpacityLoc, _uvOpacity);
	_uvPosition = _uvPosition + _uvVelocity;
	glUniform2f(_uvVelocityLoc, _uvPosition.x, _uvPosition.y);
	//ԭͼ͸���������ֲ���Ч
	//��ԭͼaֵȫ��Ϊ1 ��ȫ�����������
	glUniform1i(_uvAlphaFilterLoc, (int)_uvAlphaFilter);
	
	//����������
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 
	CHECK_GL_ERROR_DEBUG();
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
}





//void UVSprite::onDraw(const Mat4 &transform, uint32_t flags)
//{
//	//_tl = _quad.tl.vertices;
//	//_tr = _quad.tr.vertices;
//	//_bl = _quad.bl.vertices;
//	//_br = _quad.br.vertices;
//
//	//transform.transformPoint(&_quad.bl.vertices/*, bl*/);
//	//transform.transformPoint(&_quad.br.vertices/*, br*/);
//	//transform.transformPoint(&_quad.tl.vertices/*, tl*/);
//	//transform.transformPoint(&_quad.tr.vertices/*, tr*/);
//
//	/*V3F_C4B_T2F_Quad quad = _quad;
//	size_t offset2 = (size_t)&quad;
//	_verticesTransformed[0] = _quad.tl.vertices;
//	_verticesTransformed[1] = _quad.bl.vertices;
//	_verticesTransformed[2] = _quad.tr.vertices;
//	_verticesTransformed[3] = _quad.br.vertices;*/
//
//
//	// glVertexAttribPointer
//
//
//	//_quad.tl.vertices = _tl;
//	//_quad.tr.vertices = _tr;
//	//_quad.bl.vertices = _bl;
//	//_quad.br.vertices = _br;
//}
//void UVSprite::onDraw()
//{
//	//Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//	//Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//	//Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
//	//Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
//
//	auto glProgram = getGLProgram();
//
//	glProgram->use();
//
//	//set uniform values, the order of the line is very important
//	glProgram->setUniformsForBuiltins();
//
//	GL::bindTexture2D(textureId);
//	GL::bindTexture2DN(1, textureId1);
//
//	static float u = 0.0f;
//	static float v = 0.0f;
//	u += 0.001f;
//	//v += 0.001f;
//	if (u > 1) u = 0.0f;
//	if (v > 1) v = 0.0f;
//	float uv[] = { u, v };
//	glUniform2fv(uvCoordLoc, 1, uv);
//
//	//use vao����Ϊvao��¼��ÿһ���������Ժͻ�������״̬������ֻ��Ҫ�󶨾Ϳ���ʹ����
//	glBindVertexArray(vao);
//
//	//glDrawArrays(GL_TRIANGLES, 0, 6);
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
//
//
//	glBindVertexArray(0);
//
//	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
//	CHECK_GL_ERROR_DEBUG();
//
//
//	//Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
//	//Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//
//
//}