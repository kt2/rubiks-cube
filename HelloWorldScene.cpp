#include "HelloWorldScene.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "DrawNode3D.h"
#include "cube.h"

USING_NS_CC;
using namespace cocos2d::ui;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }	
	start();


	return true;
}

void HelloWorld::updateCameraTransform()
{
	Mat4 trans, rot, center;
	Mat4::createTranslation(Vec3(0, -10.0f, _distanceZ), &trans);
	Mat4::createRotation(_rotationQuat, &rot);
	Mat4::createTranslation(_center, &center);
	Mat4 result = center * rot * trans;
	_camera->setNodeToParentTransform(result);
	auto ss = _rotationQuat;

	xlabel->setString("x: " + ToString(ss.x));
	ylabel->setString("y: " + ToString(ss.y));
	zlabel->setString("z: " + ToString(ss.z));
	wlabel->setString("w: " + ToString(ss.w));
}

float HelloWorld::projectToSphere(float r, float x, float y)
{
	float d, t, z;
	d = sqrt(x*x + y*y);
	if (d < r * 0.70710678118654752440)//inside sphere
	{
		z = sqrt(r*r - d*d);
	}
	else                               //on hyperbola
	{
		t = r / 1.41421356237309504880;
		z = t*t / d;
	}
	return z;
}


void HelloWorld::calculateArcBall2(cocos2d::Vec3 & axis, float & angle, float p1x, float p1y, float p2x, float p2y, int ax)
{

	Mat4 rotation_matrix;
	Mat4::createRotation(_rotationQuat, &rotation_matrix);

	Vec3 uv = rotation_matrix * Vec3(0.0f, 1.0f, 0.0f); //rotation y
	//if (ax == 1) {
	//	uv = rotation_matrix * Vec3(0.0f, 0.0f, 0.0f);
	//}
	Vec3 sv = rotation_matrix * Vec3(1.0f, 0.0f, 0.0f); //rotation x
	//if (ax == 2) {
	//	sv = rotation_matrix * Vec3(0.0f, 0.0f, 0.0f);
	//}
	Vec3 lv = rotation_matrix * Vec3(0.0f, 0.0f, -1.0f);//rotation z

	Vec3 p1 = sv * p1x + uv * p1y - lv * projectToSphere(_radius, p1x, p1y); //start point screen transform to 3d
	Vec3 p2 = sv * p2x + uv * p2y - lv * projectToSphere(_radius, p2x, p2y); //end point screen transform to 3d


	Vec3::cross(p2, p1, &axis);  //calculate rotation axis

	axis.normalize();

	float t = (p2 - p1).length() / (2.0 * _radius);
	//clamp -1 to 1
	if (t > 1.0) t = 1.0;
	if (t < -1.0) t = -1.0;
	angle = asin(t);           //rotation angle
}


void HelloWorld::calculateArcBall(cocos2d::Vec3 & axis, float & angle, float p1x, float p1y, float p2x, float p2y)
{
	Mat4 rotation_matrix;
	Mat4::createRotation(_rotationQuat, &rotation_matrix);

	Vec3 uv = rotation_matrix * Vec3(0.0f, 1.0f, 0.0f); //rotation y
	Vec3 sv = rotation_matrix * Vec3(1.0f, 0.0f, 0.0f); //rotation x
	Vec3 lv = rotation_matrix * Vec3(0.0f, 0.0f, -1.0f);//rotation z

	Vec3 p1 = sv * p1x + uv * p1y - lv * projectToSphere(_radius, p1x, p1y); //start point screen transform to 3d
	Vec3 p2 = sv * p2x + uv * p2y - lv * projectToSphere(_radius, p2x, p2y); //end point screen transform to 3d

	Vec3::cross(p2, p1, &axis);  //calculate rotation axis
	axis.normalize();

	float t = (p2 - p1).length() / (2.0 * _radius);
	//clamp -1 to 1
	if (t > 1.0) t = 1.0;
	if (t < -1.0) t = -1.0;
	angle = asin(t);           //rotation angle
}


void HelloWorld::onTouchsBegan(const std::vector<Touch*> &touchs, Event *event) {
	if (!touchs.empty())
	{
		_pretouchlocation = touchs[0]->getLocationInView();

	}
}
void HelloWorld::onTouchsEnded(const std::vector<Touch*> &touchs, Event *event) {

	if (!touchs.empty())
	{
		if (_operate == OperateCamType::RotateFace) {

			Vec2 location = touchs[0]->getLocationInView();
			auto xdiff = location.x - _pretouchlocation.x;
			auto ydiff = location.y - _pretouchlocation.y;
			if (!_startmove) {
				snapto();
	
				//Quaternion(0, 0.70710678118654752440, 0, 0.70710678118654752440)
				//if (xdiff > 0 && abs(xdiff) > abs(ydiff)) {
				//	movementright(1);
				//}
				//else if (xdiff < 0 && abs(xdiff) > abs(ydiff)) {
				//	movementleft(1);
				//}
				//else if (ydiff > 0 && abs(ydiff) > abs(xdiff)) {
				//	movementdown(1);
				//}
				//else if (ydiff < 0 && abs(ydiff) > abs(xdiff)) {
				//	movementup(1);
				//}
			}
		}
	}
}

void HelloWorld::onTouchsMoved(const std::vector<Touch*> &touchs, Event *event)
{
	if (!touchs.empty())
	{
		if (_operate == OperateCamType::RotateCamera)           //arc ball rotate
		{
			Size visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 prelocation = touchs[0]->getPreviousLocationInView();
			Vec2 location = touchs[0]->getLocationInView();
			location.x = 2.0 * (location.x) / (visibleSize.width) - 1.0f;
			location.y = 2.0 * (visibleSize.height - location.y) / (visibleSize.height) - 1.0f;
			prelocation.x = 2.0 * (prelocation.x) / (visibleSize.width) - 1.0f;
			prelocation.y = 2.0 * (visibleSize.height - prelocation.y) / (visibleSize.height) - 1.0f;

			Vec3 axes;
			float angle;
			calculateArcBall(axes, angle, prelocation.x, prelocation.y, location.x, location.y);    //calculate  rotation quaternion parameters
			Quaternion quat(axes, angle * 4);                                              //get rotation quaternion
			_rotationQuat = quat * _rotationQuat;
			updateCameraTransform();                                                                //update camera Transform
		}
		else if (_operate == OperateCamType::RotateFace)           //arc ball rotate
		{
			
		}
		else if (_operate == OperateCamType::MoveCamera)         //camera zoom 
		{
			Point newPos = touchs[0]->getPreviousLocation() - touchs[0]->getLocation();
			_distanceZ -= newPos.y*0.1f;

			updateCameraTransform();
		}
	}
}


void HelloWorld::start() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	facelabel = Label::create();
	facelabel->setString("N/A");
	facelabel->setColor(Color3B::WHITE);
	facelabel->setPosition(Vec2(40, 240));



	alabel = Label::create();
	alabel->setString("N/A");
	alabel->setColor(Color3B::WHITE);
	alabel->setPosition(Vec2(40, 80));

	blabel = Label::create();
	blabel->setString("N/A");
	blabel->setColor(Color3B::WHITE);
	blabel->setPosition(Vec2(40, 60));

	xlabel = Label::create();
	xlabel->setString("N/A");
	xlabel->setColor(Color3B::WHITE);
	xlabel->setPosition(Vec2(40, 100));

	ylabel = Label::create();
	ylabel->setString("N/A");
	ylabel->setColor(Color3B::WHITE);
	ylabel->setPosition(Vec2(40, 140));

	zlabel = Label::create();
	zlabel->setString("N/A");
	zlabel->setColor(Color3B::WHITE);
	zlabel->setPosition(Vec2(40, 180));

	wlabel = Label::create();
	wlabel->setString("N/A");
	wlabel->setColor(Color3B::WHITE);
	wlabel->setPosition(Vec2(40, 220));

	addChild(alabel);
	addChild(blabel);	
	addChild(xlabel);
	addChild(ylabel);
	addChild(zlabel);
	addChild(wlabel);
	facelabel->setVisible(0);
	xlabel->setVisible(0);
	ylabel->setVisible(0);
	zlabel->setVisible(0);
	wlabel->setVisible(0);
	alabel->setVisible(0);
	addChild(facelabel);


	_winlabel = LabelTTF::create("YOU WIN!", "Helvetica", 65,
		Size(0,0), TextHAlignment::CENTER);
	_winlabel->setVisible(false);
	_winlabel->setColor(Color3B::WHITE);
	_winlabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_winlabel);

	//auto btn0 = Button::create();
	//btn0->setTitleText("SWITCH");
	//btn0->setContentSize(Size(100, 20));
	//btn0->setTitleFontSize(20);
	//btn0->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
	//	if (type != Widget::TouchEventType::ENDED) { return; }
	//	Button *btn = (Button*)pSender;
	//	if (_operate == OperateCamType::RotateFace) {
	//		_operate = OperateCamType::RotateCamera;
	//	}
	//	else if (_operate == OperateCamType::RotateCamera) {
	//		_operate = OperateCamType::MoveCamera;
	//	}
	//	else if (_operate == OperateCamType::MoveCamera) {
	//		_operate = OperateCamType::RotateFace;
	//	}
	//});
	//btn0->setPosition(Vec2(200, 400));
	//addChild(btn0);

	//auto btn3 = Button::create();
	//btn3->setTitleText("HIDE LABEL");
	//btn3->setContentSize(Size(100, 20));
	//btn3->setTitleFontSize(20);
	//btn3->setPosition(Vec2(100, 120));
	//addChild(btn3);
	//btn3->addTouchEventListener([=](Ref* pSender,
	//	Widget::TouchEventType type) {
	//	if (type != Widget::TouchEventType::ENDED) { return; }
	//	frontlb->setVisible(false);
	//	backlb->setVisible(false);
	//	toplb->setVisible(false);
	//	bottomlb->setVisible(false);
	//	rightlb->setVisible(false);
	//	leftlb->setVisible(false);
	//	_drawGrid->setVisible(false);

	//});

