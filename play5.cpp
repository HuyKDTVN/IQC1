#include "play5.h"
#include "mathLevel.h"
#include "mathSelect.h"
#include "SelectGame.hpp"
#include "finish.h"
#include "LanguageManager.h"
#include "IAPManager.h"
#include "ui/CocosGUI.h"
#include <SimpleAudioEngine.h>
#include <iostream>
#include <string>
#include "Variable.hpp"
#include "Huy_functions/HuyFunctions.hpp"
#include "Huy_functions/CreatePhepToan.hpp"

using namespace std;
USING_NS_CC;

int typeOfW5;
int levelOfW5;
bool isMulti5 = false;
int backTypePT5 = 0;
bool fromThuThach5;

Scene* Play5::createScene(int type, int level, int backTypePhepTinh, bool fromChallenge)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    typeOfW5 = type;
    if(typeOfW5 == 1){
        isMulti5 = true;
    }
    backTypePT5 = backTypePhepTinh;
    
    levelOfW5 = level;
    fromThuThach5 = fromChallenge;
    // 'layer' is an autorelease object
    auto layer = Play5::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
// on "init" you need to initialize your instance
bool Play5::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }
    soCau = 0;
    score = 0;
    if (UserDefault::getInstance()->getIntegerForKey("height-score", 99999) == 99999)
    {
        UserDefault::getInstance()->setIntegerForKey(HIGH_SCORE, 0);
        UserDefault::getInstance()->flush();
    }
    
    log("phien ban hien tai: %d", Variable().isProVer);

    auto mLang = UserDefault::getInstance()->getStringForKey("LANGUAGE", "en");
    maxDoiSo = HuyFunctions().SoLonNhat(mLang);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float wScene;
    float hScene;

    //khoi tao gia tri ban dau
    wScene = visibleSize.width;
    hScene = visibleSize.height;
    wScenePlay = wScene;
    hScenePlay = hScene;
    
    //log("test man SE: %i, %i, %.2f", hScenePlay, wScenePlay, double(hScenePlay / wScenePlay));

    hasGuide = false;
    
    //set bg
    //createBg(wScene, hScene);

    //countdown timer + loading bar
    coundownTimer(wScene, hScene);
    //end: countdown timer + loading bar

    realType = typeOfW5;

    back(wScene, hScene);
    //createButtonKey(wScene, hScene);

    auto arr = CreatePhepToan().createAnswerQuestions(typeOfW5, levelOfW5, prevA, prevB, maxDoiSo);
    
    auto arrNoti = CreatePhepToan().createAnswerQuestions(typeOfW5, levelOfW5, prevA, prevB, maxDoiSo);
    UserDefault::getInstance()->setStringForKey("noti-title1", HuyFunctions().createPhepToanWithEmoji(arrNoti.at(2).c_str()));
    log("test improve function");
    log("arrNoti(2): %s", arrNoti.at(2).c_str());
    log("test arr: %s, %s, %s, %s, %s, %s, %s,  %s, %s", arr.at(0).c_str(), arr.at(1).c_str(), arr.at(2).c_str(), arr.at(3).c_str(), arr.at(4).c_str(), arr.at(5).c_str(), arr.at(6).c_str(), arr.at(7).c_str(), arr.at(8).c_str()); //5, 0, 5 + 0 = ?, 5, 2, 5, 6,  0, +
    //    0 - prevA
    //    1 - prevB
    //    2 - phepToan day du
    //    3 - correctAnswer
    //    4 - answer1
    //    5 - answer2
    //    6 - answer3
    //    7 - answer4
    //    8 - Loai phep tinh
    prevA = std::stoi(arr.at(0));
    prevB = std::stoi(arr.at(1));
    
    loaiPhepTinh = cocos2d::__String::createWithFormat("%s", arr.at(8).c_str());
    sapXepPhepTinhTheoChieuDoc = CreatePhepToan().needPhepTinhTheoChieuDoc(loaiPhepTinh->getCString(), levelOfW5);
    soA = std::stoi( arr.at(0) );
    soB = std::stoi ( arr.at(1));
    phepTinhSmallA = arr.at(9);
    phepTinhSmallB = arr.at(10);
    if (sapXepPhepTinhTheoChieuDoc) {
        phepToan = cocos2d::__String::createWithFormat("?");//cocos2d::__String::createWithFormat("%s", arr.at(8).c_str())
        flagB1ChuSo = false;
        if (arr.at(1).size() < 2) {
            flagB1ChuSo = true;
        }
    } else{
        phepToan = cocos2d::__String::createWithFormat("%s", arr.at(2).c_str());
    }
    correctAnswer = std::stoi(arr.at(3));
    answer1 = std::stoi(arr.at(4));
    answer2 = std::stoi(arr.at(5));
    answer3 = std::stoi(arr.at(6));
    answer4 = std::stoi(arr.at(7));
    
    log("cau hoi: %s",  arr.at(2).c_str());
    log("4 dap an: %i, %i, %i, %i",  std::stoi(arr.at(4)), std::stoi(arr.at(5)), std::stoi(arr.at(6)), std::stoi(arr.at(7)));
    log("dap dung: %i",  std::stoi(arr.at(3)));
    
    indexThemDiem = HuyFunctions().getRandomForThemDiem();
    
    createScore(wScene, hScene);
    createMainTemplate(wScene, hScene);
    
    int randomImageNameDapAn = cocos2d::RandomHelper::random_int(1, 5);
    ImageNamDapAn = __String::create(StringUtils::format("button-dap-an-%d.png", randomImageNameDapAn));
    ImageNamDapAnX = __String::create(StringUtils::format("button-dap-an-%d-x.png", randomImageNameDapAn));
    ImageNamDapAnV = __String::create(StringUtils::format("button-dap-an-%d-v.png", randomImageNameDapAn));
    createAnswer(wScene, hScene);

    //log("HUY ----- type: %i, level: %i", typeOfW5, levelOfW5);

    createBgColor(wScene, hScene);
    createTraiDatAndPhiThuyenTenLua(wScene, hScene);
    createPopupFinish(wScene, hScene);
    
    //log("hLoadingBar: %f", hLoadingBar);
    if (Variable().isProduction == true) {
        HuyFunctions().requestAds(1);
    }

    //this->setKeypadEnabled(true);
    
    if (levelOfW5 == 1) {
        createTemplateGuideLevel1(wScene, hScene);
    } else {
        createTemplateGuideLevel2(wScene, hScene);
    }
    //log("type of w: %i", typeOfW5);
    
    createTienDo(wScene, hScene);
     
    return true;
}
void Play5::createTienDo(float w, float h) {
    float heightLoadingBar = 0.9 * h;
   
    if (h/w <= 2.168)
    {
        heightLoadingBar = 0.91 * h;
    }
    if (h/w <= 1.8)
    {
        heightLoadingBar = 0.9 * h;
    }
    if (h/w <= 1.53)
    {
        heightLoadingBar = 0.95 * h;
    }
    
    // Thêm slider
    loadingBar = ui::LoadingBar::create("tiendo_l.png");
    float scaleLoading = w / 1.3  / loadingBar->getContentSize().width;
    if (h/w <= 1.53) {
        scaleLoading = w / 2.6  / loadingBar->getContentSize().width;
    }
    loadingBar->setPosition(Vec2(w/2, heightLoadingBar));
    loadingBar->setScale(scaleLoading);
    loadingBar->setDirection(ui::LoadingBar::Direction::LEFT);
    loadingBar->setPercent(0); // Bắt đầu từ 0%
    this->addChild(loadingBar, 3);

    //auto hLoadingBar = loadingBar->getContentSize().height * scaleLoading - 5;
    star = Sprite::create("ngoisao.png");
    star->setScale(scaleLoading);
    star->setPosition(Vec2(loadingBar->getPosition().x - star->getContentSize().width * scaleLoading / 2 - loadingBar->getContentSize().width * scaleLoading / 2 , loadingBar->getPosition().y));
    star->setOpacity(0);
    this->addChild(star, 4);
    // Tạo một loading bar wrap (bao quanh)
    auto loadingBarWrap = ui::LoadingBar::create("tiendo_l_wrap.png");
    loadingBarWrap->setPosition(loadingBar->getPosition());
    loadingBarWrap->setScale(scaleLoading);
    loadingBarWrap->setDirection(ui::LoadingBar::Direction::LEFT);
    loadingBarWrap->setPercent(100);
    this->addChild(loadingBarWrap, 2);
}
void Play5::updateStarPosition(float percent)
{
    star->setOpacity(255);
    if (!loadingBar || !star) return;

    // Clamp percent (tránh lỗi >100 hoặc <0)
    percent = std::max(0.0f, std::min(percent, 100.0f));

    float width = loadingBar->getContentSize().width * loadingBar->getScaleX();
    float centerX = loadingBar->getPositionX();
    float posY = loadingBar->getPositionY();

    float startX = centerX - width * 0.5f;
    //float endX   = centerX + width * 0.5f;

    float x = 0;

    // chạy từ trái → phải
    x = startX + (percent / 100.0f) * width;

    starTargetPos = Vec2(x, posY);
    
    this->schedule([=](float dt){
        Vec2 current = star->getPosition();

        // lerp với tốc độ 0.3f
        float speed = 0.3f;
        Vec2 newPos = current + (starTargetPos - current) * speed;

        star->setPosition(newPos);
    }, "star_move");
}
void Play5::showMedal(float wScene, float hScene, std::string medal){
    auto fixIpad = 1;
    if (hScene / wScene <= 1.53)
    {
        fixIpad = 2;
    }

    auto iconMedal = Sprite::create(medal);

    float ratioMedal = 6 * fixIpad;
    float scaleiconMedal = wScene / ratioMedal / iconMedal->getContentSize().width;

    // Vị trí giữa màn hình
    iconMedal->setPosition(Vec2(wScene / 2, hScene * 0.7));

    // Bắt đầu từ 0
    iconMedal->setScale(0.0f);
    iconMedal->setOpacity(0);

    this->addChild(iconMedal, 15);

    /// ===== ANIMATION =====

    // Move lên khi xuất hiện (30px)
    auto moveUp = MoveBy::create(0.2f, Vec2(0, 30 / fixIpad));

    // Scale từ 0 -> x1.3
    auto scaleUp = ScaleTo::create(0.2f, scaleiconMedal * 1.3);

    // Fade in
    auto fadeIn = FadeTo::create(0.2f, 255);

    // Scale + Fade + Move lên cùng lúc
    auto spawnIn = Spawn::create(scaleUp, fadeIn, moveUp, nullptr);

    // Giữ 1 chút
    auto delay = DelayTime::create(0.4f);

    // Move xuống nhẹ khi hide
    auto moveDown = MoveBy::create(0.6f, Vec2(0, -15 / fixIpad));

    // Fade out
    auto fadeOut = FadeOut::create(0.6f);

    // Fade + Move xuống cùng lúc khi hide
    auto spawnOut = Spawn::create(fadeOut, moveDown, nullptr);

    // Xóa sprite sau khi xong
    auto remove = RemoveSelf::create();

    // Chạy sequence
    iconMedal->runAction(
        Sequence::create(
            spawnIn,
            delay,
            spawnOut,
            remove,
            nullptr
        )
    );
}
void Play5::createBg(float wScene, float hScene) {
//    this->addChild(cocos2d::LayerColor::create(Color4B().WHITE));
//    auto imageName = "bg-kid-top-3.png";
//    if (hScene / wScene <= 1.53)
//    {
//        imageName = "bg-top-ipad.png";
//
//    }
//    auto bgTopKid = ui::Button::create(imageName); // tao button
//    float ratioButtonkidTop = 1;
//    float scaleButtonKidTop = wScene / ratioButtonkidTop / bgTopKid->getContentSize().width;
//    bgTopKid->setScale(scaleButtonKidTop);
//    float yButtonKidTop = hScene - bgTopKid->getContentSize().height / 2 * scaleButtonKidTop;
//    float xButtonKidTop = bgTopKid->getContentSize().width / 2 * scaleButtonKidTop;
//    bgTopKid->setPosition(Vec2(xButtonKidTop, yButtonKidTop));
//    bgTopKid->setEnabled(false);
//    this->addChild(bgTopKid);//add button vao scene
//
//    auto imageNameBottom = "bg-kid.png";
//    if (hScene / wScene <= 1.53)
//    {
//        imageNameBottom = "bg-bottom-ipad.png";
//
//    }
//
//    auto bgKid = ui::Button::create(imageNameBottom); // tao button
//    float ratioButtonkid = 1;
//    float scaleButtonKid = wScene / ratioButtonkid / bgKid->getContentSize().width;
//    bgKid->setScale(scaleButtonKid);
//    float yButtonKid = 0 + bgKid->getContentSize().height / 2 * scaleButtonKid;
//    float xButtonKid = 0 + bgKid->getContentSize().width / 2 * scaleButtonKid;
//    bgKid->setPosition(Vec2(xButtonKid, yButtonKid));
//    bgKid->setEnabled(false);
//    this->addChild(bgKid);//add button vao scene
}
void Play5::createTraiDatAndPhiThuyenTenLua(float w, float h) {
    //create earth
    int earthIndex = cocos2d::RandomHelper::random_int(1, 7); //1, 7
    earth = ui::Button::create(StringUtils::format("earth3%d.png", earthIndex)); // tao button
    earth->setTouchEnabled(false);
    float ratiobgColor = 3.7;
    if (h / w <= 1.53)
    {
        ratiobgColor = 7.4;
    }
    float scalebgColor = w / ratiobgColor / earth->getContentSize().width;
    
    earth->setScale(scalebgColor);

    float ytitle = h * 0.865 - (earth->getContentSize().height / 2 * scalebgColor);
    float xtitle = w * 0.03 + (earth->getContentSize().width / 2 * scalebgColor);
    if (h / w <= 1.8)
    {
        //ytitle = 1.06 * h - (earth->getContentSize().height / 2 * scalebgColor);
    }
    
    earth->setPosition(Vec2(xtitle, ytitle));
    earth->setEnabled(true);
    this->addChild(earth, 2);
    
    earth->setOpacity(0);
    earth->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(0.5), nullptr));
    
    //animation for earth
    float timeAnimation = 0.9;
    auto playAnimationZoomOut1 = ScaleTo::create(timeAnimation, 1 * scalebgColor);
    auto playAnimationZoomOut2 = ScaleTo::create(timeAnimation, scalebgColor * 0.9);
    auto sq1 = Sequence::create(playAnimationZoomOut1, playAnimationZoomOut2, nullptr);
    
    auto sq2 = Sequence::create(RotateBy::create(30, 360), nullptr);
    
   
    
    //auto sq = Spawn::create(sq2, sq1, nullptr);
    earth->runAction((RepeatForever::create(sq1)));
    auto moveUp2 = MoveBy::create(1.3f, Vec2(0, 8));  // Bay lên nhẹ
    auto moveDown2 = MoveBy::create(1.3f, Vec2(0, -8)); // Bay xuống nhẹ
    auto bounce2 = RepeatForever::create(Sequence::create(moveUp2, moveDown2, nullptr));

    earth->runAction(bounce2);
   
    //create bom earth
    auto sprite_cacheEarthBom = SpriteFrameCache::getInstance();
    sprite_cacheEarthBom->addSpriteFramesWithFile("bom-tim.plist");
    Vector<SpriteFrame*> earthBom;
    earthBom.reserve(12);
    earthBom.pushBack(sprite_cacheEarthBom->getSpriteFrameByName("1.png"));
    earthBom.pushBack(sprite_cacheEarthBom->getSpriteFrameByName("2.png"));
    earthBom.pushBack(sprite_cacheEarthBom->getSpriteFrameByName("3.png"));
    earthBom.pushBack(sprite_cacheEarthBom->getSpriteFrameByName("4.png"));
    earthBom.pushBack(sprite_cacheEarthBom->getSpriteFrameByName("5.png"));
    earthBom.pushBack(sprite_cacheEarthBom->getSpriteFrameByName("6.png"));
    earthBom.pushBack(sprite_cacheEarthBom->getSpriteFrameByName("7.png"));
    earthBom.pushBack(sprite_cacheEarthBom->getSpriteFrameByName("8.png"));
    earthBom.pushBack(sprite_cacheEarthBom->getSpriteFrameByName("9.png"));
    earthBom.pushBack(sprite_cacheEarthBom->getSpriteFrameByName("10.png"));
    earthBom.pushBack(sprite_cacheEarthBom->getSpriteFrameByName("11.png"));
    earthBom.pushBack(sprite_cacheEarthBom->getSpriteFrameByName("12.png"));
    Animation* animationEarthBom = Animation::createWithSpriteFrames(earthBom, 0.05f, -1);
    Animate* animateEarthBom = Animate::create(animationEarthBom);

    bomEarthHienThi = Sprite::createWithSpriteFrame(earthBom.front());
    float ratioEarthBom = 3.5;
    if (h / w <= 1.53)
    {
        ratioEarthBom = 7;
    }
    float scaleEarthBom = w / ratioEarthBom / bomEarthHienThi->getContentSize().width;
    bomEarthHienThi->setScale(scaleEarthBom);

    //float ytitle = h * 0.9 - (earth->getContentSize().height / 2 * scalebgColor);
    float xtitleEarthBom = earth->getPositionX() + earth->getContentSize().width / 2 * scalebgColor - (bomEarthHienThi->getContentSize().width / 2 * scaleEarthBom) * 0.8;
    float ytitleEarthBom = earth->getPositionY() + earth->getContentSize().height / 2 * scalebgColor - (bomEarthHienThi->getContentSize().height / 2 * scaleEarthBom) * 0.9;
    
    bomEarthHienThi->setPosition(Vec2(xtitleEarthBom, ytitleEarthBom));
    bomEarthHienThi->runAction(animateEarthBom);
    this->addChild(bomEarthHienThi, 2);
    bomEarthHienThi->setOpacity(0);

    // create Phi thuyen
    auto viTriPhiThuyen = UserDefault::getInstance()->getIntegerForKey("phithuyendachon", 0) + 1;
    auto sprite_cachePhiThuyen = SpriteFrameCache::getInstance();
    sprite_cachePhiThuyen->addSpriteFramesWithFile(StringUtils::format("phithuyen0%d1.plist", viTriPhiThuyen));
    Vector<SpriteFrame*> phiThuyen;
    phiThuyen.reserve(2);
    phiThuyen.pushBack(sprite_cachePhiThuyen->getSpriteFrameByName(StringUtils::format("phithuyen0%d1a.png", viTriPhiThuyen)));
    phiThuyen.pushBack(sprite_cachePhiThuyen->getSpriteFrameByName(StringUtils::format("phithuyen0%d1b.png", viTriPhiThuyen)));
    Animation* animationPhiThuyen = Animation::createWithSpriteFrames(phiThuyen, 0.2f, -1);
    Animate* animatePhiThuyen = Animate::create(animationPhiThuyen);

    phithuyenHienThi = Sprite::createWithSpriteFrame(phiThuyen.front());
    float fixPhiThuyenType = 0;
    float fixPhiThuyenTypeEarth = 1;
    float fixPhiThuyenScale = 1;
    if (viTriPhiThuyen > 1) {
        fixPhiThuyenType = 20;
    }
    if (viTriPhiThuyen > 2) {
        fixPhiThuyenScale = 0.6;
        fixPhiThuyenTypeEarth = 0.5;
    }
