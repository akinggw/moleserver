var m_MainFrameLayer = null;
var v_number_timer_sprite = null;
var m_sprintewaiting = null;
var m_spriteNoMoneyXiaZhu = null;
var m_spriteqinxiazhu = null;
var v_gameover_bg_sprite = null;
var v_card_result_sprite_anim = null;
var v_gameover_bg_lost_sprite = null;
var v_gameover_bg_win_sprite = null;
var v_number_sprite = null;
var v_card_result_sprite = [];
var m_JettonArea = [];
var v_card_result_pos = [];
var m_card_result_tips = [];
var m_jettonresult = [];
var m_totaljettonresult = [];
var m_game_sprite_clock_bg = null;
var m_labelGameState = null;
var m_avatarstex = null;
var m_labelName = null;
var m_myuserlabelmoney = null;
var m_myuserlabelResult = null;
var m_sprYaoShaizi = null;
var m_sprShaizi1 = null;
var m_sprShaizi2 = null;
var m_dice1=0;
var m_dice2=0;
var m_yaoshaiIndex=0;
var m_yaoshaiCount=0;
var m_all_card_data = [];
var m_all_card_types = [];
var m_user_mouse_isprocess = true;
var m_MenuItemObjTag = 100;
var m_all_jettonsprites = [];
var m_all_jettonsprites_count = 0;
var m_yanzhuareatotalmoney = [];
var m_yanzhuareamemoney = [];
var v_game_waiting_colorlayer = null;
var m_game_timer_current_time = 0;
var m_game_timer_id = null;

function ClearGamingJettons() {
    if(m_MainFrameLayer == null)
        return;

    for(var i=0;i<m_all_jettonsprites_count;i++){
        m_MainFrameLayer.removeChild(m_all_jettonsprites[i]);
    }

    m_all_jettonsprites_count=0;

    for(var i=0;i<4;i++) {
        m_yanzhuareatotalmoney[i] = 0;
        m_yanzhuareamemoney[i] = 0;

        m_totaljettonresult[i].setString(m_yanzhuareatotalmoney[i]);
        m_jettonresult[i].setString(m_yanzhuareamemoney[i]);

        m_totaljettonresult[i].setVisible(false);
        m_jettonresult[i].setVisible(false);
    }

    for(var value of v_card_result_sprite) {
        value.setVisible(false);
    }

    for(var value2 of m_card_result_tips) {
        value2.setVisible(false);
    }
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
            cc.audioEngine.playEffect(v_game_sound_GAME_WARN, false);
        }

        v_number_timer_sprite.setString(m_game_timer_current_time);
    },1000);
}

