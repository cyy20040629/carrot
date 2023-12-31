#include "MonsterOne.h"
USING_NS_CC;
//����1��ʼ��
bool MonsterOne::init(Carrot* _carrotLayer, int mapCatalog)
{
    if (!Node::init())
    {
        return false;
    }
    carrotLayer = _carrotLayer;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //��ʼ����ֵ
    initialHealth = MonsterOneHealth;
    currentHealth = initialHealth;
    coinValue = MonsterOneValue;
    //����1�Ķ���֡
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("item.plist");
    auto framesMonsterOne = getAnimation("monster1/%04d.png", 2);
    objectSprite = Sprite::createWithSpriteFrame(framesMonsterOne.front());
    //�����ٶ�
    this->speed =MonsterOneSpeed;
    //��������
    setMonsterPhysicsBody();

    this->addChild(objectSprite, 2);
    //����
    objectSprite->setScale(1.8, 1.8);
    objectSprite->setOpacity(0);
    objectPosition = Origin[mapCatalog-1];
    objectSprite->setPosition(objectPosition);

    //����Ѫ��
    setHealthBar(objectSprite);
    //֡����
    auto animationMonsterOne = Animation::createWithSpriteFrames(framesMonsterOne, 1.0f / 3);
    objectSprite->runAction(RepeatForever::create(Animate::create(animationMonsterOne)));
    Sequence* moveAction;
    if (mapCatalog == 1)
        moveAction = MoveWayInMapOne(this); // ���ƶ�������������
    else
        moveAction = MoveWayInMapTwo(this);
    objectSprite->runAction(moveAction);

     //���� scheduleUpdate()��ʹ�� update ����������,ÿһ֡����λ��
    this->scheduleUpdate();
    return true;
}
MonsterOne* MonsterOne::create(Carrot* carrotLayer, int mapCatalog)
{
    MonsterOne* monsterOne = new MonsterOne();
    if (monsterOne && monsterOne->init(carrotLayer, mapCatalog))
    {
        monsterOne->autorelease();
        return monsterOne;
    }
    else
    {
        delete monsterOne;
        return nullptr;
    }
}