//    if (viTriPhiThuyen == 3) {
//        fixPhiThuyenTypeEarth = 0.3;
//    }
    float ratioPhiThuyen = 5;
    if (h / w <= 1.53)
    {
        ratioPhiThuyen = 10;
    }
    float scalePhiThuyen = w / ratioPhiThuyen / phithuyenHienThi->getContentSize().width;
    phithuyenHienThi->setScale(scalePhiThuyen * fixPhiThuyenScale);

    //float ytitle = h * 0.9 - (earth->getContentSize().height / 2 * scalebgColor);
    float xtitlePhiThuyen = w * 0.95 - (phithuyenHienThi->getContentSize().width / 2 * scalePhiThuyen);
    float ytitlePhiThuyen = h * 0.84 - (phithuyenHienThi->getContentSize().height / 2 * scalePhiThuyen) + fixPhiThuyenType;
    
    phithuyenHienThi->setPosition(Vec2(xtitlePhiThuyen, ytitlePhiThuyen));
    phithuyenHienThi->runAction(animatePhiThuyen);
    this->addChild(phithuyenHienThi, 2);
    xDichPhiThuyen = xtitle + earth->getContentSize().width / 2 * scalebgColor * 0.9 * fixPhiThuyenTypeEarth + phithuyenHienThi->getContentSize().width / 2 * scalePhiThuyen;
    //animation for phi thuyen
    auto actionMoveTo = MoveTo::create(timeRemaining, Vec2( xDichPhiThuyen, ytitlePhiThuyen));
    phithuyenHienThi->runAction(actionMoveTo);
    
    // Tạo hành động nghiêng sang trái
    auto tiltLeft = RotateBy::create(0.7f, -4);  // Xoay -2 độ
    // Tạo hành động nghiêng sang phải
    auto tiltRight = RotateBy::create(0.7f, 4);   // Xoay +2 độ
    // Lặp lại hành động
    auto tilting = RepeatForever::create(Sequence::create(tiltLeft, tiltRight, nullptr));
    // Chạy animation cho phi thuyền
    phithuyenHienThi->runAction(tilting);
    
    
    auto moveUp = MoveBy::create(0.7f, Vec2(0, 7));
    auto moveDown = MoveBy::create(0.7f, Vec2(0, -7));
    auto waveMotion = RepeatForever::create(Sequence::create(moveUp, moveDown, nullptr));

    phithuyenHienThi->runAction(waveMotion);
    
    phithuyenHienThi->setOpacity(0);
    phithuyenHienThi->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(0.5), nullptr));
    
    
    //create bom phiThuyen
    auto sprite_cachePhiThuyenBom = SpriteFrameCache::getInstance();
    sprite_cachePhiThuyenBom->addSpriteFramesWithFile("bom-tim.plist");
    Vector<SpriteFrame*> phiThuyenBom;
    phiThuyenBom.reserve(12);
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("1.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("2.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("3.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("4.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("5.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("6.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("7.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("8.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("9.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("10.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("11.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("12.png"));
    Animation* animationPhiThuyenBom = Animation::createWithSpriteFrames(phiThuyenBom, 0.06f, -1);
    Animate* animatePhiThuyenBom = Animate::create(animationPhiThuyenBom);

    bomPhiThuyenHienThi = Sprite::createWithSpriteFrame(phiThuyenBom.front());
    float ratioPhiThuyenBom = 3.5;
    if (h / w <= 1.53)
    {
        ratioPhiThuyenBom = 7;
    }
    float scalePhiThuyenBom = w / ratioPhiThuyenBom / bomPhiThuyenHienThi->getContentSize().width;
    bomPhiThuyenHienThi->setScale(scalePhiThuyenBom);

    //float ytitle = h * 0.9 - (phiThuyen->getContentSize().height / 2 * scalebgColor);
    //float xtitlePhiThuyenBom = phi->getPositionX() + bomPhiThuyenHienThi->getContentSize().width / 2 * scalebgColor - (bomPhiThuyenHienThi->getContentSize().width / 2 * scalePhiThuyenBom) * 0.8;
    //float ytitlePhiThuyenBom = bomPhiThuyenHienThi->getPositionY() + bomPhiThuyenHienThi->getContentSize().height / 2 * scalebgColor - (bomPhiThuyenHienThi->getContentSize().height / 2 * scalePhiThuyenBom) * 0.9;
    
    bomPhiThuyenHienThi->setPosition(Vec2(w/2, h*0.86));
    bomPhiThuyenHienThi->runAction(animatePhiThuyenBom);
    this->addChild(bomPhiThuyenHienThi, 2);
    bomPhiThuyenHienThi->setOpacity(0);
    
    // create ten lua
    auto viTriTenLua = UserDefault::getInstance()->getIntegerForKey("tenluadachon", 0) + 1;
    auto sprite_cache = SpriteFrameCache::getInstance();
    sprite_cache->addSpriteFramesWithFile(StringUtils::format("tenlua0%d.plist", viTriTenLua));
    Vector<SpriteFrame*> tenlua;
    tenlua.reserve(2);
    tenlua.pushBack(sprite_cache->getSpriteFrameByName(StringUtils::format("tenlua0%da.png", viTriTenLua)));
    tenlua.pushBack(sprite_cache->getSpriteFrameByName(StringUtils::format("tenlua0%db.png", viTriTenLua)));
    Animation* animation = Animation::createWithSpriteFrames(tenlua, 0.2f, -1);
    Animate* animate = Animate::create(animation);

    tenluaHienThi = Sprite::createWithSpriteFrame(tenlua.front());
    float ratioTenLua = 8;
    if (h / w <= 1.53)
    {
        ratioTenLua = 16;
    }
    float scaleTenLua = w / ratioTenLua / tenluaHienThi->getContentSize().width;
    tenluaHienThi->setScale(scaleTenLua);

    //float ytitle = h * 0.9 - (earth->getContentSize().height / 2 * scalebgColor);
    float xtitleTenLua = w * 0.7 - (tenluaHienThi->getContentSize().width / 2 * scaleTenLua);
    
    tenluaHienThi->setPosition(Vec2(xtitleTenLua, h/2 * 1.05));
    if (h / w <= 1.53)
    {
        tenluaHienThi->setPosition(Vec2(xtitleTenLua, h/2 * 0.85));
    }
    tenluaHienThi->setRotation(90);
    tenluaHienThi->runAction(animate);
    this->addChild(tenluaHienThi, 2);
    tenluaHienThi->setOpacity(0);
    yTenLua = phithuyenHienThi->getPositionY() - phithuyenHienThi->getContentSize().width / 2 * scalePhiThuyen - tenluaHienThi->getContentSize().width / 2 * scaleTenLua;
    
    // create bom ten lua
    auto sprite_cacheBomTenLua = SpriteFrameCache::getInstance();
    sprite_cacheBomTenLua->addSpriteFramesWithFile("bom-ten-lua.plist");
    Vector<SpriteFrame*> bomTenLua;
    bomTenLua.reserve(2);
    bomTenLua.pushBack(sprite_cacheBomTenLua->getSpriteFrameByName("bom-ten-lua-1.png"));
    bomTenLua.pushBack(sprite_cacheBomTenLua->getSpriteFrameByName("bom-ten-lua-2.png"));
    bomTenLua.pushBack(sprite_cacheBomTenLua->getSpriteFrameByName("bom-ten-lua-3.png"));
    bomTenLua.pushBack(sprite_cacheBomTenLua->getSpriteFrameByName("bom-ten-lua-4.png"));
    bomTenLua.pushBack(sprite_cacheBomTenLua->getSpriteFrameByName("bom-ten-lua-5.png"));
    bomTenLua.pushBack(sprite_cacheBomTenLua->getSpriteFrameByName("bom-ten-lua-6.png"));
    bomTenLua.pushBack(sprite_cacheBomTenLua->getSpriteFrameByName("bom-ten-lua-7.png"));
    Animation* animationBomTenLua = Animation::createWithSpriteFrames(bomTenLua, 0.1f, -1);
    Animate* animateBomTenLua = Animate::create(animationBomTenLua);

    bomTenLuaHienThi = Sprite::createWithSpriteFrame(bomTenLua.front());
    float ratioBomTenLua = 7;
    if (h / w <= 1.53)
    {
        ratioBomTenLua = 14;
    }
    float scaleBomTenLua = w / ratioBomTenLua / bomTenLuaHienThi->getContentSize().width;
    bomTenLuaHienThi->setScale(scaleBomTenLua);

    //float ytitle = h * 0.9 - (earth->getContentSize().height / 2 * scalebgColor);
    float xtitleBomTenLua = w * 0.7 - (bomTenLuaHienThi->getContentSize().width / 2 * scaleBomTenLua);
    
    bomTenLuaHienThi->setPosition(Vec2(xtitleBomTenLua, h/2 * 1.05));
    bomTenLuaHienThi->runAction(animateBomTenLua);
    this->addChild(bomTenLuaHienThi, 2);
    bomTenLuaHienThi->setOpacity(0);


}
void Play5::createBgColor(float w, float h) {
    //create bg color
    int bgIndex = cocos2d::RandomHelper::random_int(1, 4);
    auto bgColor = ui::Button::create(StringUtils::format("bg0%d.jpg", bgIndex)); // tao button
    bgColor->setTouchEnabled(false);
    float ratiobgColor = 1;
    float scalebgColor = h / ratiobgColor / bgColor->getContentSize().height;
    if(scalebgColor < w / ratiobgColor / bgColor->getContentSize().width) {
        scalebgColor = w / ratiobgColor / bgColor->getContentSize().width;
    }
    //log("ti le test: %f", scalebgColor);
    //log("ti le test theo w: %f", w / ratiobgColor / bgColor->getContentSize().width);
    bgColor->setScale(scalebgColor);
    if (h / w <= 1.8)
    {
        //ytitle = 1.06 * h - (bgColor->getContentSize().height / 2 * scalebgColor);
    }
    if (h / w <= 1.53)
    {
        //ytitle = 1.3 * h - (bgColor->getContentSize().height / 2 * scalebgColor);
    }
    //bgColor->setPosition(Vec2(w / 2, ytitle));
    bgColor->setAnchorPoint(Vec2(0, 0));
    bgColor->setPosition(Vec2(0, 0));

    bgColor->setEnabled(true);
    this->addChild(bgColor, 1);
    
    
}
void Play5::createPopupFinish(float w, float h) {
    //int bgIndex = cocos2d::RandomHelper::random_int(1, 4);
    //popupthanhCong = ui::Button::create(StringUtils::format("popup-thanh-cong-%d.png", bgIndex)); // tao button
    auto imgNameThemDiem = Variable().diemNgauNhienImg.at(indexThemDiem);
    popupthanhCong = ui::Button::create(imgNameThemDiem.c_str());
    popupthanhCong->setTouchEnabled(false);
    float ratiopopupthanhCong = 3;
    if (h / w <= 1.53)
    {
        ratiopopupthanhCong = 7;
    }
    float scalepopupthanhCong = w / ratiopopupthanhCong / popupthanhCong->getContentSize().width;
    

    //log("ti le test: %f", scalepopupthanhCong);
    //log("ti le test theo w: %f", w / ratiopopupthanhCong / popupthanhCong->getContentSize().width);
    popupthanhCong->setScale(scalepopupthanhCong);
    if (h / w <= 1.8)
    {
        //ytitle = 1.06 * h - (popupthanhCong->getContentSize().height / 2 * scalepopupthanhCong);
    }

    //popupthanhCong->setPosition(Vec2(w / 2, ytitle));
    //popupthanhCong->setAnchorPoint(Vec2(0, 0));
    Vec2 positionPopupThanhCong = Vec2(w/2, h * 0.75);
    if (h / w <= 1.53)
    {
        positionPopupThanhCong = Vec2(w * 3 / 4, h * 0.85);
    }
    popupthanhCong->setPosition(positionPopupThanhCong);

    popupthanhCong->setEnabled(true);
    this->addChild(popupthanhCong, 12);
    popupthanhCong->setOpacity(0);
    popupthanhCong->setScale(0.6);
    
    popupthatBai = ui::Button::create("popup-that-bai-1.png"); // tao button
    popupthatBai->setTouchEnabled(false);
   

    
    popupthatBai->setScale(scalepopupthanhCong);
    if (h / w <= 1.8)
    {
        //ytitle = 1.06 * h - (popupthanhCong->getContentSize().height / 2 * scalepopupthanhCong);
    }
    if (h / w <= 1.53)
    {
        //ytitle = 1.3 * h - (popupthanhCong->getContentSize().height / 2 * scalepopupthanhCong);
    }
    //popupthanhCong->setPosition(Vec2(w / 2, ytitle));
    //popupthanhCong->setAnchorPoint(Vec2(0, 0));
    popupthatBai->setPosition(Vec2(w/2, h * 0.75));

    popupthatBai->setEnabled(true);
    this->addChild(popupthatBai, 12);
    popupthatBai->setOpacity(0);
    popupthatBai->setScale(0.6);
}
void Play5::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event){
    //Director::getInstance()->replaceScene(TransitionFade::create(0.5, MathLevel::createScene(typeOfW5), Color3B().WHITE));
}
void Play5::coundownTimer(float w, float h){
    switch (levelOfW5)
    {
        case 1:
            timeRemaining = Variable().timeBasic;
            timeMax = Variable().timeBasic;
            if (typeOfW5 == 3 || typeOfW5 == 2) {
                timeRemaining = Variable().timeBasicNhan;
                timeMax = Variable().timeBasicNhan;
            }
            break;
        case 2:
            timeRemaining = Variable().timeHard;
            timeMax = Variable().timeHard;
            if (typeOfW5 == 3 || typeOfW5 == 2) {
                timeRemaining = Variable().timeHardNhan;
                timeMax = Variable().timeHardNhan;
            }
            break;
        case 3:
            timeRemaining = Variable().timeVeryHard;
            timeMax = Variable().timeVeryHard;
            if (typeOfW5 == 3 || typeOfW5 == 2) {
                timeRemaining = Variable().timeVeryHardNhan;
                timeMax = Variable().timeVeryHardNhan;
            }
            break;
        default:
            timeRemaining = 20.f;
            timeMax = 20.f;
            break;
    }

   
    this->scheduleUpdate();


//    switch (typeOfW5)
//    {
//        //2 - nhan; 3 - chia; 4 - cong; 5 - tru; 1 - multi; 0 - sosanh
//        case 1:
//            loadingBar = ui::LoadingBar::create("loadingbar-mul.png");
//            break;
//        case 2:
//            loadingBar = ui::LoadingBar::create("loadingbar-nhan.png");
//            break;
//        case 3:
//            loadingBar = ui::LoadingBar::create("loadingbar-chia.png");
//            break;
//        case 4:
//            loadingBar = ui::LoadingBar::create("loadingbar-cong.png");
//            break;
//        case 5:
//            loadingBar = ui::LoadingBar::create("loadingbar-tru.png");
//            break;
//        default:
//            loadingBar = ui::LoadingBar::create("loadingbar-nhan.png");
//            break;
//    }
    
}
void Play5::update(float dt) {
    //log("time remain: %f", timeRemaining);
    // make sure game is still running
    if (timeRemaining > 0.f) {
        timeRemaining -= dt;
        if (isPause == true)
        {
            //log("da pause");
        }
        else{
            
        }
        // update the label or whatever displays the time here. if displaying the number, use ceilf to round
        // up the time remaining so that you don't display lots of numbers after the decimal point

        // check if time ran out
        if (timeRemaining <= 0.f && isPause == false) {
            // timer ran out; react here

            handlerClickAnswer(0, buttonAnswer1);
            log("Het gio");
        }
    }
}
void Play5::createScore(float w, float h){
    scoreString = cocos2d::__String::createWithFormat("%i", score);//score
    lblScore = Label::createWithTTF(scoreString->getCString(), "fonts/ComicNeue-Bold.ttf", 0.08 * w);
    if (h / w <= 1.53)
    {
        lblScore = Label::createWithTTF(scoreString->getCString(), "fonts/ComicNeue-Bold.ttf", 0.05 * w);
    }
    lblScore->setColor(Color3B().WHITE);
    // position the label on the center of the screen
    float xScore = w - (lblScore->getContentSize().width / 2 + 14);
    lblScore->setPosition(Vec2(xScore, h - 6 - (lblScore->getContentSize().height / 2)));
    // add the label as a child to this layer
    addChild(lblScore, 11);
    
    //create scoreIcon
    scoreIcon = ui::Button::create("gold.png"); // tao button
    scoreIcon->setTouchEnabled(false);
    float ratioscoreIcon = 10;
    float scaleI = 1;
    if (h / w <= 1.53)
    {
        ratioscoreIcon = 20;
        scaleI = 0.55;
        
    }
    float scalescoreIcon = w / ratioscoreIcon / scoreIcon->getContentSize().width;
    scoreIcon->setScale(scalescoreIcon);
    scoreIcon->setAnchorPoint(Vec2({1, 0.5}));
    scoreIcon->setPosition(Vec2(lblScore->getPositionX() - 3 - lblScore->getContentSize().width / 2, h - 6 - (lblScore->getContentSize().height / 2)));
    scoreIcon->setEnabled(true);
    this->addChild(scoreIcon, 11);
    
    //show lblscore with animation
    lblScore->setOpacity(0);
    
    //create scoreIconEffect
//    auto scoreIconEffect = ui::Button::create("score.png"); // tao button
//    scoreIconEffect->setTouchEnabled(false);
//    float ratioscoreIconEffect = 12;
//    float scalescoreIconEffect = w / ratioscoreIconEffect / scoreIconEffect->getContentSize().width;
//    scoreIconEffect->setScale(scalescoreIconEffect);
//    scoreIconEffect->setAnchorPoint(Vec2({1, 0.5}));
//    scoreIconEffect->setPosition(Vec2(w / 2, h * 0.825));
//    scoreIconEffect->setEnabled(true);
//    this->addChild(scoreIconEffect, 11);
//
//    scoreIconEffect->setOpacity(0);
    
    float valScale = 1.4;
    float time = 0.1;
    lblScore->runAction(Sequence::create(Spawn::create(ScaleTo::create(0, valScale), FadeOut::create(0), nullptr), Spawn::create(ScaleTo::create(time, 1), FadeIn::create(time), nullptr), nullptr));

    
    
    scoreIcon->setOpacity(0);
    scoreIcon->runAction(Sequence::create(Spawn::create(ScaleTo::create(0, 1), FadeOut::create(0), nullptr), Spawn::create(ScaleTo::create(time, scaleI), FadeIn::create(time), nullptr), nullptr));
    
//    scoreIconEffect->runAction(Sequence::create(
//
//        Spawn::create(
//            ScaleTo::create(0, 0),
//            FadeOut::create(0), nullptr),
//
//            Spawn::create(
//                ScaleTo::create(0.3, 0.65),
//                FadeIn::create(0.3), nullptr),
//
//                Spawn::create(
//                  DelayTime::create(0.2),
//                  MoveTo::create(0.6, Vec2(lblScore->getPositionX() - 12, h - 6 - (lblScore->getContentSize().height / 2))),
//                        FadeOut::create(0.6), nullptr),
//
//            nullptr
//        )
//
//    );
    
}
void Play5::createAnswer(float w, float h){

    float fontSize = 0.09 * w;

    buttonAnswer1 = ui::Button::create(ImageNamDapAn->getCString()); // tao button
    buttonAnswer1->setPressedActionEnabled(true);
    buttonAnswer1->setZoomScale(0);
    buttonAnswer1->setScale9Enabled(true);
    buttonAnswer1->setTitleText(HuyFunctions().NumberToString(answer1));
    if (answer1 == correctAnswer)
    {
        buttonAnswer1->setTag(1);
    }
    else
    {
        buttonAnswer1->setTag(0);
    }
    buttonAnswer1->setTitleFontName("fonts/ComicNeue-Bold.ttf");

    buttonAnswer1->setTitleColor(Color3B().WHITE);
    buttonAnswer1->setTouchEnabled(true);
    float ratioButtonAnswer1 = 2.8;
    if (h / w <= 1.53)
    {
        ratioButtonAnswer1 = 6.5;
        fontSize = 0.04 * w;
    }
    
    scaleButtonAnswer1 = w / ratioButtonAnswer1 / buttonAnswer1->getContentSize().width;
    buttonAnswer1->setScale(scaleButtonAnswer1);
    buttonAnswer1->setTitleFontSize(fontSize / scaleButtonAnswer1);
    float yButtonAnswer1 = 50 + buttonAnswer1->getContentSize().height / 2 * scaleButtonAnswer1;
    
    if (h / w <= 1.8)
    {
        yButtonAnswer1 = 20 + buttonAnswer1->getContentSize().height / 2 * scaleButtonAnswer1;
    }
    buttonAnswer1->setPosition(Vec2(w / 4 + w/25, yButtonAnswer1));
    if (h / w <= 1.53)
    {
        buttonAnswer1->setPosition(Vec2(w / 3 + w / 20, yButtonAnswer1));
    }
    buttonAnswer1->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                if (answer1 != correctAnswer)
                {
                    buttonAnswer1->loadTextureNormal("box-wrong.png");
                    buttonAnswer1->setTitleColor(Color3B().WHITE);
                }
                handlerClickAnswer(answer1, buttonAnswer1);

                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonAnswer1, 10);//add button vao scene

    buttonAnswer2 = ui::Button::create(ImageNamDapAn->getCString()); // tao button
    buttonAnswer2->setPressedActionEnabled(true);
    buttonAnswer2->setZoomScale(0);
    buttonAnswer2->setScale9Enabled(true);
    buttonAnswer2->setTitleText(HuyFunctions().NumberToString(answer2));
    if (answer2 == correctAnswer)
    {
        buttonAnswer2->setTag(1);
    }
    else
    {
        buttonAnswer2->setTag(0);
    }
    buttonAnswer2->setTitleFontName("fonts/ComicNeue-Bold.ttf");
    buttonAnswer2->setTitleFontSize(fontSize / scaleButtonAnswer1);
    buttonAnswer2->setTitleColor(Color3B().WHITE);
    buttonAnswer2->setTouchEnabled(true);
    float scaleButtonAnswer2 = w / ratioButtonAnswer1 / buttonAnswer2->getContentSize().width;
    buttonAnswer2->setScale(scaleButtonAnswer2);
    //float yButtonAnswer2 = 50 + buttonAnswer2->getContentSize().height / 2 * scaleButtonAnswer2;

    buttonAnswer2->setPosition(Vec2(w * 3 / 4 - w/25, yButtonAnswer1));
    if (h / w <= 1.53)
    {
        buttonAnswer2->setPosition(Vec2(w * 4 / 6 - w / 20, yButtonAnswer1));
       
    }
    buttonAnswer2->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                if (answer2 != correctAnswer)
                {
                    buttonAnswer2->loadTextureNormal("box-wrong.png");
                    buttonAnswer2->setTitleColor(Color3B().WHITE);
                }
                handlerClickAnswer(answer2,buttonAnswer2);
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                buttonAnswer2->loadTextureNormal("box-wrong.png");
                break;
            default:
                break;
        }
    });
    addChild(buttonAnswer2, 10);//add button vao scene

    buttonAnswer3 = ui::Button::create(ImageNamDapAn->getCString()); // tao button
    buttonAnswer3->setPressedActionEnabled(true);
    buttonAnswer3->setZoomScale(0);
    buttonAnswer3->setScale9Enabled(true);
    buttonAnswer3->setTitleText(HuyFunctions().NumberToString(answer3));
    if (answer3 == correctAnswer)
    {
        buttonAnswer3->setTag(1);
    }
    else
    {
        buttonAnswer3->setTag(0);
    }
    buttonAnswer3->setTitleFontName("fonts/ComicNeue-Bold.ttf");
    buttonAnswer3->setTitleFontSize(fontSize / scaleButtonAnswer1);
    buttonAnswer3->setTitleColor(Color3B().WHITE);
    buttonAnswer3->setTouchEnabled(true);
    float scaleButtonAnswer3 = w / ratioButtonAnswer1 / buttonAnswer3->getContentSize().width;
    buttonAnswer3->setScale(scaleButtonAnswer3);
    float yButtonAnswer3 = yButtonAnswer1 + buttonAnswer3->getContentSize().height * scaleButtonAnswer3 + 20;

    buttonAnswer3->setPosition(Vec2(buttonAnswer1->getPositionX(), yButtonAnswer3));
    
//    if (h / w <= 1.53)
//    {
//        buttonAnswer3->setPosition(Vec2(w / 2.5 - 10, yButtonAnswer3));
//
//
//    }
    buttonAnswer3->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                if (answer3 != correctAnswer)
                {
                    buttonAnswer3->loadTextureNormal("box-wrong.png");
                    buttonAnswer3->setTitleColor(Color3B().WHITE);
                }
                handlerClickAnswer(answer3, buttonAnswer3);
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonAnswer3, 10);//add button vao scene

    buttonAnswer4 = ui::Button::create(ImageNamDapAn->getCString()); // tao button
    buttonAnswer4->setPressedActionEnabled(true);
    buttonAnswer4->setZoomScale(0);
    buttonAnswer4->setScale9Enabled(true);
    buttonAnswer4->setTitleText(HuyFunctions().NumberToString(answer4));
    if (answer4 == correctAnswer)
    {
        buttonAnswer4->setTag(1);
    }
    else
    {
        buttonAnswer4->setTag(0);
    }
    buttonAnswer4->setTitleFontName("fonts/ComicNeue-Bold.ttf");
    buttonAnswer4->setTitleFontSize(fontSize / scaleButtonAnswer1);
    buttonAnswer4->setTitleColor(Color3B().WHITE);
    buttonAnswer4->setTouchEnabled(true);
    float scaleButtonAnswer4 = w / ratioButtonAnswer1 / buttonAnswer4->getContentSize().width;
    buttonAnswer4->setScale(scaleButtonAnswer4);


    buttonAnswer4->setPosition(Vec2(buttonAnswer2->getPositionX(), yButtonAnswer3));
