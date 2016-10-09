#include "MotionBlurSprite.h"
USING_NS_CC;

MotionBlurSprite* MotionBlurSprite::create(const std::string& filename)
{
	MotionBlurSprite *sprite = new (std::nothrow) MotionBlurSprite();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
MotionBlurSprite::MotionBlurSprite()
	:_uvOpacity(1.0f)
	, _intensity(1.0f)
	, currentMotions(0)
	, bMotionBlur(false)
	, uFrame(0)
{}

MotionBlurSprite::~MotionBlurSprite()
{
}
void MotionBlurSprite::setMotionBlur(bool bBlur, unsigned short frame)
{
	// Ŀǰ������,ֱ���л���shader���ᱣ�����֡
	if (bMotionBlur != bBlur)
	{
		bMotionBlur = bBlur;
		currentMotions = 0;
	}
	uFrame = frame;

	if (bMotionBlur)
	{
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_MOTION_BLUR));
		auto program = getGLProgram();
		_uvOpacityLoc = glGetUniformLocation(program->getProgram(), "u_vOpacity");
		setIntensity(1.5f);
	}
	else
	{
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
	}
}
void MotionBlurSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if (bMotionBlur)
	{
		//��дdraw���� ʹ��customCommand��Ⱦ����
		_customCommand.init(_globalZOrder);
		_customCommand.func = CC_CALLBACK_0(MotionBlurSprite::onDraw, this, transform, flags);
		renderer->addCommand(&_customCommand);
	}
	else
	{
		Sprite::draw(renderer, transform, flags);
	}
}
void MotionBlurSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
	//1.��Ⱦ˳����
	//2.�����ɫ͸���ȵ���

	auto glProgramState = getGLProgramState();
	glProgramState->apply(transform);
	GL::blendFunc(_blendFunc.src, _blendFunc.dst);

	GL::bindTexture2D(_texture->getName());
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

	

	Mat4 currentTransform;
	bool bLast = false;
	for (int i = currentMotions - 1; i >= 0 || bLast == false; i--)
	{
		bLast = i < 0;

		currentTransform = bLast ? transform : motions[i];

		/*if (!bLast)
		{
			GL::blendFunc(GL_SRC_ALPHA, GL_ONE);
		}
		else
		{
			GL::blendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		}*/

		//int count = (1 + currentMotions + 1) * (currentMotions + 1) / 2;
		//glUniform1f(_uvOpacityLoc, 1.0f / (float)count * (currentMotions - i));
		//ģ����Ȩ
		//float t = (currentMotions - i) / ((currentMotions + 1)*0.5);
		//glUniform1f(_uvOpacityLoc, (1.0f / (currentMotions + 1)) * 0.5 * t);


		float fOpacity = 1.0f / (currentMotions + 1);
		if (bMotionBlur)fOpacity = MIN(fOpacity * _intensity, 1.0f);
		glUniform1f(_uvOpacityLoc, fOpacity);
		//����������
		glProgramState->apply(currentTransform);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		CHECK_GL_ERROR_DEBUG();
		CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
	}

	// �����˶�ģ��֡
	if (bMotionBlur) currentMotions = MIN(currentMotions + 1, uFrame);
	else currentMotions = MAX(currentMotions - 1, 0);

	for (int i = currentMotions - 1; i >= 0; i--)
	{
		motions[i] = i == 0 ? transform : motions[i - 1];
	}
}
#if 0
void MotionBlurSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
	//1.��λ���,��ͬ���(��ɫֵ*0.x��Ȩ,��ǰ֡ԭʼ����)
	//2.ʹ�ø�˹ģ��(��ǰ֡������)
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
#endif

#if 0 
//1.��λ���,��ͬ���(��ɫֵ*0.x��Ȩ,��ǰ֡ԭʼ����)
//2.ʹ�ø�˹ģ��(��ǰ֡������)
auto glProgramState = getGLProgramState();
glProgramState->apply(transform);
GL::blendFunc(_blendFunc.src, _blendFunc.dst);

//�ֱ�ȡ���������uv�������Ĭ�ϵ�0,1����Ԫ��
//����������Ԫ��shader�ж�ӦCC_Texture0/CC_Texture1
GL::bindTexture2D(_texture->getName());
//GL::bindTexture2DN(1, _uvTexture->getName());
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
glUniform1f(_uvOpacityLoc, _uvOpacity);


//����������
glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

CHECK_GL_ERROR_DEBUG();
CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
#endif



//����˳����
//do
//{
//	//ģ����Ȩ
//	float t = currentMotions + 1 - blurTimes / ((currentMotions + 1)*0.5);
//	glUniform1f(_uvOpacityLoc, (1.0f / (currentMotions + 1)) * 0.5 * t);
//	//glUniform1f(_uvOpacityLoc, (1.0f / (currentMotions + 1)));
//	//����������
//	glProgramState->apply(currentTransform);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	CHECK_GL_ERROR_DEBUG();
//	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);

//	if (blurTimes < MOTION_FRAME_MAX)
//	{
//		currentTransform = motions[blurTimes];
//	}
//} while (blurTimes++ < currentMotions);



//if (bMotionBlur)
//{
//	for (int i = 0; i < currentMotions; i++)
//	{
//		glProgramState->apply(motions[i]);
//		//����������
//		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//		CHECK_GL_ERROR_DEBUG();
//		CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
//	}
//	// �����˶�ģ��֡
//	currentMotions = MIN(currentMotions + 1, MOTION_FRAME_MAX);
//	for (int i = currentMotions - 1; i >= 0; i--)
//	{
//		if (i == 0)
//		{
//			motions[i] = transform;
//		}
//		else
//		{
//			motions[i] = motions[i - 1];
//		}
//		CCLOG("MotionBlurSprite::onDraw , currentMotions = %d" , currentMotions);
//	}
//	CCLOG("MotionBlurSprite::onDraw end");
//}