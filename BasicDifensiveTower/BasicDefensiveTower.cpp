#include "BasicDefensiveTower.h"
#include "MonsterManager.h"
#include"TowerManager.h"

bool BasicDefensiveTower::init()
{
    return true;
}
//获取炮塔精灵本身
cocos2d::Sprite* BasicDefensiveTower::getTowerSprite() {
    return tower;
}

//获取炮塔基座精灵本身
cocos2d::Sprite* BasicDefensiveTower::getBaseSprite() {
    return base;
}

//获取炮塔名字
std::string BasicDefensiveTower::getTowerName() {
    return towername;
}

//获取炮塔位置坐标
cocos2d::Vec2 BasicDefensiveTower::getTowerLocation() {
    return towerlocation;
}

//获取炮塔等级
int BasicDefensiveTower::getTowerLevel() {
    return tower_level;
}

//图标的显示
void BasicDefensiveTower::sprite_show(cocos2d::Sprite* sprite) {
    tower->setVisible(true);
}

//图标的隐藏
void BasicDefensiveTower::sprite_hide(cocos2d::Sprite* sprite) {
    tower->setVisible(false);
}

//炮塔更新索敌对象
void BasicDefensiveTower::tower_targetupdate1(float dt){
    // 如果当前没有目标或者目标超出索敌范围，重新选择目标
    for (BasicDefensiveTower* currentTower : TowerManager::getInstance()->towers)
    {
        if (currentTower->towername.compare(0, 14, "SunFlowerTower") == 0){
            //判断当前炮塔优先攻击目标是否 不为空且在当前炮塔的攻击范围内
            if (TowerManager::getInstance()->getFirstTarget() != nullptr && (TowerManager::getInstance()->getFirstTarget()->objectPosition - currentTower->getPosition()).length() <= currentTower->tower_attack_range) {
                currentTower->tower_spin(TowerManager::getInstance()->getFirstTarget()->objectPosition);
                currentTower->tower_attack(TowerManager::getInstance()->getFirstTarget()->objectPosition);
            }
            else {
                if (currentTower->currenttarget == nullptr || currentTower->currenttarget->currentHealth <= 0) {
                    currentTower->currenttarget = currentTower->findTarget();
                }
                else if ((currentTower->currenttarget->getCurrentPosition() - currentTower->getPosition()).length() > currentTower->tower_attack_range) {
                    currentTower->currenttarget = currentTower->findTarget();
                }
                if (currentTower->currenttarget != nullptr) {
                    //目标选择完成之后进行攻击操作（太阳塔不旋转！）
                    //currentTower->tower_spin(currentTower->currenttarget->objectPosition);
                    currentTower->tower_attack(currentTower->currenttarget->objectPosition);
                }
            }
        }
    }
}

//炮塔更新索敌对象
void BasicDefensiveTower::tower_targetupdate2(float dt) {
    // 如果当前没有目标或者目标超出索敌范围，重新选择目标
    for (BasicDefensiveTower* currentTower : TowerManager::getInstance()->towers)
    {
        if(currentTower->towername.compare(0, 13, "LightingTower") == 0){
            //判断当前炮塔优先攻击目标是否 不为空且在当前炮塔的攻击范围内
            if (TowerManager::getInstance()->getFirstTarget() != nullptr && (TowerManager::getInstance()->getFirstTarget()->objectPosition - currentTower->getPosition()).length() <= currentTower->tower_attack_range) {
                currentTower->tower_spin(TowerManager::getInstance()->getFirstTarget()->objectPosition);
                currentTower->tower_attack(TowerManager::getInstance()->getFirstTarget()->objectPosition);
            }
            else {
                if (currentTower->currenttarget == nullptr || currentTower->currenttarget->currentHealth <= 0) {
                    currentTower->currenttarget = currentTower->findTarget();
                }
                else if ((currentTower->currenttarget->getCurrentPosition() - currentTower->getPosition()).length() > currentTower->tower_attack_range) {
                    currentTower->currenttarget = currentTower->findTarget();
                }
                if (currentTower->currenttarget != nullptr) {
                    //目标选择完成之后进行攻击操作
                    currentTower->tower_spin(currentTower->currenttarget->objectPosition);
                    currentTower->tower_attack(currentTower->currenttarget->objectPosition);
                }
            }
        }
    }
}