//    if (h / w <= 1.53)
//    {
//        buttonAnswer4->setPosition(Vec2(w / 2.5 + 10 + buttonAnswer2->getContentSize().width * scaleButtonAnswer2, yButtonAnswer3));
//
//    }
    buttonAnswer4->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                if (answer4 != correctAnswer)
                {
                    buttonAnswer4->loadTextureNormal("box-wrong.png");
                    buttonAnswer4->setTitleColor(Color3B().WHITE);
                }
                handlerClickAnswer((answer4), buttonAnswer4);
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonAnswer4, 10);//add button vao scene


    //show 4 button with animation

    //show lblscore with animation
    float valScale = 1.4;
    float time = 0.1;
    animationShowAnswer(buttonAnswer1, time, valScale);
    animationShowAnswer(buttonAnswer2, time, valScale);
    animationShowAnswer(buttonAnswer3, time, valScale);
    animationShowAnswer(buttonAnswer4, time, valScale);

    if (strcmp(loaiPhepTinh->getCString(), "ss") == 0) {
        phepTinhSoSanh1 = true;
        
        if (buttonAnswer1->getTitleText() == "1") {
            
            buttonAnswer1->loadTextureNormal(ImageNamDapAnV->getCString());
            buttonAnswer2->loadTextureNormal(ImageNamDapAnX->getCString());
        }else{
            buttonAnswer1->loadTextureNormal(ImageNamDapAnX->getCString());
            buttonAnswer2->loadTextureNormal(ImageNamDapAnV->getCString());
        }
        buttonAnswer1->setTitleText("");
        buttonAnswer2->setTitleText("");
        buttonAnswer3->setVisible(false);
        buttonAnswer4->setVisible(false);
        //log("ban dang lam bai so sanh");
    }
    
    /*
    buttonPause = ui::Button::create("pause.png", "pause-active.png"); // tao button

    float ratiobuttonSelectLevel = 9;
    float scalebuttonPause = w / ratiobuttonSelectLevel / buttonPause->getContentSize().width;
    buttonPause->setScale(scalebuttonPause);
    float yPause = 10 + yButtonAnswer3 + buttonAnswer3->getContentSize().height * scaleButtonAnswer3 / 2 + buttonPause->getContentSize().height * scalebuttonPause / 2;
    buttonPause->setPosition(Vec2(w / 2, yPause));
    buttonPause->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        switch (type)
        {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            ((ui::Button*) sender)->setEnabled(false);
            if (UserDefault::getInstance()->getIntegerForKey("KEY") > 0)
            {
                isPause = true;
                ((ui::Button*) sender)->loadTextureNormal("pause-active.png");

                UserDefault::getInstance()->setIntegerForKey("KEY", UserDefault::getInstance()->getIntegerForKey("KEY") - 1);
                //animation -1

                //restart valueKey Label
                removeChild(valueKey3);
                createValueKey(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height);
            }
            else{
                ((ui::Button*) sender)->setEnabled(true);
                if (sound == 0)
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/buzz.mp3");
                }
                //animation key if keyvalue = 0;
            }
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            //log("Button 1 clicked");
            break;
        default:
            break;
        }
    });
    addChild(buttonPause);
    */
}
void Play5::animationShowAnswer(cocos2d::ui::Button* button, float time, float scale){
    auto opButton1 = FadeOut::create(0);
    button->setOpacity(0);

    auto scaleButton1 = ScaleTo::create(0, scaleButtonAnswer1 * scale);
    auto sqButton1 = Spawn::create(scaleButton1, opButton1, nullptr);
    auto scaleTitleB = ScaleTo::create(time, scaleButtonAnswer1);
    auto opaB = FadeIn::create(time);
    auto sqB = Spawn::create(scaleTitleB, opaB, nullptr);
    auto sq = Sequence::create(sqButton1, sqB, nullptr);
    button->runAction(sq);
}

void Play5::shakeScreen2(Node* target, float duration, float strength, float delayTime) {
    auto moveLeft = MoveBy::create(0.05f, Vec2(-strength, 0));
    auto moveRight = MoveBy::create(0.05f, Vec2(strength, 0));
    auto moveUp = MoveBy::create(0.05f, Vec2(0, strength));
    auto moveDown = MoveBy::create(0.05f, Vec2(0, -strength));

    auto shakeSequence = Sequence::create(moveLeft, moveRight, moveUp, moveDown, nullptr);
    auto repeatShake = Repeat::create(shakeSequence, duration / 0.2f);

    target->runAction(Sequence::create(DelayTime::create(delayTime), repeatShake, NULL));
}
void Play5::createTemplateGuideLevel1(float w, float h) {
    if ( typeOfW5 == 4 or typeOfW5 == 5 or typeOfW5 == 6) {
        hasGuide = true;
    }
}

