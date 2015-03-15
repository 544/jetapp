//
//  Stage.cpp
//  Jet
//
//  Created by 塚越雅斗 on 2015/03/15.
//
//

#include "Stage.h"

USING_NS_CC;

Stage::Stage()
:_tiledMap(nullptr)
,_palyer(nullptr)
{
}

Stage::~Stage()
{
    CC_SAFE_RELEASE(_tiledMap);
    CC_SAFE_RELEASE(_palyer);
}

bool Stage::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    // マップファイルからノードを作成する
    auto map = TMXTiledMap::create("map/stage1.tmx");
    this->addChild(map);
    this->setTiledMap(map);
    
    // 地形レイヤーを取得する
    auto terrainLayer = map->getLayer("terrain");
    // オブジェクトレイヤーを取得する
    auto objectLayer = map->getLayer("object");
    
    // マップサイズ
    auto mapSize = map->getMapSize();
    for (int x = 0; x < mapSize.width; ++x ) {
        for (int y = 0; y < mapSize.height; ++y) {
            auto coordinate = Vec2(x,y);
            this->addPhysicsBody(terrainLayer, coordinate);
            this->addPhysicsBody(objectLayer, coordinate);
        }
    }
    
    // プレイヤーの追加
    auto player = Player::create();
    player->setPosition(Vec2(40, 160));
    this->addChild(player);
    this->setPlayer(player);
    
    // プレイヤーの移動に画面を追従させる。
    // TODO: cocos2dx 3.4だとバグで追従できない？
    // https://github.com/cocos2d/cocos2d-x/issues/10511
    auto winSize = Director::getInstance()->getWinSize();
    this->runAction(Follow::create(player, Rect(0,
                                                0,
                                                _tiledMap->getContentSize().width
                                                    * _tiledMap->getScale(),
                                                winSize.height)));
    
    this->scheduleUpdate();
    return true;
}

void Stage::update(float dt)
{
    
}

Sprite* Stage::addPhysicsBody(cocos2d::TMXLayer *layer, cocos2d::Vec2 &coordinate)
{
    // タイルのスプライトを取り出す
    auto sprite = layer->getTileAt(coordinate);
    if (sprite) {
        // 剛体用のマテリアルを作成
        auto material = PhysicsMaterial();
        // 摩擦係数は0 (ひっかかり防止)
        material.friction = 0;
        // 剛体を設置する
        auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), material);
        // 剛体を固定する
        physicsBody->setDynamic(false);
        // 剛体をつけるスプライトのアンカーポイントを中心にする。
        sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        // 剛体をスプライトにつける
        sprite->setPhysicsBody(physicsBody);
        
        return sprite;
    }
    return nullptr;
}