/*	auto btn4 = Button::create();
	btn4->setTitleText("SHOW LABEL");
	btn4->setContentSize(Size(100, 20));
	btn4->setTitleFontSize(20);
	btn4->setPosition(Vec2(100, 140));
	addChild(btn4);
	btn4->addTouchEventListener([=](Ref* pSender,
		Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		frontlb->setVisible(true);
		backlb->setVisible(true);
		toplb->setVisible(true);
		bottomlb->setVisible(true);
		rightlb->setVisible(true);
		leftlb->setVisible(true);
		_drawGrid->setVisible(true);

	});			*/								 


	auto shufflebtn = Button::create("gn.png");
	shufflebtn->setTitleText("SHUFFLE");
	shufflebtn->setScale9Enabled(true);
	shufflebtn->setContentSize(Size(100, 30));
	shufflebtn->setTitleFontSize(20);
	shufflebtn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		Button *btn = (Button*)pSender;
		_winlabel->setVisible(false);
		for (int i = 0; i < 20; i++) {
			shuffle();
		}
	});
	shufflebtn->setPosition(Vec2(80, visibleSize.height - 35));
	addChild(shufflebtn);


	auto btnup = Button::create("gn.png");
	btnup->setTitleText("UP");
	btnup->setScale9Enabled(true);
	btnup->setContentSize(Size(70, 30));
	btnup->setTitleFontSize(20);
	btnup->setPosition(Vec2(275, 30));
	addChild(btnup);
	btnup->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		switch_face('R', -1, true);
	});

	auto btndown = Button::create("gn.png");
	btndown->setTitleText("DOWN");
	btndown->setScale9Enabled(true);
	btndown->setContentSize(Size(70, 30));
	btndown->setTitleFontSize(20);
	btndown->setPosition(Vec2(275, 65));
	addChild(btndown);
	btndown->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		switch_face('R', 1, true);
	});


	auto btnleft = Button::create("gn.png");
	btnleft->setTitleText("LEFT");
	btnleft->setScale9Enabled(true);
	btnleft->setContentSize(Size(70, 30));
	btnleft->setTitleFontSize(20);
	btnleft->setPosition(Vec2(275, 100));
	addChild(btnleft);
	btnleft->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		switch_face('F', 1, true);
	});

	auto btnright = Button::create("gn.png");
	btnright->setTitleText("RIGHT");
	btnright->setScale9Enabled(true);
	btnright->setContentSize(Size(70, 30));
	btnright->setTitleFontSize(20);
	btnright->setPosition(Vec2(275, 135));
	addChild(btnright);
	btnright->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		switch_face('F', -1, true);
	});


	auto btnsnap = Button::create("gn.png");
	btnsnap->setTitleText("SNAP TO FRONT FACE");
	btnsnap->setScale9Enabled(true);
	btnsnap->setContentSize(Size(220, 30));
	btnsnap->setTitleFontSize(20);
	btnsnap->setPosition(Vec2(200, 170));
	addChild(btnsnap);
	btnsnap->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }

		snapto();
	});

	auto move1 = Button::create("gn.png");
	move1->setTitleText("MOVE 1");
	move1->setScale9Enabled(true);
	move1->setContentSize(Size(100, 30));
	move1->setTitleFontSize(20);
	move1->setPosition(Vec2(80, 30));
	addChild(move1);
	move1->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		switch_face('R', 1, false);
	});

	auto move4 = Button::create("gn.png");
	move4->setTitleText("MOVE 4");
	move4->setContentSize(Size(100, 30));
	move4->setTitleFontSize(20);
	move4->setScale9Enabled(true);
	move4->setPosition(Vec2(80, 65));
	addChild(move4);							 
	move4->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		switch_face('R', -1, false);
	});


	auto move7 = Button::create("gn.png");
	move7->setTitleText("MOVE 7");
	move7->setContentSize(Size(100, 30));
	move7->setTitleFontSize(20);
	move7->setScale9Enabled(true);
	move7->setPosition(Vec2(80, 100));
	addChild(move7);
	move7->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		switch_face('L', 1, false);
	});


	auto move7r = Button::create("gn.png");
	move7r->setTitleText("MOVE 7'");
	move7r->setContentSize(Size(100, 30));
	move7r->setTitleFontSize(20);
	move7r->setScale9Enabled(true);
	move7r->setPosition(Vec2(80, 135));
	addChild(move7r);
	move7r->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		switch_face('L', -1, false);
	});


	auto move2 = Button::create("gn.png");
	move2->setTitleText("MOVE 2");
	move2->setContentSize(Size(100, 30));
	move2->setTitleFontSize(20);
	move2->setScale9Enabled(true);
	move2->setPosition(Vec2(185, 30));
	addChild(move2);
	move2->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		switch_face('T', 1, false);
	});


	auto move5 = Button::create("gn.png");
	move5->setTitleText("MOVE 5");
	move5->setScale9Enabled(true);
	move5->setContentSize(Size(100, 30));
	move5->setTitleFontSize(20);
	move5->setPosition(Vec2(185, 65));
	addChild(move5);
	move5->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		switch_face('T', -1, false);
	});

	auto move3 = Button::create("gn.png");
	move3->setTitleText("MOVE 3");
	move3->setScale9Enabled(true);
	move3->setContentSize(Size(100, 30));
	move3->setTitleFontSize(20);
	move3->setPosition(Vec2(185, 100));
	addChild(move3);
	move3->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		switch_face('F', 1, false);
	});

	auto move6 = Button::create("gn.png");
	move6->setTitleText("MOVE 6");
	move6->setContentSize(Size(100, 30));
	move6->setTitleFontSize(20);
	move6->setScale9Enabled(true);
	move6->setPosition(Vec2(185, 135));
	addChild(move6);
	move6->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) { return; }
		switch_face('F', -1, false);
	});



	//_rotationQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	schedule(schedule_selector(HelloWorld::update), 0.0f);
	auto s = Director::getInstance()->getWinSize();
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchsBegan, this);

	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchsMoved, this);
	
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchsEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto layer3D = Layer::create();

	addChild(layer3D, 0);
	_layer3D = layer3D;


	auto layer3D2 = Layer::create();
	addChild(layer3D2);

	if (_camera == nullptr) {
		_camera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
		_camera->setCameraFlag(CameraFlag::USER1);
		//_camera->setPosition3D(Vec3(0, 10, 0));
		//_camera->lookAt(Vec3(0, 0, 0), Vec3(0, 1, 0));
		_camera->retain();
		_layer3D->addChild(_camera);
		layer3D2->addChild(_camera);
	}
	
	float increment = cubesize + spacing;
	float positionOffset = (dimensions - 1) / 2;
	int count = 0;
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			for (int k = 0; k < dimensions; k++) {
				float x = (i - positionOffset) * increment;
				float y = (j - positionOffset) * increment;
				float z = (k - positionOffset) * increment;
				//_dn[count] = cube::create("cube6.c3t", count);
				_dn[count] = cube::create("Sprite3DTest/box_VertexCol.c3t", count);
				

				//Texture2D* texture = new Texture2D();
				//Image* img = new Image();
				//img->initWithImageFile(overridepng);
				//texture->initWithImage(img);
				//_dn[count]->setTexture(texture);


				_dn[count]->setTag(count);
				addcube(_dn[count], x, y, z);
				
				_layer3D->addChild(_dn[count]);
				count++;
			}
		}
	}


	auto dirLight2 = DirectionLight::create(Vec3(255, 255, 255), Color3B(255, 255, 255));
	dirLight2->setCameraMask((unsigned short)CameraFlag::USER1);
	dirLight2->setPosition3D(Vec3(0, 40, 0));
	_layer3D->addChild(dirLight2);


	auto ambientLight = AmbientLight::create(Color3B(64, 64, 64));
	ambientLight->setCameraMask((unsigned short)CameraFlag::USER1);
	_layer3D->addChild(ambientLight);

	auto dirLight = DirectionLight::create(Vec3(255,255,255), Color3B(255, 255, 255));
	dirLight->setCameraMask((unsigned short)CameraFlag::USER1);
	dirLight->setPosition3D(Vec3(0, 50, 50));
	_layer3D->addChild(dirLight);


	auto maxextend = (cubesize * dimensions) / 2 + spacing * (dimensions - 1) / 2;
	
	//auto dn0_w = 100;
	//auto dn0_h = 0;
	//_dn0[0] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	//_dn0[0]->setName("rr");
	//_dn0[0]->setTag(AXISX);
	//_dn0[0]->setVisible(1);
	//_dn0[0]->setScaleY(0.1);
	//_dn0[0]->setScaleZ(dn0_h);
	//_dn0[0]->setScaleX(dn0_w);
	//_dn0[0]->setPosition3D(Vec3(dn0_w / 2, dn0_h / 2, 0));
	//_layer3D->addChild(_dn0[0]);

	//_dn0[1] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	//_dn0[1]->setName("rr");
	//_dn0[1]->setTag(AXISX);
	//_dn0[1]->setVisible(1);
	//_dn0[1]->setScaleY(0.1);
	//_dn0[1]->setScaleZ(dn0_h);
	//_dn0[1]->setScaleX(dn0_w);
	//_dn0[1]->setPosition3D(Vec3(-dn0_w / 2, dn0_h / 2, 0));
	//_layer3D->addChild(_dn0[1]);

	//_dn0[2] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	//_dn0[2]->setName("rr");
	//_dn0[2]->setTag(AXISX);
	//_dn0[2]->setVisible(1);
	//_dn0[2]->setScaleY(0.1);
	//_dn0[2]->setScaleZ(dn0_h);
	//_dn0[2]->setScaleX(dn0_w);
	//_dn0[2]->setPosition3D(Vec3(-dn0_w / 2, -dn0_h / 2, 0));
	//_layer3D->addChild(_dn0[2]);

	//_dn0[3] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	//_dn0[3]->setName("rr");
	//_dn0[3]->setTag(AXISX);
	//_dn0[3]->setVisible(1);
	//_dn0[3]->setScaleY(0.1);
	//_dn0[3]->setScaleZ(dn0_h);
	//_dn0[3]->setScaleX(dn0_w);
	//_dn0[3]->setPosition3D(Vec3(dn0_w / 2, -dn0_h / 2, 0));
	//_layer3D->addChild(_dn0[3]);



	//_dn0[0]->setColor(Color3B::RED);
	//_dn0[1]->setColor(Color3B::BLUE);
	//_dn0[2]->setColor(Color3B::YELLOW);
	//_dn0[3]->setColor(Color3B::ORANGE);
	
	
	_dn2[0] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn2[0]->setName("front");
	_dn2[0]->setTag(AXISX);
	_dn2[0]->setVisible(0);
	_dn2[0]->setScaleY(0.1);
	_dn2[0]->setScaleZ(maxextend * 2);
	_dn2[0]->setScaleX(maxextend * 2);
	_dn2[0]->setPosition3D(Vec3(0, 0, maxextend));
	_layer3D->addChild(_dn2[0]);

	// FRONT
	_dn_front[0] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_front[0]->setScaleY(0.1);
	_dn_front[0]->setScaleZ(maxextend * 2);
	_dn_front[0]->setScaleX(maxextend * 2);
	_dn_front[0]->setColor(Color3B::RED);
	_dn_front[0]->setVisible(0);
	_dn_front[0]->setTag(0);
	_dn_front[0]->setPosition3D(Vec3(0, 2 * maxextend, maxextend));
	_layer3D->addChild(_dn_front[0]);

	_dn_front[1] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_front[1]->setScaleY(0.1);
	_dn_front[1]->setScaleZ(maxextend * 2);
	_dn_front[1]->setScaleX(maxextend * 2);
	_dn_front[1]->setColor(Color3B::BLUE);
	_dn_front[1]->setVisible(0);
	_dn_front[1]->setTag(1);
	_dn_front[1]->setPosition3D(Vec3(0, -2 * maxextend, maxextend));
	_layer3D->addChild(_dn_front[1]);

	_dn_front[2] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_front[2]->setScaleY(0.1);
	_dn_front[2]->setScaleZ(maxextend * 2);
	_dn_front[2]->setScaleX(maxextend * 2);
	_dn_front[2]->setColor(Color3B::GREEN);
	_dn_front[2]->setVisible(0);
	_dn_front[2]->setTag(2);
	_dn_front[2]->setPosition3D(Vec3(2 * maxextend, 0, maxextend));
	_layer3D->addChild(_dn_front[2]);

	_dn_front[3] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_front[3]->setScaleY(0.1);
	_dn_front[3]->setScaleZ(maxextend * 2);
	_dn_front[3]->setScaleX(maxextend * 2);
	_dn_front[3]->setColor(Color3B::YELLOW);
	_dn_front[3]->setVisible(0);
	_dn_front[3]->setTag(3);
	_dn_front[3]->setPosition3D(Vec3(-2 * maxextend, 0, maxextend));
	_layer3D->addChild(_dn_front[3]);
	// FRONT END


	_dn2[1] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn2[1]->setName("back");
	_dn2[1]->setTag(AXISX);
	_dn2[1]->setVisible(0);
	_dn2[1]->setScaleY(0.1);
	_dn2[1]->setScaleZ(maxextend * 2);
	_dn2[1]->setScaleX(maxextend * 2);
	_dn2[1]->setPosition3D(Vec3(0, 0, -maxextend));
	_layer3D->addChild(_dn2[1]);

	// BACK
	_dn_back[0] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_back[0]->setScaleY(0.1);
	_dn_back[0]->setScaleZ(maxextend * 2);
	_dn_back[0]->setScaleX(maxextend * 2);
	_dn_back[0]->setColor(Color3B::RED);
	_dn_back[0]->setTag(0);
	_dn_back[0]->setVisible(0);
	_dn_back[0]->setPosition3D(Vec3(0, 2 * maxextend, -maxextend));
	_layer3D->addChild(_dn_back[0]);

	_dn_back[1] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_back[1]->setScaleY(0.1);
	_dn_back[1]->setScaleZ(maxextend * 2);
	_dn_back[1]->setScaleX(maxextend * 2);
	_dn_back[1]->setColor(Color3B::BLUE);
	_dn_back[1]->setTag(1);
	_dn_back[1]->setVisible(0);
	_dn_back[1]->setPosition3D(Vec3(0, -2 * maxextend, -maxextend));
	_layer3D->addChild(_dn_back[1]);

	_dn_back[2] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_back[2]->setScaleY(0.1);
	_dn_back[2]->setScaleZ(maxextend * 2);
	_dn_back[2]->setScaleX(maxextend * 2);
	_dn_back[2]->setColor(Color3B::GREEN);
	_dn_back[2]->setTag(2);
	_dn_back[2]->setVisible(0);
	_dn_back[2]->setPosition3D(Vec3(2 * maxextend, 0, -maxextend));
	_layer3D->addChild(_dn_back[2]);

	_dn_back[3] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_back[3]->setScaleY(0.1);
	_dn_back[3]->setScaleZ(maxextend * 2);
	_dn_back[3]->setScaleX(maxextend * 2);
	_dn_back[3]->setColor(Color3B::YELLOW);
	_dn_back[3]->setTag(3);
	_dn_back[3]->setVisible(0);
	_dn_back[3]->setPosition3D(Vec3(-2 * maxextend, 0, -maxextend));
	_layer3D->addChild(_dn_back[3]);
	// BACK END

	_dn2[2] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn2[2]->setName("top");
	_dn2[2]->setTag(AXISY);
	_dn2[2]->setVisible(0);
	_dn2[2]->setScaleY(maxextend * 2);
	_dn2[2]->setScaleZ(0.1);
	_dn2[2]->setScaleX(maxextend * 2);
	_dn2[2]->setPosition3D(Vec3(0, maxextend, 0));
	_layer3D->addChild(_dn2[2]);

	// TOP
	_dn_top[0] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_top[0]->setScaleY(maxextend * 2);
	_dn_top[0]->setScaleZ(0.1);
	_dn_top[0]->setScaleX(maxextend * 2);
	_dn_top[0]->setColor(Color3B::RED);
	_dn_top[0]->setTag(0);
	_dn_top[0]->setVisible(0);
	_dn_top[0]->setPosition3D(Vec3(0, maxextend, 2 * maxextend));
	_layer3D->addChild(_dn_top[0]);

	_dn_top[1] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_top[1]->setScaleY(maxextend * 2);
	_dn_top[1]->setScaleZ(0.1);
	_dn_top[1]->setScaleX(maxextend * 2);
	_dn_top[1]->setColor(Color3B::BLUE);
	_dn_top[1]->setTag(1);
	_dn_top[1]->setVisible(0);
	_dn_top[1]->setPosition3D(Vec3(0, maxextend, -2 * maxextend));
	_layer3D->addChild(_dn_top[1]);

	_dn_top[2] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_top[2]->setScaleY(maxextend * 2);
	_dn_top[2]->setScaleZ(0.1);
	_dn_top[2]->setScaleX(maxextend * 2);
	_dn_top[2]->setColor(Color3B::GREEN);
	_dn_top[2]->setTag(2);
	_dn_top[2]->setVisible(0);
	_dn_top[2]->setPosition3D(Vec3(2 * maxextend, maxextend, 0));
	_layer3D->addChild(_dn_top[2]);

	_dn_top[3] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_top[3]->setScaleY(maxextend * 2);
	_dn_top[3]->setScaleZ(0.1);
	_dn_top[3]->setScaleX(maxextend * 2);
	_dn_top[3]->setColor(Color3B::YELLOW);
	_dn_top[3]->setTag(3);
	_dn_top[3]->setVisible(0);
	_dn_top[3]->setPosition3D(Vec3(- 2 * maxextend, maxextend, 0));
	_layer3D->addChild(_dn_top[3]);
	// TOP END


	_dn2[3] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn2[3]->setName("bottom");
	_dn2[3]->setTag(AXISY);
	_dn2[3]->setVisible(0);

	_dn2[3]->setScaleY(maxextend * 2);
	_dn2[3]->setScaleZ(0.1);
	_dn2[3]->setScaleX(maxextend * 2);
	_dn2[3]->setPosition3D(Vec3(0, -maxextend, 0));
	_layer3D->addChild(_dn2[3]);

	// BOTTOM
	_dn_bottom[0] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_bottom[0]->setScaleY(maxextend * 2);
	_dn_bottom[0]->setScaleZ(0.1);
	_dn_bottom[0]->setScaleX(maxextend * 2);
	_dn_bottom[0]->setColor(Color3B::RED);
	_dn_bottom[0]->setTag(0);
	_dn_bottom[0]->setVisible(0);
	_dn_bottom[0]->setPosition3D(Vec3(0, -maxextend, 2 * maxextend));
	_layer3D->addChild(_dn_bottom[0]);

	_dn_bottom[1] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_bottom[1]->setScaleY(maxextend * 2);
	_dn_bottom[1]->setScaleZ(0.1);
	_dn_bottom[1]->setScaleX(maxextend * 2);
	_dn_bottom[1]->setColor(Color3B::BLUE);
	_dn_bottom[1]->setTag(1);
	_dn_bottom[1]->setVisible(0);
	_dn_bottom[1]->setPosition3D(Vec3(0, -maxextend, -2 * maxextend));
	_layer3D->addChild(_dn_bottom[1]);

	_dn_bottom[2] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_bottom[2]->setScaleY(maxextend * 2);
	_dn_bottom[2]->setScaleZ(0.1);
	_dn_bottom[2]->setScaleX(maxextend * 2);
	_dn_bottom[2]->setColor(Color3B::GREEN);
	_dn_bottom[2]->setTag(2);
	_dn_bottom[2]->setVisible(0);
	_dn_bottom[2]->setPosition3D(Vec3(2 * maxextend, -maxextend, 0));
	_layer3D->addChild(_dn_bottom[2]);

	_dn_bottom[3] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_bottom[3]->setScaleY(maxextend * 2);
	_dn_bottom[3]->setScaleZ(0.1);
	_dn_bottom[3]->setScaleX(maxextend * 2);
	_dn_bottom[3]->setColor(Color3B::YELLOW);
	_dn_bottom[3]->setTag(3);
	_dn_bottom[3]->setVisible(0);
	_dn_bottom[3]->setPosition3D(Vec3(-2 * maxextend, -maxextend, 0));
	_layer3D->addChild(_dn_bottom[3]);
	// BOTTOM END

	_dn2[4] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn2[4]->setName("right");
	_dn2[4]->setTag(AXISZ);
	_dn2[4]->setVisible(0);
	_dn2[4]->setScaleY(maxextend * 2);
	_dn2[4]->setScaleZ(maxextend * 2);
	_dn2[4]->setScaleX(0.1);
	_dn2[4]->setPosition3D(Vec3(maxextend, 0, 0));
	_layer3D->addChild(_dn2[4]);


	// RIGHT
	_dn_right[0] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_right[0]->setScaleY(maxextend * 2);
	_dn_right[0]->setScaleZ(maxextend * 2);
	_dn_right[0]->setScaleX(0.1);
	_dn_right[0]->setColor(Color3B::RED);
	_dn_right[0]->setTag(0);
	_dn_right[0]->setVisible(0);
	_dn_right[0]->setPosition3D(Vec3(maxextend, maxextend * 2, 0));
	_layer3D->addChild(_dn_right[0]);

	_dn_right[1] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_right[1]->setScaleY(maxextend * 2);
	_dn_right[1]->setScaleZ(maxextend * 2);
	_dn_right[1]->setScaleX(0.1);
	_dn_right[1]->setColor(Color3B::BLUE);
	_dn_right[1]->setTag(1);
	_dn_right[1]->setVisible(0);
	_dn_right[1]->setPosition3D(Vec3(maxextend, -maxextend * 2, 0));
	_layer3D->addChild(_dn_right[1]);

	_dn_right[2] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_right[2]->setScaleY(maxextend * 2);
	_dn_right[2]->setScaleZ(maxextend * 2);
	_dn_right[2]->setScaleX(0.1);
	_dn_right[2]->setColor(Color3B::GREEN);
	_dn_right[2]->setTag(2);
	_dn_right[2]->setVisible(0);
	_dn_right[2]->setPosition3D(Vec3(maxextend, 0, -maxextend * 2));
	_layer3D->addChild(_dn_right[2]);

	_dn_right[3] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_right[3]->setScaleY(maxextend * 2);
	_dn_right[3]->setScaleZ(maxextend * 2);
	_dn_right[3]->setScaleX(0.1);
	_dn_right[3]->setColor(Color3B::YELLOW);
	_dn_right[3]->setTag(3);
	_dn_right[3]->setVisible(0);
	_dn_right[3]->setPosition3D(Vec3(maxextend, 0, maxextend * 2));
	_layer3D->addChild(_dn_right[3]);
	// RIGHT END

	_dn2[5] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn2[5]->setName("left");
	_dn2[5]->setTag(AXISZ);
	_dn2[5]->setVisible(0);

	_dn2[5]->setScaleY(maxextend * 2);
	_dn2[5]->setScaleZ(maxextend * 2);
	_dn2[5]->setScaleX(0.1);
	_dn2[5]->setPosition3D(Vec3(-maxextend, 0, 0));
	_layer3D->addChild(_dn2[5]);

	// LEFT
	_dn_left[0] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_left[0]->setScaleY(maxextend * 2);
	_dn_left[0]->setScaleZ(maxextend * 2);
	_dn_left[0]->setScaleX(0.1);
	_dn_left[0]->setColor(Color3B::RED);
	_dn_left[0]->setTag(0);
	_dn_left[0]->setVisible(0);
	_dn_left[0]->setPosition3D(Vec3(-maxextend, maxextend * 2, 0));
	_layer3D->addChild(_dn_left[0]);

	_dn_left[1] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_left[1]->setScaleY(maxextend * 2);
	_dn_left[1]->setScaleZ(maxextend * 2);
	_dn_left[1]->setScaleX(0.1);
	_dn_left[1]->setColor(Color3B::BLUE);
	_dn_left[1]->setTag(1);
	_dn_left[1]->setVisible(0);
	_dn_left[1]->setPosition3D(Vec3(-maxextend, -maxextend * 2, 0));
	_layer3D->addChild(_dn_left[1]);

	_dn_left[2] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_left[2]->setScaleY(maxextend * 2);
	_dn_left[2]->setScaleZ(maxextend * 2);
	_dn_left[2]->setScaleX(0.1);
	_dn_left[2]->setColor(Color3B::GREEN);
	_dn_left[2]->setTag(2);
	_dn_left[2]->setVisible(0);
	_dn_left[2]->setPosition3D(Vec3(-maxextend, 0, -maxextend * 2));
	_layer3D->addChild(_dn_left[2]);

	_dn_left[3] = Sprite3D::create("Sprite3DTest/box_VertexCol.c3t");
	_dn_left[3]->setScaleY(maxextend * 2);
	_dn_left[3]->setScaleZ(maxextend * 2);
	_dn_left[3]->setScaleX(0.1);
	_dn_left[3]->setColor(Color3B::YELLOW);
	_dn_left[3]->setTag(3);
	_dn_left[3]->setVisible(0);
	_dn_left[3]->setPosition3D(Vec3(-maxextend, 0, maxextend * 2));
	_layer3D->addChild(_dn_left[3]);
	// LEFT END

	_drawGrid = DrawNode3D::create();

	//draw x
	for (int j = -20; j <= 20; j++)
	{
		_drawGrid->drawLine(Vec3(-100, 0, 5 * j), Vec3(100, 0, 5 * j), Color4F(1, 0, 0, 1));
	}
	//draw z
	for (int j = -20; j <= 20; j++)
	{
		_drawGrid->drawLine(Vec3(5 * j, 0, -100), Vec3(5 * j, 0, 100), Color4F(0, 0, 1, 1));
	}
	//draw y
	_drawGrid->drawLine(Vec3(0, 0, 0), Vec3(0, 50, 0), Color4F(0, 1, 0, 1));



	auto frontpos = _dn2[0]->getPosition3D();
	frontlb = Label::create();
	frontlb->setPosition3D(frontpos);
	frontlb->setString("F");

	auto backpos = _dn2[1]->getPosition3D();
	backlb = Label::create();
	backlb->setPosition3D(backpos);
	backlb->setString("B");

	auto toppos = _dn2[2]->getPosition3D();
	auto toprot = _dn2[2]->getRotation3D();
	toplb = Label::create();
	toplb->setPosition3D(toppos);
	toplb->setRotation3D(toprot);
	toplb->setString("T");

	auto bottompos = _dn2[3]->getPosition3D();
	auto bottomrot = _dn2[3]->getRotation3D();
	bottomlb = Label::create();
	bottomlb->setPosition3D(bottompos);
	bottomlb->setRotation3D(bottomrot);
	bottomlb->setString("M");

	auto rightpos = _dn2[4]->getPosition3D();
	rightlb = Label::create();
	rightlb->setPosition3D(rightpos);
	rightlb->setRotation3D(Vec3(0, 90, 0));
	rightlb->setString("R");

	auto leftpos = _dn2[5]->getPosition3D();
	leftlb = Label::create();
	leftlb->setPosition3D(leftpos);
	leftlb->setRotation3D(Vec3(0,-90,0));
	leftlb->setString("L");


	//layer3D2->addChild(frontlb);
	//layer3D2->addChild(backlb);
	//layer3D2->addChild(toplb);
	//layer3D2->addChild(bottomlb);
	//layer3D2->addChild(rightlb);
	//layer3D2->addChild(leftlb);
	//layer3D2->addChild(_drawGrid);


	//layer3D2->setCameraMask(2);


	this->scheduleOnce(schedule_selector(HelloWorld::cubepos_init), 0.001);

	_layer3D->setCameraMask(2);
	updateCameraTransform();

	


	//for (int i = 0; i < 2; i++) {
	//	shuffle();
	//}
	this->schedule(schedule_selector(HelloWorld::queuerun), 0.001);

	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->setSwallowTouches(true);

	listener1->onTouchBegan = [=](Touch* touch, Event* event) {
		auto ss = _rotationQuat;
		xlabel->setString(ToString(ss.x));
		ylabel->setString(ToString(ss.y));
		zlabel->setString(ToString(ss.z));
		wlabel->setString(ToString(ss.w));
		auto target = static_cast<Sprite3D*>(event->getCurrentTarget());
		auto location = touch->getLocationInView();
		
		Ray ray;
		Vec3 snear(location.x, location.y, -1.0f);
		Vec3 sfar(location.x, location.y, 1.0f);

		auto csize = Director::getInstance()->getWinSize();

		_camera->unproject(csize, &snear, &snear);
		_camera->unproject(csize, &sfar, &sfar);

		ray._origin = snear;
		ray._direction = sfar - snear;

		_layer3D->setCameraMask(2);
		float facedist = 0;
		float cubedist = 0;
		Sprite3D* targetface = nullptr;
		Sprite3D* targetcube = nullptr;

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
		for (auto a : _dn) {
			if (ray.intersects(a->getAABB())) {
				auto ndist = a->getPosition3D().distance(snear);
				if (!cubedist) {
					targetcube = a;
					cubedist = ndist;
				}
				else if (ndist < cubedist) {
					targetcube = a;
					cubedist = ndist;
				}
			}
		}

		if (targetface && targetcube) {
			activeface = targetface;
			activecube = targetcube;
			clickvector = targetcube->getPosition3D();
			activelocation = location;
			return true;
		}

		return false;
	};

	listener1->onTouchMoved = [=](Touch* touch, Event* event) {

		return true;
	};

	listener1->onTouchEnded = [=](Touch* touch, Event* event) {
		auto ss = _rotationQuat;
		xlabel->setString(ToString(ss.x));
		ylabel->setString(ToString(ss.y));
		zlabel->setString(ToString(ss.z));
		wlabel->setString(ToString(ss.w));

		auto location = touch->getLocationInView();

		auto newlocation = activecube->getPosition3D();
		auto xdiff = activelocation.x - location.x;
		auto ydiff = activelocation.y - location.y;
		
	
		auto s = _camera;
		auto clickface = activeface->getTag();

		int targetaxis;
		log("%f, %f, %f, %f", ss.x, ss.y, ss.z, ss.w);


		Ray ray;
		Vec3 snear(location.x, location.y, -1.0f);
		Vec3 sfar(location.x, location.y, 1.0f);

		auto csize = Director::getInstance()->getWinSize();

		_camera->unproject(csize, &snear, &snear);
		_camera->unproject(csize, &sfar, &sfar);

		ray._origin = snear;
		ray._direction = sfar - snear;
		auto touchface = activeface->getName();

		float face_dist = 0;

		Sprite3D* face_extend = nullptr;
		Sprite3D* cube_inside = nullptr;

		face_dist = 0;
		for (auto ia : _dn) {
			if (ray.intersects(ia->getAABB())) {
				auto ndist = ia->getPosition3D().distance(snear);
				if (!face_dist) {
					cube_inside = ia;
					face_dist = ndist;
				}
				else if (ndist < face_dist) {
					cube_inside = ia;
					face_dist = ndist;
				}
			}
		}
		if (cube_inside && (cube_inside->getTag() == activecube->getTag())) {
			cube_inside = nullptr;
		}


		int direction = 0;
		if (!touchface.compare("front")) {
			auto ccc = _dn_front;
			for (auto da : _dn_front) {
				if (ray.intersects(da->getAABB())) {

					auto ndist = da->getPosition3D().distance(snear);
					if (!face_dist) {
						face_extend = da;
						face_dist = ndist;
					}
					else if (ndist < face_dist) {
						face_extend = da;
						face_dist = ndist;
					}
				}
			}

			if (face_extend) {
				auto dir = face_extend->getTag();
				if (dir == 0) {
					targetaxis = AXISX;
					direction = -1;
				}
				if (dir == 1) {
					targetaxis = AXISX;
					direction = 1;
				}
				if (dir == 2) {
					targetaxis = AXISY;
					direction = 1;
				}
				if (dir == 3) {
					targetaxis = AXISY;
					direction = -1;
				}
				setActiveGroup(targetaxis, activecube, direction, false);
			}
			else {
				if (cube_inside) {
			
					if (cube_inside->getPosition3D().z == activecube->getPosition3D().z) {
						if (cube_inside->getPosition3D().y > activecube->getPosition3D().y) {
							targetaxis = AXISX;
							direction = -1;
						}
						else {
							targetaxis = AXISX;
							direction = 1;
						}
					}
					if (cube_inside->getPosition3D().y == activecube->getPosition3D().y) {
						if (cube_inside->getPosition3D().x > activecube->getPosition3D().x) {
							targetaxis = AXISY;
							direction = 1;
						}
						else {
							targetaxis = AXISY;
							direction = -1;
						}
					}
					if (direction) {
						setActiveGroup(targetaxis, activecube, direction, false);
					}
				}
			}
		}

		if (!touchface.compare("back")) {
			auto ccc = _dn_back;
			for (auto da : _dn_back) {
				if (ray.intersects(da->getAABB())) {

					auto ndist = da->getPosition3D().distance(snear);
					if (!face_dist) {
						face_extend = da;
						face_dist = ndist;
					}
					else if (ndist < face_dist) {
						face_extend = da;
						face_dist = ndist;
					}
				}
			}

			if (face_extend) {
				auto dir = face_extend->getTag();
				if (dir == 0) {
					targetaxis = AXISX;
					direction = 1;
				}
				if (dir == 1) {
					targetaxis = AXISX;
					direction = -1;
				}
				if (dir == 2) {
					targetaxis = AXISY;
					direction = -1;
				}
				if (dir == 3) {
					targetaxis = AXISY;
					direction = 1;
				}
				setActiveGroup(targetaxis, activecube, direction, false);
			}
			else {
				if (cube_inside) {
					if (cube_inside->getPosition3D().z == activecube->getPosition3D().z) {
						if (cube_inside->getPosition3D().y > activecube->getPosition3D().y) {
							targetaxis = AXISX;
							direction = 1;
						}
						else {
							targetaxis = AXISX;
							direction = -1;
						}
					}
					if (cube_inside->getPosition3D().y == activecube->getPosition3D().y) {
						if (cube_inside->getPosition3D().x > activecube->getPosition3D().x) {
							targetaxis = AXISY;
							direction = -1;
						}
						else {
							targetaxis = AXISY;
							direction = 1;
						}
					}
					if (direction) {
						setActiveGroup(targetaxis, activecube, direction, false);
					}
				}
			}
		}

		if (!touchface.compare("left")) {
			auto ccc = _dn_left;
			for (auto da : _dn_left) {
				if (ray.intersects(da->getAABB())) {

					auto ndist = da->getPosition3D().distance(snear);
					if (!face_dist) {
						face_extend = da;
						face_dist = ndist;
					}
					else if (ndist < face_dist) {
						face_extend = da;
						face_dist = ndist;
					}
				}
			}

			if (face_extend) {
				auto dir = face_extend->getTag();
				if (dir == 0) {
					targetaxis = AXISZ;
					direction = 1;
				}
				if (dir == 1) {
					targetaxis = AXISZ;
					direction = -1;
				}
				if (dir == 2) {
					targetaxis = AXISY;
					direction = -1;
				}
				if (dir == 3) {
					targetaxis = AXISY;
					direction = 1;
				}
				setActiveGroup(targetaxis, activecube, direction, false);
			}
			else {
				if (cube_inside) {
					if (cube_inside->getPosition3D().x == activecube->getPosition3D().x) {
						if (cube_inside->getPosition3D().y > activecube->getPosition3D().y) {
							targetaxis = AXISZ;
							direction = 1;
						}
						else {
							targetaxis = AXISZ;
							direction = -1;
						}
					}
					if (cube_inside->getPosition3D().y == activecube->getPosition3D().y) {
						if (cube_inside->getPosition3D().z < activecube->getPosition3D().z) {
							targetaxis = AXISY;
							direction = -1;
						}
						else {
							targetaxis = AXISY;
							direction = 1;
						}
					}
					if (direction) {
						setActiveGroup(targetaxis, activecube, direction, false);
					}
				}
			}
		}

		if (!touchface.compare("right")) {
			auto ccc = _dn_right;
			for (auto da : _dn_right) {
				if (ray.intersects(da->getAABB())) {

					auto ndist = da->getPosition3D().distance(snear);
					if (!face_dist) {
						face_extend = da;
						face_dist = ndist;
					}
					else if (ndist < face_dist) {
						face_extend = da;
						face_dist = ndist;
					}
				}
			}

			if (face_extend) {
				auto dir = face_extend->getTag();
				if (dir == 0) {
					targetaxis = AXISZ;
					direction = -1;
				}
				if (dir == 1) {
					targetaxis = AXISZ;
					direction = 1;
				}
				if (dir == 2) {
					targetaxis = AXISY;
					direction = 1;
				}
				if (dir == 3) {
					targetaxis = AXISY;
					direction = -1;
				}
				setActiveGroup(targetaxis, activecube, direction, false);
			}
			else {
				if (cube_inside) {
					if (cube_inside->getPosition3D().x == activecube->getPosition3D().x) {
						if (cube_inside->getPosition3D().y > activecube->getPosition3D().y) {
							targetaxis = AXISZ;
							direction = -1;
						}
						else {
							targetaxis = AXISZ;
							direction = 1;
						}
					}
					if (cube_inside->getPosition3D().y == activecube->getPosition3D().y) {
						if (cube_inside->getPosition3D().z < activecube->getPosition3D().z) {
							targetaxis = AXISY;
							direction = 1;
						}
						else {
							targetaxis = AXISY;
							direction = -1;
						}
					}
					if (direction) {
						setActiveGroup(targetaxis, activecube, direction, false);
					}
				}
			}
		}

		if (!touchface.compare("top")) {
			for (auto da : _dn_top) {
				if (ray.intersects(da->getAABB())) {

					auto ndist = da->getPosition3D().distance(snear);
					if (!face_dist) {
						face_extend = da;
						face_dist = ndist;
					}
					else if (ndist < face_dist) {
						face_extend = da;
						face_dist = ndist;
					}
				}
			}

			if (face_extend) {
				auto dir = face_extend->getTag();
				if (dir == 0) {
					targetaxis = AXISX;
					direction = 1;
				}
				if (dir == 1) {
					targetaxis = AXISX;
					direction = -1;
				}
				if (dir == 2) {
					targetaxis = AXISZ;
					direction = 1;
				}
				if (dir == 3) {
					targetaxis = AXISZ;
					direction = -1;
				}
				setActiveGroup(targetaxis, activecube, direction, false);
			}
			else {
				if (cube_inside) {
					if (cube_inside->getPosition3D().x == activecube->getPosition3D().x) {
						if (cube_inside->getPosition3D().z > activecube->getPosition3D().z) {
							targetaxis = AXISX;
							direction = 1;
						}
						else {
							targetaxis = AXISX;
							direction = -1;
						}
					}
					if (cube_inside->getPosition3D().z == activecube->getPosition3D().z) {
						if (cube_inside->getPosition3D().x > activecube->getPosition3D().x) {
							targetaxis = AXISZ;
							direction = 1;
						}
						else {
							targetaxis = AXISZ;
							direction = -1;
						}
					}
					if (direction) {
						setActiveGroup(targetaxis, activecube, direction, false);
					}
				}
			}
		}


		if (!touchface.compare("bottom")) {
			for (auto da : _dn_bottom) {
				if (ray.intersects(da->getAABB())) {

					auto ndist = da->getPosition3D().distance(snear);
					if (!face_dist) {
						face_extend = da;
						face_dist = ndist;
					}
					else if (ndist < face_dist) {
						face_extend = da;
						face_dist = ndist;
					}
				}
			}

			if (face_extend) {
				auto dir = face_extend->getTag();
				if (dir == 0) {
					targetaxis = AXISX;
					direction = -1;
				}
				if (dir == 1) {
					targetaxis = AXISX;
					direction = 1;
				}
				if (dir == 2) {
					targetaxis = AXISZ;
					direction = -1;
				}
				if (dir == 3) {
					targetaxis = AXISZ;
					direction = 1;
				}
				setActiveGroup(targetaxis, activecube, direction, false);
			}
			else {
				if (cube_inside) {
					if (cube_inside->getPosition3D().x == activecube->getPosition3D().x) {
						if (cube_inside->getPosition3D().z > activecube->getPosition3D().z) {
							targetaxis = AXISX;
							direction = -1;
						}
						else {
							targetaxis = AXISX;
							direction = 1;
						}
					}
					if (cube_inside->getPosition3D().z == activecube->getPosition3D().z) {
						if (cube_inside->getPosition3D().x > activecube->getPosition3D().x) {
							targetaxis = AXISZ;
							direction = -1;
						}
						else {
							targetaxis = AXISZ;
							direction = 1;
						}
					}
					if (direction) {
						setActiveGroup(targetaxis, activecube, direction, false);
					}
				}
			}
		}
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
}