void Play5::createTemplateGuideLevel2(float w, float h) {
    if ( typeOfW5 == 4 or typeOfW5 == 5 or typeOfW5 == 6) {
        hasGuide = true;
    }
    
}
void Play5::createTemplateGuideLevel2Cong(float w, float h) {
    int congDonVi = HuyFunctions().getSoHangDonVi(soA) + HuyFunctions().getSoHangDonVi(soB);
    
    lblPhepToan->runAction(FadeOut::create(0.1));
    auto fontStyle = "fonts/DeliusSwashCaps.ttf";
    float fontSize = 0.18 * w;
    if (h / w > 1.53 && h / w <= 1.8){
        fontSize = 0.15 * w;
    }
    if (h / w <= 1.53)
    {
        fontSize = 0.09 * w;
    }

    float time = 0.2;
    float width = 50.0f;
    float fixWithIpad = 1.0f;
    if (h / w <= 1.53)
    {
        fixWithIpad = 0.5f;
    }
    if (levelOfW5 > 1) {
        width = 70.0f;
    }
    float height = 3.0f * fixWithIpad;
    width = width * fixWithIpad;
    cocos2d::Vec2 origin(w / 2 - width / 2 - 16 * fixWithIpad, lblPhepToan->getPositionY() + lblPhepToan->getContentSize().height / 2);
    gachChan = HuyFunctions().createChuNhatBoGoc(width, height, origin);
    this->addChild(gachChan, 2);
    
    lblB = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangDonVi(soB))->getCString(), fontStyle, fontSize);
    float xLblB = w / 2;
    lblB->setPosition(Vec2(xLblB, lblPhepToan->getPositionY() + lblPhepToan->getContentSize().height));
    addChild(lblB, 2);
    lblB->setAlignment(TextHAlignment:: CENTER);
    lblB->setOpacity(0);
    lblB->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    if (soB > 9) {
        lblBHangChuc = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangChuc(soB))->getCString(), fontStyle, fontSize);
        
        lblBHangChuc->setPosition(Vec2(lblB->getPositionX() - lblB->getContentSize().height * 0.51, lblB->getPositionY()));
        addChild(lblBHangChuc, 2);
        lblBHangChuc->setAlignment(TextHAlignment:: CENTER);
        lblBHangChuc->setOpacity(0);
        lblBHangChuc->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    }
    
    loaiPhepTinh = cocos2d::__String::createWithFormat("%s", "+");
    
    lblLoaiPhepTinh = Label::createWithTTF(loaiPhepTinh->getCString(), fontStyle, fontSize);
    lblLoaiPhepTinh->setPosition(Vec2(w/2 - lblB->getContentSize().height * 0.82, lblB->getPositionY() + lblB->getContentSize().height / 2));
    addChild(lblLoaiPhepTinh, 2);
    lblLoaiPhepTinh->setOpacity(0);
    lblLoaiPhepTinh->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblA = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangDonVi(soA))->getCString(), fontStyle, fontSize);
    lblA->setPosition(Vec2(w / 2, lblLoaiPhepTinh->getPositionY() + lblLoaiPhepTinh->getContentSize().height / 2));
    addChild(lblA, 2);
    lblA->setAlignment(TextHAlignment:: CENTER);
    lblA->setOpacity(0);
    lblA->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblAHangChuc = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangChuc(soA))->getCString(), fontStyle, fontSize);
    lblAHangChuc->setPosition(Vec2(lblA->getPositionX() - lblA->getContentSize().height * 0.51, lblA->getPositionY()));
    addChild(lblAHangChuc, 2);
    lblAHangChuc->setOpacity(0);
    lblAHangChuc->setAlignment(TextHAlignment:: CENTER);
    lblAHangChuc->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblDapAnHangDV = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangDonVi(soA + soB))->getCString(), fontStyle, fontSize);
    lblDapAnHangDV->setPosition(Vec2(w / 2, lblB->getPositionY() - lblB->getContentSize().height * 1.1));
    addChild(lblDapAnHangDV, 2);
    lblDapAnHangDV->setAlignment(TextHAlignment:: CENTER);
    lblDapAnHangDV->setOpacity(0);
    //lblDapAnHangDV->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblDapAnHangChuc = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangChuc(soA + soB))->getCString(), fontStyle, fontSize);
    if (congDonVi > 9) {
        lblDapAnHangChuc->setString(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangChuc(soA + soB) - 1)->getCString());
    }
    lblDapAnHangChuc->setPosition(Vec2(lblAHangChuc->getPositionX(), lblDapAnHangDV->getPositionY()));
    addChild(lblDapAnHangChuc, 2);
    lblDapAnHangChuc->setOpacity(0);
    lblDapAnHangChuc->setAlignment(TextHAlignment:: CENTER);
    //lblDapAnHangChuc->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    
    
    lblDapAnPhanNhoDV = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangDonVi(congDonVi))->getCString(), fontStyle, fontSize * 0.8);
    lblDapAnPhanNhoDV->setPosition(Vec2(w * 0.8, lblLoaiPhepTinh->getPositionY()));
    addChild(lblDapAnPhanNhoDV, 2);
    lblDapAnPhanNhoDV->setAlignment(TextHAlignment:: CENTER);
    lblDapAnPhanNhoDV->setOpacity(0);
    //lblDapAnPhanNhoDV->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblDapAnPhanNhoHangChuc = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangChuc(congDonVi))->getCString(), fontStyle, fontSize * 0.8);
    lblDapAnPhanNhoHangChuc->setPosition(Vec2(lblDapAnPhanNhoDV->getPositionX() - lblDapAnPhanNhoDV->getContentSize().height * 0.51, lblLoaiPhepTinh->getPositionY()));
    addChild(lblDapAnPhanNhoHangChuc, 2);
    lblDapAnPhanNhoHangChuc->setOpacity(0);
    lblDapAnPhanNhoHangChuc->setAlignment(TextHAlignment:: CENTER);
    //lblDapAnPhanNhoHangChuc->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    
    if (flagB1ChuSo) {
        xLblB = w / 2 + (lblA->getContentSize().width - lblB->getContentSize().width) / 2;
        lblB->setPosition(Vec2(xLblB, lblB->getPositionY()));
    }
    //animation cho hang don vi - phong to
    float timeAnimate = 0.2;
    float delayTime = 2;
    float fixTimeCoNho = 0;
    if (congDonVi > 9) {
        fixTimeCoNho = 2;
    }
    lblB->runAction(
        Sequence::create(
            DelayTime::create(delayTime),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale() * 1.3), TintTo::create(timeAnimate, Color3B::GREEN), nullptr)
            , NULL)
        );
    lblA->runAction(
        Sequence::create(
            DelayTime::create(delayTime),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale() * 1.3), TintTo::create(timeAnimate, Color3B::GREEN), nullptr)
            , NULL)
        );
    
    log("cong don vi: %i", congDonVi);
    auto playSound = CallFunc::create([this]() {
        if (sound == 0)
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/hit.mp3");
        }
    });
    
    auto congCaPhanNho = CallFunc::create([this]() {
        if (sound == 0)
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/hit.mp3");
        }
        lblDapAnHangChuc->setString(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangChuc(soA + soB))->getCString());
    });
    
    if (congDonVi > 9) { // co nho
        lblDapAnPhanNhoDV->runAction(
            Sequence::create(
                DelayTime::create(delayTime + 0.5),
                Spawn::create(
                           FadeIn::create(timeAnimate),
                           ScaleTo::create(timeAnimate, lblB->getScale() * 1.3),
                           TintTo::create(timeAnimate, Color3B::YELLOW),
                           //move to lblDapAnHangDV
                           
                           nullptr),
                 DelayTime::create(2),
                 MoveTo::create(timeAnimate * 3, Vec2(lblDapAnHangDV->getPositionX(), lblDapAnHangDV->getPositionY())),
                 FadeOut::create(timeAnimate)
                , NULL)
            );
            lblDapAnPhanNhoHangChuc->runAction(
                Sequence::create(
                    DelayTime::create(delayTime + 0.5),
                         Spawn::create(
                               FadeIn::create(timeAnimate),
                               ScaleTo::create(timeAnimate, lblB->getScale() * 1.3),
                               TintTo::create(timeAnimate, Color3B::YELLOW),
                               
                               nullptr),
                     DelayTime::create(5.5),
                     MoveTo::create(timeAnimate * 3, Vec2(lblDapAnHangChuc->getPositionX(), lblDapAnHangChuc->getPositionY())),
                     //DelayTime::create(0.3),
                     FadeOut::create(timeAnimate)
                                 
                    , NULL)
                );
    }
    lblDapAnHangDV->runAction(
        Sequence::create(
                         DelayTime::create(delayTime + fixTimeCoNho * 1.5 + 0.5),
                 Spawn::create(
                       FadeIn::create(timeAnimate),
                       ScaleTo::create(timeAnimate, lblB->getScale() * 1.3),
                        playSound,
                       TintTo::create(timeAnimate, Color3B::GREEN),
                       nullptr)
            , NULL)
        );
    
    //hang don vi - thu nho va hien dap an don vi
    lblB->runAction(
        Sequence::create(
            DelayTime::create(delayTime + fixTimeCoNho + 2),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale()), TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
            , NULL)
        );
    lblA->runAction(
        Sequence::create(
            DelayTime::create(delayTime + fixTimeCoNho + 2),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale()), TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
            , NULL)
        );
    
    
    
    lblDapAnHangDV->runAction(
        Sequence::create(
            DelayTime::create(delayTime + fixTimeCoNho + 2),
            Spawn::create(
                    ScaleTo::create(timeAnimate, lblB->getScale()),
                    
                    TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
            , NULL)
        );
    lblAHangChuc->runAction(
        Sequence::create(
            DelayTime::create(delayTime + fixTimeCoNho + 2.5),
            Spawn::create(
                  ScaleTo::create(timeAnimate, lblB->getScale() * 1.3),
                  
                  TintTo::create(timeAnimate, Color3B::GREEN), nullptr)
            , NULL)
        );
    
    if (soB > 9) {
        lblBHangChuc->runAction(
            Sequence::create(
                DelayTime::create(delayTime + fixTimeCoNho + 2.5),
                Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale() * 1.3), TintTo::create(timeAnimate, Color3B::GREEN), nullptr)
                , NULL)
            );
    }
    
    if (congDonVi > 9) {
        lblDapAnHangChuc->runAction(
            Sequence::create(
                             DelayTime::create(delayTime + fixTimeCoNho * 1.3 + 3),
                     Spawn::create(
                           FadeIn::create(timeAnimate),
                           ScaleTo::create(timeAnimate, lblB->getScale() * 1.3),
                            playSound,
                           TintTo::create(timeAnimate, Color3B::GREEN),
                           
                           nullptr),
                             DelayTime::create(1),
                             congCaPhanNho
                , NULL)
            );
    } else {
        lblDapAnHangChuc->runAction(
            Sequence::create(
                             DelayTime::create(delayTime + fixTimeCoNho * 1.3 + 3),
                     Spawn::create(
                           FadeIn::create(timeAnimate),
                           ScaleTo::create(timeAnimate, lblB->getScale() * 1.3),
                            playSound,
                           TintTo::create(timeAnimate, Color3B::GREEN),
                           nullptr)
                , NULL)
            );
    }
    
    //hang chuc thu nho
    
    lblAHangChuc->runAction(
        Sequence::create(
            DelayTime::create(delayTime + fixTimeCoNho + 5),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale()),
            TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
            , NULL)
        );
    if (soB > 9) {
        lblBHangChuc->runAction(
            Sequence::create(
                DelayTime::create(delayTime + fixTimeCoNho + 5),
                Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale()),
                TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
                , NULL)
            );
    }
    
    lblDapAnHangChuc->runAction(
        Sequence::create(
            DelayTime::create(delayTime + fixTimeCoNho * 1.5 + 5),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale()),
            TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
            , NULL)
        );
    
    //2 button retry va OK
    auto buttonRetry = ui::Button::create("button-retry.png"); // tao button
    buttonRetry->setPressedActionEnabled(true);
    buttonRetry->setZoomScale(0);
    buttonRetry->setScale9Enabled(true);
    
    buttonRetry->setTouchEnabled(true);
    float ratiobuttonRetry = 4.5;
    //float fixHeightIpad = 1.8;
    if (h / w <= 1.53)
    {
        ratiobuttonRetry = 9;
        
        //fixHeightIpad = 1.2;
    }
    
    float scalebuttonRetry = w / ratiobuttonRetry / buttonRetry->getContentSize().width;
    buttonRetry->setScale(scalebuttonRetry);
    
    float ybuttonRetry = buttonRetry->getContentSize().height * scalebuttonRetry;
   
    buttonRetry->setPosition(Vec2(w / 4, ybuttonRetry));
    
    buttonRetry->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        auto clickedButton = static_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                if (sound == 0)
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
                }
                
                HuyFunctions().animationClickedButton(clickedButton, [this, w, h, clickedButton](){
                    for (auto icon : listIcon) {
                        removeChild(icon);
                    }
                    for (auto b : listBgNumber) {
                        removeChild(b);
                    }
                    listIcon.clear();
                    listBgNumber.clear();
                    if (clickedButton) {
                        auto scaleOut = ScaleTo::create(0.2f, 0); // Thu nhỏ Button
                        auto remove = CallFunc::create([clickedButton]() {
                            clickedButton->removeFromParent();
                            log("Button đã bị xóa sau hiệu ứng!");
                        });
                        clickedButton->runAction(Sequence::create(scaleOut, remove, nullptr));
                    }
                    
                    if (buttonOK) {
                        auto scaleOut = ScaleTo::create(0.2f, 0); // Thu nhỏ Button
                       
                        buttonOK->runAction(Sequence::create(scaleOut, nullptr));
                        removeChild(lblA);
                        removeChild(lblAHangChuc);
                        removeChild(lblB);
                        removeChild(lblBHangChuc);
                        removeChild(gachChan);
                        removeChild(lblDapAnHangDV);
                        removeChild(lblDapAnHangChuc);
                        removeChild(lblDapAnPhanNhoDV);
                        removeChild(lblDapAnPhanNhoHangChuc);
                    }
                    
                    if (hasGuide) {
                        createTemplateGuideLevel2Cong(wScenePlay, hScenePlay);
                    }
                });
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonRetry, 2);//add button vao scene
    
    buttonOK = ui::Button::create("okbutton.png"); // tao button
    buttonOK->setPressedActionEnabled(true);
    buttonOK->setZoomScale(0);
    buttonOK->setScale9Enabled(true);
    
    buttonOK->setTouchEnabled(true);
   
    buttonOK->setScale(scalebuttonRetry);
  
   
    buttonOK->setPosition(Vec2(3 * w / 4, ybuttonRetry));
    
    buttonOK->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        auto button = static_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                if (sound == 0)
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
                }
                
                HuyFunctions().animationClickedButton(button, [this](){
                    moveSceneGameOver();
                });

                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonOK, 2);//add button vao scene
    buttonOK->setScale(0);
    buttonRetry->setScale(0);
    buttonOK->runAction(Sequence::create(DelayTime::create(delayTime + fixTimeCoNho + 5), ScaleTo::create(0.3, scalebuttonRetry), NULL));//step1 + step2 + 0.2

    buttonRetry->runAction(Sequence::create(DelayTime::create(delayTime + fixTimeCoNho + 5 + 0.3), ScaleTo::create(0.3, scalebuttonRetry), NULL));
}
void Play5::createTemplateGuideLevel2SoSanh(float w, float h) {
    lblPhepToan->runAction(FadeOut::create(0.1));
    auto fontStyle = "fonts/DeliusSwashCaps.ttf";
    float fontSize = 0.18 * w;
    if (h / w > 1.53 && h / w <= 1.8){
        fontSize = 0.15 * w;
    }
    if (h / w <= 1.53)
    {
        fontSize = 0.09 * w;
    }

    float time = 0.2;
    float width = 50.0f;
    float xLblA = w / 4;
    float xLblB = 3 * w / 4;
    if (h / w <= 1.53)
    {
        xLblA = 3 * w / 8;
        xLblB = 5 * w / 8;
    }
    float delayTime = 2;
    float fixTimeCoNho = 0;
    
    lblA = Label::createWithTTF(phepTinhSmallA.c_str(), fontStyle, fontSize);
    
    lblA->setPosition(Vec2(xLblA, lblPhepToan->getPositionY() + lblPhepToan->getContentSize().height));
    addChild(lblA, 2);
    lblA->setAlignment(TextHAlignment:: CENTER);
    //lblA->setOpacity(0);
    lblA->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblB = Label::createWithTTF(phepTinhSmallB.c_str(), fontStyle, fontSize);
    
    lblB->setPosition(Vec2(xLblB, lblA->getPositionY()));
    addChild(lblB, 2);
    lblB->setAlignment(TextHAlignment:: CENTER);
    //lblB->setOpacity(0);
    lblB->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblAKetQua = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", soA)->getCString(), fontStyle, fontSize * 0.8);
    lblAKetQua->setPosition(Vec2(xLblA, lblA->getPositionY() - lblA->getContentSize().height));
    addChild(lblAKetQua, 2);
    lblAKetQua->setColor(Color3B::GREEN);
    lblAKetQua->setAlignment(TextHAlignment:: CENTER);
    lblAKetQua->setOpacity(0);
    lblAKetQua->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblBKetQua = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", soB)->getCString(), fontStyle, fontSize * 0.8);
    lblBKetQua->setPosition(Vec2(xLblB, lblAKetQua->getPositionY()));
    addChild(lblBKetQua, 2);
    lblBKetQua->setAlignment(TextHAlignment:: CENTER);
    lblBKetQua->setOpacity(0);
    lblBKetQua->setColor(Color3B::GREEN);
    lblBKetQua->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));

    auto ketQuaSoSanh = ">";
    if (soA < soB) {
        ketQuaSoSanh = "<";
    }else if ( soA == soB) {
        ketQuaSoSanh = "=";
    }
    lblPhepSoSanh1 = Label::createWithTTF(ketQuaSoSanh, fontStyle, fontSize * 0.8);
    lblPhepSoSanh1->setPosition(Vec2(w / 2, lblAKetQua->getPositionY()));
    addChild(lblPhepSoSanh1, 2);
    lblPhepSoSanh1->setAlignment(TextHAlignment:: CENTER);
    lblPhepSoSanh1->setColor(Color3B::YELLOW);
    lblPhepSoSanh1->setOpacity(0);
    lblPhepSoSanh1->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
        
    //animation cho hang don vi - phong to
    float timeAnimate = 0.2;

    lblA->runAction(
        Sequence::create(
            DelayTime::create(delayTime),
            Spawn::create(ScaleTo::create(timeAnimate, lblA->getScale() * 1.3), TintTo::create(timeAnimate, Color3B::GREEN), nullptr),
            DelayTime::create(1),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale()),
                         TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
            , NULL)
    );
    lblAKetQua->runAction(
        Sequence::create(
            ScaleTo::create(0, 0),
            DelayTime::create(delayTime + 1 + timeAnimate),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale() * 1.3), TintTo::create(timeAnimate, Color3B::GREEN), nullptr),
            DelayTime::create(1 - timeAnimate),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale() * 0.8), nullptr)
            , NULL)
    );
    
    delayTime = delayTime + timeAnimate * 2 + 2;
    lblB->runAction(
        Sequence::create(
            DelayTime::create(delayTime),
            Spawn::create(ScaleTo::create(timeAnimate, lblA->getScale() * 1.3), TintTo::create(timeAnimate, Color3B::GREEN), nullptr),
            DelayTime::create(1),
            Spawn::create(ScaleTo::create(timeAnimate, lblA->getScale()),
                         TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
            , NULL)
    );
    lblBKetQua->runAction(
        Sequence::create(
            ScaleTo::create(0, 0),
            DelayTime::create(delayTime + 1 +  timeAnimate),
            Spawn::create(ScaleTo::create(timeAnimate, lblA->getScale() * 1.3), TintTo::create(timeAnimate, Color3B::GREEN), nullptr),
            DelayTime::create(1 - timeAnimate),
            Spawn::create(ScaleTo::create(timeAnimate, lblA->getScale() * 0.8), nullptr)
            , NULL)
    );
    
    delayTime = delayTime + timeAnimate * 3 + 2;
    lblPhepSoSanh1->runAction(
        Sequence::create(
            ScaleTo::create(0, 0),
            DelayTime::create(delayTime),
            Spawn::create(ScaleTo::create(timeAnimate, lblA->getScale() * 0.8), TintTo::create(timeAnimate, Color3B::YELLOW), nullptr),
            DelayTime::create(1 - timeAnimate),
            Spawn::create(ScaleTo::create(timeAnimate, lblA->getScale()), MoveTo::create(timeAnimate, Vec2(w / 2, lblA->getPositionY())), nullptr)
            , NULL)
    );

    //2 button retry va OK
    auto buttonRetry = ui::Button::create("button-retry.png"); // tao button
    buttonRetry->setPressedActionEnabled(true);
    buttonRetry->setZoomScale(0);
    buttonRetry->setScale9Enabled(true);
    
    buttonRetry->setTouchEnabled(true);
    float ratiobuttonRetry = 4.5;
    //float fixHeightIpad = 1.8;
    if (h / w <= 1.53)
    {
        ratiobuttonRetry = 9;
        
        //fixHeightIpad = 1.2;
    }
    
    float scalebuttonRetry = w / ratiobuttonRetry / buttonRetry->getContentSize().width;
    buttonRetry->setScale(scalebuttonRetry);
    
    float ybuttonRetry = buttonRetry->getContentSize().height * scalebuttonRetry;
   
    buttonRetry->setPosition(Vec2(w / 4, ybuttonRetry));
    
    buttonRetry->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        auto clickedButton = static_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                if (sound == 0)
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
                }
                
                HuyFunctions().animationClickedButton(clickedButton, [this, w, h, clickedButton](){
                    for (auto icon : listIcon) {
                        removeChild(icon);
                    }
                    for (auto b : listBgNumber) {
                        removeChild(b);
                    }
                    listIcon.clear();
                    listBgNumber.clear();
                    if (clickedButton) {
                        auto scaleOut = ScaleTo::create(0.2f, 0); // Thu nhỏ Button
                        auto remove = CallFunc::create([clickedButton]() {
                            clickedButton->removeFromParent();
                            log("Button đã bị xóa sau hiệu ứng!");
                        });
                        clickedButton->runAction(Sequence::create(scaleOut, remove, nullptr));
                    }
                    
                    if (buttonOK) {
                        auto scaleOut = ScaleTo::create(0.2f, 0); // Thu nhỏ Button
                       
                        buttonOK->runAction(Sequence::create(scaleOut, nullptr));
                        removeChild(lblA);
                        removeChild(lblAKetQua);
                        removeChild(lblB);
                        removeChild(lblBKetQua);
                        removeChild(lblPhepSoSanh1);
                    }
                    
                    if (hasGuide) {
                        createTemplateGuideLevel2SoSanh(wScenePlay, hScenePlay);
                    }
                });
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonRetry, 2);//add button vao scene
    
    buttonOK = ui::Button::create("okbutton.png"); // tao button
    buttonOK->setPressedActionEnabled(true);
    buttonOK->setZoomScale(0);
    buttonOK->setScale9Enabled(true);
    
    buttonOK->setTouchEnabled(true);
   
    buttonOK->setScale(scalebuttonRetry);
  
   
    buttonOK->setPosition(Vec2(3 * w / 4, ybuttonRetry));
    
    buttonOK->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        auto button = static_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                if (sound == 0)
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
                }
                
                HuyFunctions().animationClickedButton(button, [this](){
                    moveSceneGameOver();
                });

                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonOK, 2);//add button vao scene
    buttonOK->setScale(0);
    buttonRetry->setScale(0);
    buttonOK->runAction(Sequence::create(DelayTime::create(delayTime + 1), ScaleTo::create(0.3, scalebuttonRetry), NULL));//step1 + step2 + 0.2

    buttonRetry->runAction(Sequence::create(DelayTime::create(delayTime + 1), ScaleTo::create(0.3, scalebuttonRetry), NULL));
}
void Play5::createTemplateGuideLevel2Tru(float w, float h) {
    bool flagCoNho = false;
    int congDonVi = 0;
    float fixWithCoNho = 1;
    if ( HuyFunctions().getSoHangDonVi(soA) - HuyFunctions().getSoHangDonVi(soB) < 0) {
        congDonVi = 10 + HuyFunctions().getSoHangDonVi(soA) - HuyFunctions().getSoHangDonVi(soB);
        flagCoNho = true;
        fixWithCoNho = 1.25;
    } else {
        congDonVi = HuyFunctions().getSoHangDonVi(soA) - HuyFunctions().getSoHangDonVi(soB);
    }
    
    
    lblPhepToan->runAction(FadeOut::create(0.1));
    auto fontStyle = "fonts/DeliusSwashCaps.ttf";
    float fontSize = 0.18 * w;
    if (h / w > 1.53 && h / w <= 1.8){
        fontSize = 0.15 * w;
    }
    if (h / w <= 1.53)
    {
        fontSize = 0.09 * w;
    }

    float time = 0.2;
    float width = 50.0f;
    float fixWithIpad = 1.0f;
    if (h / w <= 1.53)
    {
        fixWithIpad = 0.5f;
    }
    if (levelOfW5 > 1) {
        width = 70.0f;
    }
    float height = 3.0f * fixWithIpad;
    width = width * fixWithIpad;
    cocos2d::Vec2 origin(w / 2 - width / 2 - 16 * fixWithIpad, lblPhepToan->getPositionY() + lblPhepToan->getContentSize().height / 2);
    gachChan = HuyFunctions().createChuNhatBoGoc(width, height, origin);
    this->addChild(gachChan, 2);
    
    lblB = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangDonVi(soB))->getCString(), fontStyle, fontSize);
    float xLblB = w / 2;
    lblB->setPosition(Vec2(xLblB, lblPhepToan->getPositionY() + lblPhepToan->getContentSize().height));
    addChild(lblB, 2);
    lblB->setAlignment(TextHAlignment:: CENTER);
    lblB->setOpacity(0);
    lblB->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    if (soB > 9) {
        lblBHangChuc = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangChuc(soB))->getCString(), fontStyle, fontSize);
        
        lblBHangChuc->setPosition(Vec2(lblB->getPositionX() - lblB->getContentSize().height * 0.51 * fixWithCoNho, lblB->getPositionY()));
        addChild(lblBHangChuc, 2);
        lblBHangChuc->setAlignment(TextHAlignment:: CENTER);
        lblBHangChuc->setOpacity(0);
        lblBHangChuc->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    }
    
    loaiPhepTinh = cocos2d::__String::createWithFormat("%s", "-");
    
    lblLoaiPhepTinh = Label::createWithTTF(loaiPhepTinh->getCString(), fontStyle, fontSize);
    lblLoaiPhepTinh->setPosition(Vec2(w/2 - lblB->getContentSize().height * 0.82 * fixWithCoNho, lblB->getPositionY() + lblB->getContentSize().height * 0.55));
    addChild(lblLoaiPhepTinh, 2);
    lblLoaiPhepTinh->setOpacity(0);
    lblLoaiPhepTinh->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblA = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangDonVi(soA))->getCString(), fontStyle, fontSize);
    lblA->setPosition(Vec2(w / 2, lblLoaiPhepTinh->getPositionY() + lblLoaiPhepTinh->getContentSize().height * 0.4));
    addChild(lblA, 2);
    lblA->setAlignment(TextHAlignment:: CENTER);
    lblA->setOpacity(0);
    lblA->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblAHangChuc = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangChuc(soA))->getCString(), fontStyle, fontSize);
    lblAHangChuc->setPosition(Vec2(lblA->getPositionX() - lblA->getContentSize().height * 0.51 * fixWithCoNho, lblA->getPositionY()));
    addChild(lblAHangChuc, 2);
    lblAHangChuc->setOpacity(0);
    lblAHangChuc->setAlignment(TextHAlignment:: CENTER);
    lblAHangChuc->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblDapAnHangDV = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangDonVi(soA - soB))->getCString(), fontStyle, fontSize);
    lblDapAnHangDV->setPosition(Vec2(w / 2, lblB->getPositionY() - lblB->getContentSize().height * 1.1 * fixWithCoNho * 0.9));
    addChild(lblDapAnHangDV, 2);
    lblDapAnHangDV->setAlignment(TextHAlignment:: CENTER);
    lblDapAnHangDV->setOpacity(0);
    //lblDapAnHangDV->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblDapAnHangChuc = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangChuc(soA - soB))->getCString(), fontStyle, fontSize);
    lblDapAnHangChuc->setPosition(Vec2(lblAHangChuc->getPositionX(), lblDapAnHangDV->getPositionY()));
    addChild(lblDapAnHangChuc, 2);
    lblDapAnHangChuc->setOpacity(0);
    lblDapAnHangChuc->setAlignment(TextHAlignment:: CENTER);
    //lblDapAnHangChuc->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    
    lblDapAnPhanNhoDV = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", HuyFunctions().getSoHangDonVi(congDonVi))->getCString(), fontStyle, fontSize * 0.8);
    lblDapAnPhanNhoDV->setPosition(Vec2(w * 0.8, lblLoaiPhepTinh->getPositionY()));
    addChild(lblDapAnPhanNhoDV, 2);
    lblDapAnPhanNhoDV->setAlignment(TextHAlignment:: CENTER);
    lblDapAnPhanNhoDV->setOpacity(0);
    //lblDapAnPhanNhoDV->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    lblDapAnPhanNhoHangChuc = Label::createWithTTF("1", fontStyle, fontSize * 0.8);
    lblDapAnPhanNhoHangChuc->setPosition(Vec2(lblDapAnPhanNhoDV->getPositionX() - lblDapAnPhanNhoDV->getContentSize().height * 0.51, lblLoaiPhepTinh->getPositionY()));
    addChild(lblDapAnPhanNhoHangChuc, 2);
    lblDapAnPhanNhoHangChuc->setOpacity(0);
    lblDapAnPhanNhoHangChuc->setAlignment(TextHAlignment:: CENTER);
    //lblDapAnPhanNhoHangChuc->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    auto playSound = CallFunc::create([this]() {
        if (sound == 0)
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/hit.mp3");
        }
    });
    
    if (flagB1ChuSo) {
        xLblB = w / 2 + (lblA->getContentSize().width - lblB->getContentSize().width) / 2;
        lblB->setPosition(Vec2(xLblB, lblB->getPositionY()));
    }
    //animation cho hang don vi - phong to
    float timeAnimate = 0.2;
    float delayTime = 2;
    float fixTimeCoNho = 0;
    if (flagCoNho) {
        fixTimeCoNho = 2;
    }
    if (flagCoNho) {
        float timePlay = 0.07f;
        auto rotateLeft = RotateBy::create(timePlay, -5.0f); // Rotate left by 10 degrees
        auto rotateRight = RotateBy::create(timePlay, 10.0f); // Rotate right by 20 degrees (to 10 degrees right)
        auto rotateBack = RotateBy::create(timePlay, -5.0f); // Rotate back to center

        // Create a sequence and repeat it forever
        auto tiltSequence = Sequence::create( rotateLeft, rotateRight, rotateBack,rotateLeft, rotateRight, rotateBack,rotateLeft, rotateRight, rotateBack, nullptr);
       
        lblB->runAction(
            Sequence::create(
                DelayTime::create(delayTime),
                Spawn::create(
                              ScaleTo::create(timeAnimate, lblB->getScale() * 1.3),
                              TintTo::create(timeAnimate, Color3B::GREEN), nullptr),
                DelayTime::create(1),
                tiltSequence,
                NULL)

            );
        rotateLeft = RotateBy::create(timePlay * 0.9, -5.0f); // Rotate left by 10 degrees
        rotateRight = RotateBy::create(timePlay * 0.9, 10.0f); // Rotate right by 20 degrees (to 10 degrees right)
        rotateBack = RotateBy::create(timePlay * 0.9, -5.0f); // Rotate back to center

        // Create a sequence and repeat it forever
        tiltSequence = Sequence::create( rotateLeft, rotateRight, rotateBack,rotateLeft, rotateRight, rotateBack,rotateLeft, rotateRight, rotateBack, nullptr);
        
        lblA->runAction(
            Sequence::create(
                DelayTime::create(delayTime),
                Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale() * 1.2), TintTo::create(timeAnimate, Color3B::GREEN), nullptr),
                             DelayTime::create(1),
                             tiltSequence
                , NULL)
            );
    } else {
        lblB->runAction(
            Sequence::create(
                DelayTime::create(delayTime),
                Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale() * 1.2), TintTo::create(timeAnimate, Color3B::GREEN), nullptr)
                , NULL)
            );
        lblA->runAction(
            Sequence::create(
                DelayTime::create(delayTime),
                Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale() * 1.2), TintTo::create(timeAnimate, Color3B::GREEN), nullptr)
                , NULL)
            );
    }
    
    
    log("cong don vi: %i", congDonVi);
    
    if (flagCoNho) { // co nho
//        lblDapAnPhanNhoDV->runAction(
//            Sequence::create(
//                DelayTime::create(delayTime + 0.5),
//                Spawn::create(
//                           FadeIn::create(timeAnimate),
//                           ScaleTo::create(timeAnimate, lblB->getScale() * 1.2),
//                           TintTo::create(timeAnimate, Color3B::YELLOW),
//                           //move to lblDapAnHangDV
//
//                           nullptr),
//                 DelayTime::create(2),
//                 MoveTo::create(timeAnimate * 3, Vec2(lblDapAnHangDV->getPositionX(), lblDapAnHangDV->getPositionY())),
//                 FadeOut::create(timeAnimate)
//                , NULL)
//            );
        auto positionPhanNho = Vec2(lblDapAnPhanNhoHangChuc->getPosition());
        
        auto changeText = CallFunc::create([this]() {
            lblDapAnPhanNhoHangChuc->setString("-1");
        });
        
        lblDapAnPhanNhoHangChuc->setPosition(Vec2(lblA->getPositionX() - lblA->getContentSize().width * 0.8, lblA->getPositionY())); // - lblA->getContentSize().width / 2)
            lblDapAnPhanNhoHangChuc->runAction(
                Sequence::create(
                    DelayTime::create(delayTime + 1.5 + 0.5),
                         Spawn::create(
                               FadeIn::create(timeAnimate),
                               ScaleTo::create(timeAnimate, lblB->getScale() * 0.7),
                                TintTo::create(timeAnimate, Color3B::GREEN),
                               nullptr),
                     DelayTime::create(2.5),
                     MoveTo::create(timeAnimate, positionPhanNho),
                     
                     DelayTime::create(2),
                                 Spawn::create(
                                        MoveTo::create(timeAnimate * 2, Vec2(lblDapAnHangChuc->getPositionX(), lblDapAnHangChuc->getPositionY() + lblDapAnHangChuc->getContentSize().height / 2)),
                                    changeText,
                                       nullptr),
                                 
                     DelayTime::create(2),
                     FadeOut::create(timeAnimate)
                    , NULL)
                );
    }
    lblDapAnHangDV->runAction(
        Sequence::create(
                         DelayTime::create(delayTime + fixTimeCoNho * 1.2 + 1),
                 Spawn::create(
                       FadeIn::create(timeAnimate),
                       ScaleTo::create(timeAnimate, lblB->getScale() * 1.2),
                        playSound,
                       TintTo::create(timeAnimate, Color3B::GREEN),
                       nullptr)
            , NULL)
        );
    
    //hang don vi - thu nho va hien dap an don vi
    lblB->runAction(
        Sequence::create(
            DelayTime::create(delayTime + fixTimeCoNho + 2),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale()), TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
            , NULL)
        );
    lblA->runAction(
        Sequence::create(
            DelayTime::create(delayTime + fixTimeCoNho + 2),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale()), TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
            , NULL)
        );
    
    lblDapAnHangDV->runAction(
        Sequence::create(
                         DelayTime::create(delayTime + fixTimeCoNho + 2),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale()), TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
            , NULL)
        );
    
