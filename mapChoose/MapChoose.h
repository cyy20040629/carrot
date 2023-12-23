#ifndef  MAP_CHOOSE_H
#define  MAP_CHOOSE_H

#include "cocos2d.h"
class BasicDifensiveTower;
class mapChoose : public cocos2d::Scene
{
public:
    cocos2d::Vec2 towerPos;//炮塔位置
    cocos2d::Sprite* obstacleTree;//障碍物
    cocos2d::Label* goldCoin;//金币
    BasicDifensiveTower* tower;//炮塔类
    static cocos2d::Scene* createScene();
    virtual bool init();//第一个地图选择场景初始化
    void returnLast(cocos2d::Ref* pSender);//回到上一场景
    void flipToNext(cocos2d::Ref* pSender);//滑动到下一个地图选择场景
    void flipToLast(cocos2d::Ref* pSender);//滑动到上一个地图选择场景
    void enterMapOne(cocos2d::Ref* pSender);//进入第一个地图中
    CREATE_FUNC(mapChoose);

};
#endif // MAP_CHOOSE_H
