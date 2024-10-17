#include "HighScore.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include<fstream>
USING_NS_CC;
using namespace std;

cocos2d::Scene* HighScore::createScene()
{
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HighScore::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool HighScore::init()
{
    if (!Layer::init())
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto backgroundSprite = Sprite::create("Background.png");
    backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    this->addChild(backgroundSprite);
    auto highScoreBoard = Sprite::create("HighScoreBoard.png");
    highScoreBoard->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 5 * 3 + origin.y));
    this->addChild(highScoreBoard);

    //auto highScoreLabel = Label::createWithTTF("High Score", "fonts/Marker Felt.ttf", 20);
    //highScoreLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 5 * 3 + origin.y));
    //this->addChild(highScoreLabel);

    auto mainMenuItem = MenuItemImage::create("Menu Button.png", "Menu Button Clicked.png", CC_CALLBACK_1(HighScore::GoToMainMenuScene, this));
    mainMenuItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 6));
    auto menu = Menu::create(mainMenuItem, NULL);
    menu->setPosition(Point::ZERO);

    this->addChild(menu);

    unsigned int highScore;
    fstream file;
    file.open("highScore.txt");
    file >> highScore;
    file.close();

    std::string tempHighScore = std::to_string(highScore);

    auto highScoreLabel = Label::createWithTTF(tempHighScore, "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE);

    highScoreLabel->setColor(Color3B::YELLOW);
    highScoreLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    this->addChild(highScoreLabel);

    return true;
}

void HighScore::GoToMainMenuScene(cocos2d::Ref* sender)
{
    auto scene = MainMenuScene::createScene();

    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