//    lblB->runAction(
//        Sequence::create(
//            DelayTime::create(delayTime),
//            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale() * 1.3), TintTo::create(timeAnimate, Color3B::GREEN), nullptr)
//            , NULL)
//        );
    lblAHangChuc->runAction(
        Sequence::create(
            DelayTime::create(delayTime + fixTimeCoNho * 1.8 + 2.5),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale() * 1.2),
                          
                          TintTo::create(timeAnimate, Color3B::GREEN), nullptr)
            , NULL)
        );
    
    if (soB > 9) {
        lblBHangChuc->runAction(
            Sequence::create(
                             DelayTime::create(delayTime + fixTimeCoNho * 1.8 + 2.5),
                Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale() * 1.2), TintTo::create(timeAnimate, Color3B::GREEN), nullptr)
                , NULL)
            );
    }
    
    lblDapAnHangChuc->runAction(
        Sequence::create(
                         DelayTime::create(delayTime + fixTimeCoNho * 2 + 4),
                 Spawn::create(
                       FadeIn::create(timeAnimate),
                       ScaleTo::create(timeAnimate, lblB->getScale() * 1.2),
                        playSound,
                       TintTo::create(timeAnimate, Color3B::GREEN),
                       nullptr)
            , NULL)
        );
    
    
    //hang chuc thu nho
    
    lblAHangChuc->runAction(
        Sequence::create(
                         DelayTime::create(delayTime + fixTimeCoNho * 1.5 + 5),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale()), TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
            , NULL)
        );
    if (soB > 9) {
        lblBHangChuc->runAction(
            Sequence::create(
                DelayTime::create(delayTime + fixTimeCoNho * 1.5 + 5),
                Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale()), TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
                , NULL)
            );
    }
    
    lblDapAnHangChuc->runAction(
        Sequence::create(
            DelayTime::create(delayTime + fixTimeCoNho * 1.5 + 6),
            Spawn::create(ScaleTo::create(timeAnimate, lblB->getScale()), TintTo::create(timeAnimate, Color3B::WHITE), nullptr)
            , NULL)
        );
    
    //2 button retry va OK
    auto buttonRetry = ui::Button::create("button-retry.png"); // tao button
    buttonRetry->setPressedActionEnabled(true);
    buttonRetry->setZoomScale(0);
    buttonRetry->setScale9Enabled(true);
    
    buttonRetry->setTouchEnabled(true);
    float ratiobuttonRetry = 4.5;
    float fixHeightIpad = 1.8;
    if (h / w <= 1.53)
    {
        ratiobuttonRetry = 9;
        
        //fixHeightIpad = 1.2;
    }
    
    float scalebuttonRetry = w / ratiobuttonRetry / buttonRetry->getContentSize().width;
    buttonRetry->setScale(scalebuttonRetry);
    
    float ybuttonRetry = buttonRetry->getContentSize().height * scalebuttonRetry;
   
    buttonRetry->setPosition(Vec2(w / 4, ybuttonRetry));
    
    buttonRetry->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        auto clickedButton = static_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                if (sound == 0)
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
                }
                
                HuyFunctions().animationClickedButton(clickedButton, [this, w, h, clickedButton](){
                    for (auto icon : listIcon) {
                        removeChild(icon);
                    }
                    for (auto b : listBgNumber) {
                        removeChild(b);
                    }
                    listIcon.clear();
                    listBgNumber.clear();
                    if (clickedButton) {
                        auto scaleOut = ScaleTo::create(0.2f, 0); // Thu nhỏ Button
                        auto remove = CallFunc::create([clickedButton]() {
                            clickedButton->removeFromParent();
                            log("Button đã bị xóa sau hiệu ứng!");
                        });
                        clickedButton->runAction(Sequence::create(scaleOut, remove, nullptr));
                    }
                    
                    if (buttonOK) {
                        auto scaleOut = ScaleTo::create(0.2f, 0); // Thu nhỏ Button
                       
                        buttonOK->runAction(Sequence::create(scaleOut, nullptr));
                        removeChild(lblA);
                        removeChild(lblAHangChuc);
                        removeChild(lblB);
                        removeChild(lblBHangChuc);
                        removeChild(gachChan);
                        removeChild(lblDapAnHangDV);
                        removeChild(lblDapAnHangChuc);
                        removeChild(lblDapAnPhanNhoDV);
                        removeChild(lblDapAnPhanNhoHangChuc);
                    }
                    
                    if (hasGuide) {
                        createTemplateGuideLevel2Tru(wScenePlay, hScenePlay);
                    }
                });
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonRetry, 2);//add button vao scene
    
    buttonOK = ui::Button::create("okbutton.png"); // tao button
    buttonOK->setPressedActionEnabled(true);
    buttonOK->setZoomScale(0);
    buttonOK->setScale9Enabled(true);
    
    buttonOK->setTouchEnabled(true);
   
    buttonOK->setScale(scalebuttonRetry);
  
   
    buttonOK->setPosition(Vec2(3 * w / 4, ybuttonRetry));
    
    buttonOK->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        auto button = static_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                if (sound == 0)
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
                }
                
                HuyFunctions().animationClickedButton(button, [this](){
                    moveSceneGameOver();
                });

                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonOK, 2);//add button vao scene
    buttonOK->setScale(0);
    buttonRetry->setScale(0);
    buttonOK->runAction(Sequence::create(DelayTime::create(delayTime + fixTimeCoNho + 6), ScaleTo::create(0.3, scalebuttonRetry), NULL));//step1 + step2 + 0.2

    buttonRetry->runAction(Sequence::create(DelayTime::create(delayTime + fixTimeCoNho + 6 + 0.3), ScaleTo::create(0.3, scalebuttonRetry), NULL));
}
void Play5::createTemplateGuideLevel1Tru(float w, float h) {
    auto viTriTenLua = UserDefault::getInstance()->getIntegerForKey("tenluadachon", 0) + 1;
    auto iconTenLua = StringUtils::format("icon-ten-lua-%d.png", viTriTenLua);
    float wLuoi = w / 10;
    int step = 1;
    for (int i = 1; i <= soA + 1; i++) {
        // create Phi thuyen
        auto viTriPhiThuyen = UserDefault::getInstance()->getIntegerForKey("phithuyendachon", 0) + 1;
        auto sprite_cachePhiThuyen = SpriteFrameCache::getInstance();
        sprite_cachePhiThuyen->addSpriteFramesWithFile(StringUtils::format("phithuyen0%d1.plist", viTriPhiThuyen));
        Vector<SpriteFrame*> phiThuyen;
        phiThuyen.reserve(2);
        phiThuyen.pushBack(sprite_cachePhiThuyen->getSpriteFrameByName(StringUtils::format("phithuyen0%d1a.png", viTriPhiThuyen)));
        phiThuyen.pushBack(sprite_cachePhiThuyen->getSpriteFrameByName(StringUtils::format("phithuyen0%d1b.png", viTriPhiThuyen)));
        Animation* animationPhiThuyen = Animation::createWithSpriteFrames(phiThuyen, 0.2f, -1);
        Animate* animatePhiThuyen = Animate::create(animationPhiThuyen);

        auto iconSoA = Sprite::createWithSpriteFrame(phiThuyen.front());
        iconSoA->setPosition(Vec2(w / 2 - (soA - 1) * wLuoi / 2 + (i - 1) * wLuoi, h * 0.65));
        if (i == soA + 1) {
            iconSoA->setPosition(Vec2(w / 2 - (soA - 1) * wLuoi / 2 + (i - 2) * wLuoi, h * 0.65));
        }
        float ratioiconSoA = 12;
        //float fontSizeNoti = 0.04 * w;
        //float fixHeightNotiIpad = 1;
        float scaleiconSoA = w / ratioiconSoA / iconSoA->getContentSize().width;
        iconSoA->setScale(scaleiconSoA);
        this->addChild(iconSoA, 2);
        iconSoA->runAction(animatePhiThuyen);
        iconSoA->setTag(i);
        listIcon.pushBack(iconSoA);
        
        float ratiobgNumber = 12;
        float fixIpad = 1;
        if (h / w <= 1.53) {
            ratiobgNumber = 20;
            fixIpad = 0.8;
        }
        
        if (i <= soA + 1 && i > soB) {
            auto bgNumber = ui::Button::create("bgNumber 2.png");
            bgNumber->setPosition(Vec2(iconSoA->getPositionX(), iconSoA->getPositionY() + iconSoA->getContentSize().height * scaleiconSoA * 1.2 * fixIpad)); // Đặt vị trí
            
            float scalebgNumber = w / ratiobgNumber / bgNumber->getContentSize().width;
            bgNumber->setScale(scalebgNumber);
            bgNumber->setTitleText(cocos2d::__String::createWithFormat("%d", i - soB)->getCString());
            if (i == soA + 1) {
                
             
                bgNumber->setTitleText(cocos2d::__String::createWithFormat("%d", i - 1 - soB )->getCString());
        
            }
            bgNumber->setTitleFontName("Arial-BoldMT");
            bgNumber->setTitleColor(Color3B().WHITE);
            bgNumber->setTitleFontSize(0.12 * w * fixIpad);
            this->addChild(bgNumber, 2);
            listBgNumber.pushBack(bgNumber);
        }

        step++;
    }
    float ratiobgNumber = 12;
    float fixIpad = 1;
    float fixHeightIpad = 0.55;
    if (h / w < 1.8 && h / w > 1.53) {
        fixHeightIpad = 0.52;
    }
    if (h / w <= 1.53) {
        ratiobgNumber = 20;
        fixIpad = 0.8;
        fixHeightIpad = 0.52;
    }
    for (int i = 1; i <= soB; i++) {
        auto iconSoB = Sprite::create(iconTenLua.c_str());
        iconSoB->setPosition(Vec2(w / 2 - (soA - 1) * wLuoi / 2 + (i - 1) * wLuoi, h * fixHeightIpad));
        if (i == soB + 1) {
            iconSoB->setPosition(Vec2(w / 2 - (soA - 1) * wLuoi / 2 + (soB - 1) * wLuoi, h * fixHeightIpad));
        }
        float ratioiconSoB = 18;
        //float fontSizeNoti = 0.04 * w;
        //float fixHeightNotiIpad = 1;
        float scaleiconSoB = w / ratioiconSoB / iconSoB->getContentSize().width;
        iconSoB->setScale(scaleiconSoB);
        iconSoB->setRotation(-45);
        iconSoB->setTag(i);
        this->addChild(iconSoB, 2);
        listIconTru.pushBack(iconSoB);
        
        step++;
    }
    
    float time = 0;
    int step1 = 0;
    float scale;
    for (auto icon : listIcon) {
        if(icon) {
            if (step1 == 0) {
                scale = icon->getScale();
            }
            icon->setScale(0);
            time = 1.5 + step1 * 0.25;
            icon->runAction(Sequence::create(DelayTime::create(time), ScaleTo::create(0.3, scale), NULL));
            
            step1++;
        }
    }
    
    step1 = 0;
    for (auto icon : listIconTru) {
        if(icon) {
            if (step1 == 0) {
                scale = icon->getScale();
            }
            icon->setScale(0);
            time = time + 0.1;
            icon->runAction(Sequence::create(DelayTime::create(time), ScaleTo::create(0.3, scale), NULL));
            step1++;
        }
    }
    step1 = 0;
    
    time = time + 0.8;
    for (auto icon : listIconTru) {
        if (icon) {
            time = time + 0.3f;
            
            cocos2d::Vec2 position = Vec2(icon->getPositionX(), h * 0.65f);
            // Tạo hiệu ứng di chuyển và mờ dần
            auto moveAction = MoveTo::create(0.4f, position);
            auto fadeOut = FadeTo::create(0.2f, 0);
            // Callback để xử lý hành động sau khi icon biến mất
            auto callback = CallFunc::create([this, icon, position]() {
                for (auto phithuyen : listIcon) {
                    if (phithuyen && phithuyen->getTag() == icon->getTag()) {
                        phithuyen->runAction(Sequence::create(
                                //DelayTime::create(0.4),
                                FadeOut::create(0.2f), NULL)
                        );
                        //createGuideBom(scale, position);
                    }
                
                }
                

            });

            // Chạy hành động với easing
            icon->runAction(
                //EaseExponentialIn::create(
                    Sequence::create(
                        DelayTime::create(time),
                        moveAction,
                        callback,
                        fadeOut,
                        nullptr
                    )
                //)
            );

            step1++;
        }
    }

    int step2 = 0;
    for (auto button : listBgNumber) {
        if(button) {
            if (step2 == 0) {
                scale = button->getScale();
            }
            if (step2 < soA - soB) {
                button->setScale(0);
                button->runAction(Sequence::create(DelayTime::create(time + 1 + step2 * 0.6 ), ScaleTo::create(0.3, scale), NULL));//(1.5 + step1 * 0.6 + step2 * 0.5)
            } else{
                button->setScale(0);
                auto moveToDapAn = CallFunc::create([button, this]() {
                    button->runAction(
                            Sequence::create(
                                Spawn::create(
                                    ScaleTo::create(0.6, button->getScale() * 1.5),
                                    MoveTo::create(0.6, Vec2(lblPhepToan->getPositionX() + lblPhepToan->getContentSize().width * lblPhepToan->getScale() / 2, lblPhepToan->getPositionY())),
                                    nullptr),
                                FadeOut::create(0.2),
                            NULL)
                        );
                });
                button->runAction(Sequence::create(DelayTime::create(time + 1 + step2 * 0.6), ScaleTo::create(0, scale), moveToDapAn, NULL)); //1.5 + step1 * 0.6 + step2 * 0.5
                time = time + 1 + step2 * 0.6;
                
            }
            step2++;
        }
    }
    
    auto buttonRetry = ui::Button::create("button-retry.png"); // tao button
    buttonRetry->setPressedActionEnabled(true);
    buttonRetry->setZoomScale(0);
    buttonRetry->setScale9Enabled(true);
    
    buttonRetry->setTouchEnabled(true);
    float ratiobuttonRetry = 4.5;
    fixHeightIpad = 1.8;
    if (h / w <= 1.53)
    {
        ratiobuttonRetry = 9;
        
        //fixHeightIpad = 1.2;
    }
    
    float scalebuttonRetry = w / ratiobuttonRetry / buttonRetry->getContentSize().width;
    buttonRetry->setScale(scalebuttonRetry);
    
    float ybuttonRetry = buttonRetry->getContentSize().height * scalebuttonRetry;
   
    buttonRetry->setPosition(Vec2(w / 4, ybuttonRetry));
    
    buttonRetry->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        auto clickedButton = static_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                if (sound == 0)
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
                }
                
                HuyFunctions().animationClickedButton(clickedButton, [this, w, h, clickedButton](){
                    for (auto icon : listIcon) {
                        removeChild(icon);
                    }
                    for (auto b : listBgNumber) {
                        removeChild(b);
                    }
                    listIcon.clear();
                    listBgNumber.clear();
                    listIconTru.clear();
                    if (clickedButton) {
                        auto scaleOut = ScaleTo::create(0.2f, 0); // Thu nhỏ Button
                        auto remove = CallFunc::create([clickedButton]() {
                            clickedButton->removeFromParent();
                            log("Button đã bị xóa sau hiệu ứng!");
                        });
                        clickedButton->runAction(Sequence::create(scaleOut, remove, nullptr));
                    }
                    
                    if (buttonOK) {
                        auto scaleOut = ScaleTo::create(0.2f, 0); // Thu nhỏ Button
                       
                        buttonOK->runAction(Sequence::create(scaleOut, nullptr));
                    }
                    
                    lblPhepToan->runAction(
                        Sequence::create(
                             FadeOut::create(0.3),
                             CallFunc::create([this]() {
                                lblPhepToan->setString(cocos2d::__String::createWithFormat("%d - %d = ?", soA, soB)->getCString());
                                // Thêm câu lệnh khác nếu cần
                             }),
                             FadeIn::create(0.2), NULL));
                    if (hasGuide) {
                        createTemplateGuideLevel1Tru(wScenePlay, hScenePlay);
                    }
                });
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonRetry, 2);//add button vao scene
    
    buttonOK = ui::Button::create("okbutton.png"); // tao button
    buttonOK->setPressedActionEnabled(true);
    buttonOK->setZoomScale(0);
    buttonOK->setScale9Enabled(true);
    
    buttonOK->setTouchEnabled(true);
   
    buttonOK->setScale(scalebuttonRetry);
  
   
    buttonOK->setPosition(Vec2(3 * w / 4, ybuttonRetry));
    
    buttonOK->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        auto button = static_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                if (sound == 0)
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
                }
                
                HuyFunctions().animationClickedButton(button, [this](){
                    moveSceneGameOver();
                });

                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonOK, 2);//add button vao scene
    
    float timeFinish = time + 0.6;
    buttonOK->setScale(0);
    buttonOK->runAction(Sequence::create(DelayTime::create(timeFinish), ScaleTo::create(0.3, scalebuttonRetry), NULL));//step1 + step2 + 0.2
    
    buttonRetry->setScale(0);
    
    lblPhepToan->runAction(
        Sequence::create(
             DelayTime::create(timeFinish + 0.2),
             FadeOut::create(0.1),
             ScaleTo::create(0, 2),
             CallFunc::create([this]() {
                lblPhepToan->setString(cocos2d::__String::createWithFormat("%d - %d = %d", soA, soB, soA - soB)->getCString());
                 if (sound == 0)
                 {
                     CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/hit.mp3");
                 }
                // Thêm câu lệnh khác nếu cần
             }),
             Spawn::create(FadeIn::create(0.2), ScaleTo::create(0.2, 1), nullptr), NULL));

    buttonRetry->runAction(Sequence::create(DelayTime::create(timeFinish + 0.3), ScaleTo::create(0.3, scalebuttonRetry), NULL));
    
}
void Play5::createGuideBom(float myScale, cocos2d::Vec2 position) {
    //create bom phiThuyen
    auto sprite_cachePhiThuyenBom = SpriteFrameCache::getInstance();
    sprite_cachePhiThuyenBom->addSpriteFramesWithFile("bom-tim.plist");
    Vector<SpriteFrame*> phiThuyenBom;
    phiThuyenBom.reserve(12);
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("1.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("2.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("3.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("4.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("5.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("6.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("7.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("8.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("9.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("10.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("11.png"));
    phiThuyenBom.pushBack(sprite_cachePhiThuyenBom->getSpriteFrameByName("12.png"));
    Animation* animationPhiThuyenBom = Animation::createWithSpriteFrames(phiThuyenBom, 0.06f, -1);
    Animate* animatePhiThuyenBom = Animate::create(animationPhiThuyenBom);

    bomPhiThuyenHienThi = Sprite::createWithSpriteFrame(phiThuyenBom.front());
    
    auto bom = Sprite::createWithSpriteFrame(phiThuyenBom.front());
    bom->setScale(myScale);
    this->addChild(bom, 2);
    bom->runAction(animatePhiThuyenBom);
    bom->setPosition(position);
    listBom.pushBack(bom);
}
void Play5::createTemplateGuideLevel1Cong(float w, float h) {
    
    auto viTriTenLua = UserDefault::getInstance()->getIntegerForKey("tenluadachon", 0) + 1;
    auto iconTenLua = StringUtils::format("icon-ten-lua-%d.png", viTriTenLua);
    
    float wLuoi = w / 10;
    int step = 1;
    for (int i = 1; i <= soA; i++) {
        auto iconSoA = Sprite::create(iconTenLua.c_str());
        iconSoA->setPosition(Vec2(w / 2 - (soA - 1) * wLuoi / 2 + (i - 1) * wLuoi, h * 0.65));
        float ratioiconSoA = 12;
        //float fontSizeNoti = 0.04 * w;
        //float fixHeightNotiIpad = 1;
        float scaleiconSoA = w / ratioiconSoA / iconSoA->getContentSize().width;
        iconSoA->setScale(scaleiconSoA);
        this->addChild(iconSoA, 2);
        listIcon.pushBack(iconSoA);
        
        float ratiobgNumber = 12;
        float fixIpad = 1;
        if (h / w <= 1.53) {
            ratiobgNumber = 20;
            fixIpad = 0.8;
        }
        
        auto bgNumber = ui::Button::create("bgNumber 2.png");
        bgNumber->setPosition(Vec2(iconSoA->getPositionX(), iconSoA->getPositionY() + iconSoA->getContentSize().height * scaleiconSoA * 1.2 * fixIpad)); // Đặt vị trí
        
        float scalebgNumber = w / ratiobgNumber / bgNumber->getContentSize().width;
        bgNumber->setScale(scalebgNumber);
        bgNumber->setTitleText(cocos2d::__String::createWithFormat("%d", i)->getCString());
        bgNumber->setTitleFontName("Arial-BoldMT");
        bgNumber->setTitleColor(Color3B().WHITE);
        bgNumber->setTitleFontSize(0.12 * w * fixIpad);
        this->addChild(bgNumber, 2);
        listBgNumber.pushBack(bgNumber);
        step++;
    }
    
    float ratiobgNumber = 12;
    float fixIpad = 1;
    float fixHeightIpad = 0.55;
    if (h / w < 1.8 && h / w > 1.53) {
        fixHeightIpad = 0.52;
    }
    if (h / w <= 1.53) {
        ratiobgNumber = 20;
        fixIpad = 0.8;
        fixHeightIpad = 0.52;
    }
    
    for (int i = 1; i <= soB + 1; i++) {
        auto iconSoB = Sprite::create(iconTenLua.c_str());
        iconSoB->setPosition(Vec2(w / 2 - (soA - 1) * wLuoi / 2 + (i - 1) * wLuoi, h * fixHeightIpad));
        if (i == soB + 1) {
            iconSoB->setPosition(Vec2(w / 2 - (soA - 1) * wLuoi / 2 + (soB - 1) * wLuoi, h * fixHeightIpad));
        }
        float ratioiconSoB = 12;
        //float fontSizeNoti = 0.04 * w;
        //float fixHeightNotiIpad = 1;
        float scaleiconSoB = w / ratioiconSoB / iconSoB->getContentSize().width;
        iconSoB->setScale(scaleiconSoB);
        this->addChild(iconSoB, 2);
        listIcon.pushBack(iconSoB);
        
        auto bgNumber = ui::Button::create("bgNumber 2.png");
        bgNumber->setPosition(Vec2(iconSoB->getPositionX(), iconSoB->getPositionY() + iconSoB->getContentSize().height * scaleiconSoB * 1.2 * fixIpad)); // Đặt vị trí
       
        //float fontSizeNoti = 0.04 * w;
        //float fixHeightNotiIpad = 1;
        float scalebgNumber = w / ratiobgNumber / bgNumber->getContentSize().width;
        bgNumber->setScale(scalebgNumber);
        bgNumber->setTitleText(cocos2d::__String::createWithFormat("%d", step)->getCString());
        if (i == soB + 1) {
            
            iconSoB->setTag(100);
            bgNumber->setTitleText(cocos2d::__String::createWithFormat("%d", soB + soA)->getCString());
            bgNumber->setTag(100);
        }
        bgNumber->setTitleFontName("Arial-BoldMT");
        bgNumber->setTitleColor(Color3B().WHITE);
        bgNumber->setTitleFontSize(0.12 * w * fixIpad);
        this->addChild(bgNumber, 2);
        listBgNumber.pushBack(bgNumber);
        step++;
    }
    
    int step1 = 0;
    float scale;
    for (auto icon : listIcon) {
        if(icon) {
            if (step1 == 0) {
                scale = icon->getScale();
            }
            if (step1 < soA + soB) {
                icon->setScale(0);
                icon->runAction(Sequence::create(DelayTime::create(1.5 + step1 * 0.6), ScaleTo::create(0.3, scale), NULL));
            } else{
                icon->setScale(0);
            }
            
            step1++;
        }
    }
    int step2 = 0;
    for (auto button : listBgNumber) {
        if(button) {
            if (step2 == 0) {
                scale = button->getScale();
            }
            if (step2 < soA + soB) {
                button->setScale(0);
                button->runAction(Sequence::create(DelayTime::create(1.5 + step2 * 0.6 ), ScaleTo::create(0.3, scale), NULL));//(1.5 + step1 * 0.6 + step2 * 0.5)
            } else{
                button->setScale(0);
                auto moveToDapAn = CallFunc::create([button, this]() {
                    button->runAction(
                            Sequence::create(
                                Spawn::create(
                                    ScaleTo::create(0.6, button->getScale() * 1.5),
                                    MoveTo::create(0.6, Vec2(lblPhepToan->getPositionX() + lblPhepToan->getContentSize().width * lblPhepToan->getScale() / 2, lblPhepToan->getPositionY())),
                                    nullptr),
                                FadeOut::create(0.2),
                            NULL)
                        );
                });
                button->runAction(Sequence::create(DelayTime::create(1.5 + step2 * 0.6), ScaleTo::create(0, scale), moveToDapAn, NULL)); //1.5 + step1 * 0.6 + step2 * 0.5
                
            }
            step2++;
        }
    }
    
    auto buttonRetry = ui::Button::create("button-retry.png"); // tao button
    buttonRetry->setPressedActionEnabled(true);
    buttonRetry->setZoomScale(0);
    buttonRetry->setScale9Enabled(true);
    
    buttonRetry->setTouchEnabled(true);
    float ratiobuttonRetry = 4.5;
    fixHeightIpad = 1.8;
    if (h / w <= 1.53)
    {
        ratiobuttonRetry = 9;
        
        //fixHeightIpad = 1.2;
    }
    
    float scalebuttonRetry = w / ratiobuttonRetry / buttonRetry->getContentSize().width;
    buttonRetry->setScale(scalebuttonRetry);
    
    float ybuttonRetry = buttonRetry->getContentSize().height * scalebuttonRetry;
   
    buttonRetry->setPosition(Vec2(w / 4, ybuttonRetry));
    
    buttonRetry->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        auto clickedButton = static_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                if (sound == 0)
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
                }
                
                HuyFunctions().animationClickedButton(clickedButton, [this, w, h, clickedButton](){
                    for (auto icon : listIcon) {
                        removeChild(icon);
                    }
                    for (auto b : listBgNumber) {
                        removeChild(b);
                    }
                    listIcon.clear();
                    listBgNumber.clear();
                    if (clickedButton) {
                        auto scaleOut = ScaleTo::create(0.2f, 0); // Thu nhỏ Button
                        auto remove = CallFunc::create([clickedButton]() {
                            clickedButton->removeFromParent();
                            log("Button đã bị xóa sau hiệu ứng!");
                        });
                        clickedButton->runAction(Sequence::create(scaleOut, remove, nullptr));
                    }
                    
                    if (buttonOK) {
                        auto scaleOut = ScaleTo::create(0.2f, 0); // Thu nhỏ Button
                       
                        buttonOK->runAction(Sequence::create(scaleOut, nullptr));
                    }
                    
                    lblPhepToan->runAction(
                        Sequence::create(
                             FadeOut::create(0.3),
                             CallFunc::create([this]() {
                                lblPhepToan->setString(cocos2d::__String::createWithFormat("%d + %d = ?", soA, soB)->getCString());
                                // Thêm câu lệnh khác nếu cần
                             }),
                             FadeIn::create(0.2), NULL));
                    if (hasGuide) {
                        createTemplateGuideLevel1Cong(wScenePlay, hScenePlay);
                    }
                });
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonRetry, 2);//add button vao scene
    
    buttonOK = ui::Button::create("okbutton.png"); // tao button
    buttonOK->setPressedActionEnabled(true);
    buttonOK->setZoomScale(0);
    buttonOK->setScale9Enabled(true);
    
    buttonOK->setTouchEnabled(true);
   
    buttonOK->setScale(scalebuttonRetry);
  
   
    buttonOK->setPosition(Vec2(3 * w / 4, ybuttonRetry));
    
    buttonOK->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        auto button = static_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                
                if (sound == 0)
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
                }
                
                HuyFunctions().animationClickedButton(button, [this](){
                    moveSceneGameOver();
                });

                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(buttonOK, 2);//add button vao scene
    
    float timeFinish = 1.5 + step1 * 0.6; //1.5 + step1 * 0.6 + step2 * 0.5
    buttonOK->setScale(0);
    buttonOK->runAction(Sequence::create(DelayTime::create(timeFinish), ScaleTo::create(0.3, scalebuttonRetry), NULL));//step1 + step2 + 0.2
    
    buttonRetry->setScale(0);
    
    lblPhepToan->runAction(
        Sequence::create(
             DelayTime::create(timeFinish + 0.2),
             FadeOut::create(0.1),
             ScaleTo::create(0, 2),
             CallFunc::create([this]() {
                lblPhepToan->setString(cocos2d::__String::createWithFormat("%d + %d = %d", soA, soB, soA + soB)->getCString());
                 if (sound == 0)
                 {
                     CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/hit.mp3");
                 }
                // Thêm câu lệnh khác nếu cần
             }),
             Spawn::create(FadeIn::create(0.2), ScaleTo::create(0.2, 1), nullptr), NULL));

    buttonRetry->runAction(Sequence::create(DelayTime::create(timeFinish + 0.3), ScaleTo::create(0.3, scalebuttonRetry), NULL));
    
}

void Play5::createTemplateGuideLevel3(float w, float h) {
    
}

void Play5::handlerClickAnswer(int v, cocos2d::ui::Button* buttonclicked){
    
    this->unscheduleUpdate();
    
    double scalePopup = 1;
    log("test man SE: %f", scalePopup);
    
    if (hScenePlay / wScenePlay <= 1.53)
    {
        scalePopup = 0.4;
    }
    //luu ket qua phep tinh vao DB
    int myTime = timeMax - timeRemaining;
    int isCorrect = 0;
    if(v == correctAnswer) {
        isCorrect = 1;
    }
    
    std::string ph = lblPhepToan->getString().c_str();
    if (sapXepPhepTinhTheoChieuDoc) {
       ph = lblLoaiPhepTinh->getString().c_str();
    }
    
    sound = UserDefault::getInstance()->getIntegerForKey("sound", 0);

    buttonAnswer1->setTouchEnabled(false);
    buttonAnswer2->setTouchEnabled(false);
    buttonAnswer3->setTouchEnabled(false);
    buttonAnswer4->setTouchEnabled(false);


    if (buttonAnswer1->getTag() == 1)
    {
        buttonAnswer1->loadTextureNormal("box-correct.png");
        buttonAnswer1->setTitleColor(Color3B().WHITE);
    }
    if (buttonAnswer2->getTag() == 1)
    {
        buttonAnswer2->loadTextureNormal("box-correct.png");
        buttonAnswer2->setTitleColor(Color3B().WHITE);
    }
    if (buttonAnswer3->getTag() == 1)
    {
        buttonAnswer3->loadTextureNormal("box-correct.png");
        buttonAnswer3->setTitleColor(Color3B().WHITE);
    }
    if (buttonAnswer4->getTag() == 1)
    {
        buttonAnswer4->loadTextureNormal("box-correct.png");
        buttonAnswer4->setTitleColor(Color3B().WHITE);
    }

    //hide pheptoan
    float time = 0.2;
    float valScale = 1.4;
    if (v == correctAnswer || hasGuide == false)
    {
        lblPhepToan->runAction(Spawn::create(ScaleTo::create(time, valScale), FadeOut::create(time), nullptr));
    }
    
    if (sapXepPhepTinhTheoChieuDoc) {
        gachChan->runAction(FadeOut::create(time));
        lblB->runAction(Spawn::create(ScaleTo::create(time, valScale), FadeOut::create(time), nullptr));
        lblLoaiPhepTinh->runAction(Spawn::create(ScaleTo::create(time, valScale), FadeOut::create(time), nullptr));
        lblA->runAction(Spawn::create(ScaleTo::create(time, valScale), FadeOut::create(time), nullptr));

    }
    
    //hide score
    lblScore->runAction(Sequence::create(DelayTime::create(0.9), FadeOut::create(time), NULL));
    scoreIcon->runAction(Sequence::create(DelayTime::create(1.0), FadeOut::create(time), NULL));
    
    /*
    //hide pause
    auto opaPause = FadeOut::create(time);
    auto sqPause = Spawn::create(opaPause, nullptr);
    buttonPause->runAction(sqPause);
    */

    //hide 4 answer
    double timeHide = 0.8;
    if (v != correctAnswer and hasGuide == true) //&&
    {
        
        if (levelOfW5 == 1) {
            if (typeOfW5 == 4) {
                createTemplateGuideLevel1Cong(wScenePlay, hScenePlay);
            } else if (typeOfW5 == 5) {
                createTemplateGuideLevel1Tru(wScenePlay, hScenePlay);
            }
            else if (typeOfW5 == 6) {
                hasGuide = false;
            }
        }else {
            if (typeOfW5 == 4) {
                createTemplateGuideLevel2Cong(wScenePlay, hScenePlay);
            }else if (typeOfW5 == 5) {
                createTemplateGuideLevel2Tru(wScenePlay, hScenePlay);
            }
            else if (typeOfW5 == 6) {
                createTemplateGuideLevel2SoSanh(wScenePlay, hScenePlay);
            }
        }
        timeHide = 0.9;
    }
    DelayTime *pauseButtonAnswer = DelayTime::create(timeHide);
    
    
    auto opaAnswer1 = FadeOut::create(time);
    auto sqAnswer1 = Sequence::create(pauseButtonAnswer, opaAnswer1, nullptr);
    buttonAnswer1->runAction(sqAnswer1);

    auto opaAnswer2 = FadeOut::create(time);
    auto sqAnswer2 = Sequence::create(pauseButtonAnswer, opaAnswer2, nullptr);
    buttonAnswer2->runAction(sqAnswer2);

    auto opaAnswer3 = FadeOut::create(time);
    auto sqAnswer3 = Sequence::create(pauseButtonAnswer, opaAnswer3, nullptr);
    buttonAnswer3->runAction(sqAnswer3);

    auto opaAnswer4 = FadeOut::create(time);
    auto sqAnswer4 = Sequence::create(pauseButtonAnswer, opaAnswer4, nullptr);
    buttonAnswer4->runAction(sqAnswer4);
    
    if (v == correctAnswer)
    {
        //auto dungLienTiep = HuyFunctions().saveSoPhepTinhDungLienTiep(true);
        auto dungMoiNgay = HuyFunctions().saveSoPhepTinhDungMoiNgay(true);
//        if (dungLienTiep != "") {
//            showMedal(wScenePlay, hScenePlay, dungLienTiep);
//        }
        if (dungMoiNgay != "") {
            showMedal(wScenePlay, hScenePlay, dungMoiNgay);
        }
        auto pop = ScaleTo::create(0.08f, scaleButtonAnswer1 * 1.1);
        auto shrink = ScaleTo::create(0.1f, scaleButtonAnswer1);
        auto glow = FadeTo::create(0.08f, 200);

        auto combinedEffect = Spawn::create(
            Sequence::create(pop, shrink, nullptr),  // Nảy lên
            glow,  // Phát sáng nhẹ
            nullptr
        );
        buttonclicked->runAction(combinedEffect);
        // Gọi khi bom nổ
        shakeScreen2(this, 0.2f, 6, 0.9);

        soCau = soCau + 1;
        auto progressTo = ProgressTo::create(0.3f, soCau * 10);
        loadingBar->runAction(progressTo);
        updateStarPosition(soCau * 10);
        
        //tra loi dung
        timeRemaining = timeRemaining + 1;
        tenluaHienThi->setOpacity(255);
        tenluaHienThi->setPositionX(phithuyenHienThi->getPositionX());
        auto actionMoveTo = MoveTo::create(0.8, Vec2(tenluaHienThi->getPositionX() - 5, yTenLua));
        tenluaHienThi->runAction(EaseExponentialIn::create(actionMoveTo));
        
        phithuyenHienThi->runAction(Sequence::create(DelayTime::create(0.8), FadeTo::create(0.2, 0), NULL));
        
        bomPhiThuyenHienThi->Node::setPosition(Vec2(phithuyenHienThi->getPositionX() - 5, phithuyenHienThi->getPositionY()));
        bomPhiThuyenHienThi->runAction(Sequence::create(DelayTime::create(0.8), FadeTo::create(0.1, 255), DelayTime::create(0.5),FadeTo::create(0.05, 0), NULL));
        
        tenluaHienThi->runAction(Sequence::create(DelayTime::create(0.9), FadeTo::create(0.2, 0), NULL));
        
        bomTenLuaHienThi->Node::setPosition(Vec2(tenluaHienThi->getPositionX() - 5, yTenLua + 5));
        bomTenLuaHienThi->runAction(Sequence::create(DelayTime::create(0.8), FadeTo::create(0.1, 255), DelayTime::create(0.5),FadeTo::create(0.05, 0), NULL));
        
        earth->runAction(Sequence::create(DelayTime::create(1.0), FadeTo::create(0.2, 0), NULL));
        
        popupthanhCong->runAction(
          Sequence::create(
               DelayTime::create(0.05),
               Spawn::create(FadeTo::create(0.1, 255), ScaleTo::create(0.07, scalePopup), nullptr),
               ScaleTo::create(0.03, 0.94 * scalePopup),DelayTime::create(0.05),
               Spawn::create(
                     MoveTo::create(0.9, Vec2(scoreIcon->getPositionX() + scoreIcon->getScale() * scoreIcon->getContentSize().width, scoreIcon->getPositionY())),
                     ScaleTo::create(0.9, scalePopup / 2.5),
                     nullptr),
               FadeTo::create(0.14, 0), NULL)
        );
    }else{
        //tra loi sai
        //HuyFunctions().saveSoPhepTinhDungLienTiep(false);
        HuyFunctions().saveSoPhepTinhDungMoiNgay(false);
        auto shake = Sequence::create(
            MoveBy::create(0.05f, Vec2(-10, 0)),
            MoveBy::create(0.05f, Vec2(10, 0)),
            MoveBy::create(0.05f, Vec2(-10, 0)),
            MoveBy::create(0.05f, Vec2(10, 0)),
            nullptr
        );

        auto flashRed = Sequence::create(
            TintTo::create(0.2f, 255, 0, 0),
            TintTo::create(0.2f, 255, 255, 255),
            nullptr
        );

        auto shrink = ScaleTo::create(0.2f, 0.97 * scaleButtonAnswer1);  // Co lại một chút

        auto failEffect = Spawn::create(shake, flashRed, shrink, nullptr);
        buttonclicked->runAction(failEffect);

        
        auto actionMoveTo = MoveTo::create(0.7, Vec2(xDichPhiThuyen, phithuyenHienThi->getPositionY()));
        phithuyenHienThi->runAction(Sequence::create(EaseExponentialIn::create(actionMoveTo), DelayTime::create(0.1), FadeTo::create(0.2, 0), NULL));
        
        bomPhiThuyenHienThi->Node::setPosition(Vec2(xDichPhiThuyen - 5, phithuyenHienThi->getPositionY()));
        bomPhiThuyenHienThi->runAction(Sequence::create(DelayTime::create(0.7), FadeTo::create(0.1, 255), DelayTime::create(0.4),FadeTo::create(0.05, 0), NULL));
        
        // Gọi khi bom nổ
        shakeScreen2(this, 0.3f, 20, 0.7);
        
        earth->runAction(Sequence::create(DelayTime::create(0.8), FadeTo::create(0.2, 0), NULL));
        bomEarthHienThi->runAction(Sequence::create(DelayTime::create(0.7), FadeTo::create(0.1, 255), DelayTime::create(0.4),FadeTo::create(0.05, 0), NULL));
        
        popupthatBai->runAction(Sequence::create(DelayTime::create(0.7), Spawn::create(FadeTo::create(0.1, 255), ScaleTo::create(0.07, scalePopup), nullptr), ScaleTo::create(0.03, 0.94 * scalePopup),DelayTime::create(0.6), FadeTo::create(0.1, 0), NULL));

    }
    DelayTime *pause = DelayTime::create(1.4);

    if (soCau > 9) {
        auto sqBOver = Sequence::create(pause, CCCallFuncN::create(CC_CALLBACK_0(Play5::moveSceneGameOver, this)), NULL);
        runAction(sqBOver);
    } else {
        if (v == correctAnswer)
        {
            score = HuyFunctions().getKetQuaDiemSoNgauNhien(score, Variable().diemNgauNhienPhepTinh.at(indexThemDiem), Variable().diemNgauNhien.at(indexThemDiem));
            
            if (sound == 0)
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/hit.mp3");
            }

            log("ban da tra loi dung");
            auto sqBOver = Sequence::create(pause, CCCallFuncN::create(CC_CALLBACK_0(Play5::nextPlay, this)), NULL);
            runAction(sqBOver);
            
            
        }
        else{
            log("ban da tra loi sai");
            if (sound == 0)
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/bom-no-cham.mp3");
                
                //runAction(Sequence::create(DelayTime::create(0.3), CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/bom-no.mp3"), NULL));
            }
            //isPause = false;
            //auto opaB = FadeOut::create(v);
            if (hasGuide) {
                pause = DelayTime::create(20000);
            }
            
            auto sqBOver = Sequence::create(pause, CCCallFuncN::create(CC_CALLBACK_0(Play5::moveSceneGameOver, this)), NULL);
            runAction(sqBOver);

            //::getInstance()->replaceScene(TransitionFade::create(0.5, Finish::createScene(score, typeOfW5, levelOfW5), Color3B().WHITE));
        }
    }
    if(phepTinhSoSanh1){
    //if (strcmp(loaiPhepTinh->getCString(), "ss") == 0) {
        
        if (v == correctAnswer) {
            HuyFunctions().savePhepTinhToDataBase(isCorrect, ph, 9999, myTime, levelOfW5, score);
        } else {
            HuyFunctions().savePhepTinhToDataBase(isCorrect, ph, 2222, myTime, levelOfW5, score);
        }
    }else {
        if (sapXepPhepTinhTheoChieuDoc) {
            HuyFunctions().savePhepTinhToDataBase(isCorrect, cocos2d::__String::createWithFormat("%d%s%d=?", soA, ph.c_str(), soB)->getCString(), v, myTime, levelOfW5, score);
        } else {
            HuyFunctions().savePhepTinhToDataBase(isCorrect, ph, v, myTime, levelOfW5, score);
            
        }
    }
}
void Play5::nextPlay()
{
    
    if (realType == 1)
    {
        typeOfW5 = cocos2d::RandomHelper::random_int(2, 5);
    }
    removeChild(lblPhepToan);
    if (sapXepPhepTinhTheoChieuDoc) {
        removeChild(lblB);
        removeChild(lblLoaiPhepTinh);
        removeChild(lblA);
        removeChild(gachChan);
    }
    removeChild(earth);
    removeChild(popupthanhCong);
    removeChild(popupthatBai);
    removeChild(phithuyenHienThi);
    removeChild(tenluaHienThi);
    removeChild(lblScore);
    removeChild(buttonAnswer1);
    removeChild(buttonAnswer2);
    removeChild(buttonAnswer3);
    removeChild(buttonAnswer4);

    /*
    //reset pause
    isPause = false;
    buttonPause->loadTextureNormal("pause.png");
    */
    coundownTimer(wScenePlay, hScenePlay);
    createScore(wScenePlay, hScenePlay);
    
    auto arr = CreatePhepToan().createAnswerQuestions(typeOfW5, levelOfW5, prevA, prevB, maxDoiSo);
    
    auto arrNoti = CreatePhepToan().createAnswerQuestions(typeOfW5, levelOfW5, prevA, prevB, maxDoiSo);
    UserDefault::getInstance()->setStringForKey("noti-title1", HuyFunctions().createPhepToanWithEmoji(arrNoti.at(2).c_str()));
    
    log("test improve function");
    //    0 - prevA
    //    1 - prevB
    //    2 - phepToan
    //    3 - correctAnswer
    //    4 - answer1
    //    5 - answer2
    //    6 - answer3
    //    7 - answer4
    prevA = std::stoi(arr.at(0));
    prevB = std::stoi(arr.at(1));
    
    loaiPhepTinh = cocos2d::__String::createWithFormat("%s", arr.at(8).c_str());
    sapXepPhepTinhTheoChieuDoc = CreatePhepToan().needPhepTinhTheoChieuDoc(loaiPhepTinh->getCString(), levelOfW5);
    soA = std::stoi( arr.at(0));
    soB = std::stoi( arr.at(1));
    phepTinhSmallA = arr.at(9);
    phepTinhSmallB = arr.at(10);
    if (sapXepPhepTinhTheoChieuDoc) {
        phepToan = cocos2d::__String::createWithFormat("?");//cocos2d::__String::createWithFormat("%s", arr.at(8).c_str())
        
        flagB1ChuSo = false;
        if (arr.at(1).size() < 2) {
            flagB1ChuSo = true;
        }
    } else {
        phepToan = cocos2d::__String::createWithFormat("%s", arr.at(2).c_str());
        
    }
    correctAnswer = std::stoi(arr.at(3));
    answer1 = std::stoi(arr.at(4));
    answer2 = std::stoi(arr.at(5));
    answer3 = std::stoi(arr.at(6));
    answer4 = std::stoi(arr.at(7));
    
    log("cau hoi: %s",  arr.at(2).c_str());
    log("4 dap an: %i, %i, %i, %i",  std::stoi(arr.at(4)), std::stoi(arr.at(5)), std::stoi(arr.at(6)), std::stoi(arr.at(7)));
    log("dap dung: %i",  std::stoi(arr.at(3)));
    
    indexThemDiem = HuyFunctions().getRandomForThemDiem(score);
    createMainTemplate(wScenePlay, hScenePlay);
    createTraiDatAndPhiThuyenTenLua(wScenePlay, hScenePlay);
    createPopupFinish(wScenePlay, hScenePlay);
    
    int randomImageNameDapAn = cocos2d::RandomHelper::random_int(1, 5);
    ImageNamDapAn = __String::create(StringUtils::format("button-dap-an-%d.png", randomImageNameDapAn));
    ImageNamDapAnX = __String::create(StringUtils::format("button-dap-an-%d-x.png", randomImageNameDapAn));
    ImageNamDapAnV = __String::create(StringUtils::format("button-dap-an-%d-v.png", randomImageNameDapAn));
    createAnswer(wScenePlay, hScenePlay);
}
void Play5::createBgPro(float wScene, float hScene) {
    auto appLang = UserDefault::getInstance()->getStringForKey("LANGUAGE", "en");
    LanguageManager::getInstanceRestart();
    
    auto bgTexture = Director::getInstance()->getTextureCache()->addImage("bg-nen-tet.png");

    // Set texture parameters to repeat
    Texture2D::TexParams texParams = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    bgTexture->setTexParameters(texParams);

    // Create a sprite with repeated texture
    bgNenPro = Sprite::createWithTexture(bgTexture, Rect(0, 0, wScene, hScene));
    bgNenPro->setAnchorPoint(Vec2(0, 0)); // Align to bottom-left
    bgNenPro->setOpacity(245);
    //bgNenPro->setOpacity(0);
    this->addChild(bgNenPro, 100);
    
    //close
    auto btn = ui::Button::create("close-btn.png"); // tao button
    float ratio = 10;
    if (hScene/wScene <= 1.53)
    {
        ratio = 17;
    }
    //CCLOG("Debug: 2");
    auto btnHeight = btn->getContentSize().height;
    auto btnWidth = btn->getContentSize().width;

    float scale = wScene / ratio / btnWidth;
    btn->setScale(scale);
    btn->setPosition(Vec2((btnWidth / 2 * scale + 13), hScene - (btnHeight / 2 * scale + 13)));
    btn->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                //CCLOG("Button setting");
                //redirect to previous scene
                finish();
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //CCLOG("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(btn, 101);//add button vao scene
    
    float fontSize = 0.1 * wScene;
    float fontSizePrice = 0.135 * wScene;
    auto fontStyle = "fonts/ArialUnicodeMS.ttf";
    
    if (UserDefault::getInstance()->getStringForKey("LANGUAGE", "en") == "ja")
    {
        fontStyle = "fonts/KosugiMaru.ttf";
        fontSize = 0.07 * wScene;
    }
    if (hScene / wScene <= 1.53)
    {
        fontSize = 0.05 * wScene;
        fontSizePrice = 0.06 * wScene;
        if (UserDefault::getInstance()->getStringForKey("LANGUAGE", "en") == "ja")
        {
           
            fontSize = 0.035 * wScene;
        }
    }
    //create title pro version
    std::string titleValue = LanguageManager::getInstance()->getStringForKey("TITLE_PRO").c_str();
    auto titlePro = Label::createWithTTF(titleValue, fontStyle, fontSize);
    titlePro->setColor(Color3B().WHITE);
    // position the label on the center of the
    auto btnHeightKey = titlePro->getContentSize().height;
    double ytitlePro = hScene * 0.9 - (btnHeightKey / 2 + 5);
    double xtitlePro = wScene / 2;// - (btnWidthKey / 2 + 5);
    titlePro->setPosition(Vec2(xtitlePro, ytitlePro));
    // add the label as a child to this layer
    addChild(titlePro, 101);
    
    auto bgVIP = Sprite::create("tv-ads.png");
    float ratiobgVIP = 2.4;

    float fixHeightIpad = 20;
    if (hScene / wScene <= 1.8 && hScene / wScene > 1.53)
    {
        fixHeightIpad = 5;
    }
    
    if (hScene / wScene <= 1.53)
    {
        ratiobgVIP = 4.4;
    }
    
    float scalebgVIP = wScene / ratiobgVIP / bgVIP->getContentSize().width;
    bgVIP->setScale(scalebgVIP);
    bgVIP->setPosition(Vec2(wScene / 2, titlePro->getPositionY() - titlePro->getContentSize().height - 20 - bgVIP->getContentSize().height * scalebgVIP / 2)); // Đặt vị trí
    this->addChild(bgVIP, 101);
    
    
    //create desc pro version
    std::string descValue = LanguageManager::getInstance()->getStringForKey("REMOVE_ADS").c_str();
    auto descPro = Label::createWithTTF(descValue, fontStyle , fontSize * 0.8);
    descPro->setColor(Color3B().WHITE);
    // position the label on the center of the
    double ydescPro = bgVIP->getPositionY() - descPro->getContentSize().height /2 - 20 - bgVIP->getContentSize().height * scalebgVIP / 2;
    double xdescPro = wScene / 2;// - (btnWidthKey / 2 + 5);
    descPro->setPosition(Vec2(xdescPro, ydescPro));
    // add the label as a child to this layer
    addChild(descPro, 101);
    
    //create PRICE
    std::string priceValue = UserDefault::getInstance()->getStringForKey("gia", "");
    auto price = Label::createWithTTF(priceValue, "fonts/NunitoSans-Bold.ttf", fontSizePrice);
    price->setColor(Color3B(255, 195, 77));
    // position the label on the center of the
    double yprice = descPro->getPositionY() - descPro->getContentSize().height /2 - fixHeightIpad - price->getContentSize().height / 2;
    double xprice = wScene / 2;// - (btnWidthKey / 2 + 5);
    price->setPosition(Vec2(xprice, yprice));
    // add the label as a child to this layer
    addChild(price, 101);
    
    //create Underline
    auto underlineImg = Sprite::create("underline-pro.png");
    float ratiounderlineImg = 2;
    //float fixHeightNotiIpad = 1;
    if (hScene / wScene <= 1.53)
    {
        ratiounderlineImg = 4;
        //fixHeightNotiIpad = 0.5;
    }
    
    float scaleunderlineImg = wScene / ratiounderlineImg / underlineImg->getContentSize().width;
    underlineImg->setScale(scaleunderlineImg);
    underlineImg->setPosition(Vec2(wScene / 2, price->getPositionY() - price->getContentSize().height / 2 - underlineImg->getContentSize().height * scaleunderlineImg * 0.15)); // Đặt vị trí
    this->addChild(underlineImg, 101);
    
    
    
    fontSize = 0.05 * wScene;
    if (hScene / wScene <= 1.53)
    {
        fontSize = 0.02 * wScene;
    }
    auto restorePurchaseBtn = ui::Button::create("empty.png");
    restorePurchaseBtn->setPressedActionEnabled(true);
    restorePurchaseBtn->setZoomScale(-0.1f);
    restorePurchaseBtn->setScale9Enabled(true);
    restorePurchaseBtn->setTitleText("Restore Purchase");
    if(appLang == "en" || appLang == "vi"){
        restorePurchaseBtn->setTitleFontName("fonts/NunitoSans-Bold.ttf");
    }
    restorePurchaseBtn->setTitleColor(Color3B().WHITE);
    restorePurchaseBtn->setOpacity(200);
    restorePurchaseBtn->setTouchEnabled(true);
    float ratiotitle = 3;
    if (hScene / wScene <= 1.53)
    {
        ratiotitle = 6;
        
    }
    auto scalerestoreBtn = wScene / ratiotitle / restorePurchaseBtn->getContentSize().width;
    restorePurchaseBtn->setScale(scalerestoreBtn);
    restorePurchaseBtn->setTitleFontSize(fontSize / scalerestoreBtn);
    if (appLang == "ja")
    {
        restorePurchaseBtn->setTitleFontName("fonts/KosugiMaru.ttf");
        restorePurchaseBtn->setTitleFontSize(0.045 * wScene / scalerestoreBtn);
    }
    float ytitle = 40;
    if (hScene / wScene <= 1.8 && hScene / wScene > 1.53)
    {
        
        ytitle = 20;
    }
    if (hScene / wScene <= 1.53)
    {
        
        if (appLang == "ja")
        {
            
            restorePurchaseBtn->setTitleFontSize(0.02 * wScene / scalerestoreBtn);
        }
    }
    restorePurchaseBtn->setPosition(Vec2(wScene / 2, ytitle));
    restorePurchaseBtn->setEnabled(true);
    restorePurchaseBtn->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        //auto buttonIcon = dynamic_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                log("restore purchasing");
                IAPManager::getInstance()->restorePurchases();
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //CCLOG("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    
    addChild(restorePurchaseBtn, 101);
    
    float fontSize2 = 0.07 * wScene;
    if (hScene / wScene <= 1.53)
    {
        fontSize2 = 0.035 * wScene;
    }
    auto upProBtn = ui::Button::create("button-pro.png");
    upProBtn->setPressedActionEnabled(true);
    upProBtn->setZoomScale(-0.1f);
    upProBtn->setScale9Enabled(true);
    upProBtn->setTitleText(LanguageManager::getInstance()->getStringForKey("PRO_BUY").c_str());
    if(appLang == "en" || appLang == "vi"){
        upProBtn->setTitleFontName("fonts/NunitoSans-Bold.ttf");
    }
    upProBtn->setTitleColor(Color3B().WHITE);
    upProBtn->setTouchEnabled(true);
    ratiotitle = 1.5;
    if (hScene / wScene <= 1.53)
    {
        ratiotitle = 3.4;
       
    }
    auto scaleUpProBtn = wScene / ratiotitle / upProBtn->getContentSize().width;
    upProBtn->setScale(scaleUpProBtn);
    upProBtn->setTitleFontSize(fontSize2 / scaleUpProBtn);
    if (appLang == "ja")
    {
        upProBtn->setTitleFontName("fonts/KosugiMaru.ttf");
        upProBtn->setTitleFontSize(0.05 * wScene / scaleUpProBtn);
    }
    ytitle = ytitle + restorePurchaseBtn->getContentSize().height * scalerestoreBtn / 2 + upProBtn->getContentSize().height * scaleUpProBtn / 2 + 7;
    if (hScene / wScene <= 1.53)
    {
      
        if (appLang == "ja")
        {
            upProBtn->setTitleFontSize(0.025 * wScene / scaleUpProBtn);
        }
    }
    upProBtn->setPosition(Vec2(wScene / 2, ytitle));
    upProBtn->setEnabled(true);
    upProBtn->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        //auto buttonIcon = dynamic_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                log("up to pro");
                IAPManager::getInstance()->purchase(Variable().proProductionID);
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //CCLOG("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    
    addChild(upProBtn, 101);
    
}

