var m_MainFrameLayer = null;
var m_bgresult_sprite = null;
var m_GamingState_sprite1 = null;
var m_GamingState_sprite2 = null;
var m_GamingState_sprite3 = null;
var m_number_timer_sprite = null;
var m_spriteresult_sprite = null;
var m_cbCardListData = [];
var m_cbCardListSprite = [];
var m_cbCarFlashResult = [];
var m_all_jettonsprites = [];
var m_all_jettonsprites_count = 0;
var m_yanzhuarea = [];
var m_curAnimTime = 0;
var m_curAnimSpeed = 150.0;
var m_isAnimFanSpeed = false;
var m_curAnimCircle = 0;
var m_curDecPosition = 0;
var m_curShowAnimFrame = 0;
var m_user_mouse_isprocess = false;
var m_MenuItemObjTag = 100;
var m_game_timer_id = null;
var m_game_timer_current_time = 0;
var m_labelallJettons = null;
var m_labelName = null;
var m_labelMoney = null;
var m_labelCurJetton = null;
var m_labelTotalResult = null;
var v_number_sprite = null;
var v_gameover_bg_sprite = null;
var v_gameover_bg_lost_sprite = null;
var v_gameover_bg_win_sprite = null;
var v_game_state_timer_id = null;
var v_game_waiting_colorlayer = null;
var v_game_waiting_sprite = null;
var v_nomoney_xiazhu_sprite = null;
var v_qingxiazhu_sprite = null;
var v_gaming_result_vector = [];
var v_game_record_sprite_list = [];

function GetStopPosition(curPos)
{
    if(curPos-10 >= 0)
        return curPos-10;

    return 16+(curPos-10);
}

function addvaluetogamerecord(value) {
    v_gaming_result_vector.push(value);

    if(v_gaming_result_vector.length > 9) {
        v_gaming_result_vector.splice(0,1);
    }
}

function ShowGamingRecords() {
    var i = 0;
    for(var value of v_gaming_result_vector) {
        switch(value)
        {
            case SSZP_CARDTYPE_QINGLONG:
            {
                v_game_record_sprite_list[i].setTextureRect(cc.rect(56*3,0,56,50));
            }
                break;
            case SSZP_CARDTYPE_BAIHU:
            {
                v_game_record_sprite_list[i].setTextureRect(cc.rect(56*2,0,56,50));
            }
                break;
            case SSZP_CARDTYPE_ZHUQUE:
            {
                v_game_record_sprite_list[i].setTextureRect(cc.rect(56,0,56,50));
            }
                break;
            case SSZP_CARDTYPE_XUANWU:
            {
                v_game_record_sprite_list[i].setTextureRect(cc.rect(0,0,56,50));
            }
                break;
            case SSZP_CARDTYPE_XIAOBAILONG:
            {
                v_game_record_sprite_list[i].setTextureRect(cc.rect(56*7,0,56,50));
            }
                break;
            case SSZP_CARDTYPE_XIAOLAOHU:
            {
                v_game_record_sprite_list[i].setTextureRect(cc.rect(56*6,0,56,50));
            }
                break;
            case SSZP_CARDTYPE_XIAOFENGHUANG:
            {
                v_game_record_sprite_list[i].setTextureRect(cc.rect(56*5,0,56,50));
            }
                break;
            case SSZP_CARDTYPE_XIAOWUGUI:
            {
                v_game_record_sprite_list[i].setTextureRect(cc.rect(56*4,0,56,50));
            }
                break;
            default:
                break;
        }

        v_game_record_sprite_list[i].setVisible(true);
        i=i+1;
    }
}

function showgameresult(result,isshow) {
    var tmpStr = ";";

    if(result > 0) {
        tmpStr = "<";
    }

    tmpStr = tmpStr + (Math.abs(result)).toString();
    v_number_sprite.setString(tmpStr);

    v_gameover_bg_sprite.setVisible(isshow);
    v_gameover_bg_lost_sprite.setVisible(isshow);
    v_gameover_bg_win_sprite.setVisible(isshow);
    v_number_sprite.setVisible(isshow);
}

function ShowGameState(type,isshow) {
    v_game_waiting_sprite.setVisible(false);
    v_game_waiting_colorlayer.setVisible(false);
    v_nomoney_xiazhu_sprite.setVisible(false);
    v_qingxiazhu_sprite.setVisible(false);

    if(type == 1) {
        v_game_waiting_sprite.setVisible(isshow);
        v_game_waiting_colorlayer.setVisible(isshow);
    }
    else if(type == 2) {
        v_nomoney_xiazhu_sprite.setVisible(isshow);
    }
    else if(type == 3) {
        v_qingxiazhu_sprite.setVisible(isshow);
    }

    if(isshow) {
        v_game_state_timer_id = setInterval( function(){
            clearInterval(v_game_state_timer_id);

            if(type == 2) {
                v_nomoney_xiazhu_sprite.setVisible(!isshow);
            }
            else if(type == 3) {
                v_qingxiazhu_sprite.setVisible(!isshow);
            }
        },2000);
    }
}

function EnableMouseProcess(isenable) {
    m_user_mouse_isprocess = isenable;
}

