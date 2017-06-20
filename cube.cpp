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






	CustomCommand *_customCommand = new CustomCommand();
	_customCommand->init(_globalZOrder, transform, flags);
	_customCommand->func = CC_CALLBACK_0(cube::drawCustom, this, transform, flags);
	renderer->addCommand(_customCommand);
	
}

void cube::drawCustom(const Mat4 &transform, uint32_t flags) {
	//	glBegin(GL_POLYGON);
	//
	//	glVertex3f(-20.5, -0.5, -20.5);
	//	glVertex3f(-20.5, 0.5, -20.5);
	//	glVertex3f(20.5, 0.5, -20.5);
	//	glVertex3f(20.5, -0.5, -20.5);
	//
	//	glEnd();
	//}
	auto mesh = this->getMesh();
	glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

}