//炮塔更新索敌对象
void BasicDefensiveTower::tower_targetupdate3(float dt) {
    // 如果当前没有目标或者目标超出索敌范围，重新选择目标
    for (BasicDefensiveTower* currentTower : TowerManager::getInstance()->towers)
    {
        if (currentTower->towername.compare(0, 9, "LeafTower") == 0){
            //判断当前炮塔优先攻击目标是否 不为空且在当前炮塔的攻击范围内
            if (TowerManager::getInstance()->getFirstTarget() != nullptr && (TowerManager::getInstance()->getFirstTarget()->objectPosition - currentTower->getPosition()).length() <= currentTower->tower_attack_range) {
                currentTower->tower_spin(TowerManager::getInstance()->getFirstTarget()->objectPosition);
                currentTower->tower_attack(TowerManager::getInstance()->getFirstTarget()->objectPosition);
            }
            else {
                if (currentTower->currenttarget == nullptr || currentTower->currenttarget->currentHealth <= 0) {
                    currentTower->currenttarget = currentTower->findTarget();
                }
                else if ((currentTower->currenttarget->getCurrentPosition() - currentTower->getPosition()).length() > currentTower->tower_attack_range) {
                    currentTower->currenttarget = currentTower->findTarget();
                }
                if (currentTower->currenttarget != nullptr) {
                    //目标选择完成之后进行攻击操作（其实你也不该转的）
                    //currentTower->tower_spin(currentTower->currenttarget->objectPosition);
                    currentTower->tower_attack(currentTower->currenttarget->objectPosition);
                }
            }
        }
    }
}

//炮塔最近距离索敌
GameObject* BasicDefensiveTower::findTarget() {
    // 获取场景中的怪物列表，这里假设怪物是通过 MonsterManager 管理的
    std::vector<GameObject*> monsters = MonsterManager::getInstance()->getMonsters();

    // 用于保存最近的怪物
    GameObject* nearestMonster = nullptr;

    // 遍历怪物列表
    for (GameObject* monster : monsters) {
        // 计算怪物与炮塔之间的距离
        float distance = towerlocation.distance(monster->getCurrentPosition());

        // 判断怪物是否在炮塔的攻击范围内【只要找到一个符合要求的怪物就结束遍历】
        if (distance <= tower_attack_range) {
            nearestMonster = monster;
            break;
        }
    }
    return nearestMonster;
}

//炮塔攻击
void BasicDefensiveTower::tower_attack(const cocos2d::Vec2& targetlocation) {
    //虚函数待覆盖
}

//炮塔转动（参数为当前攻击目标的所处的位置）
void BasicDefensiveTower::tower_spin(const cocos2d::Vec2& targetlocation) {
    //计算目标向量（目标->炮塔起点）
    cocos2d::Vec2 direction = targetlocation - towerlocation;
    // 计算旋转的角度（实际得到的是弧度）
    float angle = atan2(direction.y, direction.x);
    // 将弧度转换为角度
    float angleInDegrees = CC_RADIANS_TO_DEGREES(angle);
    // 创建旋转动作
    //【这里加了一个-号是因为计算得到的是正角度需要逆时针旋转，而cocos负角度才执行逆时针旋转】
    auto rotateAction = cocos2d::RotateTo::create(0.3, -angleInDegrees);
    // 执行旋转动作
    tower->runAction(rotateAction);
}

//炮塔发射粒子并产生飞行特效
void BasicDefensiveTower::tower_bullet_shoot(cocos2d::Sprite* bullet, const cocos2d::Vec2& targetlocation) {
    //虚函数待覆盖
}

//炮塔升级
void BasicDefensiveTower::towerUpgrade() {
    //虚函数待覆盖
}

void BasicDefensiveTower::setPhysicsBody(cocos2d::Sprite* Bullet, float value)
{
    //物理引擎
    cocos2d::Size smallerSize(Bullet->getContentSize().width * 0.5f, Bullet->getContentSize().height * 0.5f);
    auto physicsBody = cocos2d::PhysicsBody::createBox(smallerSize, cocos2d::PhysicsMaterial(0.1f, 1.0f, 0.0f));
    physicsBody->setPositionOffset(cocos2d::Vec2(smallerSize.width * 0.5f, smallerSize.height * 0.5f));//偏移量
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(0x05);    // 0101
    physicsBody->setContactTestBitmask(0x09); // 1001
    Bullet->setTag(value);
    Bullet->setPhysicsBody(physicsBody);

}