void HelloWorld::queuerun(float dt) {
	if (!running) {
		if (!queues.empty()) {
			auto q = queues.back();
			setActiveGroup(q.axis, q.cube, q.dir, false);
			queues.pop_back();
		}
	}
}
void HelloWorld::shuffle() {
	queue q;
	
	auto face = RandomHelper::random_int(0, 5);
	//auto randaxis = getRandFaceAxis(face);
	//auto randcube = getfacecubes(face);
	auto randaxis = RandomHelper::random_int(0, 2);
	auto randcube = _dn[RandomHelper::random_int(0, 26)];
	auto randdirection = RandomHelper::random_int(0, 1) ? 1 : -1;
	
	q.axis = randaxis;
	q.cube = randcube;
	q.dir = randdirection;
	queues.push_back(q);
}


void HelloWorld::setActiveGroup(int axis, Sprite3D* sp, int direction, bool group) {
	if (running) {
		return;
	}
	running = 1;
	testface = Node::create();
	testface->setPosition3D(Vec3(0, 0, 0));
	testface->setAnchorPoint(Vec2(0, 0));
	_layer3D->addChild(testface);
	tmpcubes.clear();

	_layer3D->setCameraMask(2);
	auto ori_rotate = testface->getRotation3D();

	if (axis == AXISX) {
		auto samex = sp->getPosition3D().x;
		for (auto i : _dn) {
			if (!group) {
				if (round(i->getPosition3D().x) == round(samex)) {
					testface->addChild(i);
					tmpcubes.push_back(i);
				}
			}
			else {
				testface->addChild(i);
				tmpcubes.push_back(i);
			}
		}
		ori_rotate.x += 90 * direction;
	}
	if (axis == AXISY) {
		auto samey = sp->getPosition3D().y;
		for (auto i : _dn) {
			if (!group) {
				if (round(i->getPosition3D().y) == round(samey)) {
					testface->addChild(i);
					tmpcubes.push_back(i);

				}
			}
			else {
				testface->addChild(i);
				tmpcubes.push_back(i);
			}
		}
		ori_rotate.y += 90 * direction;
	}
	if (axis == AXISZ) {
		auto samez = sp->getPosition3D().z;
		
		for (auto i : _dn) {
			if (!group) {
				if (round(i->getPosition3D().z) == round(samez)) {
					testface->addChild(i);
					tmpcubes.push_back(i);
				}
			}
			else {
				testface->addChild(i);
				tmpcubes.push_back(i);
			}
		}
		ori_rotate.z += 90 * direction;
	}

	auto rot = RotateBy::create(0.2f, ori_rotate);
	auto seq = Sequence::create(rot, CallFunc::create([=] {
		testface->removeFromParentAndCleanup(true);
		_layer3D->removeChild(testface);
		
		for (auto me : tmpcubes) {
			//_layer3D->addChild(me);
			const float pi = (float)M_PI;
			auto cs = cos(direction * 90 * pi / 180.0);
			auto ss = sin(direction * 90 * pi / 180.0);
		
			auto x = me->getPositionX();
			auto y = me->getPositionY();
			auto z = me->getPositionZ();

			auto ab = me->getAABB();
			Vec3 testvec3[8];
			ab.getCorners(testvec3);
			auto testnode = DrawNode3D::create();
			testnode->drawCube(testvec3, Color4F::BLACK);
			//_layer3D->addChild(testnode);

			if (axis == AXISZ) {
				auto oriquat = me->getRotationQuat();
				me->setRotationQuat(Quaternion(0, 0, 0, 1));
				me->setRotation3D(Vec3(0, 0, direction * 90));
				auto newquat = me->getRotationQuat();
				me->setRotationQuat(newquat * oriquat);
				me->setPosition3D(Vec3(round((x * cs + y * ss) * 100) / 100, round((-x * ss + y * cs)*100) / 100, z));
			}
			else if (axis == AXISX) {
			
				auto oriquat = me->getRotationQuat();
				me->setRotationQuat(Quaternion(0, 0, 0, 1));
				me->setRotation3D(Vec3(direction * 90, 0, 0 ));
				auto newquat = me->getRotationQuat();
				me->setRotationQuat(newquat * oriquat);


				me->setPosition3D(Vec3(x, round((y * cs - z * ss) * 100) / 100, round((y * ss + z * cs) * 100) / 100));
			}
			else if (axis == AXISY) {
				auto oriquat = me->getRotationQuat();
				me->setRotationQuat(Quaternion(0, 0, 0, 1));
				me->setRotation3D(Vec3(0, direction * 90, 0));
				auto newquat = me->getRotationQuat();
				me->setRotationQuat(newquat * oriquat);

				me->setPosition3D(Vec3(round((x * cs + z * ss) * 100) / 100, y, round((-x * ss + z * cs) * 100)/ 100));
			}
		}

	
		if (isSolve()) {
			_winlabel->setVisible(true);
		}
		running = 0;
	}), NULL);
	testface->runAction(seq);
}

