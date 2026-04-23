#ifndef __PLAY5_H__
#define __PLAY5_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Play5 : public cocos2d::Layer
{
public:
    cocos2d::__String *ImageNamDapAnX;
    cocos2d::__String *ImageNamDapAnV;
    bool phepTinhSoSanh1 = false;
    
    const char *HIGH_SCORE = "height-score";
    virtual void onEnter() override;
    
    int soCau;
    int realType;
    int answer1;
    int answer2;
    int answer3;
    int answer4;

    cocos2d::ui::LoadingBar* loadingBar;
    
    double wScenePlay;
    double hScenePlay;
    int correctAnswer;

    int score;

    cocos2d::Vector<cocos2d::ui::Button*> listBgNumber;
    cocos2d::Vector<cocos2d::Sprite*> listIcon;
    
    cocos2d::Vector<cocos2d::Sprite*> listIconTru;
    cocos2d::Vector<cocos2d::Sprite*> listBom;
    
    cocos2d::ui::Button* buttonOK;
    
    int maxDoiSo;
    cocos2d::__String *ImageNamDapAn;
    
    bool flagB1ChuSo;
    cocos2d::__String *phepToan;
    int soA;
    int soB;
    std::string phepTinhSmallA;
    std::string phepTinhSmallB;
    
    cocos2d::__String *loaiPhepTinh;
    
    cocos2d::__String *scoreString;

    bool sapXepPhepTinhTheoChieuDoc;
    bool hasGuide;
    cocos2d::Label* lblPhepToan;
    cocos2d::DrawNode* gachChan;
    cocos2d::Label* lblB;
    cocos2d::Label* lblBHangChuc;
    cocos2d::Label* lblLoaiPhepTinh;
    cocos2d::Label* lblA;
    cocos2d::Label* lblAKetQua;
    cocos2d::Label* lblBKetQua;
    cocos2d::Label* lblPhepSoSanh1;
    cocos2d::Label* lblAHangChuc;
    cocos2d::Label* lblDapAnHangChuc;
    cocos2d::Label* lblDapAnHangDV;
    cocos2d::Label* lblDapAnPhanNhoDV;
    cocos2d::Label* lblDapAnPhanNhoHangChuc;
    
    
    cocos2d::Label* lblScore;
    cocos2d::ui::Button* scoreIcon;
    cocos2d::ui::Button* earth;
    cocos2d::Sprite* phithuyenHienThi;
    cocos2d::Sprite* tenluaHienThi;
    cocos2d::Sprite* bomPhiThuyenHienThi;
    cocos2d::Sprite* bomEarthHienThi;
    cocos2d::Sprite* bomTenLuaHienThi;
    
    cocos2d::ui::Button* popupthanhCong;
    cocos2d::ui::Button* popupthatBai;

    float xDichPhiThuyen;
    float yTenLua;

    cocos2d::ui::Button* buttonAnswer1;
    cocos2d::ui::Button* buttonAnswer2;
    cocos2d::ui::Button* buttonAnswer3;
    cocos2d::ui::Button* buttonAnswer4;
    cocos2d::ui::Button* buttonPause;
    
    int indexThemDiem;

    float scaleButtonAnswer1;

    float timeRemaining;
    float timeMax;
    bool isPause = false;

    cocos2d::Label* valueKey3;
    float yValueKey3;
    float xValueKey3;

    int sound;
    int prevA = 2;
    int prevB = 2;
    cocos2d::Sprite* bgNenPro;
    cocos2d::Sprite* star;
    cocos2d::Vec2 starTargetPos;
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int type, int level, int backTypePhepTinh, bool fromChallenge = false);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void update(float dt);
    void coundownTimer(float w, float h);
    void back(float w, float h);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    void createButtonKey(float w, float h);
    void createValueKey(float w, float h);
    void createBg(float w, float h);
    void createScore(float w, float h);
    void createMainTemplate(float w, float h);
    void createTienDo(float w, float h);
    void updateStarPosition(float percent);
    void shakeScreen2(Node* target, float duration, float strength, float delayTime);
    void replaceAll(std::string& str, const std::string& from, const std::string& to);
    void createBg();
    void createBgColor(float w, float h);
    void createTraiDatAndPhiThuyenTenLua(float w, float h);
    void createPopupFinish(float w, float h);
    void createAnswer(float w, float h);
    void animationShowAnswer(cocos2d::ui::Button* button, float time, float scale);
    void handlerClickAnswer(int v, cocos2d::ui::Button* button);
    
    void createTemplateGuideLevel1(float w, float h);
    void createTemplateGuideLevel1Cong(float w, float h);
    void createTemplateGuideLevel1Tru(float w, float h);
    void createTemplateGuideLevel1Nhan(float w, float h);
    void createTemplateGuideLevel1Chia(float w, float h);
    
    void createTemplateGuideLevel2(float w, float h);
    void createTemplateGuideLevel2Cong(float w, float h);
    void createTemplateGuideLevel2SoSanh(float w, float h);
    void createTemplateGuideLevel2Tru(float w, float h);
    void createTemplateGuideLevel2Nhan(float w, float h);
    void createTemplateGuideLevel2Chia(float w, float h);
    
    void createTemplateGuideLevel3(float w, float h);
    void createTemplateGuideLevel3Cong(float w, float h);
    void createTemplateGuideLevel3Tru(float w, float h);
    void createTemplateGuideLevel3Nhan(float w, float h);
    void createTemplateGuideLevel3Chia(float w, float h);
    void showMedal(float w, float h, std::string medal);
    
    void createGuideBom(float myScale, cocos2d::Vec2 position);
    
    void createBgPro(float w, float h);
    void finish();
    
    void moveSceneGameOver();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    
    void nextPlay();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Play5);
};

#endif // __PLAY5_H__