function StartAnimation(decPos) {
    var dateObject = new Date();
    m_curAnimTime = dateObject.getTime();
    m_curAnimSpeed = 150.0;
    m_isAnimFanSpeed = false;
    m_curAnimCircle = 0;
    m_curDecPosition = decPos;

    m_MainFrameLayer.schedule(OnProcessAnimationMessage, 0.01);
}

function clearAnimation() {
    m_MainFrameLayer.unschedule(OnProcessAnimationMessage);

    for(var i=0;i<16;i++) {
        m_cbCardListSprite[i].setVisible(false);
    }

    for(var i=0;i<8;i++) {
        m_cbCarFlashResult[i].setVisible(false);
    }
}

function starttimer(timenum) {
    if(m_game_timer_id != null) {
        clearInterval(m_game_timer_id);
    }

    m_game_timer_current_time = timenum;
    m_number_timer_sprite.setString(m_game_timer_current_time);
    m_number_timer_sprite.setVisible(true);

    m_game_timer_id = setInterval( function(){
        m_game_timer_current_time = m_game_timer_current_time - 1;
        if(m_game_timer_current_time < 0) {
            m_number_timer_sprite.setVisible(false);
            clearInterval(m_game_timer_id);
        }

        if(m_game_timer_current_time < 4) {
            cc.audioEngine.playEffect(v_sound_clock, false);
        }

        m_number_timer_sprite.setString(m_game_timer_current_time);
    },1000);
}

function ShowGamingState(pGameState)
{
    m_GamingState_sprite1.setVisible(false);
    m_GamingState_sprite2.setVisible(false);
    m_GamingState_sprite3.setVisible(false);

    switch(pGameState)
    {
        case STATE_XIAZHU:
        {
            m_GamingState_sprite2.setVisible(true);
        }
            break;
        case STATE_GAMING:
        {
            m_GamingState_sprite3.setVisible(true);
        }
            break;
        case STATE_GAMEEND:
        {
            m_GamingState_sprite1.setVisible(true);
        }
            break;
        default:
            break;
    }
}