void HelloWorld::addcube(Sprite3D* c, float x, float y, float z) {
	c->setScale(8);
	c->setRotation3D(Vec3(0, 0, 0));
	cube* ccube = dynamic_cast<cube*>(c);
	ccube->setori(x, y, z);
	c->setPosition3D(Vec3(x, y, z));
}

bool HelloWorld::isSolve() {
	for (auto a : _dn) {
		cube* ccube = dynamic_cast<cube*>(a);
		if (!ccube->inpos()) {
			return false;
		}
	}
	return true;
}

void HelloWorld::update(float dt) {


	if (_startmove) {

		Quaternion dst;
		_rotationQuat.slerp(_rotationQuat, _destquat, _quatcount, &dst);
		_rotationQuat = dst;
		updateCameraTransform();
		_quatcount += 0.01;
		if (_quatcount >= 0.5) {
			_startmove = 0;
		}
	}

	
	//if (_startmove) {
	//	_rotationQuat = Quaternion(0, 0.70710678118654752440, 0, 0.70710678118654752440);
	//	updateCameraTransform();

	//	_startmove = 0;
	//}
	//if (_startmove) {
	//	float ratioy = 1;
	//	float ratiox = 1;
	//	if (_destx != 0 && _desty != 0) {
	//		if (_destx > _desty) {
	//			ratioy = _desty / _destx;
	//		}
	//		else {
	//			ratioy = _destx / _desty;
	//		}
	//	}
	//	auto ratio = _destx / _desty;


	//	if ((abs(_desty) - abs(_location.y) > 0.03) ||
	//		(abs(_destx) - abs(_location.x) > 0.03)) {
	//		if (_destx > 0) {
	//			_location.x += 0.03;
	//		}
	//		else {
	//			_location.x -= -0.03;
	//		}

	//		if (_desty > 0) {
	//			_location.y += 0.03;
	//		}
	//		else {
	//			_location.y -= 0.03;
	//		}

	//		Vec3 axes;
	//		float angle;
	//		calculateArcBall2(axes, angle, _prelocation.x, _prelocation.y, _location.x, _location.y, _axis);    //calculate  rotation quaternion parameters

	//		Quaternion quat(axes, angle * 4);


	//		_rotationQuat = quat * _rotationQuat;
	//		updateCameraTransform();
	//		_prelocation = _location;
	//		if ((abs(_desty) - abs(_location.y) < 0.03)) {
	//			_prelocation.y = _location.y;
	//		}
	//		if ((abs(_destx) - abs(_location.x) < 0.03)) {
	//			_prelocation.x = _location.x;
	//		}
	//	}
	//	else {

	//		auto fakeangle = _rotationQuat.toAxisAngle(&_camera->getPosition3D());
	//		auto fakedeg = CC_RADIANS_TO_DEGREES(fakeangle);
	//		alabel->setString("degree:" + ToString(fakedeg));

	//		_startmove = 0;
	//		_desty = 0;
	//		_destx = 0;
	//	}









		//if (abs(_location.y) < _desty || abs(_location.x) < _destx) {
		//	if (abs(_location.y) < _desty && _desty != 0) {
		//		_location.y = _location.y + _dir * 0.03;
		//		if (abs(_location.y) > _desty) {
		//			_location.y = _dir * _desty;
		//		}
		//	}
		//	if (abs(_location.x) < _destx && _destx != 0) {
		//		_location.x = _location.x + _dir *  0.03;
		//		if (abs(_location.x) > _destx) {
		//			_location.x = _dir * _destx;
		//		}
		//	}

		//	Vec3 axes;
		//	float angle;
		//	calculateArcBall2(axes, angle, _prelocation.x, _prelocation.y, _location.x, _location.y, _axis);    //calculate  rotation quaternion parameters


		//	if (_axis == 1) {
		//		//axes = _camera->getRotation3D();
		//		//axes.x = _dir * axes.x;

		//		auto oriquat = _camera->getRotationQuat();

		//		_camera->setRotationQuat(Quaternion(0, 0, 0, 1));
		//		updateCameraTransform();

		//		_camera->setRotation3D(Vec3(90 * _dir, 0, 0));
		//		auto newquat = _camera->getRotationQuat();
		//		_camera->setRotationQuat(oriquat * newquat);
		//		axes = _camera->getRotation3D();
		//		_camera->setRotationQuat(oriquat);
		//		
		//	}
		//	if (_axis == 2) {
		//		auto oriquat = _camera->getRotationQuat();
		//		_camera->setRotationQuat(Quaternion(0, 0, 0, 1));
		//		updateCameraTransform();

		//		_camera->setRotation3D(Vec3(0, 0, 90 * _dir));
		//		auto newquat = _camera->getRotationQuat();
		//		_camera->setRotationQuat(oriquat * newquat);
		//		axes = _camera->getRotation3D();
		//		_camera->setRotationQuat(oriquat);
		//		
		//	}


		//	axes = Vec3(0, 1, 0);
		//	Quaternion quat(axes, angle * 4);

		//
		//	_rotationQuat = quat * _rotationQuat;
		//	updateCameraTransform();
		//	_prelocation = _location;
		//	if (abs(_location.y) <= _desty && _desty != 0) {
		//		_prelocation.y = _location.y;
		//	}
		//	if (abs(_location.x) <= _destx && _destx != 0) {
		//		_prelocation.x = _location.x;
		//	}
		//
		//}
		//else {
	
		//	auto fakeangle = _rotationQuat.toAxisAngle(&_camera->getPosition3D());
		//	auto fakedeg = CC_RADIANS_TO_DEGREES(fakeangle);
		//	alabel->setString("degree:" + ToString(fakedeg));

		//	_startmove = 0;
		//	_desty = 0;
		//	_destx = 0;
		//}
	//}

}



