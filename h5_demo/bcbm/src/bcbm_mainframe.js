var m_MainFrameLayer = null;
var m_watingcollayer = null;
var m_spriteGamingState0 = null;
var m_spriteGamingState1 = null;
var m_spriteGamingState2 = null;
var m_cbCardListData = [];
var m_cbCardListSprite = [];
var m_cbCarFlashResult = [];
var m_user_mouse_isprocess = true;
var m_MenuItemObjTag = 100;
var m_gamerecordspr = [];
var m_yanzhuarea = [];
var m_sprintewaiting = null;
var m_labelallJettons = null;
var m_labelName = null;
var m_labelMoney = null;
var m_labelCurJetton = null;
var m_labelTotalResult = null;
var m_spriteNoMoneyXiaZhu = null;
var m_spriteqinxiazhu = null;
var v_gameover_bg_sprite = null;
var v_gameover_bg_lost_sprite = null;
var v_gameover_bg_win_sprite = null;
var v_number_timer_sprite = null;
var v_number_sprite = null;
var v_game_state_timer_id = null;
var m_game_timer_current_time = 0;
var m_game_timer_id = null;
var v_gaming_result_vector = [];
var m_curAnimTime = 0;
var m_curAnimSpeed = 150.0;
var m_isAnimFanSpeed = false;
var m_curAnimCircle = 0;
var m_curDecPosition = 0;
var m_all_jettonsprites_count = 0;
var m_all_jettonsprites = [];
var m_curShowAnimFrame = 0;

