//
//  MainScene.cpp
//  Jet
//
//  Created by 塚越雅斗 on 2015/03/15.
//
//

#include "MainScene.h"

USING_NS_CC;

// 重力を表すベクトル
const Vec2 GRAVITY_ACCELERATION = Vec2(0, -3);

// 浮力を表すベクトル
const Vec2 IMPULSE_ACCELERATION = Vec2(0, 180);

MainScene::MainScene()
:_stage(nullptr)
,_isPress(false)
{
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE_NULL(_stage);
}

Scene* MainScene::createSceneWithLevel(int level)
{
    // 物理エンジンを有効にしたシーンを作成する
    auto scene = Scene::createWithPhysics();
    
    // 物理空間を取り出す。
    auto world = scene->getPhysicsWorld();
    // 重力を設定する。
    world->setGravity(GRAVITY_ACCELERATION);
    
#if COCOS2D_DEBUG > 0
    // デバッグビルドの場合は物理空間にデバッグ用表示を追加
   // world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif
    
    // 物理空間のスピードを設定する
    world->setSpeed(6.0);
    
//    auto layer = MainScene::create();
    
    auto layer = new MainScene();
    if ( layer && layer->initWithLevel(level)) {
        layer->autorelease();
    } else {
        CC_SAFE_DELETE(layer);
    }
    scene->addChild(layer);
    
    return scene;
}

bool MainScene::initWithLevel(int level)
{
    if (!Layer::init()) {
        return false;
    }
    
    auto stage = Stage::createWithLabel(level);
    this->addChild(stage);
    this->setStage(stage);
    
    // タッチを検知してフラグを立てる
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this] (Touch* touch, Event* event) {
        this->setIsPress(true);
        return true;
    };
    listener->onTouchEnded = [this] (Touch* touch, Event* event) {
        this->setIsPress(false);
    };
    listener->onTouchCancelled = [this] (Touch* touch, Event* event) {
        this->setIsPress(false);
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    // 物体が接触したことを検知するEventListener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [] (PhysicsContact& ccontact) {
        log("hit");
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    
    this->scheduleUpdate();
    
    
    
    
    return true;
}

void MainScene::update(float dt)
{
    if (this->getIsPress()) {
        // プレイヤーに上方向の推進力を与える。
        _stage->getPlayer()->getPhysicsBody()->applyImpulse(IMPULSE_ACCELERATION);
    }
    
}