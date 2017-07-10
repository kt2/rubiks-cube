#ifndef __cube_H__
#define __cube_H__

#include "cocos2d.h"

using namespace cocos2d;
class cube : public Sprite3D
{
public:
	static cube* create(const std::string& path, int tag);
	virtual bool init() override;
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void drawCustom(const Mat4 &transform, uint32_t flags);
	void setori(float x, float y, float z) {
		orix = x;
		oriy = y;
		oriz = z;
	}
	bool inpos() {
		auto pos = this->getPosition3D();
		return pos.x - orix < 0.1 &&
			pos.y - oriy < 0.1 &&
			pos.z - oriz < 0.1;
	}
private:
	int _tag = 0;
	Sprite3D* _sprite;
	GLProgram* glProgram;
	GLuint vao;

	GLuint positionVBO;

	GLuint colorVBO;

	GLuint positionLocation;

	GLuint colorLocation;

	float orix = 0;
	float oriy = 0;
	float oriz = 0;

	CustomCommand _customCommand;

	//std::map<char, std::map<char, char>> transition2 = {
	//	{ 'x' ,{ { 'y' , 'z' },{ 'z' , 'y' }, {'x', 'x' } }},
	//	{ 'y' ,{ { 'x' , 'z' },{ 'z' , 'x' }, {'y', 'y' } }},
	//	{ 'z' ,{ { 'x' , 'y' },{ 'y' , 'x' }, {'z', 'z' } }}
	//};
};
#endif // __cube_H__