void Play5::onEnter() {
    Layer::onEnter();

    // Đăng ký callback khi mua thành công
    IAPManager::getInstance()->setOnPurchaseSuccess([this](std::string productID) {
        log("🎉 Mua thành công - restore thanh cong: %s", productID.c_str());
        UserDefault::getInstance()->setBoolForKey("isProVer", true);
        //Variable().isProVer = true;
        UserDefault::getInstance()->flush();
        this->finish();
    });

    // Đăng ký callback khi mua thất bại
    IAPManager::getInstance()->setOnPurchaseFailed([this](std::string productID, std::string error) {
        log("❌ Mua thất bại: %s | Lỗi: %s", productID.c_str(), error.c_str());
        //this->finish();
        
        
    });
    
    // Đăng ký callback lấy giá
    IAPManager::getInstance()->setOnPriceRetrieved([this](std::string productID, std::string price) {
        log("📦 Giá của %s là: %s", productID.c_str(), price.c_str());
        // Gợi ý: update label nút mua
        //this->price->setString(price.c_str());
        UserDefault::getInstance()->setStringForKey("gia", price.c_str());
    });

    // Yêu cầu lấy giá từ App Store
    IAPManager::getInstance()->requestProductInfo(Variable().proProductionID);
}
void Play5::finish() {
    if (Variable().isProduction == true) {
        HuyFunctions().showAds();
    }
    HuyFunctions().saveSoGame("g1");
    
    if(isMulti5 == true){
        typeOfW5 = 1;
    }
    if (soCau > 9) {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, Finish::createScene(score, typeOfW5, levelOfW5, 6, fromThuThach5, true), Color3B().WHITE));
    }else{
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, Finish::createScene(score, typeOfW5, levelOfW5, 6, fromThuThach5), Color3B().WHITE));
    }
    
}
void Play5::moveSceneGameOver()
{
    //kiem tra xem co show man hinh mua hang khong
    if (Variable().isProVer == false && score >= Variable().soDiemShowPro &&  UserDefault::getInstance()->getIntegerForKey("diemshowpro", 0) >= Variable().showProSauBaoNhieuLan) {
        UserDefault::getInstance()->setIntegerForKey("diemshowpro", 0);
        createBgPro(wScenePlay, hScenePlay);
    } else {
        UserDefault::getInstance()->setIntegerForKey("diemshowpro", UserDefault::getInstance()->getIntegerForKey("diemshowpro", 0) + 1);
        finish();
    }
}
void Play5::createMainTemplate(float w, float h){
    auto fontStyle = "fonts/DeliusSwashCaps.ttf";
    float fontSize = 0.18 * w;
    if (h / w > 1.53 && h / w <= 1.8){
        fontSize = 0.15 * w;
    }
    if (h / w <= 1.53)
    {
        fontSize = 0.09 * w;
    }
    lblPhepToan = Label::createWithTTF(phepToan->getCString(), fontStyle, fontSize);
    lblPhepToan->setColor(Color3B().WHITE);//Color3B().BLACK
    // position the label on the center of the screen
    
    lblPhepToan->setPosition(Vec2(w / 2, h * 0.42));
    if (h / w > 1.53 && h / w <= 1.8){
        lblPhepToan->setPosition(Vec2(w / 2, h * 0.44));
    }
    if (h / w <= 1.53)
    {
        lblPhepToan->setPosition(Vec2(w / 2, h * 0.35));
    }
    // add the label as a child to this layer
    addChild(lblPhepToan, 2);
    lblPhepToan->setOpacity(0);
    float time = 0.2;
    lblPhepToan->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
    
    if (sapXepPhepTinhTheoChieuDoc) {
        float width = 50.0f;
        float fixWithIpad = 1.0f;
        if (h / w <= 1.53)
        {
            fixWithIpad = 0.5f;
        }
        if (levelOfW5 > 1) {
            width = 70.0f;
        }
        float height = 3.0f * fixWithIpad;
        width = width * fixWithIpad;
        cocos2d::Vec2 origin(w / 2 - width / 2 - 13 * fixWithIpad, lblPhepToan->getPositionY() + lblPhepToan->getContentSize().height / 2);
        gachChan = HuyFunctions().createChuNhatBoGoc(width, height, origin);
        this->addChild(gachChan, 2);
        
        lblB = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", soB)->getCString(), fontStyle, fontSize);
        float xLblB = w / 2;
        lblB->setPosition(Vec2(xLblB, lblPhepToan->getPositionY() + lblPhepToan->getContentSize().height));
        addChild(lblB, 2);
        lblB->setOpacity(0);
        lblB->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
        
        lblLoaiPhepTinh = Label::createWithTTF(loaiPhepTinh->getCString(), fontStyle, fontSize);
        lblLoaiPhepTinh->setPosition(Vec2(w/2 - lblB->getContentSize().width / 2 - 13, lblB->getPositionY() + lblPhepToan->getContentSize().height / 2));
        addChild(lblLoaiPhepTinh, 2);
        lblLoaiPhepTinh->setOpacity(0);
        lblLoaiPhepTinh->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
        
        lblA = Label::createWithTTF(cocos2d::__String::createWithFormat("%d", soA)->getCString(), fontStyle, fontSize);
        lblA->setPosition(Vec2(w / 2, lblLoaiPhepTinh->getPositionY() + lblLoaiPhepTinh->getContentSize().height / 2));
        addChild(lblA, 2);
        lblA->setOpacity(0);
        lblA->runAction(Sequence::create(FadeOut::create(0), FadeIn::create(time), nullptr));
        
        if (flagB1ChuSo) {
            xLblB = w / 2 + (lblA->getContentSize().width - lblB->getContentSize().width) / 2;
            lblB->setPosition(Vec2(xLblB, lblB->getPositionY()));
        }
    }
}
void Play5::back(float w, float h){
    auto btn = ui::Button::create("back3.png", "back3.png"); // tao button
    float ratio = 10;
    if (h / w <= 1.53)
    {
        
        ratio = 17;
    }
    auto btnHeight = btn->getContentSize().height;
    auto btnWidth = btn->getContentSize().width;

    float scale = w / ratio / btnWidth;
    btn->setScale(scale);
    btn->setPosition(Vec2((btnWidth / 2 * scale + 13), h - (btnHeight / 2 * scale + 13)));
    btn->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        auto button = static_cast<cocos2d::ui::Button*>(sender);
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                //log("Button setting");
                //redirect to previous scene
                HuyFunctions().animationClickedButton(button, [](){
                    if (Variable().isProduction == true) {
                        HuyFunctions().showAds();
                    }
                    if (fromThuThach5) {
                        Director::getInstance()->replaceScene(TransitionFade::create(0.5, SelectGame::createScene(typeOfW5, levelOfW5), Color3B().WHITE));
                    } else {
                        Director::getInstance()->replaceScene(TransitionFade::create(0.5, MathSelect::createScene(0, backTypePT5), Color3B().WHITE));
                    }
                    
                });

                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(btn, 2);//add button vao scene
}
void Play5::createValueKey(float w, float h){
    //valueKey
    auto valueK = cocos2d::__String::createWithFormat("%i", UserDefault::getInstance()->getIntegerForKey("KEY", 0));
    valueKey3 = Label::createWithTTF(valueK->getCString(), "fonts/ComicNeue-Bold.ttf", 0.06 * w);
    valueKey3->setColor(Color3B().BLACK);
    // position the label on the center of the
    auto btnHeightKey = valueKey3->getContentSize().height;
    auto btnWidthKey = valueKey3->getContentSize().width;
    yValueKey3 = h - (btnHeightKey / 2 + 5);
    xValueKey3 = w - (btnWidthKey / 2 + 5);
    valueKey3->setPosition(Vec2(xValueKey3, yValueKey3));
    // add the label as a child to this layer
    addChild(valueKey3);
}
void Play5::createButtonKey(float w, float h){
    createValueKey(w, h);
    auto btn = ui::Button::create("key1.png", "key-active.png"); // tao button
    float ratio = 14;
    auto btnWidth = btn->getContentSize().width;
    float scale = w / ratio / btnWidth;
    float xBtn = xValueKey3 - 7 - valueKey3->getContentSize().width / 2 - btn->getContentSize().width / 2 * scale;
    btn->setScale(scale);
    btn->setPosition(Vec2(xBtn, yValueKey3));
    btn->setEnabled(true);
    btn->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){ // xu ly su kien button
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                //code admob
                UserDefault::getInstance()->setIntegerForKey("KEY", UserDefault::getInstance()->getIntegerForKey("KEY", 0) + 1);
                //restart valueKey Label
                removeChild(valueKey3);
                createValueKey(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height);
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                //log("Button 1 clicked");
                break;
            default:
                break;
        }
    });
    addChild(btn);//add button vao scene
}
