#ifndef MAP_TWO_H
#define MAP_TWO_H

#include "cocos2d.h"
#include"MapChoose.h"

class mapTwo : public mapChoose
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();//地图场景初始化(UI设计)
    virtual bool ifSafe(cocos2d::Vec2 mousePos);//判断点击到的是否为炮塔
    virtual void selectedPosSet(float mouseLocX, float mouseLocY);//复选框位置设置（不同位置有细小差别）
    virtual void tryAgain(cocos2d::Ref* pSender);
    CREATE_FUNC(mapTwo);

};

#endif // MAP_TWO_H