var CMainFrameLayer = cc.Layer.extend({
    bgsprite:null,
    bgcentesprite:null,
    s_100_btn:null,
    s_500_btn:null,
    s_1000_btn:null,
    s_5000_btn:null,
    s_10000_btn:null,
    s_100000_btn:null,

    init:function () {
        this._super();

        var size = cc.director.getWinSize();

        this.bgsprite = new cc.Sprite(v_sprite_Image_bj);
        this.bgsprite.setAnchorPoint(0.5, 0.5);
        this.bgsprite.setPosition(size.width / 2, size.height / 2);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(this.bgsprite, 0);

        this.bgcentesprite = new cc.Sprite(v_sprite_center_bg);
        this.bgcentesprite.setAnchorPoint(0.5, 0.5);
        this.bgcentesprite.setPosition(size.width / 2, size.height / 2);
        this.bgcentesprite.setScaleY(0.67);
        this.bgcentesprite.setScaleX(0.78);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(this.bgcentesprite, 0);

        m_cbCardListData[0] = SSZP_CARDTYPE_XIAOBAILONG;
        m_cbCardListSprite[0] = new cc.Sprite(v_sprite_xiaobailong_0);
        m_cbCardListSprite[0].setPosition(size.width/2+80,size.height/2+167);
        m_cbCardListSprite[0].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[0].setScaleX(0.72);
        m_cbCardListSprite[0].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[0]);

        m_cbCardListData[1] = SSZP_CARDTYPE_XIAOLAOHU;
        m_cbCardListSprite[1] = new cc.Sprite(v_sprite_xiaolaohu_0);
        m_cbCardListSprite[1].setPosition(size.width/2+147,size.height/2+130);
        m_cbCardListSprite[1].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[1].setScaleX(0.72);
        m_cbCardListSprite[1].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[1]);

        m_cbCardListData[2] = SSZP_CARDTYPE_XIAOFENGHUANG;
        m_cbCardListSprite[2] = new cc.Sprite(v_sprite_xiaofenghuang_0);
        m_cbCardListSprite[2].setPosition(size.width/2+191,size.height/2+78);
        m_cbCardListSprite[2].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[2].setScaleX(0.72);
        m_cbCardListSprite[2].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[2]);

        m_cbCardListData[3] = SSZP_CARDTYPE_QINGLONG;
        m_cbCardListSprite[3] = new cc.Sprite(v_sprite_qinglong_0);
        m_cbCardListSprite[3].setPosition(size.width/2+211,size.height/2+6);
        m_cbCardListSprite[3].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[3].setScaleX(0.72);
        m_cbCardListSprite[3].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[3]);

        m_cbCardListData[4] = SSZP_CARDTYPE_XIAOLAOHU;
        m_cbCardListSprite[4] = new cc.Sprite(v_sprite_xiaolaohu_0);
        m_cbCardListSprite[4].setPosition(size.width/2+193,size.height/2-62);
        m_cbCardListSprite[4].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[4].setScaleX(0.72);
        m_cbCardListSprite[4].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[4]);

        m_cbCardListData[5] = SSZP_CARDTYPE_XIAOFENGHUANG;
        m_cbCardListSprite[5] = new cc.Sprite(v_sprite_xiaofenghuang_0);
        m_cbCardListSprite[5].setPosition(size.width/2+148,size.height/2-120);
        m_cbCardListSprite[5].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[5].setScaleX(0.72);
        m_cbCardListSprite[5].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[5]);

        m_cbCardListData[6] = SSZP_CARDTYPE_XIAOWUGUI;
        m_cbCardListSprite[6] = new cc.Sprite(v_sprite_xiaowugui_0);
        m_cbCardListSprite[6].setPosition(size.width/2+80,size.height/2-160);
        m_cbCardListSprite[6].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[6].setScaleX(0.72);
        m_cbCardListSprite[6].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[6]);

        m_cbCardListData[7] = SSZP_CARDTYPE_BAIHU;
        m_cbCardListSprite[7] = new cc.Sprite(v_sprite_baihu_0);
        m_cbCardListSprite[7].setPosition(size.width/2-7,size.height/2-174);
        m_cbCardListSprite[7].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[7].setScaleX(0.72);
        m_cbCardListSprite[7].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[7]);

        m_cbCardListData[8] = SSZP_CARDTYPE_XIAOFENGHUANG;
        m_cbCardListSprite[8] = new cc.Sprite(v_sprite_xiaofenghuang_0);
        m_cbCardListSprite[8].setPosition(size.width/2-93,size.height/2-159);
        m_cbCardListSprite[8].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[8].setScaleX(0.72);
        m_cbCardListSprite[8].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[8]);

        m_cbCardListData[9] = SSZP_CARDTYPE_XIAOWUGUI;
        m_cbCardListSprite[9] = new cc.Sprite(v_sprite_xiaowugui_0);
        m_cbCardListSprite[9].setPosition(size.width/2-157,size.height/2-119);
        m_cbCardListSprite[9].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[9].setScaleX(0.72);
        m_cbCardListSprite[9].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[9]);

        m_cbCardListData[10] = SSZP_CARDTYPE_XIAOBAILONG;
        m_cbCardListSprite[10] = new cc.Sprite(v_sprite_xiaobailong_0);
        m_cbCardListSprite[10].setPosition(size.width/2-198,size.height/2-64);
        m_cbCardListSprite[10].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[10].setScaleX(0.72);
        m_cbCardListSprite[10].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[10]);

        m_cbCardListData[11] = SSZP_CARDTYPE_ZHUQUE;
        m_cbCardListSprite[11] = new cc.Sprite(v_sprite_fenghuang_0);
        m_cbCardListSprite[11].setPosition(size.width/2-216,size.height/2+6);
        m_cbCardListSprite[11].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[11].setScaleX(0.72);
        m_cbCardListSprite[11].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[11]);

        m_cbCardListData[12] = SSZP_CARDTYPE_XIAOWUGUI;
        m_cbCardListSprite[12] = new cc.Sprite(v_sprite_xiaowugui_0);
        m_cbCardListSprite[12].setPosition(size.width/2-197,size.height/2+78);
        m_cbCardListSprite[12].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[12].setScaleX(0.72);
        m_cbCardListSprite[12].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[12]);

        m_cbCardListData[13] = SSZP_CARDTYPE_XIAOLAOHU;
        m_cbCardListSprite[13] = new cc.Sprite(v_sprite_xiaolaohu_0);
        m_cbCardListSprite[13].setPosition(size.width/2-155,size.height/2+130);
        m_cbCardListSprite[13].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[13].setScaleX(0.72);
        m_cbCardListSprite[13].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[13]);

        m_cbCardListData[14] = SSZP_CARDTYPE_XIAOBAILONG;
        m_cbCardListSprite[14] = new cc.Sprite(v_sprite_xiaobailong_0);
        m_cbCardListSprite[14].setPosition(size.width/2-92,size.height/2+167);
        m_cbCardListSprite[14].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[14].setScaleX(0.72);
        m_cbCardListSprite[14].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[14]);

        m_cbCardListData[15] = SSZP_CARDTYPE_XUANWU;
        m_cbCardListSprite[15] = new cc.Sprite(v_sprite_xuanwu_0);
        m_cbCardListSprite[15].setPosition(size.width/2-7,size.height/2+182);
        m_cbCardListSprite[15].setTextureRect(cc.rect(0,0,124,124));
        m_cbCardListSprite[15].setScaleX(0.72);
        m_cbCardListSprite[15].setScaleY(0.62);
        this.addChild(m_cbCardListSprite[15]);

        m_cbCarFlashResult[SSZP_CARDTYPE_QINGLONG] = new cc.Sprite(v_sprite_qinglong_1);
        m_cbCarFlashResult[SSZP_CARDTYPE_QINGLONG].setPosition(size.width / 2 + 82, size.height / 2 + 40);
        m_cbCarFlashResult[SSZP_CARDTYPE_QINGLONG].setScaleY(0.69);
        m_cbCarFlashResult[SSZP_CARDTYPE_QINGLONG].setScaleX(0.77);
        m_cbCarFlashResult[SSZP_CARDTYPE_QINGLONG].setVisible(false);
        this.addChild(m_cbCarFlashResult[SSZP_CARDTYPE_QINGLONG]);

        m_cbCarFlashResult[SSZP_CARDTYPE_BAIHU] = new cc.Sprite(v_sprite_baihu_1);
        m_cbCarFlashResult[SSZP_CARDTYPE_BAIHU].setPosition(size.width/2+82,size.height/2-31);
        m_cbCarFlashResult[SSZP_CARDTYPE_BAIHU].setScaleY(0.69);
        m_cbCarFlashResult[SSZP_CARDTYPE_BAIHU].setScaleX(0.77);
        m_cbCarFlashResult[SSZP_CARDTYPE_BAIHU].setVisible(false);
        this.addChild(m_cbCarFlashResult[SSZP_CARDTYPE_BAIHU]);

        m_cbCarFlashResult[SSZP_CARDTYPE_ZHUQUE] = new cc.Sprite(v_sprite_zhuque_1);
        m_cbCarFlashResult[SSZP_CARDTYPE_ZHUQUE].setPosition(size.width/2-86,size.height/2-31);
        m_cbCarFlashResult[SSZP_CARDTYPE_ZHUQUE].setScaleY(0.69);
        m_cbCarFlashResult[SSZP_CARDTYPE_ZHUQUE].setScaleX(0.77);
        m_cbCarFlashResult[SSZP_CARDTYPE_ZHUQUE].setVisible(false);
        this.addChild(m_cbCarFlashResult[SSZP_CARDTYPE_ZHUQUE]);

        m_cbCarFlashResult[SSZP_CARDTYPE_XUANWU] = new cc.Sprite(v_sprite_xuanwu_1);
        m_cbCarFlashResult[SSZP_CARDTYPE_XUANWU].setPosition(size.width/2-86,size.height/2+40);
        m_cbCarFlashResult[SSZP_CARDTYPE_XUANWU].setScaleY(0.69);
        m_cbCarFlashResult[SSZP_CARDTYPE_XUANWU].setScaleX(0.77);
        m_cbCarFlashResult[SSZP_CARDTYPE_XUANWU].setVisible(false);
        this.addChild(m_cbCarFlashResult[SSZP_CARDTYPE_XUANWU]);

        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOBAILONG] = new cc.Sprite(v_sprite_xiaobailong_1);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOBAILONG].setPosition(size.width/2+71,size.height/2+110);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOBAILONG].setScaleY(0.66);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOBAILONG].setScaleX(0.77);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOBAILONG].setVisible(false);
        this.addChild(m_cbCarFlashResult[SSZP_CARDTYPE_XIAOBAILONG]);

        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOLAOHU] = new cc.Sprite(v_sprite_xiaolaohu_1);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOLAOHU].setPosition(size.width/2+71,size.height/2-103);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOLAOHU].setScaleY(0.66);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOLAOHU].setScaleX(0.77);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOLAOHU].setVisible(false);
        this.addChild(m_cbCarFlashResult[SSZP_CARDTYPE_XIAOLAOHU]);

        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOFENGHUANG] = new cc.Sprite(v_sprite_xiaofenghuang_1);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOFENGHUANG].setPosition(size.width/2-74,size.height/2-103);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOFENGHUANG].setScaleY(0.65);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOFENGHUANG].setScaleX(0.77);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOFENGHUANG].setVisible(false);
        this.addChild(m_cbCarFlashResult[SSZP_CARDTYPE_XIAOFENGHUANG]);

        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOWUGUI] = new cc.Sprite(v_sprite_xiaowugui_1);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOWUGUI].setPosition(size.width/2-74,size.height/2+110);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOWUGUI].setScaleY(0.65);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOWUGUI].setScaleX(0.77);
        m_cbCarFlashResult[SSZP_CARDTYPE_XIAOWUGUI].setVisible(false);
        this.addChild(m_cbCarFlashResult[SSZP_CARDTYPE_XIAOWUGUI]);

        for(var i=0;i<9;i++) {
            v_game_record_sprite_list[i] = new cc.Sprite(v_sprite_luzi);
            v_game_record_sprite_list[i].setAnchorPoint(0, 1.0);
            v_game_record_sprite_list[i].setScale(0.77);
            v_game_record_sprite_list[i].setTextureRect(cc.rect(0,0,56,50));
            v_game_record_sprite_list[i].setPosition(size.width/2-387,size.height/2+200-i*40);
            v_game_record_sprite_list[i].setVisible(false);
            this.addChild(v_game_record_sprite_list[i], 0);
        }

        m_bgresult_sprite = new cc.Sprite(v_sprite_result_bg);
        m_bgresult_sprite.setAnchorPoint(0.5, 0.5);
        m_bgresult_sprite.setPosition(size.width / 2, size.height / 2);
        m_bgresult_sprite.setScaleY(0.67);
        m_bgresult_sprite.setScaleX(0.79);
        m_bgresult_sprite.setVisible(false);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(m_bgresult_sprite, 9900);

        m_spriteresult_sprite = new cc.Sprite(v_sprite_result);
        m_spriteresult_sprite.setPosition(size.width/2, size.height/2);
        m_spriteresult_sprite.setTextureRect(cc.rect(0,0,425,425));
        m_spriteresult_sprite.setVisible(false);
        m_spriteresult_sprite.setScaleY(0.67);
        m_spriteresult_sprite.setScaleX(0.79);
        this.addChild(m_spriteresult_sprite,9900);

        v_game_waiting_colorlayer = new cc.LayerColor(new cc.color(0, 0, 0, 200), size.width, size.height);
        v_game_waiting_colorlayer.setAnchorPoint(0.5, 0.5);
        v_game_waiting_colorlayer.setPosition(0,0);
        v_game_waiting_colorlayer.setVisible(false);
        this.addChild(v_game_waiting_colorlayer, 19000);

        v_game_waiting_sprite = new cc.Sprite(v_sprite_dengdai);
        v_game_waiting_sprite.setAnchorPoint(0.5, 0.5);
        //s_game_waiting_sprite.setScale(1.25);
        v_game_waiting_sprite.setPosition(size.width / 2, size.height / 2);
        v_game_waiting_sprite.setVisible(false);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_game_waiting_sprite, 19000);

        v_nomoney_xiazhu_sprite = new cc.Sprite(v_sprite_nomoney_xiazhu);
        v_nomoney_xiazhu_sprite.setAnchorPoint(0.5, 0.5);
        //s_game_waiting_sprite.setScale(1.25);
        v_nomoney_xiazhu_sprite.setPosition(size.width / 2, size.height / 2);
        v_nomoney_xiazhu_sprite.setVisible(false);
        this.addChild(v_nomoney_xiazhu_sprite, 19000);

        v_qingxiazhu_sprite = new cc.Sprite(v_sprite_qingxiazhu);
        v_qingxiazhu_sprite.setAnchorPoint(0.5, 0.5);
        //s_game_waiting_sprite.setScale(1.25);
        v_qingxiazhu_sprite.setPosition(size.width / 2, size.height / 2);
        v_qingxiazhu_sprite.setVisible(false);
        this.addChild(v_qingxiazhu_sprite, 19000);

        m_number_timer_sprite = new cc.LabelAtlas("6",v_sprite_num_people,19,22,"0");
        m_number_timer_sprite.setAnchorPoint(0.5, 0.5);
        m_number_timer_sprite.setPosition(size.width/2+5, size.height/2+2);
        m_number_timer_sprite.setScale(1.7,1.7);
        this.addChild(m_number_timer_sprite);

        m_GamingState_sprite1 = new cc.Sprite(v_sprite_Time_0);
        m_GamingState_sprite1.setPosition(size.width/2+7,size.height/2-42);
        this.addChild(m_GamingState_sprite1);

        m_GamingState_sprite2 = new cc.Sprite(v_sprite_Time_1);
        m_GamingState_sprite2.setPosition(size.width/2+7,size.height/2-42);
        m_GamingState_sprite2.setVisible(false);
        this.addChild(m_GamingState_sprite2);

        m_GamingState_sprite3 = new cc.Sprite(v_sprite_Time_2);
        m_GamingState_sprite3.setPosition(size.width/2+7,size.height/2-42);
        m_GamingState_sprite3.setVisible(false);
        this.addChild(m_GamingState_sprite3);

        m_labelallJettons = new cc.LabelTTF.create("总下注:0", "黑体", 20);
        m_labelallJettons.setColor(cc.color(255, 255, 0));
        m_labelallJettons.setAnchorPoint(0.5,0.5);
        m_labelallJettons.setPosition(size.width/2,size.height/2+222);
        this.addChild(m_labelallJettons);

        m_labelName = new cc.LabelTTF.create("0", "黑体", 18);
        m_labelName.setColor(cc.color(255, 255, 255));
        m_labelName.setAnchorPoint(0,0.5);
        m_labelName.setPosition(size.width/2-345, size.height/2-187);
        this.addChild(m_labelName);

        m_labelMoney = new cc.LabelTTF.create("0", "黑体", 18);
        m_labelMoney.setColor(cc.color(255, 255, 255));
        m_labelMoney.setAnchorPoint(0,0.5);
        m_labelMoney.setPosition(size.width/2-329, size.height/2-204);
        this.addChild(m_labelMoney);

        m_labelCurJetton = new cc.LabelTTF.create("当前下注:0", "黑体", 20);
        m_labelCurJetton.setColor(cc.color(255, 255, 255));
        m_labelCurJetton.setAnchorPoint(0.5,0.5);
        m_labelCurJetton.setPosition(size.width/2, size.height/2-229);
        this.addChild(m_labelCurJetton);

        m_labelTotalResult = new cc.LabelTTF.create("0", "黑体", 18);
        m_labelTotalResult.setColor(cc.color(255, 255, 255));
        m_labelTotalResult.setAnchorPoint(0,0.5);
        m_labelTotalResult.setPosition(size.width/2-314, size.height/2-222);
        this.addChild(m_labelTotalResult);

        v_gameover_bg_sprite = new cc.Sprite(v_sprite_gameoverbg);
        v_gameover_bg_sprite.setAnchorPoint(0.5, 0.5);
        v_gameover_bg_sprite.setPosition(size.width / 2, size.height / 2+20);
        v_gameover_bg_sprite.setVisible(false);
        this.addChild(v_gameover_bg_sprite, 9000);

        v_gameover_bg_lost_sprite = new cc.Sprite(v_sprite_gameovershu);
        v_gameover_bg_lost_sprite.setAnchorPoint(0.5, 0.5);
        v_gameover_bg_lost_sprite.setPosition(size.width / 2, size.height / 2+120);
        v_gameover_bg_lost_sprite.setVisible(false);
        this.addChild(v_gameover_bg_lost_sprite, 9000);

        v_gameover_bg_win_sprite = new cc.Sprite(v_sprite_gameoveryin);
        v_gameover_bg_win_sprite.setAnchorPoint(0.5, 0.5);
        v_gameover_bg_win_sprite.setPosition(size.width / 2, size.height / 2+162);
        v_gameover_bg_win_sprite.setVisible(false);
        this.addChild(v_gameover_bg_win_sprite, 9000);

        // ;表示减 < 表示加
        v_number_sprite = new cc.LabelAtlas("<123223432",v_sprite_OTHERS_NUM,11,17,"0");
        v_number_sprite.setAnchorPoint(0.5, 0.5);
        v_number_sprite.setPosition(size.width / 2+20, size.height / 2-7);
        v_number_sprite.setVisible(false);
        this.addChild(v_number_sprite, 9000);

        m_GamingState_sprite2.setVisible(false);
        m_GamingState_sprite3.setVisible(false);

        for(var i=0;i<16;i++) {
            m_cbCardListSprite[i].setVisible(false);
        }

        this.s_100_btn = new cc.MenuItemImage.create(
            v_sprite_100_1,
            v_sprite_100_2,
            v_sprite_100_3,
            MenuItemProcess,this);
        this.s_500_btn = new cc.MenuItemImage.create(
            v_sprite_500_1,
            v_sprite_500_2,
            v_sprite_500_3,
            MenuItemProcess,this);
        this.s_1000_btn = new cc.MenuItemImage.create(
            v_sprite_1000_1,
            v_sprite_1000_2,
            v_sprite_1000_3,
            MenuItemProcess,this);
        this.s_5000_btn = new cc.MenuItemImage.create(
            v_sprite_5000_1,
            v_sprite_5000_2,
            v_sprite_5000_3,
            MenuItemProcess,this);
        this.s_10000_btn = new cc.MenuItemImage.create(
            v_sprite_10000_1,
            v_sprite_10000_2,
            v_sprite_10000_3,
            MenuItemProcess,this);
        this.s_100000_btn = new cc.MenuItemImage.create(
            v_sprite_100000_1,
            v_sprite_100000_2,
            v_sprite_100000_3,
            MenuItemProcess,this);

        var menu = new cc.Menu.create(this.s_100_btn,this.s_500_btn,this.s_1000_btn,this.s_5000_btn,this.s_10000_btn,this.s_100000_btn);
        menu.setPosition(0, 0);
        this.addChild(menu, 1);

        this.s_100_btn.setTag(100);
        this.s_100_btn.setScale(0.8);
        this.s_100_btn.setPosition(size.width / 2+338, size.height / 2-45);
        this.s_500_btn.setTag(500);
        this.s_500_btn.setScale(0.8);
        this.s_500_btn.setPosition(size.width / 2+338, size.height / 2-100);
        this.s_1000_btn.setTag(1000);
        this.s_1000_btn.setScale(0.8);
        this.s_1000_btn.setPosition(size.width / 2+338, size.height / 2-155);
        this.s_5000_btn.setTag(5000);
        this.s_5000_btn.setScale(0.8);
        this.s_5000_btn.setPosition(size.width / 2+338, size.height / 2-205);
        this.s_10000_btn.setTag(10000);
        this.s_10000_btn.setScale(0.8);
        this.s_10000_btn.setVisible(false);
        this.s_10000_btn.setPosition(size.width / 2+338, size.height / 2-255);
        this.s_100000_btn.setTag(100000);
        this.s_100000_btn.setScale(0.8);
        this.s_100000_btn.setVisible(false);
        this.s_100000_btn.setPosition(size.width / 2+338, size.height / 2-350);

        for(var i=0;i<8;i++) {
            m_yanzhuarea[i] = [];
            m_yanzhuarea[i].totalmoney = 0;
            m_yanzhuarea[i].memoney = 0;
        }

        m_yanzhuarea[0].nareapos = cc.rect(409,252,128,60);
        m_yanzhuarea[1].nareapos = cc.rect(405,176,110,50);
        m_yanzhuarea[2].nareapos = cc.rect(254,174,110,50);
        m_yanzhuarea[3].nareapos = cc.rect(235,246,110,50);
        m_yanzhuarea[4].nareapos = cc.rect(404,316,110,50);
        m_yanzhuarea[5].nareapos = cc.rect(405, 116, 100, 45);
        m_yanzhuarea[6].nareapos = cc.rect(282,116,100,45);
        m_yanzhuarea[7].nareapos = cc.rect(269,318,100,50);

        for(var i=0;i<8;i++) {
            var decPos2 = new cc.Point(m_yanzhuarea[i].nareapos.x + m_yanzhuarea[i].nareapos.width/2+10,
                m_yanzhuarea[i].nareapos.y + 10);

            m_yanzhuarea[i].lableTotal = new cc.LabelTTF.create("0", "黑体", 20);
            m_yanzhuarea[i].lableTotal.setPosition(decPos2.x, decPos2.y);
            m_yanzhuarea[i].lableTotal.setAnchorPoint(0.5, 0.5);
            m_yanzhuarea[i].lableTotal.setColor(cc.color(255, 255, 0));
            this.addChild(m_yanzhuarea[i].lableTotal, 2000);
            m_yanzhuarea[i].lableTotaoMySelf = new cc.LabelTTF.create("0", "黑体", 20);
            m_yanzhuarea[i].lableTotaoMySelf.setPosition(decPos2.x, decPos2.y + 25);
            m_yanzhuarea[i].lableTotaoMySelf.setAnchorPoint(0.5, 0.5);
            m_yanzhuarea[i].lableTotaoMySelf.setColor(cc.color(255, 255, 255));
            this.addChild(m_yanzhuarea[i].lableTotaoMySelf, 2000);
        }

        cc.audioEngine.playMusic(v_sound_BGM_Room_4, true);

        //事件监听器
        var pigListener = cc.EventListener.create({
            //单击事件
            event:cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches:true,
            target:this,
            onTouchBegan : MenuItemMousePro,
            onTouchMoved : function (touch,event) {

            },
            onTouchEnded:function (touch,event) {

            }
        });

        //将事件监听器叫由事件管理器管理
        cc.eventManager.addListener(pigListener,this.bgsprite);

        //为事件监听器添加优先级，第二个参数越小，则优先级越高
        cc.eventManager.setPriority(pigListener,-200);

        this.bgsprite.listener = pigListener;

        try{
            v_account_socket = new WebSocket(host);

            v_account_socket.onopen = OnProcessAccountSocketOnOpen

            v_account_socket.onmessage = OnProcessAccountSocketOnMessage

            v_account_socket.onclose = OnProcessAccountSocketOnClose

        } catch(exception){
            var exceptionlayer = new MessageBoxLayer();
            this.addChild(exceptionlayer,8);
            exceptionlayer.init(exception);
        }
    }
});

