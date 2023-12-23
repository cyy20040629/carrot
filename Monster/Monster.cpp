#include "Monster.h"
#define MONSTER 200
#define CARROT 100

USING_NS_CC;
//背景
//Scene* basic::createScene()
//{
//    return basic::create();
//}
//bool basic::init()
//{
//    if (!Scene::init())
//    {
//        return false;
//    }
//    if (!Scene::initWithPhysics())
//    {
//        return false;
//    }
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//    auto listener = EventListenerMouse::create();
//    listener->onMouseMove = CC_CALLBACK_1(basic::onMouseMove, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//
//    auto background = Sprite::create("mapBackground.png");//地图背景图
//    background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
//    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
//    this->addChild(background, 0);
//    auto route = Sprite::create("route.png");//怪兽路线
//    route->setScale(visibleSize.width / route->getContentSize().width, visibleSize.height / route->getContentSize().height);
//    route->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
//    this->addChild(route, 1);
//    Monster* MonsterLayer = Monster::create();
//    this->addChild(MonsterLayer,2);
//    return true;
//}

bool Monster::init()
{
    if (!Node::init())
    {
        return false;
    }
    auto listener = EventListenerMouse::create();
    listener->onMouseMove = CC_CALLBACK_1(Monster::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Carrot* carrotLayer = Carrot::create();
    this->addChild(carrotLayer, 2);
    srand((unsigned int)time(nullptr));
    // 每隔1.5秒生成一个怪物
    this->schedule(CC_SCHEDULE_SELECTOR(Monster::addMonster), 1.5, 10, 0);
    //接触监听
    //auto contactListener = EventListenerPhysicsContact::create();
    //contactListener->onContactBegin = CC_CALLBACK_1(Monster::onContactBegin, this);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
//萝卜初始化
bool Carrot::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //萝卜的动画帧
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("item.plist");
    auto framesCarrot = getAnimation("carrot/%04d.png", 3);
    spriteCarrot = Sprite::createWithSpriteFrame(framesCarrot.front());
    //设置接触
    //auto physicsBody = PhysicsBody::createBox(spriteCarrot->getContentSize(), PhysicsMaterial(0.1f, 1.0f, 0.0f));// 密度，修复，摩擦
    //physicsBody->setDynamic(false);
    //physicsBody->setCategoryBitmask(0x04);    // 0100
    //physicsBody->setContactTestBitmask(0x01); // 0001
    //physicsBody->setCollisionBitmask(0x06);   // 0110
    //spriteCarrot->setTag(CARROT);
    //spriteCarrot->setPhysicsBody(physicsBody);
    this->addChild(spriteCarrot, 2);
    spriteCarrot->setPosition(702, 156);
    //镜像翻转
    carrot = spriteCarrot;
    spriteCarrot->setScaleX(-3.8);
    spriteCarrot->setScaleY(3.8);

    //帧速率
    auto animationCarrot = Animation::createWithSpriteFrames(framesCarrot, 1.0f / 3);
    spriteCarrot->runAction(RepeatForever::create(Animate::create(animationCarrot)));
    return true;
}
//怪物1初始化
bool MonsterOne::init()
{
    if (!Node::init())
    {
        return false;
    }
    float currentHealthValue=InitialHealthValue;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //怪物1的动画帧
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("item.plist");
    auto framesMonsterOne = getAnimation("monster1/%04d.png", 2);
    auto spriteMonsterOne = Sprite::createWithSpriteFrame(framesMonsterOne.front());
    //auto physicsBody = PhysicsBody::createBox(spriteMonsterOne->getContentSize(), PhysicsMaterial(0.1f, 1.0f, 0.0f));// 密度，修复，摩擦
    //physicsBody->setDynamic(false);
    //physicsBody->setCategoryBitmask(0x01);    // 0001
    //physicsBody->setContactTestBitmask(0x04); // 0100
    //physicsBody->setCollisionBitmask(0x03);   // 0011
    //spriteMonsterOne->setTag(MONSTER);
    //spriteMonsterOne->setPhysicsBody(physicsBody);

    this->addChild(spriteMonsterOne, 2);
    //缩放
    spriteMonsterOne->setScale(1.9, 1.9);
    spriteMonsterOne->setOpacity(0);
    spriteMonsterOne->setPosition(383, 532);
    //设置血条
    setHealthBar(spriteMonsterOne, currentHealthValue, InitialHealthValue);
    //帧速率
    auto animationMonsterOne = Animation::createWithSpriteFrames(framesMonsterOne, 1.0f / 3);
    spriteMonsterOne->runAction(RepeatForever::create(Animate::create(animationMonsterOne)));
    auto moveAction = MoveWayInMapOne(); // 将移动动作保存起来
    spriteMonsterOne->runAction(moveAction);
    return true;
}
//设置血条
void Monster::setHealthBar(Node* monster,float currentHealthValue,float InitialHealthValue)
{
    auto healthBarBG = Sprite::create("loadingBar.png");
    healthBarBG->setScale(0.7);
    healthBarBG->setPosition(Vec2(monster->getContentSize().width / 2, monster->getContentSize().height));  // 设置血条底座的位置
    monster->addChild(healthBarBG, 1);
    auto healthBar = ProgressTimer::create(Sprite::create("loadingBarBlood.png"));
    healthBar->setType(ProgressTimer::Type::BAR);
    healthBar->setScale(0.7);
    healthBar->setMidpoint(Vec2(0, 0.5));
    healthBar->setBarChangeRate(Vec2(1, 0));
    healthBar->setPosition(Vec2(monster->getContentSize().width / 2, monster->getContentSize().height));  // 设置血条的位置
    monster->addChild(healthBar, 2);
    float healthPercentage = (currentHealthValue / InitialHealthValue) * 100.0f;
    updateHealthBar(healthBar, healthPercentage);
}
//怪物二
bool MonsterTwo::init()
{
    if (!Node::init())
    {
        return false;
    }
    float currentHealthValue = InitialHealthValue;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //怪物2的动画帧
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("item.plist");
    auto framesMonsterTwo = getAnimation("monster2/%04d.png", 3);
    auto spriteMonsterTwo = Sprite::createWithSpriteFrame(framesMonsterTwo.front());
    this->addChild(spriteMonsterTwo, 2);
    //缩放
    spriteMonsterTwo->setScale(1.9, 1.9);
    spriteMonsterTwo->setOpacity(100);
    spriteMonsterTwo->setPosition(383, 532);
    setHealthBar(spriteMonsterTwo, currentHealthValue, InitialHealthValue);
    //帧速率
    auto animationMonsterTwo = Animation::createWithSpriteFrames(framesMonsterTwo, 1.0f / 3);
    spriteMonsterTwo->runAction(RepeatForever::create(Animate::create(animationMonsterTwo)));
    auto moveAction = MoveWayInMapOne(); // 将移动动作保存起来
    spriteMonsterTwo->runAction(moveAction);
    return true;
}

void Monster::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}
//生成帧序列
Vector<SpriteFrame*> Monster::getAnimation(const char* format, int count)
{
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> animFrames;
    char str[100];
    for (int i = 1; i <= count; i++)
    {
        sprintf(str, format, i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
    return animFrames;
}
//第一个地图的路径
Sequence* Monster::MoveWayInMapOne()
{
    auto fadeIn = FadeIn::create(0.5f);
    auto moveTo1 = MoveTo::create(2, Vec2(885, 532));
    auto moveTo2 = MoveTo::create(2, Vec2(885, 335));
    auto scaleXAction = ScaleBy::create(0.5f, -1.0f, 1.0f);
    auto moveTo3 = MoveTo::create(2, Vec2(210, 335));
    auto moveTo4 = MoveTo::create(2, Vec2(210, 153));
    auto moveTo5 = MoveTo::create(2, Vec2(702, 153));
    auto fadeOut = FadeOut::create(0.5f);
    auto actionRemove = RemoveSelf::create();
    auto seq = Sequence::create(fadeIn,moveTo1, moveTo2, scaleXAction, moveTo3,  moveTo4, scaleXAction, moveTo5, fadeOut,actionRemove, nullptr);
    return seq;
}
//鼠标移动显示坐标
void Monster::onMouseMove(Event* event)
{
    EventMouse* e = dynamic_cast<EventMouse*>(event);
    if (e)
    {
        // 获取鼠标坐标
        Vec2 mousePosition = Vec2(e->getCursorX(), e->getCursorY());
        //// 将鼠标坐标转换为 OpenGL 坐标系下的坐标
        //mousePosition = Director::getInstance()->convertToGL(mousePosition);

        // 显示坐标信息（左下角）
        auto label = Label::createWithTTF(StringUtils::format("Mouse Position: %.2f, %.2f", mousePosition.x, mousePosition.y), "fonts/arial.ttf", 24);
        label->setAnchorPoint(Vec2::ZERO);
        label->setPosition(Vec2::ZERO);
        this->removeChildByTag(123); // 移除之前的 label
        this->addChild(label, 5, 123);
    }
}
//添加怪物
void Monster::addMonster(float dt)
{
    MonsterOne* MonsterOneLayer = MonsterOne::create();
    this->addChild(MonsterOneLayer, 2);
    //MonsterManager::getInstance()->addMonster(MonsterOneLayer);
    auto delayAction = DelayTime::create(2.0f);
    auto addMonsterTwoAction = CallFunc::create([this]() {
        MonsterTwo* MonsterTwoLayer = MonsterTwo::create();
        this->addChild(MonsterTwoLayer, 2);
        //MonsterManager::getInstance()->addMonster(MonsterTwoLayer);
        });
    // 将两个动作组合在一起
    auto sequence = Sequence::create(delayAction, addMonsterTwoAction, nullptr);

    // 运行 Sequence
    this->runAction(sequence);
}
//生命值
void Monster::updateHealthBar(ProgressTimer* healthBar,float percentage) {
    healthBar->setPercentage(percentage);
}
//接触
bool Monster::onContactBegin(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB)
    {
        int tagA = nodeA->getTag();
        int tagB = nodeB->getTag();
        if (tagA != tagB)
        {
            // 怪物和萝卜发生碰撞
            Carrot* carrot ;
            Monster* monster ;
            if (tagA == CARROT)
            {
                carrot = dynamic_cast<Carrot*>(nodeA);
                monster = dynamic_cast<Monster*>(nodeB);
            }
            else
            {
                carrot = dynamic_cast<Carrot*>(nodeB);
                monster = dynamic_cast<Monster*>(nodeA);
            }
            // 减少萝卜的生命值
            carrot->decreaseHealth();
        }
    }
    return true;
}
//Carrot::Carrot() : spriteNum(nullptr), healthValue(10.0f)
//{
//    healthValue=10.0f;
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("item.plist");
//    auto framesCarrot = getAnimation("carrot/%04d.png", 3);
//    spriteCarrot = Sprite::createWithSpriteFrame(framesCarrot.front());
//    this->addChild(spriteCarrot, 2);
//
//}
void Carrot::decreaseHealth()
{
    healthValue--;
    if (healthValue == 9.0f)
    {
        spriteNum = Sprite::create("icon_num(9).png");
        spriteNum->setPosition(Vec2(spriteCarrot->getContentSize().width , 0));
        spriteCarrot->addChild(spriteNum);
        auto framesCarrot = getAnimation("carrot-1%04d.png", 2);
        auto animationCarrot = Animation::createWithSpriteFrames(framesCarrot, 1.0f / 3);
        spriteCarrot->stopAllActions();
        spriteCarrot->setSpriteFrame(framesCarrot.front());
        spriteCarrot->runAction(RepeatForever::create(Animate::create(animationCarrot)));
    }
    else if (healthValue == 8.0f)
    {
        spriteNum->setTexture("icon_num(8).png");
        // 可以根据需要调整纹理矩形
        spriteNum->setTextureRect(Rect(Vec2::ZERO, spriteNum->getContentSize()));
    }
    else if (healthValue == 7.0f)
    {
        spriteNum->setTexture("icon_num(7).png");
        // 可以根据需要调整纹理矩形
        spriteNum->setTextureRect(Rect(Vec2::ZERO, spriteNum->getContentSize()));
        spriteCarrot->stopAllActions();
        spriteCarrot->setTexture("carrot7).png");
        // 可以根据需要调整纹理矩形
        spriteCarrot->setTextureRect(Rect(Vec2::ZERO, spriteCarrot->getContentSize()));
    }
    else if (healthValue == 6.0f)
    {
        spriteNum->setTexture("icon_num(6).png");
        // 可以根据需要调整纹理矩形
        spriteNum->setTextureRect(Rect(Vec2::ZERO, spriteNum->getContentSize()));
    }
    else if (healthValue == 5.0f)
    {
        spriteNum->setTexture("icon_num(5).png");
        // 可以根据需要调整纹理矩形
        spriteNum->setTextureRect(Rect(Vec2::ZERO, spriteNum->getContentSize()));
        spriteCarrot->stopAllActions();
        spriteCarrot->setTexture("carrot5).png");
        // 可以根据需要调整纹理矩形
        spriteCarrot->setTextureRect(Rect(Vec2::ZERO, spriteCarrot->getContentSize()));

    }
    else if (healthValue == 4.0f)
    {
        spriteNum->setTexture("icon_num(4).png");
        // 可以根据需要调整纹理矩形
        spriteNum->setTextureRect(Rect(Vec2::ZERO, spriteNum->getContentSize()));

        spriteCarrot->stopAllActions();
        spriteCarrot->setTexture("carrot4).png");
        // 可以根据需要调整纹理矩形
        spriteCarrot->setTextureRect(Rect(Vec2::ZERO, spriteCarrot->getContentSize()));
    }
    else if (healthValue == 3.0f)
    {
        spriteNum->setTexture("icon_num(3).png");
        // 可以根据需要调整纹理矩形
        spriteNum->setTextureRect(Rect(Vec2::ZERO, spriteNum->getContentSize()));
    }
    else if (healthValue == 2.0f)
    {
        spriteNum->setTexture("icon_num2).png");
        // 可以根据需要调整纹理矩形
        spriteNum->setTextureRect(Rect(Vec2::ZERO, spriteNum->getContentSize()));
        spriteCarrot->stopAllActions();
        spriteCarrot->setTexture("carrot3).png");
        // 可以根据需要调整纹理矩形
        spriteCarrot->setTextureRect(Rect(Vec2::ZERO, spriteCarrot->getContentSize()));
    }
    else if (healthValue == 1.0f)
    {
        spriteNum->setTexture("icon_num(1).png");
        // 可以根据需要调整纹理矩形
        spriteNum->setTextureRect(Rect(Vec2::ZERO, spriteNum->getContentSize()));
        spriteCarrot->stopAllActions();
        spriteCarrot->setTexture("carrot3).png");
        // 可以根据需要调整纹理矩形
        spriteCarrot->setTextureRect(Rect(Vec2::ZERO, spriteCarrot->getContentSize()));
    }
}