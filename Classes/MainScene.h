//
//  MainScene.h
//  Jet
//
//  Created by 塚越雅斗 on 2015/03/15.
//
//

#ifndef __Jet__MainScene__
#define __Jet__MainScene__

#include "cocos2d.h"
#include "Stage.h"

class MainScene :public cocos2d::Layer
{
protected:
    MainScene();
    virtual ~MainScene();
    bool init() override;

public:
    static cocos2d::Scene* createScene();
    void update(float dt) override;
    
    // ステージ
    CC_SYNTHESIZE_RETAIN(Stage*, _stage, Stage);
    
    // 画面がタップされているかどうか
    CC_SYNTHESIZE(bool, _isPress, IsPress);
    
    CREATE_FUNC(MainScene);
};

#endif /* defined(__Jet__MainScene__) */
