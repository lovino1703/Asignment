#include "GameScene.h"
#include "Definitions.h"
#include "GameOverScene.h"
#include "AudioManager.h"
#include "AudioEngine.h"
#include <fstream>   
#include <iostream>

USING_NS_CC;
using namespace std;
Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    /*scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);*/
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));

    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    layer->SetPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto backgroundSprite = Sprite::create("Background.png");
    backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(backgroundSprite);

    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edgeBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
    edgeBody->setContactTestBitmask(true);

    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    edgeNode->setPhysicsBody(edgeBody);

    this->addChild(edgeNode);

    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::SpawnPipe), PIPE_SPAWN_FREQUENCY * visibleSize.width);

    bird = new Bird(this);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    score = 0;

    std::string tempScore = std::to_string(score);

    scoreLabel = Label::createWithTTF(tempScore, "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE);
    scoreLabel->setColor(Color3B::WHITE);
    scoreLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.75 + origin.y));

    this->addChild(scoreLabel, 10000);

    this->scheduleUpdate();

    return true;
}

void GameScene::SpawnPipe(float dt)
{
    pipe.SpawnPipe(this);
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact& contact)
{
    PhysicsBody* a = contact.getShapeA()->getBody();
    PhysicsBody* b = contact.getShapeB()->getBody();

    if ((BIRD_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask()) || (BIRD_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()))
    {
        //AudioManager::getInstance()->playSFX("Hit.mp3");
        AudioEngine::play2d("Audio/SFX/Hit.mp3");
        /*fstream file;
        file.open("C:\\Users\\Hi Windows 11 Home\\Documents\\highScore.txt");
        file << score << endl;
        file.close();*/

        auto scene = GameOverScene::createScene(score);

        Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
    }
    else if ((BIRD_COLLISION_BITMASK == a->getCollisionBitmask() && POINT_COLLISION_BITMASK == b->getCollisionBitmask()) || (BIRD_COLLISION_BITMASK == b->getCollisionBitmask() && POINT_COLLISION_BITMASK == a->getCollisionBitmask()))
    {
        AudioEngine::play2d("Audio/SFX/Point.mp3");

        score++;

        std::string tempScore = std::to_string(score);

        scoreLabel->setString(tempScore);
    }

    return true;
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    bird->Fly();
    //AudioEngine::play2d("Audio/SFX/Wing.mp3");

    this->scheduleOnce(CC_SCHEDULE_SELECTOR(GameScene::StopFlying), BIRD_FLY_DURATION);

    return true;
}

void GameScene::StopFlying(float dt)
{
    bird->StopFlying();
}

void GameScene::update(float dt)
{
    bird->Fall();
}