var CSszpScene = cc.Scene.extend({
    onEnter:function () {
        this._super();

        m_MainFrameLayer = new CMainFrameLayer();
        this.addChild(m_MainFrameLayer);
        m_MainFrameLayer.init();
    }
});

function OnProcessAnimationMessage() {
    var dateObject = new Date();
    var pTmpTime = dateObject.getTime();

    if(pTmpTime > m_curAnimTime + m_curAnimSpeed)
    {
        m_cbCardListSprite[m_curShowAnimFrame].setVisible(false);

        m_curShowAnimFrame += 1;

        if(m_curShowAnimFrame > 15)
            m_curShowAnimFrame = 0;

        m_cbCardListSprite[m_curShowAnimFrame].setVisible(true);

        cc.audioEngine.playEffect(v_sound_Animal1, false);

        dateObject = new Date();
        m_curAnimTime = dateObject.getTime();

        if(m_isAnimFanSpeed)
        {
            if(m_curAnimSpeed < 300.0)
            m_curAnimSpeed += 38.0;

            if(m_curShowAnimFrame == m_curDecPosition)
            {
                //m_curAnimCircle += 1;

                //if(m_curAnimCircle > 2)
                m_MainFrameLayer.unschedule(OnProcessAnimationMessage);

                OnProcessAnimationResultMessage();
            }
        }
        else
        {
            if(m_curAnimSpeed > 10.0)
            m_curAnimSpeed -= 5.0;
        else
            {
                var tmpPos = GetStopPosition(m_curDecPosition);

                if(m_curShowAnimFrame == tmpPos)
                {
                    m_curAnimCircle += 1;

                    if(m_curAnimCircle > 2)
                    {
                        m_curAnimCircle = 0;
                        m_isAnimFanSpeed = true;
                    }
                }
            }
        }
    }
}

