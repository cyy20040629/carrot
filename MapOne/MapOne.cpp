#include"MapOne.h"

#define cellHeight 60
#define cellWidth 60

USING_NS_CC;

Scene* mapOne::createScene()
{
    Scene* startScene = Scene::create();
    mapOne* map = mapOne::create();
    startScene->addChild(map);
    return startScene;
}
bool mapOne::init()//��һ�ŵ�ͼ�ĳ�ʼ��
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();//��ͼ�Ŀɼ���С
    Vec2 origin = Director::getInstance()->getVisibleOrigin();//��ͼ��ʼ��ʱ�Ŀɼ���С

    //��ͼ����ͼ����
    auto background = Sprite::create("mapBackground.png");
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);
    //����·��ͼ����
    route = Sprite::create("route.png");
    route->setPosition(Vec2(visibleSize.width / 2 + origin.x, route->getContentSize().height / 2));
    this->addChild(route, 1);
    //�����������
    auto monsterEnter = Sprite::create("monsterEnter.png");
    monsterEnter->setPosition(Vec2(6 * cellWidth, 9 * cellHeight));
    this->addChild(monsterEnter, 2);

    //�ϰ�������
    obstacleDispatch();

    //�ص���ť����
    auto returnItem = MenuItemImage::create(
        "return.png",
        "returnSelected.png",
        CC_CALLBACK_1(sceneChoose::returnLast, this));//�ص���һ�����İ���
    returnItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100, origin.y + 3 * returnItem->getContentSize().height));
    auto menu = Menu::create(returnItem, NULL);//�����˵�������������
    this->addChild(menu, 2);


    label = cocos2d::Label::createWithTTF("Mouse Coordinates: ", "fonts/arial.ttf", 24);
    label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 40));
    this->addChild(label, 3);

    fireBottle = nullptr;
    //������������
    touchListener = EventListenerTouchOneByOne::create();
    // �󶨴����ĵĻص�����
    touchListener->onTouchEnded = CC_CALLBACK_2(mapOne::onTouchEnded, this);
    touchListener->onTouchBegan = CC_CALLBACK_2(mapOne::onTouchBegan, this);
    // �����������ӵ��¼��ַ�����
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}
bool  mapOne::onTouchEnded(Touch* touch, Event* event)
{
    CCLOG("onTouchEnded - Start");

    auto visibleSize = Director::getInstance()->getVisibleSize();//��ͼ�Ŀɼ���С
    Vec2 origin = Director::getInstance()->getVisibleOrigin();//��ͼ��ʼ��ʱ�Ŀɼ���С

    cocos2d::Vec2 mousePos = touch->getLocation();
    float mouseLocX = mousePos.x;
    float mouseLocY = mousePos.y;
    label->setString("Mouse Coordinates: " + std::to_string(mouseLocX) + ", " + std::to_string(mouseLocY));

    if (fireBottle == nullptr)
    {
        if (mouseLocY < 90 || (mouseLocY > 185 && mouseLocY < 290) || (mouseLocY > 360 && mouseLocY < 470) || mouseLocY > 570 && mousePos != obstacleTree->getPosition())//���λ�ÿ��԰�װ����
        {
            fireBottle = MenuItemImage::create(
                "fireBottle.png",
                "fireBottle.png",
                CC_CALLBACK_1(mapOne::menuCloseCallback, this));
            fireBottle->setPosition(static_cast<int>(mouseLocX / cellWidth) * cellWidth - visibleSize.width / 2 - static_cast<int>(cellWidth / 2), static_cast<int>(mouseLocY / cellHeight) * cellHeight + cellHeight - visibleSize.height / 2);
            Menu* menu = Menu::create(fireBottle, NULL);
            this->addChild(menu, 3);
            //��ѡ������
            selectedPos = Sprite::create("choose.png");
            towerPos.x = static_cast<int>(mouseLocX / cellWidth) * cellWidth + static_cast<int>(selectedPos->getContentSize().width / 2);
            if (mouseLocY >= 570 && mouseLocY < 610)
            {
                towerPos.y = static_cast<int>((mouseLocY - 10) / cellHeight) * cellHeight + static_cast<int>(selectedPos->getContentSize().height / 2) + 20;
                selectedPos->setPosition(towerPos.x, towerPos.y);//����λ��Ϊ����������λ��
            }
            else
            {
                towerPos.y = static_cast<int>(mouseLocY / cellHeight) * cellHeight + static_cast<int>(selectedPos->getContentSize().height / 2);
                selectedPos->setPosition(towerPos.x, towerPos.y);//����λ��Ϊ����������λ��
            }
            this->addChild(selectedPos, 2);
        }
        else
            CCLOG("Click invalid");
    }
    else
    {
        if (fireBottle->getBoundingBox().containsPoint(mousePos))
        {
            // �����fireBottleͼ�ִ꣬�лص�����
            CCLOG("Clicked on the sprite!");
            menuCloseCallback(fireBottle);
        }
        else
        {
            // ����˷�fireBottleͼ�꣬�Ƴ�fireBottleͼ��
            CCLOG("Removing fireBottle");
            CCLOG("Clicked outside the sprite!");
            this->removeChild(fireBottle->getParent());//�Ƴ�ѡ��ͼ�����ڲ˵�
            fireBottle = nullptr;  // ��������Ϊ�գ��Ա��´ε�����´���
        }
        removeChild(selectedPos);//ѡ��ȡ�����߽�����������ѡ�����ʧ
    }
    CCLOG("onTouchEnded - End");
    event->stopPropagation();
    return true;
}
bool  mapOne::onTouchBegan(Touch* touch, Event* event)//��֤��������������
{
    return true;
}
void mapOne::obstacleDispatch()
{
    CCLOG("onTouchEnded - Start");

    //�ϰ���ͼ������
    obstacleTree = Sprite::create("obstacleTree.png");//�ϰ���������ͣ��������������Ǯ����
    obstacleTree->setPosition(Vec2(6 * cellWidth + 15, 10 * cellHeight));
    this->addChild(obstacleTree, 2);
    //�ϰ���Ľ���������
    loadingBar = Sprite::create("loadingBar.png");
    loadingBar->setPosition(Vec2(obstacleTree->getPositionX() - obstacleTree->getContentSize().width / 2, obstacleTree->getPositionY() + obstacleTree->getContentSize().height / 2));
    //�ϰ���Ѫ������
    loadingBarBlood = ProgressTimer::create(Sprite::create("loadingBarBlood.png"));
    loadingBarBlood->setPosition(Vec2(obstacleTree->getPositionX() + loadingBarBlood->getContentSize().width, obstacleTree->getPositionY() + obstacleTree->getContentSize().height / 2));
    loadingBarBlood->setMidpoint(Vec2(0, obstacleTree->getPositionX()));
    loadingBar->addChild(loadingBarBlood);
    loadingBarBlood->setPercentage(66);//���ü�ʱ���ٷֱ�
    this->addChild(loadingBar, 2);
}
//��ɾ��
void mapOne::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->popScene();//�ӵ�ͼѡ��ĳ���������ջ�ｫ���泡��������Ϊ��ǰ���г���
    return;

}

void mapOne::towerGenerate(Ref* pSender)
{

}
cocos2d::Vec2 mapOne::passPos()//�ѷ����������괫�ݸ�������
{
    return towerPos;
}