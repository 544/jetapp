//
//  Player.h
//  Jet
//
//  Created by 塚越雅斗 on 2015/03/15.
//
//

#ifndef __Jet__Player__
#define __Jet__Player__

#include "cocos2d.h"

class Player :public cocos2d::Sprite
{
protected:
    bool init() override;
    
public:
    void update(float dt) override;
    // ジェットの推進力
    CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vec2, _acceleration, Acceleration);
    
    CREATE_FUNC(Player);
    
};

#endif /* defined(__Jet__Player__) */

