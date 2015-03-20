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
    bool initWithLevel(int level);

public:
    
    /**
     * 任意の番号のステージでシーンを作成する。
     */
    static cocos2d::Scene* createSceneWithLevel(int level);
    
    
    void update(float dt) override;
    
    // ステージ
    CC_SYNTHESIZE_RETAIN(Stage*, _stage, Stage);
    
    // 画面がタップされているかどうか
    CC_SYNTHESIZE(bool, _isPress, IsPress);

private:
    void onGameOver();
    
};

#endif /* defined(__Jet__MainScene__) */