function ShowGamingState(pGameState)
{
    switch(pGameState)
    {
        case STATE_XIAZHU:
        {
            m_labelGameState.setString("正在下注");
        }
            break;
        case STATE_GAMING:
        {
            m_labelGameState.setString("正在开牌");
        }
            break;
        case STATE_GAMEEND:
        {
            m_labelGameState.setString("正在结算");
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

function ShowGameState(type,isshow) {
    m_sprintewaiting.setVisible(false);
    v_game_waiting_colorlayer.setVisible(false);
    m_spriteNoMoneyXiaZhu.setVisible(false);
    m_spriteqinxiazhu.setVisible(false);

    if(type == 1) {
        m_sprintewaiting.setVisible(isshow);
        v_game_waiting_colorlayer.setVisible(isshow);
    }
    else if(type == 2) {
        m_spriteNoMoneyXiaZhu.setVisible(isshow);
    }
    else if(type == 3) {
        m_spriteqinxiazhu.setVisible(isshow);
    }

    if(isshow) {
        var v_game_state_timer_id = setInterval( function(){
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

function GamingJetton(areaIndex,pMenuItemObjTag,isme) {
    var decPos = new cc.Point(cc.rectGetMidX(m_JettonArea[areaIndex]) + Math.random()*8,
        cc.rectGetMidY(m_JettonArea[areaIndex]) + Math.random()*5 - 10);

    var jettonsprite = new cc.Sprite(v_game_sprite_game_pngJettonView);
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

    m_jettonresult[areaIndex].setVisible(true);
    m_totaljettonresult[areaIndex].setVisible(true);

    m_all_jettonsprites[m_all_jettonsprites_count] = jettonsprite;
    m_all_jettonsprites_count = m_all_jettonsprites_count + 1;

    m_yanzhuareatotalmoney[areaIndex] = m_yanzhuareatotalmoney[areaIndex] + pMenuItemObjTag;
    m_totaljettonresult[areaIndex].setString(m_yanzhuareatotalmoney[areaIndex]);

    if(isme) {
        m_yanzhuareamemoney[areaIndex] = m_yanzhuareamemoney[areaIndex] + pMenuItemObjTag;
        m_jettonresult[areaIndex].setString(m_yanzhuareamemoney[areaIndex]);
    }
}

function FaPai(cards,index=0) {
    var size = cc.director.getWinSize();
    var tmp = cards[index];
    var color = (tmp & 240) >> 4;
    var value = (tmp & 15);

    v_card_result_sprite_anim.setPosition(size.width / 2, size.height / 2+50);
    v_card_result_sprite_anim.setVisible(true);

    v_card_result_sprite[index].setTextureRect(cc.rect(value*55,color*73,55,73));

    var showPosition = cc.callFunc(function(){
        v_card_result_sprite_anim.setVisible(false);
        v_card_result_sprite[index].setVisible(true);

        if(index+1 < cards.length) {
            cc.audioEngine.playEffect(v_game_sound_DispatchCard, false);

            FaPai(cards,index+1);
        }
        else {
            var i = 0;

            var p_game_state_timer_id = setInterval( function(){
                m_card_result_tips[i].setVisible(true);
                m_card_result_tips[i].setString(getcardtypename(m_all_card_types[i]));

                playcardtypesound(m_all_card_types[i]);
                i = i + 1;

                if(i >= m_card_result_tips.length)
                    clearInterval(p_game_state_timer_id);
            },1000);
        }
    },v_card_result_sprite_anim);
    var moveTo = cc.moveTo(0.1, v_card_result_sprite[index].getPosition());
    var seq = cc.sequence(moveTo, showPosition);
    v_card_result_sprite_anim.runAction(seq);
}

function startYaoShaiAnimitor(dice1,dice2)
{
    m_dice1 = dice1;
    m_dice2 = dice2;

    m_sprShaizi1.setVisible(false);
    m_sprShaizi2.setVisible(false);
    m_yaoshaiIndex=0;
    m_yaoshaiCount=0;
    m_sprYaoShaizi.setTextureRect(cc.rect(0,0,150,300));
    m_sprYaoShaizi.setVisible(true);

    m_MainFrameLayer.schedule(OnProcessStartYaoShaiziAnimMessage, 0.12);
}

function OnProcessStartYaoShaiziAnimMessage()
{
    if(m_yaoshaiCount >= 1)
    {
        m_sprYaoShaizi.setVisible(false);
        m_MainFrameLayer.unschedule(OnProcessStartYaoShaiziAnimMessage);

        m_sprShaizi1.setTextureRect(cc.rect(55*m_dice1,0,55,55));
        m_sprShaizi1.setVisible(true);
        m_sprShaizi2.setTextureRect(cc.rect(55*m_dice2,0,55,55));
        m_sprShaizi2.setVisible(true);

        var p_game_timer_id = setInterval( function(){
            clearInterval(p_game_timer_id);

            m_sprShaizi1.setVisible(false);
            m_sprYaoShaizi.setVisible(false);
            m_sprShaizi2.setVisible(false);

            FaPai(m_all_card_data);
        },2000);

        return;
    }

    m_yaoshaiIndex += 1;

    m_sprYaoShaizi.setTextureRect(cc.rect(150*m_yaoshaiIndex,0,150,300));

    if(m_yaoshaiIndex >= 8)
    {
        m_yaoshaiIndex = 0;
        m_yaoshaiCount += 1;
    }
}

var CMainFrameLayer = cc.Layer.extend({
    bgsprite:null,
    bgsprite1:null,
    bgsprite2:null,
    bgsprite3:null,
    bgsprite4:null,
    bgsprite6:null,
    s_100_btn:null,
    s_500_btn:null,
    s_1000_btn:null,
    s_5000_btn:null,
    s_10000_btn:null,
    s_50000_btn:null,

    init:function () {
        this._super();

        var size = cc.director.getWinSize();

        this.bgsprite = new cc.Sprite(v_game_sprite_game_bg);
        this.bgsprite.setAnchorPoint(0.5, 0.5);
        this.bgsprite.setPosition(size.width / 2, size.height / 2);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(this.bgsprite, 0);
        this.bgsprite1 = new cc.Sprite(v_game_sprite_game_bg_tian);
        this.bgsprite1.setPosition(cc.p(size.width/2-290, size.height/2));
        this.bgsprite1.setScale(1.0);
        this.addChild(this.bgsprite1);
        this.bgsprite2 = new cc.Sprite(v_game_sprite_game_bg_di);
        this.bgsprite2.setPosition(cc.p(size.width/2-95, size.height/2));
        this.bgsprite2.setScale(1.0);
        this.addChild(this.bgsprite2);
        this.bgsprite3 = new cc.Sprite(v_game_sprite_game_bg_xuan);
        this.bgsprite3.setPosition(cc.p(size.width/2+100, size.height/2));
        this.bgsprite3.setScale(1.0);
        this.addChild(this.bgsprite3);
        this.bgsprite4 = new cc.Sprite(v_game_sprite_game_bg_huang);
        this.bgsprite4.setPosition(cc.p(size.width/2+295, size.height/2));
        this.bgsprite4.setScale(1.0);
        this.addChild(this.bgsprite4);
        this.bgsprite4 = new cc.Sprite(v_game_sprite_game_game_nav);
        this.bgsprite4.setPosition(cc.p(0,113));
        this.bgsprite4.setAnchorPoint(cc.p(0.0, 1));
        this.addChild(this.bgsprite4);

        m_game_sprite_clock_bg = new cc.Sprite(v_game_sprite_clock_bg);
        m_game_sprite_clock_bg.setPosition(cc.p(size.width/2-250,size.height/2+128));
        //m_game_sprite_clock_bg.setVisible(false);
        this.addChild(m_game_sprite_clock_bg);
        m_labelGameState = new cc.LabelTTF.create("正在下注", "黑体", 20);
        m_labelGameState.setPosition(size.width/2-250, size.height/2+139);
        m_labelGameState.setAnchorPoint(0.5, 1);
        m_labelGameState.setColor(cc.color(255, 255, 255));
        this.addChild(m_labelGameState);

        v_number_timer_sprite = new cc.LabelAtlas("6",v_game_sprite_game_num_people,19,22,"0");
        v_number_timer_sprite.setAnchorPoint(0.5, 0.5);
        v_number_timer_sprite.setPosition(size.width/2-250, size.height/2+158);
        this.addChild(v_number_timer_sprite, 10);

        v_game_waiting_colorlayer = new cc.LayerColor(new cc.color(0, 0, 0, 200), size.width, size.height);
        v_game_waiting_colorlayer.setAnchorPoint(0.5, 0.5);
        v_game_waiting_colorlayer.setPosition(0,0);
        v_game_waiting_colorlayer.setVisible(false);
        this.addChild(v_game_waiting_colorlayer, 19000);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        m_avatarstex= new cc.Sprite(v_game_sprite_avator);
        m_avatarstex.setPosition(cc.p(size.width/2-330,size.height/2-195));
        this.addChild(m_avatarstex);

        m_labelName = new cc.LabelTTF.create("昵称:", "黑体", 18);
        m_labelName.setPosition(size.width/2-285,size.height/2-170);
        m_labelName.setAnchorPoint(0.0, 0.5);
        m_labelName.setColor(cc.color(255, 255, 255));
        this.addChild(m_labelName);
        m_myuserlabelmoney = new cc.LabelTTF.create("财富:", "黑体", 18);
        m_myuserlabelmoney.setPosition(size.width/2-285,size.height/2-195);
        m_myuserlabelmoney.setAnchorPoint(0.0, 0.5);
        m_myuserlabelmoney.setColor(cc.color(255, 255, 255));
        this.addChild(m_myuserlabelmoney);
        m_myuserlabelResult = new cc.LabelTTF.create("成绩:", "黑体", 18);
        m_myuserlabelResult.setPosition(size.width/2-285,size.height/2-220);
        m_myuserlabelResult.setAnchorPoint(0.0, 0.5);
        m_myuserlabelResult.setColor(cc.color(255, 255, 255));
        this.addChild(m_myuserlabelResult);

        m_sprYaoShaizi=new cc.Sprite(v_game_sprite_aniTouzi);
        m_sprYaoShaizi.setPosition(cc.p(size.width/2, size.height/2+60));
        m_sprYaoShaizi.setTextureRect(cc.rect(150*5,0,150,300));
        //spriteanimitor.setScale(0.7f);
        m_sprYaoShaizi.setVisible(false);
        this.addChild(m_sprYaoShaizi,99998);

        m_sprShaizi1=new cc.Sprite(v_game_sprite_game_pngDiShu1);
        m_sprShaizi1.setPosition(cc.p(size.width/2-35, size.height/2-10));
        //m_sprShaizi1.setScale(0.7f);
        m_sprShaizi1.setVisible(false);
        this.addChild(m_sprShaizi1,99998);

        m_sprShaizi2=new cc.Sprite(v_game_sprite_game_pngDiShu1);
        m_sprShaizi2.setPosition(cc.p(size.width/2+35, size.height/2-10));
        //m_sprShaizi2.setScale(0.7f);
        m_sprShaizi2.setVisible(false);
        this.addChild(m_sprShaizi2,99998);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        m_sprintewaiting= new cc.Sprite(v_game_sprite_dengdai);
        m_sprintewaiting.setPosition(cc.p(size.width/2, size.height/2));
        m_sprintewaiting.setScale(0.7);
        m_sprintewaiting.setVisible(false);
        this.addChild(m_sprintewaiting,99998);

        m_spriteNoMoneyXiaZhu=new cc.Sprite(v_game_sprite_nomoney_xiazhu);
        m_spriteNoMoneyXiaZhu.setPosition(cc.p(size.width/2, size.height/2));
        m_spriteNoMoneyXiaZhu.setVisible(false);
        m_spriteNoMoneyXiaZhu.setScale(0.9);
        this.addChild(m_spriteNoMoneyXiaZhu,99998);

        m_spriteqinxiazhu=new cc.Sprite(v_game_sprite_game_qingxiazhu1);
        m_spriteqinxiazhu.setPosition(cc.p(size.width/2, size.height/2));
        m_spriteqinxiazhu.setVisible(false);
        m_spriteqinxiazhu.setScale(0.9);
        this.addChild(m_spriteqinxiazhu,99998);

        v_gameover_bg_sprite = new cc.Sprite(v_game_sprite_game_gameoverbg);
        v_gameover_bg_sprite.setAnchorPoint(0.5, 0.5);
        v_gameover_bg_sprite.setPosition(size.width / 2, size.height / 2+20);
        v_gameover_bg_sprite.setVisible(false);
        this.addChild(v_gameover_bg_sprite, 19000);

        v_gameover_bg_lost_sprite = new cc.Sprite(v_game_sprite_game_gameovershu);
        v_gameover_bg_lost_sprite.setAnchorPoint(0.5, 0.5);
        v_gameover_bg_lost_sprite.setPosition(size.width / 2, size.height / 2+120);
        v_gameover_bg_lost_sprite.setVisible(false);
        this.addChild(v_gameover_bg_lost_sprite, 19000);

        v_gameover_bg_win_sprite = new cc.Sprite(v_game_sprite_game_gameoveryin);
        v_gameover_bg_win_sprite.setAnchorPoint(0.5, 0.5);
        v_gameover_bg_win_sprite.setPosition(size.width / 2, size.height / 2+162);
        v_gameover_bg_win_sprite.setVisible(false);
        this.addChild(v_gameover_bg_win_sprite, 19000);

        // ;表示减 < 表示加
        v_number_sprite = new cc.LabelAtlas("<123223432",v_game_sprite_game_OTHERS_NUM,11,17,"0");
        v_number_sprite.setAnchorPoint(0.5, 0.5);
        v_number_sprite.setPosition(size.width / 2+20, size.height / 2-7);
        v_number_sprite.setVisible(false);
        this.addChild(v_number_sprite, 19000);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        m_JettonArea[0] = cc.rect(16,178,180,120);
        m_JettonArea[1] = cc.rect(217,178,175,120);
        m_JettonArea[2] = cc.rect(418,178,167,120);
        m_JettonArea[3] = cc.rect(607,178,170,120);

        v_card_result_pos[0] = cc.p(size.width/2,size.height-65);
        v_card_result_pos[1] = cc.p(cc.rectGetMidX(m_JettonArea[0]),cc.rectGetMidY(m_JettonArea[0])-100);
        v_card_result_pos[2] = cc.p(cc.rectGetMidX(m_JettonArea[1]),cc.rectGetMidY(m_JettonArea[1])-100);
        v_card_result_pos[3] = cc.p(cc.rectGetMidX(m_JettonArea[2]),cc.rectGetMidY(m_JettonArea[2])-100);
        v_card_result_pos[4] = cc.p(cc.rectGetMidX(m_JettonArea[3]),cc.rectGetMidY(m_JettonArea[3])-100);

        var v_card_result_sprite_pos = [];
        for(var i=0;i<5;i++) {
            v_card_result_sprite_pos[i*5+0] = cc.p(v_card_result_pos[i].x-30,v_card_result_pos[i].y);
            v_card_result_sprite_pos[i*5+1] = cc.p(v_card_result_pos[i].x-15,v_card_result_pos[i].y);
            v_card_result_sprite_pos[i*5+2] = cc.p(v_card_result_pos[i].x,v_card_result_pos[i].y);
            v_card_result_sprite_pos[i*5+3] = cc.p(v_card_result_pos[i].x+15,v_card_result_pos[i].y);
            v_card_result_sprite_pos[i*5+4] = cc.p(v_card_result_pos[i].x+30,v_card_result_pos[i].y);

            m_card_result_tips[i] = new cc.LabelBMFont("牛牛",v_game_sprite_chipall_fnt,520);
            m_card_result_tips[i].setColor(cc.color(255, 0, 0));
            m_card_result_tips[i].setAnchorPoint(cc.p(0.5,0.5));
            m_card_result_tips[i].setVisible(false);
            m_card_result_tips[i].setPosition(cc.p(v_card_result_pos[i].x,v_card_result_pos[i].y));
            this.addChild(m_card_result_tips[i],8843);

            if(i<4) {
                m_totaljettonresult[i] = new cc.LabelAtlas("1000",v_game_sprite_game_num_people,19,22,"0");
                m_totaljettonresult[i].setAnchorPoint(0.5, 0.5);
                m_totaljettonresult[i].setPosition(cc.rectGetMidX(m_JettonArea[i]),cc.rectGetMidY(m_JettonArea[i])+20);
                this.addChild(m_totaljettonresult[i], 8843);

                m_jettonresult[i] = new cc.LabelAtlas("100",v_game_sprite_game_sd_number,15,15,"0");
                m_jettonresult[i].setAnchorPoint(0.5, 0.5);
                m_jettonresult[i].setPosition(cc.rectGetMidX(m_JettonArea[i]),cc.rectGetMidY(m_JettonArea[i])-20);
                this.addChild(m_jettonresult[i], 8843);

                m_jettonresult[i].setVisible(false);
                m_totaljettonresult[i].setVisible(false);

                m_yanzhuareatotalmoney[i] = 0;
                m_yanzhuareamemoney[i] = 0;
            }
        }


        for(var i = 0;i<v_card_result_sprite_pos.length;i++) {
            v_card_result_sprite[i] = new cc.Sprite(v_game_sprite_CARD);
            v_card_result_sprite[i].setAnchorPoint(0.5, 0.5);
            v_card_result_sprite[i].setScale(1.05);
            v_card_result_sprite[i].setPosition(v_card_result_sprite_pos[i]);
            v_card_result_sprite[i].setTextureRect(cc.rect(55,73,55,73));
            v_card_result_sprite[i].setVisible(false);
            //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
            this.addChild(v_card_result_sprite[i]);
        }

        v_card_result_sprite_anim = new cc.Sprite(v_game_sprite_CARD);
        v_card_result_sprite_anim.setAnchorPoint(0.5, 0.5);
        v_card_result_sprite_anim.setScale(1.05);
        v_card_result_sprite_anim.setPosition(size.width / 2, size.height / 2);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        v_card_result_sprite_anim.setTextureRect(cc.rect(2*55,4*73,55,73));
        v_card_result_sprite_anim.setVisible(false);
        this.addChild(v_card_result_sprite_anim, 9000);

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
        this.s_50000_btn = new cc.MenuItemImage.create(
            v_sprite_50000_1,
            v_sprite_50000_2,
            v_sprite_50000_3,
            MenuItemProcess,this);

        var menu = new cc.Menu.create(this.s_100_btn,this.s_500_btn,this.s_1000_btn,this.s_5000_btn,this.s_10000_btn,this.s_50000_btn);
        menu.setPosition(size.width/2+120, size.height/2+20);
        this.addChild(menu, 1);

        var pStartX = -220;
        this.s_100_btn.setTag(100);
        this.s_100_btn.setScale(1.2);
        this.s_100_btn.setPosition(pStartX,-215);
        this.s_500_btn.setTag(500);
        this.s_500_btn.setScale(1.2);
        this.s_500_btn.setPosition(pStartX+65,-215);
        this.s_1000_btn.setTag(1000);
        this.s_1000_btn.setScale(1.2);
        this.s_1000_btn.setPosition(pStartX+65*2,-215);
        this.s_5000_btn.setTag(5000);
        this.s_5000_btn.setScale(1.2);
        this.s_5000_btn.setPosition(pStartX+65*3,-215);
        this.s_10000_btn.setTag(10000);
        this.s_10000_btn.setScale(1.2);
        this.s_10000_btn.setPosition(pStartX+65*4,-215);
        this.s_50000_btn.setTag(50000);
        this.s_50000_btn.setScale(1.2);
        this.s_50000_btn.setPosition(pStartX+65*5,-215);

        cc.audioEngine.playMusic(v_game_sound_BGM_Room_4, true);

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

var CBrnnScene = cc.Scene.extend({
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

    for (var i = 0; i<4; i++)
    {
        var iscontains = cc.rectContainsPoint(m_JettonArea[i],posInNode);

        if(iscontains) {
            GamingJetton(i,m_MenuItemObjTag,true);

            var row1 = {};
            row1.MsgId = 1000;
            row1.MsgSubId = IDD_MESSAGE_ROOM_XIAZHU;
            row1.YaZhuType = i;
            row1.Money = m_MenuItemObjTag;

            m_gameserversocket.send(JSON.stringify(row1));

            m_myselfusermoney = m_myselfusermoney - m_MenuItemObjTag;
            m_myuserlabelmoney.setString("财富:"+m_myselfusermoney);

            cc.audioEngine.playEffect(v_game_sound_AddGold, false);

            break;
        }
    }

    return boolRet;
}