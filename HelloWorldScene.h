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
	RotateFace = 2
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
	Vec2 _pretouchlocation;
	void onTouchsBegan(const std::vector<cocos2d::Touch*> &touchs, cocos2d::Event *event);
	void onTouchsEnded(const std::vector<cocos2d::Touch*> &touchs, cocos2d::Event *event);
	void onTouchsMoved(const std::vector<cocos2d::Touch*> &touchs, cocos2d::Event *event);
	void updateCameraTransform();
	void calculateArcBall(cocos2d::Vec3 & axis, float & angle, float p1x, float p1y, float p2x, float p2y);
	float projectToSphere(float r, float x, float y);
	void update(float dt);

protected:
	cocos2d::Layer* _layer3D;
	cocos2d::Layer* _layer3D0;
	LabelTTF* _winlabel;
	float _distanceZ = 80.0f;
	cocos2d::Camera* _camera;
	OperateCamType _operate = OperateCamType::RotateCamera;
	DrawNode3D* _drawGrid = nullptr;
private:
	cocos2d::Camera* camera;
	cocos2d::Vec3 _center = Vec3(0, 0, 0);
	
	cocos2d::Sprite3D*  _dn[27];
	cocos2d::Sprite3D*  _dn2[6];
	cocos2d::Sprite3D*  _dn0[8];
	cocos2d::Sprite3D* _dn_front[4];
	cocos2d::Sprite3D* _dn_back[4];
	cocos2d::Sprite3D* _dn_right[4];
	cocos2d::Sprite3D* _dn_left[4];
	cocos2d::Sprite3D* _dn_top[4];
	cocos2d::Sprite3D* _dn_bottom[4];

	int dimensions = 3;
	float cubesize = 8.0f;
	double spacing = 0.4;
	float _radius = 1.0f;

	cocos2d::Node* testface;
	void addcube(Sprite3D* cube, float x, float y, float z);

	Sprite3D* activecube;
	Sprite3D* activeface;
	Vec2 activelocation;
	Vec3 clickvector;

	void setActiveGroup(int axis, Sprite3D* cube, int direction, bool group);
	std::vector<Sprite3D*> tmpcubes;

	Label* facelabel;
	Label* alabel;
	Label* blabel;
	Label* xlabel;
	Label* ylabel;
	Label* zlabel;
	Label* wlabel;
	void shuffle();
	struct queue {
		int axis;
		Sprite3D* cube;
		int dir;
	};
	Label* frontlb = nullptr;
	Label* backlb = nullptr;
	Label* toplb = nullptr;
	Label* bottomlb = nullptr;
	Label* rightlb = nullptr;
	Label* leftlb = nullptr;

	void switchface(string face) {
		char faceorder[6] = { 'F', 'M', 'T', 'B', 'R', 'L' };
		auto rq = _rotationQuat;
		if (!face.compare("front")) {
			faceorder[0] = 'F';
			faceorder[1] = 'B';
			faceorder[2] = 'T';
			faceorder[3] = 'M';
			faceorder[4] = 'R';
			faceorder[5] = 'L';
			if (abs(rq.z) >= 0.5 && abs(rq.z) <= 1) {
				if (rq.z > 0) {
					faceorder[2] = 'R';
					faceorder[3] = 'L';
				}
				else {
					faceorder[2] = 'L';
					faceorder[3] = 'R';
				}
				faceorder[4] = 'T';
				faceorder[5] = 'M';
			}
		}
		if (!face.compare("back")) {
			faceorder[0] = 'B';
			faceorder[1] = 'F';
			faceorder[2] = 'T';
			faceorder[3] = 'M';
			faceorder[4] = 'L';
			faceorder[5] = 'R';
			if (abs(rq.z) >= 0.5 && abs(rq.z) <= 1) {
				if (rq.z > 0) {
					faceorder[2] = 'L';
					faceorder[3] = 'R';
				}
				else {
					faceorder[2] = 'R';
					faceorder[3] = 'L';
				}
				faceorder[4] = 'T';
				faceorder[5] = 'M';
			}
		}
		if (!face.compare("top")) {
			faceorder[0] = 'M';
			faceorder[1] = 'T';
			faceorder[2] = 'F';
			faceorder[3] = 'B';
			faceorder[4] = 'R';
			faceorder[5] = 'L';
			if (abs(rq.x) >= 0.5 && abs(rq.x) <= 1) {
				if (rq.x > 0) {
					faceorder[0] = 'R';
					faceorder[1] = 'L';
				}
				else {
					faceorder[0] = 'L';
					faceorder[1] = 'R';
				}
				faceorder[4] = 'M';
				faceorder[5] = 'T';
			}
		}
		if (!face.compare("bottom")) {
			faceorder[0] = 'T';
			faceorder[1] = 'M';
			faceorder[2] = 'B';
			faceorder[3] = 'F';
			faceorder[4] = 'R';
			faceorder[5] = 'L';
			if (abs(rq.x) >= 0.5 && abs(rq.x) <= 1) {
				faceorder[0] = 'R';
				faceorder[1] = 'L';
				faceorder[4] = 'T';
				faceorder[5] = 'M';
			}
		}
		if (!face.compare("right")) {
			faceorder[0] = 'L';
			faceorder[1] = 'R';
			faceorder[2] = 'T';
			faceorder[3] = 'M';
			faceorder[4] = 'F';
			faceorder[5] = 'B';
			if (abs(rq.x) >= 0.5 && abs(rq.x) <= 1) {
				faceorder[0] = 'T';
				faceorder[1] = 'M';
				faceorder[2] = 'L';
				faceorder[3] = 'R';
			}
		}
		if (!face.compare("left")) {
			faceorder[0] = 'R';
			faceorder[1] = 'L';
			faceorder[2] = 'T';
			faceorder[3] = 'M';
			faceorder[4] = 'B';
			faceorder[5] = 'F';
			if (abs(rq.x) >= 0.5 && abs(rq.x) <= 1) {
				faceorder[0] = 'T';
				faceorder[1] = 'M';
				faceorder[2] = 'R';
				faceorder[3] = 'L';
			}
		}

		frontlb->setString(ToString(faceorder[0]));
		backlb->setString(ToString(faceorder[1]));
		toplb->setString(ToString(faceorder[2]));
		bottomlb->setString(ToString(faceorder[3]));
		rightlb->setString(ToString(faceorder[4]));
		leftlb->setString(ToString(faceorder[5]));

	}

	bool isSolve();
	void queuerun(float dt);
	std::vector<queue> queues;
	int running = 0;
	int getRandFaceAxis(int face) {
		auto randaxis = 0;
		auto r = RandomHelper::random_int(0, 1);
		if (face == 0) {
			randaxis = AXISZ;
		}
		if (face == 1) {
			randaxis = AXISZ;
		}
		if (face == 2) {
			randaxis = AXISY;
		}
		if (face == 3) {
			randaxis = AXISY;
		}
		if (face == 4) {
			randaxis = AXISX;
		}		
		if (face == 5) {
			randaxis = AXISX;
		}
		return randaxis;
	}

	void cubemove(std::string face, int dir, bool group);
	void switch_face(char face, int dir, bool group);



	void cubepos_init(float dt) {
		//Vec3 axes;
		//float angle;
		//calculateArcBall2(axes, angle, 0, 0, (-0.70710678118654752440), -0.70710678118654752440, 0);

		//Quaternion quat(axes, angle * 4);                                              //get rotation quaternion
		//_rotationQuat = quat * _rotationQuat;
		//updateCameraTransform();



	}

	
	Vec2 _prelocation = Vec2(0,0);
	Vec2 _location = Vec2(0, 0);
	bool _startmove = 0;
	float _desty;
	float _destx;
	int _dir;
	char _axis;
	void calculateArcBall2(cocos2d::Vec3 & axis, float & angle, float p1x, float p1y, float p2x, float p2y, int ax);
	void movementup(int dir) {
		_prelocation = Vec2(0, 0);
		_location = Vec2(0, 0);
		_desty = 0.70710678118654752440;
		_destx = 0;
		_dir = 1;
		_startmove = 1;
		_axis = 2;
	}
	void movementdown(int dir) {
		_prelocation = Vec2(0, 0);
		_location = Vec2(0, 0);
		_desty = 0.70710678118654752440;
		_destx = 0;
		_dir = -1;
		_startmove = 1;
		_axis = 2;
	}
	void movementright(int dir) {
		_prelocation = Vec2(0, 0);
		_location = Vec2(0, 0);
		_destx = 0.70710678118654752440;
		_desty = 0;
		_dir = -1;
		_startmove = 1;
		_axis = 1;

	}
	void movementleft(int dir) {
		auto camera_quat = _rotationQuat;
		//Quaternion(0, 0.70710678118654752440, 0, 0.70710678118654752440)
		_prelocation = Vec2(0, 0);
		_location = Vec2(0, 0);
		_destx = camera_quat.x;
		if (camera_quat.x > 0) {
			_destx = -camera_quat.x;
		}
		_desty = camera_quat.y;
		if (camera_quat.y > 0) {
			_desty = -camera_quat.y;
		}
	_dir = 1;
		_startmove = 1;
//		_axis = 1;

	}

	Quaternion _destquat;
	float _quatcount = 0;
	void snapto() {
		_quatcount = 0;
		_startmove = 1;

		Ray ray;
		Size vs = Director::getInstance()->getVisibleSize();
		Vec3 snear(vs.width/2, vs.height/2, -1.0f);
		Vec3 sfar(vs.width/2, vs.height/2, 1.0f);

		auto csize = Director::getInstance()->getWinSize();

		_camera->unproject(csize, &snear, &snear);
		_camera->unproject(csize, &sfar, &sfar);

		ray._origin = snear;
		ray._direction = sfar - snear;

		_layer3D->setCameraMask(2);
		float facedist = 0;
		std::string touchface = "front";
		Sprite3D* targetface = nullptr;

		for (auto da : _dn2) {
			if (ray.intersects(da->getAABB())) {

				auto ndist = da->getPosition3D().distance(snear);
				if (!facedist) {
					targetface = da;
					facedist = ndist;
				}
				else if (ndist < facedist) {
					targetface = da;
					facedist = ndist;
				}
			}
		}
		if (targetface) {
			touchface = targetface->getName();
		}


		if (!touchface.compare("front")) {
			_destquat = Quaternion(0, 0, 0, 1);
		}
		if (!touchface.compare("right")) {
			_destquat = Quaternion(0, 0.70710678118654752440, 0, 0.70710678118654752440);
		}
		if (!touchface.compare("left")) {
			_destquat = Quaternion(0, -0.70710678118654752440, 0, 0.70710678118654752440);
		}
		if (!touchface.compare("top")) {
			_destquat = Quaternion(-0.70710678118654752440, 0, 0, 0.70710678118654752440);
		}
		if (!touchface.compare("bottom")) {
			_destquat = Quaternion(0.70710678118654752440, 0, 0, 0.70710678118654752440);
		}
		if (!touchface.compare("back")) {
			_destquat = Quaternion(0, -1, 0, 0);
		}
	}

};



#endif // __HELLOWORLD_SCENE_H__
