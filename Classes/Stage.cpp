//
//  Stage.cpp
//  Jet
//
//  Created by 塚越雅斗 on 2015/03/15.
//
//

#include "Stage.h"

USING_NS_CC;

// ステージファイルのフォーマット
const char* STAGE_FILE_FORMAT = "map/stage%d.tmx";

Stage::Stage()
:_tiledMap(nullptr)
,_palyer(nullptr)
,_level(0)
{
}

Stage::~Stage()
{
    CC_SAFE_RELEASE(_tiledMap);
    CC_SAFE_RELEASE(_palyer);
}

bool Stage::initWithLabel(int level)
{
    if (!Layer::init()) {
        return false;
    }
    
    // ステージ番号を格納
    _level = level;
    
    // マップファイルからノードを作成する
    auto stageFile = StringUtils::format(STAGE_FILE_FORMAT, level);
    auto map = TMXTiledMap::create(stageFile);
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

Stage* Stage::createWithLabel(int level)
{
    Stage *ret = new Stage();
    if (ret->initWithLabel(level)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
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
        
        // タイルのIDを取り出す
        auto gid = layer->getTileGIDAt(coordinate);
        // タイルのプロパティを取り出す
        auto a = _tiledMap->getPropertiesForGID(gid);
        auto properties = a.asValueMap();
        
        
        // "category"というプロパティが存在しているかチェック
        if (properties.count("category") > 0) {
            // プロパティからcategoryの値を取り出す
            auto category = properties.at("category").asInt();
            // 剛体にカテゴリーをセットする
            physicsBody->setCategoryBitmask(category);
            // 剛体と接触判定を取るカテゴリーを指定する
            physicsBody->setContactTestBitmask(static_cast<int>(TileType::PLAYER));
            // 剛体と衝突を取るカテゴリを指定する
            physicsBody->setCollisionBitmask(static_cast<int>(TileType::PLAYER));
        }
        
        // 剛体をスプライトにつける
        sprite->setPhysicsBody(physicsBody);
        
        return sprite;
    }
    return nullptr;
}
