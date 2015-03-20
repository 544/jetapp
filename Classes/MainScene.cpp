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
    contactListener->onContactBegin = [this] (PhysicsContact& contact) {
        
        // 二つの剛体のうち、プレイヤーではない方を取り出す。
        auto otherShape = contact.getShapeA()->getBody() == _stage->getPlayer()->getPhysicsBody()
        ? contact.getShapeB() : contact.getShapeA();
        
        auto body = otherShape->getBody();
        auto category = body->getCategoryBitmask();
        
        if (category & static_cast<int>(Stage::TileType::ENEMY)) {
            this->onGameOver();
        }
        
        log("hit");
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    
    this->scheduleUpdate();
    
    
    
    
    return true;
}

void MainScene::update(float dt)
{
    // タップで上昇
    if (this->getIsPress()) {
        // プレイヤーに上方向の推進力を与える。
        _stage->getPlayer()->getPhysicsBody()->applyImpulse(IMPULSE_ACCELERATION);
    }
    
    // 画面外に出たら終了
    auto winSize = Director::getInstance()->getWinSize();
    auto position = _stage->getPlayer()->getPosition();
    const auto margin = 50;
    
    
}

void MainScene::onGameOver()
{
    // プレイヤーをステージから削除する
    _stage->getPlayer()->removeFromParent();
    
    // 画面サイズを取り出す
    auto winSize = Director::getInstance()->getWinSize();
    
    // GAME OVERの表示
    auto gameover = Sprite::create("gameover.png");
    gameover->setPosition(Vec2(winSize.width/2, winSize.height/1.5));
    this->addChild(gameover);
    
    // もう一度遊ぶメニュー
    auto menuItem = MenuItemImage::create("replay.png", "replay_pressed.png", [this](Ref *sender){
        auto scene = MainScene::createSceneWithLevel(_stage->getLevel());
        auto transition = TransitionFade::create(1.0, scene);
        Director::getInstance()->replaceScene(transition);
    });
    auto menu = Menu::create(menuItem, nullptr);
    menu->setPosition(Vec2(winSize.width/2.0, winSize.height/3));
    this->addChild(menu);
    
    
}