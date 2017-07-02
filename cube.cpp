#include "cube.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace cocos2d::ui;

cube* cube::create(const std::string &path)
{
	if (path.length() < 4)
		CCASSERT(false, "improper name specified when creating Sprite3D");

	auto sprite = new (std::nothrow) cube();
	if (sprite && sprite->initWithFile(path))
	{
		sprite->init();
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool cube::init() {
	
	return true;
}


void cube::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags) {
	Sprite3D::draw(renderer, transform, flags);
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(cube::drawCustom, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void cube::drawCustom(const Mat4 &transform, uint32_t flags) {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	this->setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
	auto glprogram = getGLProgram();
	glprogram->use();
	glprogram->setUniformsForBuiltins(transform);
	auto mesh = this->getMesh();
	CC_NODE_DRAW_SETUP();
	


	GLfloat colour[] = {
		1,0,0,
		1,0,0,
		1,0,0,
		1,0,0,

		0,1,0,
		0,1,0,
		0,1,0,
		0,1,0,

		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,

		1,1,0,
		1,1,0,
		1,1,0,
		1,1,0,

		1,1,1,
		1,1,1,
		1,1,1,
		1,1,1,

		1,0.5,0,
		1,0.5,0,
		1,0.5,0,
		1,0.5,0,
	};
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 3, GL_FLOAT, GL_FALSE, 0, colour);

	glDrawElements((GLenum)mesh->getPrimitiveType(), (GLsizei)mesh->getIndexCount(), (GLenum)mesh->getIndexFormat(), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, mesh->getIndexCount());
}