void HelloWorld::cubemove(std::string face, int direction, bool group) {
	Sprite3D* facecube = nullptr;
	Sprite3D* facecube2 = nullptr;
	Sprite3D* facecube3 = nullptr;
	int targetaxis;
	auto maxextend = (cubesize * dimensions) / 2 + spacing * (dimensions - 1) / 2;

	if (!face.compare("front")) {
		auto pos = _dn2[0]->getPosition3D().z - (cubesize / 2);
		for (auto a : _dn) {
			if (round(a->getPosition3D().z) == round(pos)) {
				facecube = a;
				targetaxis = AXISZ;
				break;
			}
		}

		direction *= -1;
		auto ppos = _dn2[0]->getPosition3D();
		ppos.y = ppos.y + maxextend / 2 + 1;
		for (auto faceextend : _dn_front) {
			auto ab = faceextend->getAABB();
			if (ab.containPoint(ppos)) {
				auto dir = faceextend->getTag();
		
			}
		}
	}
	if (!face.compare("back")) {
		auto pos = _dn2[1]->getPosition3D().z + (cubesize / 2);
		for (auto a : _dn) {
			if (round(a->getPosition3D().z) == round(pos)) {
				facecube = a;
				targetaxis = AXISZ;
				break;
			}
		}
		auto ppos = _dn2[1]->getPosition3D();
		ppos.y = ppos.y + maxextend / 2 + 1;
		for (auto faceextend : _dn_back) {
			auto ab = faceextend->getAABB();
			if (ab.containPoint(ppos)) {
				auto dir = faceextend->getTag();
	
			}
		}
	}
	if (!face.compare("top")) {
		auto pos = _dn2[2]->getPosition3D().y - (cubesize / 2);
		for (auto a : _dn) {
			if (round(a->getPosition3D().y) == round(pos)) {
				facecube = a;
				targetaxis = AXISY;
				break;
			}
		}
		auto ppos = _dn2[2]->getPosition3D();
		ppos.y = ppos.y + maxextend + 1;
		for (auto faceextend : _dn_top) {
			auto ab = faceextend->getAABB();
			if (ab.containPoint(ppos)) {
				auto dir = faceextend->getTag();
	
			}
		}
	}
	if (!face.compare("bottom")) {

		auto pos = _dn2[3]->getPosition3D().y + (cubesize / 2);
		for (auto a : _dn) {
			if (round(a->getPosition3D().y) == round(pos)) {
				facecube = a;
				targetaxis = AXISY;
				break;
			}
		}
		auto ppos = _dn2[3]->getPosition3D();
		ppos.y = ppos.y + maxextend / 2 + 1;
		for (auto faceextend : _dn_bottom) {
			auto ab = faceextend->getAABB();
			if (ab.containPoint(ppos)) {
				auto dir = faceextend->getTag();

			}
		}
		direction *= -1;

	}
	if (!face.compare("right")) {
		auto pos = _dn2[4]->getPosition3D().x - (cubesize / 2);
		for (auto a : _dn) {
			if (round(a->getPosition3D().x) == round(pos)) {
				facecube = a;
				targetaxis = AXISX;
				break;
			}
		}
		auto ppos = _dn2[4]->getPosition3D();
		ppos.y = ppos.y + maxextend / 2 +1;
		for (auto faceextend : _dn_right) {
			auto ab = faceextend->getAABB();
			if (ab.containPoint(ppos)) {
				auto dir = faceextend->getTag();
	
			}
		}
	}
	if (!face.compare("left")) {
		auto pos = _dn2[5]->getPosition3D().x + (cubesize / 2);
		for (auto a : _dn) {
			if (round(a->getPosition3D().x) == round(pos)) {
				facecube = a;
				targetaxis = AXISX;
				break;
			}
		}
		direction *= -1;
		auto ppos = _dn2[5]->getPosition3D();
		ppos.y = ppos.y + maxextend / 2 + 1;
		for (auto faceextend : _dn_left) {
			auto ab = faceextend->getAABB();
			if (ab.containPoint(ppos)) {
				auto dir = faceextend->getTag();
			
			}
		}
	}
	setActiveGroup(targetaxis, facecube, direction, group);
}


