#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "DrawNode3D.h"
#include "cube.h"
#include <iostream>
using namespace std;

template<typename T>
string ToString(T t) {
	stringstream s;
	s << t;
	return s.str();
}

using namespace cocos2d;

enum class OperateCamType
{
	MoveCamera = 0,
	RotateCamera = 1,
};

enum {
	AXISX = 0,
	AXISY = 1,
	AXISZ = 2
};



class HelloWorld : public cocos2d::Layer
{
public:

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();


    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	void start();
	void onTouchsMoved(const std::vector<cocos2d::Touch*> &touchs, cocos2d::Event *event);
	void updateCameraTransform();
	void calculateArcBall(cocos2d::Vec3 & axis, float & angle, float p1x, float p1y, float p2x, float p2y);
	float projectToSphere(float r, float x, float y);
	void update(float dt);

protected:
	
	cocos2d::Layer* _layer3D;
	float _distanceZ = 50.0f;
	cocos2d::Camera* _camera;
	OperateCamType _operate = OperateCamType::RotateCamera;
	Sprite3D* _sprite3D1 = nullptr;
	Sprite3D* _sprite3D2 = nullptr;
	DrawNode3D* _drawGrid = nullptr;
private:
	cocos2d::Camera* camera;
	cocos2d::Vec3 _center = Vec3(0, 0, 0);
	Color3B randcolor[6] = { Color3B::RED, Color3B::BLUE, Color3B::WHITE, Color3B::GREEN, Color3B::YELLOW, Color3B::ORANGE };
	
	cocos2d::Sprite3D*  _dn2[6];
	//cocos2d::Sprite3D*  _dn3[6][4];
	
	cocos2d::Sprite3D* _dn_front[4];
	cocos2d::Sprite3D* _dn_back[4];
	cocos2d::Sprite3D* _dn_right[4];
	cocos2d::Sprite3D* _dn_left[4];
	cocos2d::Sprite3D* _dn_top[4];
	cocos2d::Sprite3D* _dn_bottom[4];

	cocos2d::Sprite3D*  _dn[27];
	int dimensions = 3;
	cocos2d::Node* testface;
	int iturn = 0;
	void addcube(Sprite3D* cube, float x, float y, float z);
	void addface();
	Sprite3D* activecube;
	Sprite3D* activeface;
	Vec2 activelocation;
	float cubesize = 5.0f;
	float spacing = 1.0f;
	float _radius = 1.0f;
	DrawNode3D* faces[6];
	DrawNode3D* debugray;
	Vec3 convertToWorldSpace3D(Node* container, Vec3 position);
	Vec3 convertToNodeSpace3D(Node* container, Vec3 position);
	Mat4 getNodeToWorldSpaceMatrix(Node* node);

	void setActiveGroup(int axis, Sprite3D* cube, int direction);
	std::vector<Sprite3D*> tmpcubes;
	Vec3* v4[4] = {};
	std::map<int, char> m = { { 1, 'a' },{ 3, 'b' },{ 5, 'c' },{ 7, 'd' } };
	std::map<char, std::map<char, char>> transition = {
		{'x' , {{'y' , 'z'}, {'z' , 'y'}}},
		{'y' , {{'x' , 'z'}, {'z' , 'x'}}},
		{'z' , {{'x' , 'y'}, {'y' , 'x'}}}
	};
	Vec3 clickvector;
	char calcrq2(Quaternion pos) {
	
		pos.normalize();
	//	pos.createFromAxisAngle(_camera->getPosition3D(), 0, &dst);
		return calcrq(pos);
	/*	if ((abs(pos.x) <= 0.5) &&
			(abs(pos.y) <= 0.5) &&
			(pos.z > 0.5)) {
			return 'A';
		}
		if ((abs(pos.x) <= 0.5) &&
			(abs(pos.y) <= 0.5) &&
			(pos.z < -0.5)) {
			return 'B';
		}
		if ((pos.x < -0.5) &&
			(pos.y > 0.5) &&
			(abs(pos.z) <= 0.5)) {
			return 'C';
		}
		if ((pos.x > 0.5) &&
			(pos.y > 0.5) &&
			(abs(pos.z) <= 0.5)) {
			return 'D';
		}
		if ((pos.x > 0.5) &&
			(pos.y < -0.5) &&
			(abs(pos.z) <= 0.5)) {
			return 'E';
		}
		if ((pos.x < -0.5) &&
			(pos.y < -0.5) &&
			(abs(pos.z) <= 0.5)) {
			return 'F';
		}*/
	}
	char calcrq(Quaternion pos) {
		if ((abs(pos.x) <= 0.5) &&
			(abs(pos.y) <= 0.5) &&
			(abs(pos.z) <= 0.5)) {
			return 'A';
		}
		if ((abs(pos.x) <= 0.5) &&
			(abs(pos.y) > 0.5) &&
			(abs(pos.z) <= 0.5)) {
			return 'B';
		}
		if ((abs(pos.x) <= 0.5) &&
			(abs(pos.y) <= 0.5) &&
			(abs(pos.z) > 0.5)) {
			return 'C';
		}
		if ((abs(pos.x) > 0.5) &&
			(abs(pos.y) <= 0.5) &&
			(abs(pos.z) <= 0.5)) {
			return 'D';
		}
		return '-';
	}
	Label* xlabel;
	Label* ylabel;
	Label* zlabel;
	Label* wlabel;

	//int principalComponent(int v) {
	//	var maxAxis = 'x',
	//		max = Math.abs(v.x);
	//	if (Math.abs(v.y) > max) {
	//		maxAxis = 'y';
	//		max = Math.abs(v.y);
	//	}
	//	if (Math.abs(v.z) > max) {
	//		maxAxis = 'z';
	//		max = Math.abs(v.z);
	//	}
	//	return maxAxis;
	//}


};

#endif // __HELLOWORLD_SCENE_H__