function OnProcessAnimationResultMessage() {
    if (m_cbCardListData[m_curShowAnimFrame] < SSZP_CARDTYPE_XIAOBAILONG)
    {
        for (var i = 0; i < 16; i++)
        {
            var blinkAction = new cc.blink(3, 5);
            m_cbCardListSprite[i].runAction(blinkAction);
        }

        cc.audioEngine.playEffect(v_sound_snd_flas, false);
    }
    else
    {
        var blinkAction = new cc.blink(3, 5);
        m_cbCardListSprite[m_curShowAnimFrame].runAction(blinkAction);
    }

    var p_game_timer_id = setInterval( function(){
        clearInterval(p_game_timer_id);

        CarFlashResultTimer();
    },2000);
}

function CarFlashResultTimer() {
    var blinkAction2 = new cc.blink(3, 5);
    m_cbCarFlashResult[m_cbCardListData[m_curShowAnimFrame]].runAction(blinkAction2);

    switch(m_cbCardListData[m_curShowAnimFrame])
    {
        case SSZP_CARDTYPE_XUANWU:
            m_spriteresult_sprite.setTextureRect(cc.rect(0,0,425,425));
            break;
        case SSZP_CARDTYPE_ZHUQUE:
            m_spriteresult_sprite.setTextureRect(cc.rect(425,0,425,425));
            break;
        case SSZP_CARDTYPE_BAIHU:
            m_spriteresult_sprite.setTextureRect(cc.rect(425*2,0,425,425));
            break;
        case SSZP_CARDTYPE_QINGLONG:
            m_spriteresult_sprite.setTextureRect(cc.rect(425*3,0,425,425));
            break;
        case SSZP_CARDTYPE_XIAOWUGUI:
            m_spriteresult_sprite.setTextureRect(cc.rect(425*4,0,425,425));
            break;
        case SSZP_CARDTYPE_XIAOFENGHUANG:
            m_spriteresult_sprite.setTextureRect(cc.rect(425*5,0,425,425));
            break;
        case SSZP_CARDTYPE_XIAOLAOHU:
            m_spriteresult_sprite.setTextureRect(cc.rect(425*6,0,425,425));
            break;
        case SSZP_CARDTYPE_XIAOBAILONG:
            m_spriteresult_sprite.setTextureRect(cc.rect(425*7,0,425,425));
            break;
        default:
            break;
    }
    m_spriteresult_sprite.setVisible(true);

    var blinkAction3 = new cc.blink(3, 5);
    m_spriteresult_sprite.runAction(blinkAction3);

    m_bgresult_sprite.setVisible(true);

    var p_game_timer_id = setInterval( function(){
        clearInterval(p_game_timer_id);

        m_spriteresult_sprite.setVisible(false);
        m_bgresult_sprite.setVisible(false);
    },6000);
}

