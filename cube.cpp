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
	//this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));

	//GLProgram * p = new GLProgram();
	//p->initWithFilenames("cube.vsh", "cube.fsh");
	//auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(p);
	//this->setGLProgramState(glprogramstate);
	//p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	//p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	//p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	//p->link();
	//p->updateUniforms();
	//this->setShaderProgram(p);
	//this->setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
	return true;
}


void cube::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags) {
	
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(cube::drawCustom, this, transform, flags);
	Director::getInstance()->getRenderer()->addCommand(&_customCommand);


	Sprite3D::draw(renderer, transform, flags);

}

void cube::drawCustom(const Mat4 &transform, uint32_t flags) {

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	///* Depth buffer setup */
	glClearDepthf( 1.0f );
	/* Enables Depth Testing */
	glEnable( GL_DEPTH_TEST );
	/* The Type Of Depth Test To Do */
	glDepthFunc( GL_LEQUAL );
	//glDepthFunc(GL_LESS);
	//glEnable(GL_DEPTH_TEST);
	this->setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
	this->getGLProgram()->use();
	this->getGLProgram()->setUniformsForBuiltins(transform);
	GLfloat colour[] = {
		//1,0,0,  1,0,0,  1,0,0,  1,0,0,      // face #1 is red
		//0,1,0,  0,1,0,  0,1,0,  0,1,0,      // face #2 is green
		//0,0,1,  0,0,1,  0,0,1,  0,0,1,      // face #3 is blue
		//1,1,0,  1,1,0,  1,1,0,  1,1,0,      // face #4 is yellow
		//0,1,1,  0,1,1,  0,1,1,  0,1,1,      // face #5 is cyan
		//1,0,1,  1,0,1,  1,0,1,  1,0,1,

		1,0,0,1,0,0,1,0,0,1,0,0,
		1,0,0,1,0,0,1,0,0,1,0,0,
		1,0,0,1,0,0,1,0,0,1,0,0,
		1,0,0,1,0,0,1,0,0,1,0,0,
		1,0,0,1,0,0,1,0,0,1,0,0,
		1,0,0,1,0,0,1,0,0,1,0,0,


	};
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_COLOR | GL::VERTEX_ATTRIB_FLAG_POSITION);
// glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, transform.m);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 3, GL_FLOAT, GL_FALSE, 0, colour);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 24);

		 //glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
		 //glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices);

			//glEnableClientState(GL_VERTEX_ARRAY);
			//glEnableClientState(GL_COLOR_ARRAY);
			//glVertexPointer(3, GL_FLOAT, 0, vertices);
			//glColorPointer(3, GL_FLOAT, 0, colour);
			//glDrawArrays(GL_QUADS, 0, 24);
			//glDisableClientState(GL_VERTEX_ARRAY);
			//glDisableClientState(GL_COLOR_ARRAY);
	//auto a = 0;
	//auto maxextend = 5;
	//GLfloat vertices[] = {

		//auto ftl = Vec3(-maxextend, maxextend, maxextend);
		//auto ftr = Vec3(maxextend, maxextend, maxextend);
		//auto fbl = Vec3(-maxextend, -maxextend, maxextend);
		//auto fbr = Vec3(maxextend, -maxextend, maxextend);

		//auto btl = Vec3(-maxextend, maxextend, -maxextend);
		//auto btr = Vec3(maxextend, maxextend, -maxextend);
		//auto bbl = Vec3(-maxextend, -maxextend, -maxextend);
		//auto bbr = Vec3(maxextend, -maxextend, -maxextend);


	//	//front
	//	//ftl
	//	-maxextend, maxextend, maxextend,
	//	//ftr
	//	maxextend, maxextend, maxextend,
	//	//fbr
	//	maxextend, -maxextend, maxextend,
	//	//fbl
	//	-maxextend, -maxextend, maxextend,



	//	// right
	//	//ftr
	//	maxextend, maxextend, maxextend,
	//	//btr
	//	maxextend, maxextend, -maxextend,
	//	//bbr
	//	maxextend, -maxextend, -maxextend,
	//	//fbr
	//	maxextend, -maxextend, maxextend,


	//	//top
	//	//ftl
	//	-maxextend, maxextend, maxextend,
	//	//ftr
	//	maxextend, maxextend, maxextend,
	//	//btr
	//	maxextend, maxextend, -maxextend,
	//	//btl
	//	-maxextend, maxextend, -maxextend,


	//	//back
	//	//btl
	//	-maxextend, maxextend, -maxextend,
	//	//btr
	//	maxextend, maxextend, -maxextend,
	//	//bbr
	//	maxextend, -maxextend, -maxextend,
	//	//bbl
	//	-maxextend, -maxextend, -maxextend,

	//	// left
	//	//ftl
	//	-maxextend, maxextend, maxextend,
	//	//fbl
	//	-maxextend, -maxextend, maxextend,
	//	//bbl
	//	-maxextend, -maxextend, -maxextend,
	//	//btl
	//	-maxextend, maxextend, -maxextend,


	//	//btm
	//	//fbr
	//	maxextend, -maxextend, maxextend,
	//	//fbl
	//	-maxextend, -maxextend, maxextend,
	//	//bbl
	//	-maxextend, -maxextend, -maxextend,
	//	//bbr
	//	maxextend, -maxextend, -maxextend,
	//};
}
