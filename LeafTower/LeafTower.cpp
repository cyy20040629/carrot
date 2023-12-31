#include "LeafTower.h"
#include "SimpleAudioEngine.h"

//���캯�������������ĳ�ʼ������ʾ
LeafTower::LeafTower(const cocos2d::Vec2& touchlocation) {
    tower = cocos2d::Sprite::create("LeafTower_small.png");
    towerlocation = touchlocation;
    category_code = 3;
    tower_level = 1;
    //��ʼ��������������ֵ
    tower_attack_number = 1;
    tower_attack_range = 130;
    tower_attack_power = 50;
    tower_attack_speed = 0.1;
    tower_sustained_attack_time;

    // ʹ��ȫ�ּ�������ΪΨһID �Ա������ʶ����
    towername = "LeafTower_" + std::to_string(count);
    count++;

    //������������ʾ
    tower->setPosition(touchlocation);
    //����������С
    tower->setScale(1);
    sprite_show(tower);
    //�����龰���ڽ��������֮��һ�䣬�������н��з���

    //����������������
    base = cocos2d::Sprite::create("LeafTower_Base_middle.png");
    base->setAnchorPoint(cocos2d::Vec2(0.5, 1));
    //������������
    base->setPosition(touchlocation);
    //������������
    base->setScale(1);
    sprite_show(base);
    //�����龰���ڽ��������֮��һ�䣬�������н��з���
}

//��������
void LeafTower::tower_attack(const cocos2d::Vec2& targetlocation) {
    // ���������ȼ�������Ӧ��С���ӵ�
    cocos2d::Sprite* Bullet;
    if (tower_level == 1) {
        Bullet = cocos2d::Sprite::create("LeafTower_bullet_small.png");
        //��������
        setPhysicsBody(Bullet, LEAFBULLET1);
    }
    else if (tower_level == 2) {
        Bullet = cocos2d::Sprite::create("LeafTower_bullet_middle.png");
        //��������
        setPhysicsBody(Bullet, LEAFBULLET2);
    }
    else if (tower_level == 3) {
        Bullet = cocos2d::Sprite::create("LeafTower_bullet_big.png");
        //��������
        setPhysicsBody(Bullet, LEAFBULLET3);
    }

    //�������Ӳ����ô�С
    Bullet->setPosition(towerlocation);
    Bullet->setScale(1);

    //���ӵ�Ҳ���뵱ǰ������
    Scene* currentScene = cocos2d::Director::getInstance()->getRunningScene();
    currentScene->addChild(Bullet, 2);

    // �������Ӳ�����������Ч
    tower_bullet_shoot(Bullet, targetlocation);
}

//�����������Ӳ�����������Ч
void LeafTower::tower_bullet_shoot(cocos2d::Sprite* bullet, const cocos2d::Vec2& targetlocation) {
    //��ʹ�ӵ���׼��������
    // ��������ָ�����ķ�������
    cocos2d::Vec2 direction = targetlocation - towerlocation;
    //�����ӵ���ʼ����ĽǶȣ�ʵ�ʵõ����ǻ��ȣ�
    float angle = atan2(direction.y, direction.x);
    //������ת��Ϊ�Ƕ�
    float angleInDegrees = CC_RADIANS_TO_DEGREES(angle);
    //��ת�ӵ�
    bullet->setRotation(-angleInDegrees);

    //������Ч
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->playEffect("LeafTowerAttackSound.MP3", false, 1.0f, 1.0f, 1.0f);

    //�����ӵ��ɡ�
    // �������ж���
    direction.normalize();//��������һ����
    auto moveBy = cocos2d::MoveBy::create(5.0, direction * 1000); // �����ٶ�Ϊÿ��1000������
    // ������ת������ÿ����ת360�ȳ�������
    auto rotateBy = cocos2d::RotateBy::create(5, 1800);
    // ����Spawn��������ʹ�ӵ�ͬʱ���з��к���ת
    auto spawn = cocos2d::Spawn::create(
        moveBy,
        rotateBy,
        nullptr
    );
    // �������ж�������
    auto sequence = cocos2d::Sequence::create(
        spawn,
        // ����һ���ص��������ڷ�����ɺ�ִ��
        cocos2d::CallFunc::create([=]() {
            //�Ƴ�����
            bullet->removeFromParent();
            }),
        nullptr
    );
    // ִ�з��ж�������
    bullet->runAction(sequence);
}

//��������
void LeafTower::towerUpgrade() {
    if (tower_level == 1) {
        tower_level++;
        tower_attack_range = 145;
        tower_attack_power = 70;
        tower_attack_speed = 0.1;

        // ʹ����ͼ���滻��ͼ��
        tower->setTexture("LeafTower_middle.png");
    }
    else if (tower_level == 2) {
        tower_level++;
        tower_attack_range = 160;
        tower_attack_power = 100;
        tower_attack_speed = 0.1;

        // ʹ����ͼ���滻��ͼ��
        tower->setTexture("LeafTower_big.png");
    }
}