void HelloWorld::switch_face(char face, int dir, bool group) {
	Ray ray;
	auto ori_rotate = _rotationQuat;
	Size visibleSize = Director::getInstance()->getVisibleSize();


	if (face == 'R') {
		_rotationQuat = _rotationQuat *Quaternion(0, 0.70710678118654752440, 0, 0.70710678118654752440);
	}
	if (face == 'L') {
		_rotationQuat = _rotationQuat *Quaternion(0, -0.70710678118654752440, 0, 0.70710678118654752440);
	}
	if (face == 'T') {
		_rotationQuat = _rotationQuat *Quaternion(-0.70710678118654752440, 0, 0, 0.70710678118654752440);
	}
	if (face == 'M') {
		_rotationQuat = _rotationQuat *Quaternion(0.70710678118654752440, 0, 0, 0.70710678118654752440);
	}
	if (face == 'B') {
		_rotationQuat = _rotationQuat *Quaternion(0, -1, 0, 0);
	}
	updateCameraTransform();
	// ?????
	auto new_w = visibleSize.height / 2 - _distanceZ;
	Vec3 snear(visibleSize.width / 2, new_w, -1.0f);
	Vec3 sfar(visibleSize.width / 2, new_w, 1.0f);

	auto csize = Director::getInstance()->getWinSize();

	_camera->unproject(csize, &snear, &snear);
	_camera->unproject(csize, &sfar, &sfar);

	ray._origin = snear;
	ray._direction = sfar - snear;

	_layer3D->setCameraMask(2);
	float facedist = 0;
	float cubedist = 0;
	Sprite3D* targetface = nullptr;
	Sprite3D* targetcube = nullptr;

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
		auto facename = targetface->getName();
		facelabel->setString(facename);
		_rotationQuat = ori_rotate;
		updateCameraTransform();
		cubemove(facename, dir, group);
	}
}