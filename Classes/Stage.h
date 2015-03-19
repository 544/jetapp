//
//  Stage.h
//  Jet
//
//  Created by 塚越雅斗 on 2015/03/15.
//
//

#ifndef __Jet__Stage__
#define __Jet__Stage__

#include "cocos2d.h"
#include "Player.h"

class Stage :public cocos2d::Layer
{
protected:
    Stage();
    virtual ~Stage();
    bool init() override;

public:
    // タイルの種類
    enum class TileType {
        WALL = 1 << 0,
        PLAYER = 1 << 1,
        ENEMY = 1 << 2,
        COIN = 1 << 3
    };
    
    void update(float dt) override;
    
    // タイルマップ
    CC_SYNTHESIZE_RETAIN(cocos2d::TMXTiledMap*, _tiledMap, TiledMap);
    
    // プレイヤー
    CC_SYNTHESIZE_RETAIN(Player*, _palyer, Player);

    CREATE_FUNC(Stage);

private:
    /**
     * 指定のレイヤーの特定位置のタイルに剛体を設定する。
     * 指定座標にタイルがなかった場合はnullptrを返す。
     * @param layer 対象のレイヤー
     * @param coordinate 対象タイルのあるマップ上の座標
     * @return タイルのスプライト / nullptr
     */
    cocos2d::Sprite* addPhysicsBody(cocos2d::TMXLayer *layer, cocos2d::Vec2& coordinate);
};


#endif /* defined(__Jet__Stage__) */