function ClearGamingJettons() {
    if(m_MainFrameLayer == null)
        return;

    for(var i=0;i<m_all_jettonsprites_count;i++){
        m_MainFrameLayer.removeChild(m_all_jettonsprites[i]);
    }

    m_all_jettonsprites_count=0;

    for(var i=0;i<8;i++) {
        m_yanzhuarea[i].totalmoney = 0;
        m_yanzhuarea[i].memoney = 0;
        m_yanzhuarea[i].lableTotal.setString(m_yanzhuarea[i].totalmoney);
        m_yanzhuarea[i].lableTotaoMySelf.setString(m_yanzhuarea[i].memoney);
    }
}

function GamingJetton(areaIndex,pMenuItemObjTag,isme) {
    var decPos = new cc.Point(cc.rectGetMidX(m_yanzhuarea[areaIndex].nareapos) + Math.random()*8,
        cc.rectGetMidY(m_yanzhuarea[areaIndex].nareapos) + Math.random()*5 - 10);

    var jettonsprite = new cc.Sprite(v_sprite_pngJettonView);
    jettonsprite.setAnchorPoint(0, 0);
    jettonsprite.setScale(0.5);

    if(pMenuItemObjTag == 100) jettonsprite.setTextureRect(cc.rect(0,0,52,55));
    else if(pMenuItemObjTag == 500) jettonsprite.setTextureRect(cc.rect(52.0,0.0,52.0,55.0));
    else if(pMenuItemObjTag == 1000) jettonsprite.setTextureRect(cc.rect(52.0*2.0,0.0,52.0,55.0));
    else if(pMenuItemObjTag == 5000) jettonsprite.setTextureRect(cc.rect(52.0*3.0,0.0,52.0,55.0));
    else if(pMenuItemObjTag == 10000) jettonsprite.setTextureRect(cc.rect(52.0*4.0,0.0,52.0,55.0));
    else if(pMenuItemObjTag == 100000) jettonsprite.setTextureRect(cc.rect(52.0*5.0,0.0,52.0,55.0));

    jettonsprite.setPosition(decPos);
    m_MainFrameLayer.addChild(jettonsprite, 1000);

    m_all_jettonsprites[m_all_jettonsprites_count] = jettonsprite;
    m_all_jettonsprites_count = m_all_jettonsprites_count + 1;

    m_yanzhuarea[areaIndex].totalmoney = m_yanzhuarea[areaIndex].totalmoney + pMenuItemObjTag;
    m_yanzhuarea[areaIndex].lableTotal.setString(m_yanzhuarea[areaIndex].totalmoney);

    if(isme) {
        m_yanzhuarea[areaIndex].memoney = m_yanzhuarea[areaIndex].memoney + pMenuItemObjTag;
        m_yanzhuarea[areaIndex].lableTotaoMySelf.setString(m_yanzhuarea[areaIndex].memoney);
    }
}