function GetStopPosition(curPos) {
    if(curPos-20 >= 0)
        return curPos-20;

    return 32+(curPos-20);
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

function clearAnimation() {
    m_MainFrameLayer.unschedule(OnProcessAnimationMessage);

    for(var i=0;i<16;i++) {
        m_cbCardListSprite[i].setVisible(false);
    }

    for(var i=0;i<8;i++) {
        m_cbCarFlashResult[i].setVisible(false);
    }
}

function addvaluetogamerecord(value) {
    v_gaming_result_vector.push(value);

    if(v_gaming_result_vector.length > 15) {
        v_gaming_result_vector.splice(0,1);
    }
}

function ShowGamingRecords() {
    var i = 0;
    for(var value of v_gaming_result_vector) {
        switch(value)
        {
            case CARTYPE_FALALI:
                m_gamerecordspr[i].setTextureRect(cc.rect(0*113,0,113,84));
                break;
            case CARTYPE_LANBOJILI:
                m_gamerecordspr[i].setTextureRect(cc.rect(2*113,0,113,84));
                break;
            case CARTYPE_MASAILADI:
                m_gamerecordspr[i].setTextureRect(cc.rect(4*113,0,113,84));
                break;
            case CARTYPE_BAOSHIJIE:
                m_gamerecordspr[i].setTextureRect(cc.rect(6*113,0,113,84));
                break;
            case CARTYPE_BENCI:
                m_gamerecordspr[i].setTextureRect(cc.rect(1*113,0,113,84));
                break;
            case CARTYPE_BAOMA:
                m_gamerecordspr[i].setTextureRect(cc.rect(3*113,0,113,84));
                break;
            case CARTYPE_AODI:
                m_gamerecordspr[i].setTextureRect(cc.rect(5*113,0,113,84));
                break;
            case CARTYPE_DAZONG:
                m_gamerecordspr[i].setTextureRect(cc.rect(7*113,0,113,84));
                break;
            default:
                break;
        }

        m_gamerecordspr[i].setVisible(true);
        i=i+1;
    }
}

function ShowGameState(type,isshow) {
    m_sprintewaiting.setVisible(false);
    m_watingcollayer.setVisible(false);
    m_spriteNoMoneyXiaZhu.setVisible(false);
    m_spriteqinxiazhu.setVisible(false);

    if(type == 1) {
        m_sprintewaiting.setVisible(isshow);
        m_watingcollayer.setVisible(isshow);
    }
    else if(type == 2) {
        m_spriteNoMoneyXiaZhu.setVisible(isshow);
    }
    else if(type == 3) {
        m_spriteqinxiazhu.setVisible(isshow);
    }

    if(isshow) {
        v_game_state_timer_id = setInterval( function(){
            clearInterval(v_game_state_timer_id);

            if(type == 2) {
                m_spriteNoMoneyXiaZhu.setVisible(!isshow);
            }
            else if(type == 3) {
                m_spriteqinxiazhu.setVisible(!isshow);
            }
        },2000);
    }
}

function EnableMouseProcess(isenable) {
    m_user_mouse_isprocess = isenable;
}

function starttimer(timenum) {
    if(m_game_timer_id != null) {
        clearInterval(m_game_timer_id);
    }

    m_game_timer_current_time = timenum;
    v_number_timer_sprite.setString(m_game_timer_current_time);
    v_number_timer_sprite.setVisible(true);

    m_game_timer_id = setInterval( function(){
        m_game_timer_current_time = m_game_timer_current_time - 1;
        if(m_game_timer_current_time < 0) {
            v_number_timer_sprite.setVisible(false);
            clearInterval(m_game_timer_id);
        }

        if(m_game_timer_current_time < 4) {
            cc.audioEngine.playEffect(v_sound_clock, false);
        }

        v_number_timer_sprite.setString(m_game_timer_current_time);
    },1000);
}

function ShowGamingState(pGameState)
{
    m_spriteGamingState0.setVisible(false);
    m_spriteGamingState1.setVisible(false);
    m_spriteGamingState2.setVisible(false);

    switch(pGameState)
    {
        case STATE_XIAZHU:
        {
            m_spriteGamingState1.setVisible(true);
        }
            break;
        case STATE_GAMING:
        {
            m_spriteGamingState2.setVisible(true);
        }
            break;
        case STATE_GAMEEND:
        {
            m_spriteGamingState0.setVisible(true);
        }
            break;
        default:
            break;
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

var CMainFrameLayer = cc.Layer.extend({
    bgsprite:null,
    s_100_btn:null,
    s_500_btn:null,
    s_1000_btn:null,
    s_5000_btn:null,
    s_10000_btn:null,
    s_100000_btn:null,

    init:function () {
        this._super();

        var size = cc.director.getWinSize();

        this.bgsprite = new cc.Sprite(v_sprite_gamebg);
        this.bgsprite.setAnchorPoint(0.5, 0.5);
        this.bgsprite.setPosition(size.width / 2, size.height / 2);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(this.bgsprite, 0);

        m_watingcollayer = new cc.LayerColor(new cc.color(0, 0, 0, 200), size.width, size.height);
        m_watingcollayer.setPosition(0,0);
        m_watingcollayer.setAnchorPoint(0.5, 0.5);
        m_watingcollayer.setVisible(false);
        this.addChild(m_watingcollayer,99998);

        m_spriteGamingState0 = new cc.Sprite(v_sprite_Time_0);
        m_spriteGamingState0.setAnchorPoint(0.5, 0.5);
        m_spriteGamingState0.setPosition(size.width/2-80,size.height/2+135);
        this.addChild(m_spriteGamingState0);

        m_spriteGamingState1 = new cc.Sprite(v_sprite_Time_1);
        m_spriteGamingState1.setAnchorPoint(0.5, 0.5);
        m_spriteGamingState1.setPosition(size.width/2-80,size.height/2+135);
        m_spriteGamingState1.setVisible(false);
        this.addChild(m_spriteGamingState1);

        m_spriteGamingState2 = new cc.Sprite(v_sprite_Time_2);
        m_spriteGamingState2.setAnchorPoint(0.5, 0.5);
        m_spriteGamingState2.setPosition(size.width/2-80,size.height/2+135);
        m_spriteGamingState2.setVisible(false);
        this.addChild(m_spriteGamingState2);

        m_cbCardListData[0] = CARTYPE_FALALI;
        m_cbCardListSprite[0] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[0].setPosition(cc.p(size.width/2-334,size.height/2+171));
        m_cbCardListSprite[0].setTextureRect(cc.rect(0,0,113,84));
        m_cbCardListSprite[0].setScaleY(0.8);
        m_cbCardListSprite[0].setVisible(false);
        this.addChild(m_cbCardListSprite[0]);

        m_cbCardListData[1] = CARTYPE_BENCI;
        m_cbCardListSprite[1] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[1].setPosition(cc.p(size.width/2-248,size.height/2+171));
        m_cbCardListSprite[1].setTextureRect(cc.rect(113,0,113,84));
        m_cbCardListSprite[1].setScaleY(0.8);
        m_cbCardListSprite[1].setVisible(false);
        this.addChild(m_cbCardListSprite[1]);

        m_cbCardListData[2] = CARTYPE_LANBOJILI;
        m_cbCardListSprite[2] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[2].setPosition(cc.p(size.width/2-166,size.height/2+171));
        m_cbCardListSprite[2].setTextureRect(cc.rect(113*2,0,113,84));
        m_cbCardListSprite[2].setScaleY(0.8);
        m_cbCardListSprite[2].setVisible(false);
        this.addChild(m_cbCardListSprite[2]);

        m_cbCardListData[3] = CARTYPE_BAOMA;
        m_cbCardListSprite[3] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[3].setPosition(cc.p(size.width/2-82,size.height/2+171));
        m_cbCardListSprite[3].setTextureRect(cc.rect(113*3,0,113,84));
        m_cbCardListSprite[3].setScaleY(0.8);
        m_cbCardListSprite[3].setVisible(false);
        this.addChild(m_cbCardListSprite[3]);

        m_cbCardListData[4] = CARTYPE_MASAILADI;
        m_cbCardListSprite[4] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[4].setPosition(cc.p(size.width/2,size.height/2+171));
        m_cbCardListSprite[4].setTextureRect(cc.rect(113*4,0,113,84));
        m_cbCardListSprite[4].setScaleY(0.8);
        m_cbCardListSprite[4].setVisible(false);
        this.addChild(m_cbCardListSprite[4]);

        m_cbCardListData[5] = CARTYPE_AODI;
        m_cbCardListSprite[5] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[5].setPosition(cc.p(size.width/2+82,size.height/2+171));
        m_cbCardListSprite[5].setTextureRect(cc.rect(113*5,0,113,84));
        m_cbCardListSprite[5].setScaleY(0.8);
        m_cbCardListSprite[5].setVisible(false);
        this.addChild(m_cbCardListSprite[5]);

        m_cbCardListData[6] = CARTYPE_BAOSHIJIE;
        m_cbCardListSprite[6] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[6].setPosition(cc.p(size.width/2+169,size.height/2+171));
        m_cbCardListSprite[6].setTextureRect(cc.rect(113*6,0,113,84));
        m_cbCardListSprite[6].setScaleY(0.8);
        m_cbCardListSprite[6].setVisible(false);
        this.addChild(m_cbCardListSprite[6]);

        m_cbCardListData[7] = CARTYPE_DAZONG;
        m_cbCardListSprite[7] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[7].setPosition(cc.p(size.width/2+250,size.height/2+171));
        m_cbCardListSprite[7].setTextureRect(cc.rect(113*7,0,113,84));
        m_cbCardListSprite[7].setScaleY(0.8);
        m_cbCardListSprite[7].setVisible(false);
        this.addChild(m_cbCardListSprite[7]);

        m_cbCardListData[8] = CARTYPE_FALALI;
        m_cbCardListSprite[8] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[8].setPosition(cc.p(size.width/2+332,size.height/2+171));
        m_cbCardListSprite[8].setTextureRect(cc.rect(0,0,113,84));
        m_cbCardListSprite[8].setScaleY(0.8);
        m_cbCardListSprite[8].setVisible(false);
        this.addChild(m_cbCardListSprite[8]);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        m_cbCardListData[9] = CARTYPE_BENCI;
        m_cbCardListSprite[9] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[9].setPosition(cc.p(size.width/2+332,size.height/2+131));
        m_cbCardListSprite[9].setTextureRect(cc.rect(113,0,113,84));
        m_cbCardListSprite[9].setScaleY(0.8);
        m_cbCardListSprite[9].setVisible(false);
        this.addChild(m_cbCardListSprite[9]);

        m_cbCardListData[10] = CARTYPE_LANBOJILI;
        m_cbCardListSprite[10] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[10].setPosition(cc.p(size.width/2+332,size.height/2+91));
        m_cbCardListSprite[10].setTextureRect(cc.rect(113*2,0,113,84));
        m_cbCardListSprite[10].setScaleY(0.8);
        m_cbCardListSprite[10].setVisible(false);
        this.addChild(m_cbCardListSprite[10]);

        m_cbCardListData[11] = CARTYPE_BAOMA;
        m_cbCardListSprite[11] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[11].setPosition(cc.p(size.width/2+332,size.height/2+51));
        m_cbCardListSprite[11].setTextureRect(cc.rect(113*3,0,113,84));
        m_cbCardListSprite[11].setScaleY(0.8);
        m_cbCardListSprite[11].setVisible(false);
        this.addChild(m_cbCardListSprite[11]);

        m_cbCardListData[12] = CARTYPE_MASAILADI;
        m_cbCardListSprite[12] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[12].setPosition(cc.p(size.width/2+332,size.height/2+11));
        m_cbCardListSprite[12].setTextureRect(cc.rect(113*4,0,113,84));
        m_cbCardListSprite[12].setScaleY(0.8);
        m_cbCardListSprite[12].setVisible(false);
        this.addChild(m_cbCardListSprite[12]);

        m_cbCardListData[13] = CARTYPE_AODI;
        m_cbCardListSprite[13] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[13].setPosition(cc.p(size.width/2+332,size.height/2-29));
        m_cbCardListSprite[13].setTextureRect(cc.rect(113*5,0,113,84));
        m_cbCardListSprite[13].setScaleY(0.8);
        m_cbCardListSprite[13].setVisible(false);
        this.addChild(m_cbCardListSprite[13]);

        m_cbCardListData[14] = CARTYPE_BAOSHIJIE;
        m_cbCardListSprite[14] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[14].setPosition(cc.p(size.width/2+332,size.height/2-69));
        m_cbCardListSprite[14].setTextureRect(cc.rect(113*6,0,113,84));
        m_cbCardListSprite[14].setScaleY(0.8);
        m_cbCardListSprite[14].setVisible(false);
        this.addChild(m_cbCardListSprite[14]);

        m_cbCardListData[15] = CARTYPE_DAZONG;
        m_cbCardListSprite[15] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[15].setPosition(cc.p(size.width/2+332,size.height/2-109));
        m_cbCardListSprite[15].setTextureRect(cc.rect(113*7,0,113,84));
        m_cbCardListSprite[15].setScaleY(0.8);
        m_cbCardListSprite[15].setVisible(false);
        this.addChild(m_cbCardListSprite[15]);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        m_cbCardListData[16] = CARTYPE_FALALI;
        m_cbCardListSprite[16] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[16].setPosition(cc.p(size.width/2+332,size.height/2-150));
        m_cbCardListSprite[16].setTextureRect(cc.rect(0,0,113,84));
        m_cbCardListSprite[16].setScaleY(0.8);
        m_cbCardListSprite[16].setVisible(false);
        this.addChild(m_cbCardListSprite[16]);

        m_cbCardListData[17] = CARTYPE_BENCI;
        m_cbCardListSprite[17] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[17].setPosition(cc.p(size.width/2+250,size.height/2-150));
        m_cbCardListSprite[17].setTextureRect(cc.rect(113,0,113,84));
        m_cbCardListSprite[17].setScaleY(0.8);
        m_cbCardListSprite[17].setVisible(false);
        this.addChild(m_cbCardListSprite[17]);

        m_cbCardListData[18] = CARTYPE_LANBOJILI;
        m_cbCardListSprite[18] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[18].setPosition(cc.p(size.width/2+169,size.height/2-150));
        m_cbCardListSprite[18].setTextureRect(cc.rect(113*2,0,113,84));
        m_cbCardListSprite[18].setScaleY(0.8);
        m_cbCardListSprite[18].setVisible(false);
        this.addChild(m_cbCardListSprite[18]);

        m_cbCardListData[19] = CARTYPE_BAOMA;
        m_cbCardListSprite[19] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[19].setPosition(cc.p(size.width/2+82,size.height/2-150));
        m_cbCardListSprite[19].setTextureRect(cc.rect(113*3,0,113,84));
        m_cbCardListSprite[19].setScaleY(0.8);
        m_cbCardListSprite[19].setVisible(false);
        this.addChild(m_cbCardListSprite[19]);

        m_cbCardListData[20] = CARTYPE_MASAILADI;
        m_cbCardListSprite[20] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[20].setPosition(cc.p(size.width/2,size.height/2-150));
        m_cbCardListSprite[20].setTextureRect(cc.rect(113*4,0,113,84));
        m_cbCardListSprite[20].setScaleY(0.8);
        m_cbCardListSprite[20].setVisible(false);
        this.addChild(m_cbCardListSprite[20]);

        m_cbCardListData[21] = CARTYPE_AODI;
        m_cbCardListSprite[21] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[21].setPosition(cc.p(size.width/2-82,size.height/2-150));
        m_cbCardListSprite[21].setTextureRect(cc.rect(113*5,0,113,84));
        m_cbCardListSprite[21].setScaleY(0.8);
        m_cbCardListSprite[21].setVisible(false);
        this.addChild(m_cbCardListSprite[21]);

        m_cbCardListData[22] = CARTYPE_BAOSHIJIE;
        m_cbCardListSprite[22] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[22].setPosition(cc.p(size.width/2-166,size.height/2-150));
        m_cbCardListSprite[22].setTextureRect(cc.rect(113*6,0,113,84));
        m_cbCardListSprite[22].setScaleY(0.8);
        m_cbCardListSprite[22].setVisible(false);
        this.addChild(m_cbCardListSprite[22]);

        m_cbCardListData[23] = CARTYPE_DAZONG;
        m_cbCardListSprite[23] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[23].setPosition(cc.p(size.width/2-248,size.height/2-150));
        m_cbCardListSprite[23].setTextureRect(cc.rect(113*7,0,113,84));
        m_cbCardListSprite[23].setScaleY(0.8);
        m_cbCardListSprite[23].setVisible(false);
        this.addChild(m_cbCardListSprite[23]);

        m_cbCardListData[24] = CARTYPE_FALALI;
        m_cbCardListSprite[24] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[24].setPosition(cc.p(size.width/2-334,size.height/2-150));
        m_cbCardListSprite[24].setTextureRect(cc.rect(0,0,113,84));
        m_cbCardListSprite[24].setScaleY(0.8);
        m_cbCardListSprite[24].setVisible(false);
        this.addChild(m_cbCardListSprite[24]);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        m_cbCardListData[25] = CARTYPE_BENCI;
        m_cbCardListSprite[25] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[25].setPosition(cc.p(size.width/2-334,size.height/2-109));
        m_cbCardListSprite[25].setTextureRect(cc.rect(113,0,113,84));
        m_cbCardListSprite[25].setScaleY(0.8);
        m_cbCardListSprite[25].setVisible(false);
        this.addChild(m_cbCardListSprite[25]);

        m_cbCardListData[26] = CARTYPE_LANBOJILI;
        m_cbCardListSprite[26] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[26].setPosition(cc.p(size.width/2-334,size.height/2-69));
        m_cbCardListSprite[26].setTextureRect(cc.rect(113*2,0,113,84));
        m_cbCardListSprite[26].setScaleY(0.8);
        m_cbCardListSprite[26].setVisible(false);
        this.addChild(m_cbCardListSprite[26]);

        m_cbCardListData[27] = CARTYPE_BAOMA;
        m_cbCardListSprite[27] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[27].setPosition(cc.p(size.width/2-334,size.height/2-29));
        m_cbCardListSprite[27].setTextureRect(cc.rect(113*3,0,113,84));
        m_cbCardListSprite[27].setScaleY(0.8);
        m_cbCardListSprite[27].setVisible(false);
        this.addChild(m_cbCardListSprite[27]);

        m_cbCardListData[28] = CARTYPE_MASAILADI;
        m_cbCardListSprite[28] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[28].setPosition(cc.p(size.width/2-334,size.height/2+11));
        m_cbCardListSprite[28].setTextureRect(cc.rect(113*4,0,113,84));
        m_cbCardListSprite[28].setScaleY(0.8);
        m_cbCardListSprite[28].setVisible(false);
        this.addChild(m_cbCardListSprite[28]);

        m_cbCardListData[29] = CARTYPE_AODI;
        m_cbCardListSprite[29] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[29].setPosition(cc.p(size.width/2-334,size.height/2+51));
        m_cbCardListSprite[29].setTextureRect(cc.rect(113*5,0,113,84));
        m_cbCardListSprite[29].setScaleY(0.8);
        m_cbCardListSprite[29].setVisible(false);
        this.addChild(m_cbCardListSprite[29]);

        m_cbCardListData[30] = CARTYPE_BAOSHIJIE;
        m_cbCardListSprite[30] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[30].setPosition(cc.p(size.width/2-334,size.height/2+91));
        m_cbCardListSprite[30].setTextureRect(cc.rect(113*6,0,113,84));
        m_cbCardListSprite[30].setScaleY(0.8);
        m_cbCardListSprite[30].setVisible(false);
        this.addChild(m_cbCardListSprite[30]);

        m_cbCardListData[31] = CARTYPE_DAZONG;
        m_cbCardListSprite[31] = new cc.Sprite(v_sprite_xuanzhong_bg);
        m_cbCardListSprite[31].setPosition(cc.p(size.width/2-334,size.height/2+131));
        m_cbCardListSprite[31].setTextureRect(cc.rect(113*7,0,113,84));
        m_cbCardListSprite[31].setScaleY(0.8);
        m_cbCardListSprite[31].setVisible(false);
        this.addChild(m_cbCardListSprite[31]);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        m_cbCarFlashResult[CARTYPE_FALALI] = new cc.Sprite(v_sprite_ractangleBigEnd_1);
        m_cbCarFlashResult[CARTYPE_FALALI].setPosition(cc.p(size.width/2-220,size.height/2+67));
        m_cbCarFlashResult[CARTYPE_FALALI].setScaleY(0.7);
        m_cbCarFlashResult[CARTYPE_FALALI].setVisible(false);
        this.addChild(m_cbCarFlashResult[CARTYPE_FALALI]);

        m_cbCarFlashResult[CARTYPE_LANBOJILI] = new cc.Sprite(v_sprite_ractangleBigEnd_2);
        m_cbCarFlashResult[CARTYPE_LANBOJILI].setPosition(cc.p(size.width/2-75,size.height/2+67));
        m_cbCarFlashResult[CARTYPE_LANBOJILI].setScaleY(0.7);
        m_cbCarFlashResult[CARTYPE_LANBOJILI].setVisible(false);
        this.addChild(m_cbCarFlashResult[CARTYPE_LANBOJILI]);

        m_cbCarFlashResult[CARTYPE_MASAILADI] = new cc.Sprite(v_sprite_ractangleBigEnd_4);
        m_cbCarFlashResult[CARTYPE_MASAILADI].setPosition(cc.p(size.width/2+70,size.height/2+67));
        m_cbCarFlashResult[CARTYPE_MASAILADI].setScaleY(0.7);
        m_cbCarFlashResult[CARTYPE_MASAILADI].setVisible(false);
        this.addChild(m_cbCarFlashResult[CARTYPE_MASAILADI]);

        m_cbCarFlashResult[CARTYPE_BAOSHIJIE] = new cc.Sprite(v_sprite_ractangleBigEnd_6);
        m_cbCarFlashResult[CARTYPE_BAOSHIJIE].setPosition(cc.p(size.width/2+215,size.height/2+67));
        m_cbCarFlashResult[CARTYPE_BAOSHIJIE].setScaleY(0.7);
        m_cbCarFlashResult[CARTYPE_BAOSHIJIE].setVisible(false);
        this.addChild(m_cbCarFlashResult[CARTYPE_BAOSHIJIE]);

        m_cbCarFlashResult[CARTYPE_BENCI] = new cc.Sprite(v_sprite_ractangleBigEnd_1);
        m_cbCarFlashResult[CARTYPE_BENCI].setPosition(cc.p(size.width/2-220,size.height/2-54));
        m_cbCarFlashResult[CARTYPE_BENCI].setScaleY(0.7);
        m_cbCarFlashResult[CARTYPE_BENCI].setVisible(false);
        this.addChild(m_cbCarFlashResult[CARTYPE_BENCI]);

        m_cbCarFlashResult[CARTYPE_BAOMA] = new cc.Sprite(v_sprite_ractangleBigEnd_3);
        m_cbCarFlashResult[CARTYPE_BAOMA].setPosition(cc.p(size.width/2-75,size.height/2-54));
        m_cbCarFlashResult[CARTYPE_BAOMA].setScaleY(0.7);
        m_cbCarFlashResult[CARTYPE_BAOMA].setVisible(false);
        this.addChild(m_cbCarFlashResult[CARTYPE_BAOMA]);

        m_cbCarFlashResult[CARTYPE_AODI] = new cc.Sprite(v_sprite_ractangleBigEnd_5);
        m_cbCarFlashResult[CARTYPE_AODI].setPosition(cc.p(size.width/2+70,size.height/2-54));
        m_cbCarFlashResult[CARTYPE_AODI].setScaleY(0.7);
        m_cbCarFlashResult[CARTYPE_AODI].setVisible(false);
        this.addChild(m_cbCarFlashResult[CARTYPE_AODI]);

        m_cbCarFlashResult[CARTYPE_DAZONG] = new cc.Sprite(v_sprite_ractangleBigEnd_7);
        m_cbCarFlashResult[CARTYPE_DAZONG].setPosition(cc.p(size.width/2+215,size.height/2-54));
        m_cbCarFlashResult[CARTYPE_DAZONG].setScaleY(0.7);
        m_cbCarFlashResult[CARTYPE_DAZONG].setVisible(false);
        this.addChild(m_cbCarFlashResult[CARTYPE_DAZONG]);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        var pStartrecordPos = cc.p(size.width/2-250,size.height/2-120);

        for(var i=0;i<15;i++)
        {
            m_gamerecordspr[i] = new cc.Sprite(v_sprite_xuanzhong_bg);
            m_gamerecordspr[i].setPosition(cc.p(pStartrecordPos.x+i*35,pStartrecordPos.y));
            m_gamerecordspr[i].setTextureRect(cc.rect(0,0,113,84));
            m_gamerecordspr[i].setScale(0.3);
            m_gamerecordspr[i].setVisible(false);
            this.addChild(m_gamerecordspr[i]);
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        for(var i=0;i<8;i++) {
            m_yanzhuarea[i] = [];
            m_yanzhuarea[i].totalmoney = 0;
            m_yanzhuarea[i].memoney = 0;
        }

        m_yanzhuarea[0].nareapos = cc.rect(size.width/2-290, size.height/2,143,126);
        m_yanzhuarea[1].nareapos = cc.rect(size.width/2-144, size.height/2,143,126);
        m_yanzhuarea[2].nareapos = cc.rect(size.width/2+1, size.height/2,141,126);
        m_yanzhuarea[3].nareapos = cc.rect(size.width/2+144, size.height/2,143,126);
        m_yanzhuarea[4].nareapos = cc.rect(size.width/2-290, size.height/2-126,143,126);
        m_yanzhuarea[5].nareapos = cc.rect(size.width/2-144, size.height/2-126,143,126);
        m_yanzhuarea[6].nareapos = cc.rect(size.width/2+1, size.height/2-126,141,126);
        m_yanzhuarea[7].nareapos = cc.rect(size.width/2+144, size.height/2-126,143,126);

        for(var i=0;i<8;i++) {
            var decPos2 = new cc.Point(m_yanzhuarea[i].nareapos.x + m_yanzhuarea[i].nareapos.width/2,
                m_yanzhuarea[i].nareapos.y + m_yanzhuarea[i].nareapos.height/2);

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

        v_number_timer_sprite = new cc.LabelAtlas("6",v_sprite_num_people,19,22,"0");
        v_number_timer_sprite.setAnchorPoint(0.5, 0.5);
        v_number_timer_sprite.setPosition(size.width/2+10, size.height/2+135);
        this.addChild(v_number_timer_sprite, 10);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        m_sprintewaiting= new cc.Sprite(v_sprite_dengdai);
        m_sprintewaiting.setPosition(cc.p(size.width/2, size.height/2));
        m_sprintewaiting.setScale(0.7);
        m_sprintewaiting.setVisible(false);
        this.addChild(m_sprintewaiting,99998);

        m_labelallJettons = new cc.LabelTTF.create("0", "黑体", 20);
        m_labelallJettons.setColor(cc.color(255, 255, 0));
        m_labelallJettons.setAnchorPoint(0,0.5);
        m_labelallJettons.setPosition(size.width-265,size.height/2+130);
        this.addChild(m_labelallJettons);

        m_labelName = new cc.LabelTTF.create("test", "黑体", 12);
        m_labelName.setColor(cc.color(255, 255, 255));
        m_labelName.setAnchorPoint(0,0.5);
        m_labelName.setPosition(70, 50);
        this.addChild(m_labelName);

        m_labelMoney = new cc.LabelTTF.create("212323", "黑体", 12);
        m_labelMoney.setColor(cc.color(255, 255, 255));
        m_labelMoney.setAnchorPoint(0,0.5);
        m_labelMoney.setPosition(70, 38);
        this.addChild(m_labelMoney);

        m_labelCurJetton = new cc.LabelTTF.create("0", "黑体", 12);
        m_labelCurJetton.setColor(cc.color(255, 255, 255));
        m_labelCurJetton.setAnchorPoint(0,0.5);
        m_labelCurJetton.setPosition(70, 13);
        this.addChild(m_labelCurJetton);

        m_labelTotalResult = new cc.LabelTTF.create("0", "黑体", 12);
        m_labelTotalResult.setColor(cc.color(255, 255, 255));
        m_labelTotalResult.setAnchorPoint(0,0.5);
        m_labelTotalResult.setPosition(70, 25);
        this.addChild(m_labelTotalResult);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        m_spriteNoMoneyXiaZhu=new cc.Sprite(v_sprite_nomoney_xiazhu);
        m_spriteNoMoneyXiaZhu.setPosition(cc.p(size.width/2, size.height/2));
        m_spriteNoMoneyXiaZhu.setVisible(false);
        m_spriteNoMoneyXiaZhu.setScale(0.9);
        this.addChild(m_spriteNoMoneyXiaZhu,99998);

        m_spriteqinxiazhu=new cc.Sprite(v_sprite_qingxiazhu);
        m_spriteqinxiazhu.setPosition(cc.p(size.width/2, size.height/2));
        m_spriteqinxiazhu.setVisible(false);
        m_spriteqinxiazhu.setScale(0.9);
        this.addChild(m_spriteqinxiazhu,99998);

        v_gameover_bg_sprite = new cc.Sprite(v_sprite_gameoverbg);
        v_gameover_bg_sprite.setAnchorPoint(0.5, 0.5);
        v_gameover_bg_sprite.setPosition(size.width / 2, size.height / 2+20);
        v_gameover_bg_sprite.setVisible(false);
        this.addChild(v_gameover_bg_sprite, 19000);

        v_gameover_bg_lost_sprite = new cc.Sprite(v_sprite_gameovershu);
        v_gameover_bg_lost_sprite.setAnchorPoint(0.5, 0.5);
        v_gameover_bg_lost_sprite.setPosition(size.width / 2, size.height / 2+120);
        v_gameover_bg_lost_sprite.setVisible(false);
        this.addChild(v_gameover_bg_lost_sprite, 19000);

        v_gameover_bg_win_sprite = new cc.Sprite(v_sprite_gameoveryin);
        v_gameover_bg_win_sprite.setAnchorPoint(0.5, 0.5);
        v_gameover_bg_win_sprite.setPosition(size.width / 2, size.height / 2+162);
        v_gameover_bg_win_sprite.setVisible(false);
        this.addChild(v_gameover_bg_win_sprite, 19000);

        // ;表示减 < 表示加
        v_number_sprite = new cc.LabelAtlas("<123223432",v_sprite_OTHERS_NUM,11,17,"0");
        v_number_sprite.setAnchorPoint(0.5, 0.5);
        v_number_sprite.setPosition(size.width / 2+20, size.height / 2-7);
        v_number_sprite.setVisible(false);
        this.addChild(v_number_sprite, 19000);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

        var m_btnjettonStart = cc.p(-185,-205);

        this.s_100_btn.setTag(100);
        this.s_100_btn.setScale(1.4);
        this.s_100_btn.setPosition(m_btnjettonStart.x+0*75,m_btnjettonStart.y);
        this.s_500_btn.setTag(500);
        this.s_500_btn.setScale(1.4);
        this.s_500_btn.setPosition(m_btnjettonStart.x+1*75,m_btnjettonStart.y);
        this.s_1000_btn.setTag(1000);
        this.s_1000_btn.setScale(1.4);
        this.s_1000_btn.setPosition(m_btnjettonStart.x+2*75,m_btnjettonStart.y);
        this.s_5000_btn.setTag(5000);
        this.s_5000_btn.setScale(1.4);
        this.s_5000_btn.setPosition(m_btnjettonStart.x+3*75,m_btnjettonStart.y);
        this.s_10000_btn.setTag(10000);
        this.s_10000_btn.setScale(1.4);
        //this.s_10000_btn.setVisible(false);
        this.s_10000_btn.setPosition(m_btnjettonStart.x+4*75,m_btnjettonStart.y);
        this.s_100000_btn.setTag(100000);
        this.s_100000_btn.setScale(1.4);
        //this.s_100000_btn.setVisible(false);
        this.s_100000_btn.setPosition(m_btnjettonStart.x+5*75,m_btnjettonStart.y);

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

var CBcbmScene = cc.Scene.extend({
    onEnter:function () {
        this._super();

        m_MainFrameLayer = new CMainFrameLayer();
        this.addChild(m_MainFrameLayer);
        m_MainFrameLayer.init();
    }
});

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
            row1.JettonArea = i;
            row1.JettonMoney = m_MenuItemObjTag;

            m_gameserversocket.send(JSON.stringify(row1));

            m_myselfusermoney = m_myselfusermoney - m_MenuItemObjTag;
            m_labelMoney.setString(m_myselfusermoney);

            cc.audioEngine.playEffect(v_sound_AddGold, false);

            break;
        }
    }

    return boolRet;
}

function OnProcessAnimationMessage() {
    var dateObject = new Date();
    var pTmpTime = dateObject.getTime();

    if(pTmpTime > m_curAnimTime + m_curAnimSpeed)
    {
        m_cbCardListSprite[m_curShowAnimFrame].setVisible(false);

        m_curShowAnimFrame += 1;

        if(m_curShowAnimFrame > 31)
            m_curShowAnimFrame = 0;

        m_cbCardListSprite[m_curShowAnimFrame].setVisible(true);

        cc.audioEngine.playEffect(v_sound_Animal1, false);

        dateObject = new Date();
        m_curAnimTime = dateObject.getTime();

        if(m_isAnimFanSpeed)
        {
            if(m_curAnimSpeed < 300.0)
                m_curAnimSpeed += 18.0;

            if(m_curShowAnimFrame == m_curDecPosition)
            {
                //m_curAnimCircle += 1;

                //if(m_curAnimCircle > 2)
                m_MainFrameLayer.unschedule(OnProcessAnimationMessage);

                var p_game_state_timer_id = setInterval( function(){
                    clearInterval(p_game_state_timer_id);

                    OnProcessAnimationResultMessage();
                },1000);
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
    var blinkAction = new cc.blink(3, 5);
    m_cbCardListSprite[m_curShowAnimFrame].runAction(blinkAction);

    var blinkAction2 = new cc.blink(3, 5);
    m_cbCarFlashResult[m_cbCardListData[m_curShowAnimFrame]].runAction(blinkAction2);
}