function MenuItemProcess(obj) {
    if(m_user_mouse_isprocess == false)
        return;

    cc.audioEngine.playEffect(v_sound_MENU, false);

    m_MenuItemObjTag = obj.getTag();
};

function MenuItemMousePro(touch,event) {
    if (m_user_mouse_isprocess == false)
        return;

    //获取当前事件触发对象
    var target = event.getCurrentTarget();

    //将触发点转换为基于当前事件触发对象的本地坐标
    var posInNode = target.convertToNodeSpace(touch.getLocation());

    //获取当前事件触发对象的大小
    var size = target.getContentSize();

    //区域（一般为需要拖动对象的大小范围）
    var rect = cc.rect(0, 0, size.width, size.height);

    //当前触摸点是否在rect范围内
    var boolRet = cc.rectContainsPoint(rect, posInNode);

    if (m_MenuItemObjTag == 0)
        return boolRet;

    if(m_myselfusermoney - m_MenuItemObjTag < 0) {
        ShowGameState(2,true);
        return;
    }

    for (var i = 0; i<8; i++)
    {
        var iscontains = cc.rectContainsPoint(m_yanzhuarea[i].nareapos,posInNode);

        if(iscontains) {
            GamingJetton(i,m_MenuItemObjTag,true);

            var row1 = {};
            row1.MsgId = 1000;
            row1.MsgSubId = 1002;
            row1.CardType = i;
            row1.Money = m_MenuItemObjTag;

            m_gameserversocket.send(JSON.stringify(row1));

            m_myselfusermoney = m_myselfusermoney - m_MenuItemObjTag;
            m_labelMoney.setString(m_myselfusermoney);

            cc.audioEngine.playEffect(v_sound_AddGold, false);

            break;
        }
    }

    return boolRet;
}





