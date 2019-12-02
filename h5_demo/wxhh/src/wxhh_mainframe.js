var socket;
var gameserversocket;
var myselfUserId = 0;
var m_oldJvHao = 1;
var last_health;
var last_health_game;
var health_timeout=6000;
//var game_jv_count=65;
var keepalivetimer;
var keepalivetimer_game;
var host = "ws://182.61.5.226:6789";
//var host = "ws://127.0.0.1:3333";
var isLoginSuccuss=false;
var gamestate = 0;
var gamepielement = 0;
var m_myselfusermoney = 0;
var m_myselftempusermoney = 0;
var m_GameItemBeiLv = [];
var m_myselfJettonTotal = [];
var m_AllJettonTotal = [];
var m_colorrecordcount = [];
var m_cardrecourdcount = [];
var m_gamerecordsprites = [];
var m_myselftotaljettons = 0;
var m_myselftotalresult = 0;
//var m_myselfgamegonggaostr="";
var m_spritetimerbg = null;
var m_spriteTimer1 = null;
var m_spriteTimer2 = null;
var m_curTimerId = 0;
var m_curTime = 10;
var m_timexiazhu = 5;
var m_timekaipai = 3;
var m_timejiesuan = 3;
var m_isClearConntineData = false;
var m_isCurrentJetton = false;
var m_tempcolorjettons = [];
var m_gameserverlist = [];
var WebSocket = WebSocket || window.WebSocket || window.MozWebSocket;
var m_isplaymusic=true;
var m_registerLayer = null;
var MyMainLoginlayer = null;
var m_chongzhiLayer = null;
var m_CZhuanZhangLayer = null;
var m_isLoginSuccess = false;
var m_userloginname = "";
var m_userloginpassword = "";
var m_gameserver = "";

function keepalive(ws,type=0) {
    var time = new Date();
    var curtime = last_health.getTime();

    if(type == 1)
        curtime = last_health_game.getTime();

    if(time.getTime() - curtime > health_timeout)
    {
        if(type == 0)
            clearInterval(keepalivetimer);
        else
            clearInterval(keepalivetimer_game);
    }
    else
    {
        if (ws.bufferedAmount == 0) {
            ws.send('100');
            last_health = time;
            last_health_game = time;
        }
    }
};

function Timeralive(isdec) {

    if(isdec == true)
        m_curTime -= 1;

    if(m_curTime > 0 && m_curTime < 10)
    {
        m_spriteTimer1.setTextureRect(cc.rect(0,0,15,15));
        m_spriteTimer2.setTextureRect(cc.rect(m_curTime*15,0,15,15));

        if(!m_spriteTimer1.isVisible())
        {
            m_spriteTimer1.setVisible(true);
            m_spriteTimer2.setVisible(true);
            m_spritetimerbg.setVisible(true);
        }

        if(m_curTime <= 3 && gamestate == 1 && m_isplaymusic){
            //cc.audioEngine.playEffect(sound_GameAnteWarn,false);
        }
    }
    else if(m_curTime > 10)
    {
        var first = m_curTime / 10;
        var second = m_curTime % 10;

        if(first >= 0){
            first = Math.floor(first);
        }
        else{
            first = Math.ceil(first);
        }

        m_spriteTimer1.setTextureRect(cc.rect(first*15,0,15,15));
        m_spriteTimer2.setTextureRect(cc.rect(second*15,0,15,15));

        if(!m_spriteTimer1.isVisible())
        {
            m_spriteTimer1.setVisible(true);
            m_spriteTimer2.setVisible(true);
            m_spritetimerbg.setVisible(true);
        }
    }
    else if(m_curTime <= 0)
    {
        m_spriteTimer1.setVisible(false);
        m_spriteTimer2.setVisible(false);
        m_spritetimerbg.setVisible(false);
        clearInterval(m_curTimerId);
    }
};

function splitStr(str, width, fontName, fontSize) {
    var getWidth = function(str) {
        var tmpLabel = cc.LabelTTF.create(str, fontName, fontSize);
        return tmpLabel.getContentSize().width;
    };
    
    var c = str.length,
        i = 0,
        l = parseInt(width / fontSize, 10),
        lines = [];

    while(i < c) {
        var last = current = '';

        while(true) {
            if(i + l > c) {
                break;
            }

            var s = str.substr(i, l),
                w = getWidth(s);

            if(w != width) {
                if(w > width) {
                    current = '-';
                    l--;
                } else {
                    current = '+';
                    l++;
                }

                if(last && last != current) {
                    if(current == '+') {
                        l--;
                    }
                    break;
                }

                last = current;

            } else {
                break;
            }
        }

        lines.push(str.substr(i, l));
        i += l;
    }

    return lines;
};

var MyMessageBoxLayer = cc.LayerColor.extend({
    helloLabel:null,
    sprite:null,

    init:function (msg) {
        // 1. super init first
        // 必须调用父类init()方法，很多bug都是由于没有调用父类init()方法造成的
        this._super();

        // 设置Layer的背景
        this.setColor(cc.color(0,0,0));
        this.setOpacity(200);

        var size = cc.director.getWinSize();

        this.sprite = new cc.Sprite.create(s_common_pop_bkg);
        this.sprite.setAnchorPoint(0.5, 0.5);
        this.sprite.setPosition(size.width / 2, size.height / 2-20);
        this.sprite.setScale(0.65);
        this.addChild(this.sprite);

		var str2 = splitStr(msg, 300, "黑体", 16).join('\n');
		
        this.helloLabel = new cc.LabelTTF.create(str2, "黑体", 16);
        // position the label on the center of the screen
        this.helloLabel.setPosition(size.width / 2, size.height/2);
        this.helloLabel.setColor(cc.color(0,0,0));
        // add the label as a child to this layer
        this.addChild(this.helloLabel);

        var closeItem = new cc.MenuItemImage.create(
            s_pop_ok_1,
            s_pop_ok_2,
            function () {
                this.removeFromParentAndCleanup(true);
            },this);
        closeItem.setAnchorPoint(0.5, 0.5);
        closeItem.setScale(0.8);

        var menu = new cc.Menu.create(closeItem);
        menu.setPosition(0, 0);
        this.addChild(menu, 1);
        closeItem.setPosition(size.width/2,size.height/2-60);
    }
});

var CRegisterLayer = cc.LayerColor.extend({
    _boxusername: null,
    sprite:null,
    _boxuserpassword:null,

    init: function () {
        this._super();

        //this.setColor(cc.color(0,0,0));
        this.setOpacity(200);

        var size = cc.director.getWinSize();

        this.sprite = new cc.Sprite(s_registerbg);
        this.sprite.setAnchorPoint(0.5, 0.5);
        this.sprite.setPosition(size.width / 2, size.height / 2);
        this.sprite.setScale(0.65);
        this.addChild(this.sprite, 0);

        this._boxusername = new cc.EditBox.create(cc.size(170, 50), new cc.Scale9Sprite.create(), new cc.Scale9Sprite.create());
        this._boxusername.setPlaceholderFontColor(cc.color(0, 0, 0));
        this._boxusername.setPlaceHolder("请输入用户名");
        this._boxusername.setPosition(size.width/2+50, size.height/2+33);
        this._boxusername.setFontColor(cc.color(0, 0, 0));
        this._boxusername.setDelegate(this);
        this.addChild(this._boxusername);

        this._boxuserpassword = new cc.EditBox.create(cc.size(170, 50), new cc.Scale9Sprite.create(),new cc.Scale9Sprite.create());
        this._boxuserpassword.setPlaceholderFontColor(cc.color(0, 0, 0));
        this._boxuserpassword.setPlaceHolder("请输入用户密码");
        this._boxuserpassword.setInputFlag(cc.EDITBOX_INPUT_FLAG_PASSWORD);
        this._boxuserpassword.setPosition(size.width/2+50, size.height/2-5);
        this._boxuserpassword.setFontColor(cc.color(0, 0, 0));
        this._boxuserpassword.setDelegate(this);
        this.addChild(this._boxuserpassword);

        var LoginBtnItem = new cc.MenuItemImage.create(
            s_pop_ok_1,
            s_pop_ok_2,
            function () {
                if(this._boxusername.getString() == "")
                {
                    var loginlayer = new MyMessageBoxLayer();
                    this.addChild(loginlayer,8);
                    loginlayer.init("用户名不能为空，请重新输入！");
                    return;
                }
                if(this._boxuserpassword.getString() == "")
                {
                    loginlayer = new MyMessageBoxLayer();
                    this.addChild(loginlayer,8);
                    loginlayer.init("用户密码不能为空，请重新输入！");
                    return;
                }

                var row1 = {};
                row1.MsgId = 700;
                row1.UserName = this._boxusername.getString();
                row1.UserPW = hex_md5(this._boxuserpassword.getString());

                socket.send(JSON.stringify(row1));
            },this);
        LoginBtnItem.setAnchorPoint(0.5, 0.5);
        LoginBtnItem.setScale(0.7);

        var RegisterBtnItem = new cc.MenuItemImage.create(
            s_pop_cancel_1,
            s_pop_cancel_2,
            function () {
                this.setVisible(false);
                MyMainLoginlayer.setVisible(true);
            },this);
        RegisterBtnItem.setAnchorPoint(0.5, 0.5);
        RegisterBtnItem.setScale(0.7);

        var menu = new cc.Menu.create(LoginBtnItem,RegisterBtnItem);
        menu.setPosition(0, 0);
        this.addChild(menu, 1);
        LoginBtnItem.setPosition(size.width/2-40,size.height/2-60);
        RegisterBtnItem.setPosition(size.width/2+50,size.height/2-60);
    }
});

var CLoginLayer = cc.LayerColor.extend({
    _boxusername: null,
    sprite:null,
    _boxuserpassword:null,

    init: function () {
        this._super();

        this.setColor(cc.color(0,0,0));
        this.setOpacity(200);

        var size = cc.director.getWinSize();

        this.sprite = new cc.Sprite(s_loginframe);
        this.sprite.setAnchorPoint(0.5, 0.5);
        this.sprite.setPosition(size.width / 2, size.height / 2);
        this.sprite.setScale(0.65);
        this.addChild(this.sprite, 0);

        this._boxusername = new cc.EditBox.create(cc.size(170, 50), new cc.Scale9Sprite.create(), new cc.Scale9Sprite.create());
        this._boxusername.setPlaceholderFontColor(cc.color(0, 0, 0));
        this._boxusername.setPlaceHolder("请输入用户名");
        this._boxusername.setPosition(size.width/2+41, size.height/2+28);
        this._boxusername.setFontColor(cc.color(0, 0, 0));
        this._boxusername.setDelegate(this);
        this.addChild(this._boxusername);

        this._boxuserpassword = new cc.EditBox.create(cc.size(170, 50), new cc.Scale9Sprite.create(),new cc.Scale9Sprite.create());
        this._boxuserpassword.setPlaceholderFontColor(cc.color(0, 0, 0));
        this._boxuserpassword.setPlaceHolder("请输入用户密码");
        this._boxuserpassword.setInputFlag(cc.EDITBOX_INPUT_FLAG_PASSWORD);
        this._boxuserpassword.setPosition(size.width/2+41, size.height/2-2);
        this._boxuserpassword.setFontColor(cc.color(0, 0, 0));
        this._boxuserpassword.setDelegate(this);
        this.addChild(this._boxuserpassword);

        var LoginBtnItem = new cc.MenuItemImage.create(
            s_login_login_1,
            s_login_login_2,
            function () {
                if(this._boxusername.getString() == "")
                {
                    var loginlayer = new MyMessageBoxLayer();
                    this.addChild(loginlayer,8);
                    loginlayer.init("用户名不能为空，请重新输入！");
                    return;
                }
                if(this._boxuserpassword.getString() == "")
                {
                    loginlayer = new MyMessageBoxLayer();
                    this.addChild(loginlayer,8);
                    loginlayer.init("用户密码不能为空，请重新输入！");
                    return;
                }

                m_userloginname = this._boxusername.getString();
                m_userloginpassword = hex_md5(this._boxuserpassword.getString());

                var row1 = {};
                row1.MsgId = 400;
                row1.username = m_userloginname;
                row1.userpwd = m_userloginpassword;
                row1.machinecode = 'html5';

                socket.send(JSON.stringify(row1));
            },this);
        LoginBtnItem.setAnchorPoint(0.5, 0.5);
        LoginBtnItem.setScale(0.6);

        var RegisterBtnItem = new cc.MenuItemImage.create(
            s_login_register_1,
            s_login_register_2,
            function () {
                this.setVisible(false);
                m_registerLayer.setVisible(true);
            },this);
        RegisterBtnItem.setAnchorPoint(0.5, 0.5);
        RegisterBtnItem.setScale(0.6);

        var menu = new cc.Menu.create(LoginBtnItem,RegisterBtnItem);
        menu.setPosition(0, 0);
        this.addChild(menu, 1);
        LoginBtnItem.setPosition(size.width/2-40,size.height/2-50);
        RegisterBtnItem.setPosition(size.width/2+50,size.height/2-50);
    }
});

var CChongZhiLayer = cc.LayerColor.extend({
    _boxchongzhimoney: null,
    sprite:null,

    init: function () {
        this._super();

        //this.setColor(cc.color(0,0,0));
        this.setOpacity(200);

        var size = cc.director.getWinSize();

        this.sprite = new cc.Sprite(s_chongzhi_bg);
        this.sprite.setAnchorPoint(0.5, 0.5);
        this.sprite.setPosition(size.width / 2, size.height / 2);
        this.sprite.setScale(0.65);
        this.addChild(this.sprite, 0);

        this._boxchongzhimoney = new cc.EditBox.create(cc.size(170, 50), new cc.Scale9Sprite.create(), new cc.Scale9Sprite.create());
        this._boxchongzhimoney.setPlaceholderFontColor(cc.color(255, 255, 255));
        this._boxchongzhimoney.setPlaceHolder("请输入充值卡号");
        this._boxchongzhimoney.setPosition(size.width/2-20, size.height/2);
        this._boxchongzhimoney.setFontColor(cc.color(255, 255, 255));
        this._boxchongzhimoney.setDelegate(this);
        this.addChild(this._boxchongzhimoney);

        var ChongZhiBtnItem = new cc.MenuItemImage.create(
            s_pop_ok_1,
            s_pop_ok_2,
            function () {
                if(this._boxchongzhimoney.getString() == "")
                {
                    var czMesBoxlayer = new MyMessageBoxLayer();
                    this.addChild(czMesBoxlayer,8);
                    czMesBoxlayer.init("充值卡号不能为空，请重新输入！");
                    return;
                }
				
                var row1 = {};
                row1.MsgId = 404;
				row1.opretype = 431;
                row1.userid = myselfUserId;
                row1.cardnum = this._boxchongzhimoney.getString();

                socket.send(JSON.stringify(row1));
				
				this.setVisible(false);
            },this);
        ChongZhiBtnItem.setAnchorPoint(0.5, 0.5);
        ChongZhiBtnItem.setScale(0.7);

        var CloseBtnItem = new cc.MenuItemImage.create(
            s_btn_close_1,
            s_btn_close_2,
            function () {
                this.setVisible(false);
            },this);
        CloseBtnItem.setAnchorPoint(0.5, 0.5);
        CloseBtnItem.setScale(0.7);

        var menu = new cc.Menu.create(ChongZhiBtnItem,CloseBtnItem);
        menu.setPosition(0, 0);
        this.addChild(menu, 1);
        ChongZhiBtnItem.setPosition(size.width/2+85,size.height/2-45);
        CloseBtnItem.setPosition(size.width/2+135,size.height/2+53);
    }
});

var CZhuanZhangLayer = cc.LayerColor.extend({
    _boxzhuanzhangmoney: null,
	_boxshoukuanren: null,
	MyUserMoney:null,
    sprite:null,

    init: function () {
        this._super();

        //this.setColor(cc.color(0,0,0));
        this.setOpacity(200);

        var size = cc.director.getWinSize();

        this.sprite = new cc.Sprite(s_zhuangzhang_bg);
        this.sprite.setAnchorPoint(0.5, 0.5);
        this.sprite.setPosition(size.width / 2, size.height / 2);
        this.sprite.setScale(0.65);
        this.addChild(this.sprite, 0);
		
        this.MyUserMoney = new cc.LabelTTF.create("", "黑体", 18);
        // position the label on the center of the screen
        this.MyUserMoney.setPosition(size.width / 2+20, size.height/2+48);
        this.MyUserMoney.setColor(cc.color(255,255,255));
        // add the label as a child to this layer
        this.addChild(this.MyUserMoney);			

        this._boxzhuanzhangmoney = new cc.EditBox.create(cc.size(170, 50), new cc.Scale9Sprite.create(), new cc.Scale9Sprite.create());
        this._boxzhuanzhangmoney.setPlaceholderFontColor(cc.color(255, 255, 255));
        this._boxzhuanzhangmoney.setPlaceHolder("请输入转账金额");
        this._boxzhuanzhangmoney.setPosition(size.width/2+45, size.height/2-32);
        this._boxzhuanzhangmoney.setFontColor(cc.color(255, 255, 255));
        this._boxzhuanzhangmoney.setDelegate(this);
        this.addChild(this._boxzhuanzhangmoney);
		
        this._boxshoukuanren = new cc.EditBox.create(cc.size(170, 50), new cc.Scale9Sprite.create(), new cc.Scale9Sprite.create());
        this._boxshoukuanren.setPlaceholderFontColor(cc.color(255, 255, 255));
        this._boxshoukuanren.setPlaceHolder("请输入收款用户账号");
        this._boxshoukuanren.setPosition(size.width/2+45, size.height/2+16);
        this._boxshoukuanren.setFontColor(cc.color(255, 255, 255));
        this._boxshoukuanren.setDelegate(this);
        this.addChild(this._boxshoukuanren);		

        var ChongZhiBtnItem = new cc.MenuItemImage.create(
            s_pop_ok_1,
            s_pop_ok_2,
            function () {
                if(this._boxzhuanzhangmoney.getString() == "")
                {
                    var czMesBoxlayer = new MyMessageBoxLayer();
                    this.addChild(czMesBoxlayer,8);
                    czMesBoxlayer.init("转账金币不能为空，请重新输入！");
                    return;
                }
                if(this._boxshoukuanren.getString() == "")
                {
                    var czMesBoxlayer = new MyMessageBoxLayer();
                    this.addChild(czMesBoxlayer,8);
                    czMesBoxlayer.init("收款人不能为空，请重新输入！");
                    return;
                }	
				
				var pzzmoney = Number(this._boxzhuanzhangmoney.getString());
				
				if(pzzmoney > m_myselftempusermoney || pzzmoney <= 0)
				{
                    var czMesBoxlayer = new MyMessageBoxLayer();
                    this.addChild(czMesBoxlayer,8);
                    czMesBoxlayer.init("转账金额不对，请重新输入！");
                    return;				
				}
				
                var row1 = {};
                row1.MsgId = 404;
				row1.MsgSubId = 416;
                row1.UserID = myselfUserId;
                row1.Money = Number(this._boxzhuanzhangmoney.getString());
				row1.Receiver = this._boxshoukuanren.getString();

                socket.send(JSON.stringify(row1));
				
				this.setVisible(false);
            },this);
        ChongZhiBtnItem.setAnchorPoint(0.5, 0.5);
        ChongZhiBtnItem.setScale(0.7);

        var CloseBtnItem = new cc.MenuItemImage.create(
            s_btn_close_1,
            s_btn_close_2,
            function () {
                this.setVisible(false);
            },this);
        CloseBtnItem.setAnchorPoint(0.5, 0.5);
        CloseBtnItem.setScale(0.7);

        var menu = new cc.Menu.create(ChongZhiBtnItem,CloseBtnItem);
        menu.setPosition(0, 0);
        this.addChild(menu, 1);
        ChongZhiBtnItem.setPosition(size.width/2,size.height/2-68);
        CloseBtnItem.setPosition(size.width/2+119,size.height/2+83);
    }
});

var CMainFrameLayer = cc.Layer.extend({
    sprite:null,
    ConnectServerLabel:null,
    MyUserName:null,
    MyUserMoney:null,
    MyFangKuiBtnItem:null,
    MyMeiHuaBtnItem:null,
    MyHongTaoBtnItem:null,
    MyHeiTaoBtnItem:null,
    MyWangBtnItem:null,
    MyCardImage:null,
    MyTimerStateSprite:null,
    MyFangKuiPeiLv:null,
    MyMeiHuaPeiLv:null,
    MyHongTaoPeiLv:null,
    MyHeiTaoPeiLv:null,
    MyWangPeiLv:null,
	CurrentGameJvCount:null,
	CurrentGameLunCount:null,
    MyFangKuiSelfJettonTotal:null,
    MyFangKuaiAllJettonTotal:null,
    MyMeiHuaSelfJettonTotal:null,
    MyMeiHuaAllJettonTotal:null,
    MyHongTaoSelfJettonTotal:null,
    MyHongTaoAlljettonTotal:null,
    MyHeiTaoSelfJettonTotal:null,
    MyHeiTaoAllJettonTotal:null,
    MyWangSelfJettonTotal:null,
    MyWangAllJettonTotal:null,
    MySelfJettonTotalLable:null,
    MySelfResultTotalLable:null,
    MyColorCountFangkuai:null,
    MyColorCountMeiHua:null,
    MyColorCountHongTao:null,
    MyColorCountHeiTao:null,
    MyColorCountWang:null,
    MyBtnClearJetton:null,
    MyBtnContinueJetton:null,
    MyBtnMusicControl:null,
	MyBtnZhuanZhang:null,
	MyBtnChongZhi:null,

    init:function () {
        this._super();

        var size = cc.director.getWinSize();

        for(i=0;i<5;i++)
        {
            m_myselfJettonTotal[i] = 0;
            m_AllJettonTotal[i] = 0;
            m_tempcolorjettons[i] = 0;
        }

         // add "Helloworld" splash screen"
        this.sprite = new cc.Sprite(s_bgsprite);
        this.sprite.setAnchorPoint(0.5, 0.5);
        this.sprite.setPosition(size.width / 2, size.height / 2);
        //this.sprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(this.sprite, 0);

        this.MyCardImage = new cc.Sprite(s_01);
        this.MyCardImage.setAnchorPoint(0.5, 0.5);
        this.MyCardImage.setPosition(size.width / 2-95, size.height / 2+100);
        var pSpriteIndex = Math.floor(Math.random()*10);
        this.MyCardImage.setTextureRect(cc.rect(pSpriteIndex*264,0,264,369));
        this.MyCardImage.setScaleX(0.45);
        this.MyCardImage.setScaleY(0.5);
        this.addChild(this.MyCardImage);

        m_spritetimerbg = new cc.Sprite(s_TIMER);
        m_spritetimerbg.setAnchorPoint(0.5, 0.5);
        m_spritetimerbg.setPosition(size.width / 2+130, size.height / 2-83);
        m_spritetimerbg.setScale(0.8);
        this.addChild(m_spritetimerbg);
        m_spritetimerbg.setVisible(false);

        m_spriteTimer1 = new cc.Sprite(s_sdnumber);
        m_spriteTimer1.setAnchorPoint(0.5, 0.5);
        m_spriteTimer1.setPosition(size.width / 2+123, size.height / 2-80);
        m_spriteTimer1.setTextureRect(cc.rect(0,0,15,15));
        m_spriteTimer1.setScale(0.9);
        this.addChild(m_spriteTimer1);
        m_spriteTimer1.setVisible(false);

        m_spriteTimer2 = new cc.Sprite(s_sdnumber);
        m_spriteTimer2.setAnchorPoint(0.5, 0.5);
        m_spriteTimer2.setPosition(size.width / 2+137, size.height / 2-80);
        m_spriteTimer2.setTextureRect(cc.rect(0,0,15,15));
        m_spriteTimer2.setScale(0.9);
        this.addChild(m_spriteTimer2);
        m_spriteTimer2.setVisible(false);

        this.MyTimerStateSprite = new cc.Sprite(s_timebg);
        this.MyTimerStateSprite.setAnchorPoint(0.5, 0.5);
        this.MyTimerStateSprite.setPosition(size.width / 2+131, size.height / 2-107);
        this.MyTimerStateSprite.setTextureRect(cc.rect(0,0,111,31));
        this.MyTimerStateSprite.setScale(0.6);
        this.addChild(this.MyTimerStateSprite);
        this.MyTimerStateSprite.setVisible(false);

        var index=0;
        for(i=0;i<13;i++) {
            for (k = 0; k < 5; k++) {
                m_gamerecordsprites[index] = new cc.Sprite(s_gamerecord);
                m_gamerecordsprites[index].setAnchorPoint(0.5, 0.5);
                m_gamerecordsprites[index].setPosition(size.width / 2 - 16 + k*37, size.height / 2 + 197 - i * 16.6);
                m_gamerecordsprites[index].setTextureRect(cc.rect(0, 0, 94, 22));
                m_gamerecordsprites[index].setScale(0.39);
                this.addChild(m_gamerecordsprites[index]);
                m_gamerecordsprites[index].setVisible(false);

                index += 1;
            }
        }

        this.ConnectServerLabel = new cc.LabelTTF.create("正在连接服务器，请稍后...", "黑体", 18);
        // position the label on the center of the screen
        this.ConnectServerLabel.setPosition(size.width / 2, size.height/2+10);
        this.ConnectServerLabel.setColor(cc.color(255,0,0));
        // add the label as a child to this layer
        this.addChild(this.ConnectServerLabel);

        this.MyUserName = new cc.LabelTTF.create("bsq219", "黑体", 14);
        // position the label on the center of the screen
        this.MyUserName.setPosition(size.width / 2-106, size.height/2-228);
        this.MyUserName.setColor(cc.color(255,255,255));
        // add the label as a child to this layer
        this.addChild(this.MyUserName);
        this.MyUserName.setVisible(false);

        this.MyUserMoney = new cc.LabelTTF.create("23434", "黑体", 14);
        // position the label on the center of the screen
        this.MyUserMoney.setPosition(size.width / 2-19, size.height/2-228);
        this.MyUserMoney.setColor(cc.color(255,255,255));
        // add the label as a child to this layer
        this.addChild(this.MyUserMoney);
        this.MyUserMoney.setVisible(false);

       /* this.MyUserMoney = new cc.LabelTTF.create("", "黑体", 17);
        // position the label on the center of the screen
        this.MyUserMoney.setPosition(size.width / 2+87, size.height/2-227);
        this.MyUserMoney.setColor(cc.color(255,255,255));
        // add the label as a child to this layer
        this.addChild(this.MyUserMoney);
        this.MyUserMoney.setVisible(false);*/

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        this.MyColorCountFangkuai = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyColorCountFangkuai.setPosition(size.width / 2-109, size.height/2-39);
        this.MyColorCountFangkuai.setColor(cc.color(255,255,255));
        this.MyColorCountFangkuai.setAnchorPoint(0.5, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyColorCountFangkuai);

        this.MyColorCountMeiHua = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyColorCountMeiHua.setPosition(size.width / 2-51, size.height/2-39);
        this.MyColorCountMeiHua.setColor(cc.color(255,255,255));
        this.MyColorCountMeiHua.setAnchorPoint(0.5, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyColorCountMeiHua);

        this.MyColorCountHongTao = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyColorCountHongTao.setPosition(size.width / 2+6, size.height/2-39);
        this.MyColorCountHongTao.setColor(cc.color(255,255,255));
        this.MyColorCountHongTao.setAnchorPoint(0.5, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyColorCountHongTao);

        this.MyColorCountHeiTao = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyColorCountHeiTao.setPosition(size.width / 2+63, size.height/2-39);
        this.MyColorCountHeiTao.setColor(cc.color(255,255,255));
        this.MyColorCountHeiTao.setAnchorPoint(0.5, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyColorCountHeiTao);

        this.MyColorCountWang = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyColorCountWang.setPosition(size.width / 2+124, size.height/2-39);
        this.MyColorCountWang.setColor(cc.color(255,255,255));
        this.MyColorCountWang.setAnchorPoint(0.5, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyColorCountWang);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        this.MySelfJettonTotalLable = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MySelfJettonTotalLable.setPosition(size.width / 2, size.height/2-69);
        this.MySelfJettonTotalLable.setColor(cc.color(255,255,255));
        this.MySelfJettonTotalLable.setAnchorPoint(0, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MySelfJettonTotalLable);

        this.MySelfResultTotalLable = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MySelfResultTotalLable.setPosition(size.width / 2+65, size.height/2-68);
        this.MySelfResultTotalLable.setColor(cc.color(255,255,255));
        this.MySelfResultTotalLable.setAnchorPoint(0, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MySelfResultTotalLable);

        /***************************************************************/
        this.MyFangKuiSelfJettonTotal = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyFangKuiSelfJettonTotal.setPosition(size.width / 2-100, size.height/2-110);
        this.MyFangKuiSelfJettonTotal.setColor(cc.color(255,255,0));
        this.MyFangKuiSelfJettonTotal.setAnchorPoint(1, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyFangKuiSelfJettonTotal);

        this.MyFangKuaiAllJettonTotal = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyFangKuaiAllJettonTotal.setPosition(size.width / 2-100, size.height/2-90);
        this.MyFangKuaiAllJettonTotal.setColor(cc.color(255,255,0));
        this.MyFangKuaiAllJettonTotal.setAnchorPoint(1, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyFangKuaiAllJettonTotal);

        this.MyMeiHuaSelfJettonTotal = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyMeiHuaSelfJettonTotal.setPosition(size.width / 2-50, size.height/2-110);
        this.MyMeiHuaSelfJettonTotal.setColor(cc.color(255,255,0));
        this.MyMeiHuaSelfJettonTotal.setAnchorPoint(1, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyMeiHuaSelfJettonTotal);

        this.MyMeiHuaAllJettonTotal = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyMeiHuaAllJettonTotal.setPosition(size.width / 2-50, size.height/2-90);
        this.MyMeiHuaAllJettonTotal.setColor(cc.color(255,255,0));
        this.MyMeiHuaAllJettonTotal.setAnchorPoint(1, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyMeiHuaAllJettonTotal);

        this.MyHongTaoSelfJettonTotal = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyHongTaoSelfJettonTotal.setPosition(size.width / 2, size.height/2-110);
        this.MyHongTaoSelfJettonTotal.setColor(cc.color(255,255,0));
        this.MyHongTaoSelfJettonTotal.setAnchorPoint(1, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyHongTaoSelfJettonTotal);

        this.MyHongTaoAlljettonTotal = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyHongTaoAlljettonTotal.setPosition(size.width / 2, size.height/2-90);
        this.MyHongTaoAlljettonTotal.setColor(cc.color(255,255,0));
        this.MyHongTaoAlljettonTotal.setAnchorPoint(1, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyHongTaoAlljettonTotal);

        this.MyHeiTaoSelfJettonTotal = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyHeiTaoSelfJettonTotal.setPosition(size.width / 2+50, size.height/2-110);
        this.MyHeiTaoSelfJettonTotal.setColor(cc.color(255,255,0));
        this.MyHeiTaoSelfJettonTotal.setAnchorPoint(1, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyHeiTaoSelfJettonTotal);

        this.MyHeiTaoAllJettonTotal = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyHeiTaoAllJettonTotal.setPosition(size.width / 2+50, size.height/2-90);
        this.MyHeiTaoAllJettonTotal.setColor(cc.color(255,255,0));
        this.MyHeiTaoAllJettonTotal.setAnchorPoint(1, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyHeiTaoAllJettonTotal);

        this.MyWangSelfJettonTotal = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyWangSelfJettonTotal.setPosition(size.width / 2+100, size.height/2-110);
        this.MyWangSelfJettonTotal.setColor(cc.color(255,255,0));
        this.MyWangSelfJettonTotal.setAnchorPoint(1, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyWangSelfJettonTotal);

        this.MyWangAllJettonTotal = new cc.LabelTTF.create("0", "黑体", 15);
        // position the label on the center of the screen
        this.MyWangAllJettonTotal.setPosition(size.width / 2+100, size.height/2-90);
        this.MyWangAllJettonTotal.setColor(cc.color(255,255,0));
        this.MyWangAllJettonTotal.setAnchorPoint(1, 0.5);
        // add the label as a child to this layer
        this.addChild(this.MyWangAllJettonTotal);
        /***************************************************************/

        this.MyFangKuiPeiLv = new cc.LabelTTF.create("3.7", "黑体", 15);
        // position the label on the center of the screen
        this.MyFangKuiPeiLv.setPosition(size.width / 2-124, size.height/2-200);
        this.MyFangKuiPeiLv.setColor(cc.color(255,255,0));
        // add the label as a child to this layer
        this.addChild(this.MyFangKuiPeiLv);
        this.MyFangKuiPeiLv.setVisible(false);

        this.MyMeiHuaPeiLv = new cc.LabelTTF.create("3.7", "黑体", 15);
        // position the label on the center of the screen
        this.MyMeiHuaPeiLv.setPosition(size.width / 2-68, size.height/2-200);
        this.MyMeiHuaPeiLv.setColor(cc.color(255,255,0));
        // add the label as a child to this layer
        this.addChild(this.MyMeiHuaPeiLv);
        this.MyMeiHuaPeiLv.setVisible(false);

        this.MyHongTaoPeiLv = new cc.LabelTTF.create("4.0", "黑体", 15);
        // position the label on the center of the screen
        this.MyHongTaoPeiLv.setPosition(size.width / 2-15, size.height/2-200);
        this.MyHongTaoPeiLv.setColor(cc.color(255,255,0));
        // add the label as a child to this layer
        this.addChild(this.MyHongTaoPeiLv);
        this.MyHongTaoPeiLv.setVisible(false);

        this.MyHeiTaoPeiLv = new cc.LabelTTF.create("4.0", "黑体", 15);
        // position the label on the center of the screen
        this.MyHeiTaoPeiLv.setPosition(size.width / 2+38, size.height/2-200);
        this.MyHeiTaoPeiLv.setColor(cc.color(255,255,0));
        // add the label as a child to this layer
        this.addChild(this.MyHeiTaoPeiLv);
        this.MyHeiTaoPeiLv.setVisible(false);

        this.MyWangPeiLv = new cc.LabelTTF.create("20", "黑体", 15);
        // position the label on the center of the screen
        this.MyWangPeiLv.setPosition(size.width / 2+94, size.height/2-200);
        this.MyWangPeiLv.setColor(cc.color(255,255,0));
        // add the label as a child to this layer
        this.addChild(this.MyWangPeiLv);
        this.MyWangPeiLv.setVisible(false);
		
        this.CurrentGameJvCount = new cc.LabelTTF.create("第 1 局", "黑体", 16);
        // position the label on the center of the screen
        this.CurrentGameJvCount.setPosition(size.width / 2+46, size.height/2+220);
        this.CurrentGameJvCount.setColor(cc.color(255,255,0));
        // add the label as a child to this layer
        this.addChild(this.CurrentGameJvCount);	

        this.CurrentGameLunCount = new cc.LabelTTF.create("第 1 轮", "黑体", 16);
        // position the label on the center of the screen
        this.CurrentGameLunCount.setPosition(size.width / 2-96, size.height/2+205);
        this.CurrentGameLunCount.setColor(cc.color(255,255,255));
        // add the label as a child to this layer
        this.addChild(this.CurrentGameLunCount);			

        this.MyBtnMusicControl = new cc.MenuItemImage.create(
            s_music_2,
            s_music_2,
            s_music_2,
            function () {

                m_isplaymusic = !m_isplaymusic;

                if(m_isplaymusic) {
                    this.MyBtnMusicControl.setNormalSpriteFrame(s_music_2);
                    this.MyBtnMusicControl.setSelectedSpriteFrame(s_music_2);
                    this.MyBtnMusicControl.setDisabledSpriteFrame(s_music_2);

                    //cc.audioEngine.playMusic(soud_01, true);
                }
                else{
                    this.MyBtnMusicControl.setNormalSpriteFrame(s_music_1);
                    this.MyBtnMusicControl.setSelectedSpriteFrame(s_music_1);
                    this.MyBtnMusicControl.setDisabledSpriteFrame(s_music_1);

                    cc.audioEngine.pauseAllEffects();
                    cc.audioEngine.pauseMusic();
                }

                if(m_isplaymusic) {
                    //cc.audioEngine.playEffect(sound_GamePushAnte, false);
                }
            },this);
        this.MyBtnMusicControl.setScale(0.6);

        this.MyFangKuiBtnItem = new cc.MenuItemImage.create(
            s_btnfangkuai1,
            s_btnfangkuai2,
            s_btnfangkuai3,
            function () {
                if(m_myselftempusermoney - gamepielement >= 0) {
                    m_myselftempusermoney -= gamepielement;
                    this.MyUserMoney.setString(m_myselftempusermoney);

                    var row1 = {};
                    var jetton = [];
                    row1.MsgId = 1000;
                    row1.MsgSubId = 1005;
                    jetton[0] = gamepielement;
                    row1.jettonmoney = jetton;
                    gameserversocket.send(JSON.stringify(row1));

                    m_myselfJettonTotal[0] += gamepielement;
                    m_AllJettonTotal[0] += gamepielement;
                    m_myselftotaljettons += gamepielement;
                    this.MySelfJettonTotalLable.setString(m_myselftotaljettons);
                    this.MyFangKuiSelfJettonTotal.setString(m_myselfJettonTotal[0]);
                    this.MyFangKuaiAllJettonTotal.setString(m_AllJettonTotal[0]);

                    this.MyBtnClearJetton.setEnabled(true);
                    this.MyBtnContinueJetton.setEnabled(false);

                    if(m_isClearConntineData == false){
                        m_isClearConntineData = true;

                        for(i=0;i<5;i++){
                            m_tempcolorjettons[i] = 0;
                        }
                    }

                    m_tempcolorjettons[0] += gamepielement;
                    m_isCurrentJetton = true;

                    if(m_isplaymusic) {
                        //cc.audioEngine.playEffect(sound_GamePushAnte, false);
                    }
                }
            },this);
        this.MyFangKuiBtnItem.setAnchorPoint(0.5, 0.5);
        this.MyFangKuiBtnItem.setScale(0.5);
        this.MyFangKuiBtnItem.setEnabled(false);

        this.MyMeiHuaBtnItem = new cc.MenuItemImage.create(
            s_btnmeihua1,
            s_btnmeihua2,
            s_btnmeihua3,
            function () {
                if(m_myselftempusermoney - gamepielement >= 0) {
                    m_myselftempusermoney -= gamepielement;
                    this.MyUserMoney.setString(m_myselftempusermoney);

                    var row1 = {};
                    var jetton = [];
                    row1.MsgId = 1000;
                    row1.MsgSubId = 1005;
                    jetton[1] = gamepielement;
                    row1.jettonmoney = jetton;
                    gameserversocket.send(JSON.stringify(row1));

                    m_myselfJettonTotal[1] += gamepielement;
                    m_AllJettonTotal[1] += gamepielement;
                    m_myselftotaljettons += gamepielement;
                    this.MySelfJettonTotalLable.setString(m_myselftotaljettons);
                    this.MyMeiHuaSelfJettonTotal.setString(m_myselfJettonTotal[1]);
                    this.MyMeiHuaAllJettonTotal.setString(m_AllJettonTotal[1]);

                    this.MyBtnClearJetton.setEnabled(true);
                    this.MyBtnContinueJetton.setEnabled(false);

                    if(m_isClearConntineData == false){
                        m_isClearConntineData = true;

                        for(i=0;i<5;i++){
                            m_tempcolorjettons[i] = 0;
                        }
                    }

                    m_tempcolorjettons[1] += gamepielement;
                    m_isCurrentJetton = true;

                    if(m_isplaymusic) {
                       // cc.audioEngine.playEffect(sound_GamePushAnte, false);
                    }
                }
            },this);
        this.MyMeiHuaBtnItem.setAnchorPoint(0.5, 0.5);
        this.MyMeiHuaBtnItem.setScale(0.5);
        this.MyMeiHuaBtnItem.setEnabled(false);

        this.MyHongTaoBtnItem = new cc.MenuItemImage.create(
            s_btnhongtao1,
            s_btnhongtao2,
            s_btnhongtao3,
            function () {
                if(m_myselftempusermoney - gamepielement >= 0) {
                    m_myselftempusermoney -= gamepielement;
                    this.MyUserMoney.setString(m_myselftempusermoney);

                    var row1 = {};
                    var jetton = [];
                    row1.MsgId = 1000;
                    row1.MsgSubId = 1005;
                    jetton[2] = gamepielement;
                    row1.jettonmoney = jetton;
                    gameserversocket.send(JSON.stringify(row1));

                    m_myselfJettonTotal[2] += gamepielement;
                    m_AllJettonTotal[2] += gamepielement;
                    m_myselftotaljettons += gamepielement;
                    this.MySelfJettonTotalLable.setString(m_myselftotaljettons);
                    this.MyHongTaoSelfJettonTotal.setString(m_myselfJettonTotal[2]);
                    this.MyHongTaoAlljettonTotal.setString(m_AllJettonTotal[2]);

                    this.MyBtnClearJetton.setEnabled(true);
                    this.MyBtnContinueJetton.setEnabled(false);

                    if(m_isClearConntineData == false){
                        m_isClearConntineData = true;

                        for(i=0;i<5;i++){
                            m_tempcolorjettons[i] = 0;
                        }
                    }

                    m_tempcolorjettons[2] += gamepielement;
                    m_isCurrentJetton = true;

                    if(m_isplaymusic) {
                       // cc.audioEngine.playEffect(sound_GamePushAnte, false);
                    }
                }
            },this);
        this.MyHongTaoBtnItem.setAnchorPoint(0.5, 0.5);
        this.MyHongTaoBtnItem.setScale(0.5);
        this.MyHongTaoBtnItem.setEnabled(false);

        this.MyHeiTaoBtnItem = new cc.MenuItemImage.create(
            s_btnheitao1,
            s_btnheitao2,
            s_btnheitao3,
            function () {
                if(m_myselftempusermoney - gamepielement >= 0) {
                    m_myselftempusermoney -= gamepielement;
                    this.MyUserMoney.setString(m_myselftempusermoney);

                    var row1 = {};
                    var jetton = [];
                    row1.MsgId = 1000;
                    row1.MsgSubId = 1005;
                    jetton[3] = gamepielement;
                    row1.jettonmoney = jetton;
                    gameserversocket.send(JSON.stringify(row1));

                    m_myselfJettonTotal[3] += gamepielement;
                    m_AllJettonTotal[3] += gamepielement;
                    m_myselftotaljettons += gamepielement;
                    this.MySelfJettonTotalLable.setString(m_myselftotaljettons);
                    this.MyHeiTaoSelfJettonTotal.setString(m_myselfJettonTotal[3]);
                    this.MyHeiTaoAllJettonTotal.setString(m_AllJettonTotal[3]);

                    this.MyBtnClearJetton.setEnabled(true);
                    this.MyBtnContinueJetton.setEnabled(false);

                    if(m_isClearConntineData == false){
                        m_isClearConntineData = true;

                        for(i=0;i<5;i++){
                            m_tempcolorjettons[i] = 0;
                        }
                    }

                    m_tempcolorjettons[3] += gamepielement;
                    m_isCurrentJetton = true;

                    if(m_isplaymusic) {
                      //  cc.audioEngine.playEffect(sound_GamePushAnte, false);
                    }
                }
            },this);
        this.MyHeiTaoBtnItem.setAnchorPoint(0.5, 0.5);
        this.MyHeiTaoBtnItem.setScale(0.5);
        this.MyHeiTaoBtnItem.setEnabled(false);

        this.MyWangBtnItem = new cc.MenuItemImage.create(
            s_btndaxiaowang1,
            s_btndaxiaowang2,
            s_btndaxiaowang3,
            function () {
                if(m_myselftempusermoney - gamepielement >= 0) {
                    m_myselftempusermoney -= gamepielement;
                    this.MyUserMoney.setString(m_myselftempusermoney);

                    var row1 = {};
                    var jetton = [];
                    row1.MsgId = 1000;
                    row1.MsgSubId = 1005;
                    jetton[4] = gamepielement;
                    row1.jettonmoney = jetton;
                    gameserversocket.send(JSON.stringify(row1));

                    m_myselfJettonTotal[4] += gamepielement;
                    m_AllJettonTotal[4] += gamepielement;
                    m_myselftotaljettons += gamepielement;
                    this.MySelfJettonTotalLable.setString(m_myselftotaljettons);
                    this.MyWangSelfJettonTotal.setString(m_myselfJettonTotal[4]);
                    this.MyWangAllJettonTotal.setString(m_AllJettonTotal[4]);

                    this.MyBtnClearJetton.setEnabled(true);
                    this.MyBtnContinueJetton.setEnabled(false);

                    if(m_isClearConntineData == false){
                        m_isClearConntineData = true;

                        for(i=0;i<5;i++){
                            m_tempcolorjettons[i] = 0;
                        }
                    }

                    m_tempcolorjettons[4] += gamepielement;
                    m_isCurrentJetton = true;

                    if(m_isplaymusic) {
                      //  cc.audioEngine.playEffect(sound_GamePushAnte, false);
                    }
                }
            },this);
        this.MyWangBtnItem.setAnchorPoint(0.5, 0.5);
        this.MyWangBtnItem.setScale(0.5);
        this.MyWangBtnItem.setEnabled(false);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        this.MyBtnClearJetton = new cc.MenuItemImage.create(
            s_btnclear1,
            s_btnclear2,
            s_btnclear3,
            function () {
                this.MyBtnClearJetton.setEnabled(false);

                var row1 = {};
                var jetton = [];
                row1.MsgId = 1000;
                row1.MsgSubId = 1006;
                jetton = m_tempcolorjettons;
                row1.jettons = jetton;
                gameserversocket.send(JSON.stringify(row1));

                var totalJettons = 0;
                for(i=0;i<5;i++)
                {
                    m_myselfJettonTotal[i] -= m_tempcolorjettons[i];
                    m_AllJettonTotal[i] -= m_tempcolorjettons[i];

                    totalJettons += m_tempcolorjettons[i];
                }

                m_myselftempusermoney += totalJettons;
                this.MyUserMoney.setString(m_myselftempusermoney);

                m_myselftotaljettons -= totalJettons;
                this.MySelfJettonTotalLable.setString(m_myselftotaljettons);

                this.MyFangKuiSelfJettonTotal.setString(m_myselfJettonTotal[0]);
                this.MyFangKuaiAllJettonTotal.setString(m_AllJettonTotal[0]);
                this.MyMeiHuaSelfJettonTotal.setString(m_myselfJettonTotal[1]);
                this.MyMeiHuaAllJettonTotal.setString(m_AllJettonTotal[1]);
                this.MyHongTaoSelfJettonTotal.setString(m_myselfJettonTotal[2]);
                this.MyHongTaoAlljettonTotal.setString(m_AllJettonTotal[2]);
                this.MyHeiTaoSelfJettonTotal.setString(m_myselfJettonTotal[3]);
                this.MyHeiTaoAllJettonTotal.setString(m_AllJettonTotal[3]);
                this.MyWangSelfJettonTotal.setString(m_myselfJettonTotal[4]);
                this.MyWangAllJettonTotal.setString(m_AllJettonTotal[4]);

                for(i=0;i<5;i++) {
                    m_tempcolorjettons[i] = 0;
                }

                /*if(m_isplaymusic) {
                    cc.audioEngine.playEffect(sound_Ante, false);
                }*/
            },this);
        this.MyBtnClearJetton.setAnchorPoint(0.5, 0.5);
        this.MyBtnClearJetton.setScale(0.55);
        this.MyBtnClearJetton.setEnabled(false);

        this.MyBtnContinueJetton = new cc.MenuItemImage.create(
            s_btncontinue1,
            s_btncontinue2,
            s_btncontinue3,
            function () {
                this.MyFangKuiBtnItem.setEnabled(false);
                this.MyMeiHuaBtnItem.setEnabled(false);
                this.MyHongTaoBtnItem.setEnabled(false);
                this.MyHeiTaoBtnItem.setEnabled(false);
                this.MyWangBtnItem.setEnabled(false);
                this.MyBtnContinueJetton.setEnabled(false);
                this.MyBtnClearJetton.setEnabled(false);

                var totalJettons = 0;
                for(i=0;i<5;i++)
                {
                    totalJettons += m_tempcolorjettons[i];
                }

                if(m_myselftempusermoney - totalJettons >= 0) {
                    m_myselftempusermoney -= totalJettons;
                    this.MyUserMoney.setString(m_myselftempusermoney);

                    var row1 = {};
                    var jetton = [];
                    row1.MsgId = 1000;
                    row1.MsgSubId = 1005;
                    jetton = m_tempcolorjettons;
                    row1.jettonmoney = jetton;
                    gameserversocket.send(JSON.stringify(row1));

                    for (i = 0; i < 5; i++) {
                        m_myselfJettonTotal[i] += m_tempcolorjettons[i];
                        m_AllJettonTotal[i] += m_tempcolorjettons[i];
                    }

                    m_myselftotaljettons += totalJettons;
                    this.MySelfJettonTotalLable.setString(m_myselftotaljettons);

                    this.MyFangKuiSelfJettonTotal.setString(m_myselfJettonTotal[0]);
                    this.MyFangKuaiAllJettonTotal.setString(m_AllJettonTotal[0]);
                    this.MyMeiHuaSelfJettonTotal.setString(m_myselfJettonTotal[1]);
                    this.MyMeiHuaAllJettonTotal.setString(m_AllJettonTotal[1]);
                    this.MyHongTaoSelfJettonTotal.setString(m_myselfJettonTotal[2]);
                    this.MyHongTaoAlljettonTotal.setString(m_AllJettonTotal[2]);
                    this.MyHeiTaoSelfJettonTotal.setString(m_myselfJettonTotal[3]);
                    this.MyHeiTaoAllJettonTotal.setString(m_AllJettonTotal[3]);
                    this.MyWangSelfJettonTotal.setString(m_myselfJettonTotal[4]);
                    this.MyWangAllJettonTotal.setString(m_AllJettonTotal[4]);

                    m_isCurrentJetton = true;

                    if(m_isplaymusic) {
                       // cc.audioEngine.playEffect(sound_GamePushAnte2, false);
                    }
                }
            },this);
        this.MyBtnContinueJetton.setAnchorPoint(0.5, 0.5);
        this.MyBtnContinueJetton.setScale(0.55);
        this.MyBtnContinueJetton.setEnabled(false);
		
        this.MyBtnZhuanZhang = new cc.MenuItemImage.create(
            s_btn_zhuanzhang_1,
            s_btn_zhuanzhang_2,
            s_btn_zhuanzhang_2,
            function () {
				m_CZhuanZhangLayer.MyUserMoney.setString(m_myselftempusermoney);
				m_CZhuanZhangLayer._boxzhuanzhangmoney.setString("");
				m_CZhuanZhangLayer.setVisible(true);					
            },this);
        this.MyBtnZhuanZhang.setAnchorPoint(0.5, 0.5);
        this.MyBtnZhuanZhang.setScale(0.5);
		
        this.MyBtnChongZhi = new cc.MenuItemImage.create(
            s_btn_chongzhi_1,
            s_btn_chongzhi_2,
            s_btn_chongzhi_2,
            function () {
				m_chongzhiLayer._boxchongzhimoney.setString("");
				m_chongzhiLayer.setVisible(true);
            },this);
        this.MyBtnChongZhi.setAnchorPoint(0.5, 0.5);
        this.MyBtnChongZhi.setScale(0.5);		
		
		//this.MyBtnZhuanZhang.setVisible(false);
		//this.MyBtnChongZhi.setVisible(false);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        var menu = new cc.Menu.create(this.MyFangKuiBtnItem,this.MyMeiHuaBtnItem,this.MyHongTaoBtnItem,this.MyHeiTaoBtnItem,
            this.MyWangBtnItem,this.MyBtnClearJetton,this.MyBtnContinueJetton,this.MyBtnMusicControl,this.MyBtnZhuanZhang,this.MyBtnChongZhi);
        menu.setPosition(0, 0);
        this.addChild(menu, 1);
        this.MyFangKuiBtnItem.setPosition(size.width/2-125,size.height/2-160);
        this.MyMeiHuaBtnItem.setPosition(size.width/2-73,size.height/2-160);
        this.MyHongTaoBtnItem.setPosition(size.width/2-21,size.height/2-160);
        this.MyHeiTaoBtnItem.setPosition(size.width/2+32,size.height/2-160);
        this.MyWangBtnItem.setPosition(size.width/2+85,size.height/2-160);
        this.MyBtnClearJetton.setPosition(size.width/2+128,size.height/2-145);
        this.MyBtnContinueJetton.setPosition(size.width/2+135,size.height/2-171);
        this.MyBtnMusicControl.setPosition(size.width/2+140,size.height/2-228);
		this.MyBtnZhuanZhang.setPosition(size.width/2+105,size.height/2-228);
		this.MyBtnChongZhi.setPosition(size.width/2+75,size.height/2-228);

        var self = this;

        try{
            socket = new WebSocket(host);

            socket.onopen = function(){
                socket.send('100');

                last_health = new Date();
                clearInterval(keepalivetimer);
                keepalivetimer = setInterval( function(){keepalive(socket)},1000);

                if(m_isLoginSuccess == true)
                {
                    var row1 = {};
                    row1.MsgId = 500;
                    row1.UserName = m_userloginname;
                    row1.UserPW = m_userloginpassword;
                    row1.DeviceType=1;
                    socket.send(JSON.stringify(row1));
                }
                else
                {
                    self.ConnectServerLabel.removeFromParentAndCleanup(true);

                    MyMainLoginlayer = new CLoginLayer();
                    self.addChild(MyMainLoginlayer,5);
                    MyMainLoginlayer.init();

                    m_registerLayer = new CRegisterLayer();
                    self.addChild(m_registerLayer,7);
                    m_registerLayer.init();
                    m_registerLayer.setVisible(false);

                    m_chongzhiLayer = new CChongZhiLayer();
                    self.addChild(m_chongzhiLayer,6);
                    m_chongzhiLayer.init();
                    m_chongzhiLayer.setVisible(false);

                    m_CZhuanZhangLayer = new CZhuanZhangLayer();
                    self.addChild(m_CZhuanZhangLayer,8);
                    m_CZhuanZhangLayer.init();
                    m_CZhuanZhangLayer.setVisible(false);
                }
            }

            socket.onmessage = function(msg){
                var str = "";
                str = msg.data;

                console.info(str);

                var obj = eval('(' + str + ')');

                switch(obj.MsgId)
                {
                    case 300:
                    {
                        if(obj.MsgSubId == 301)
                        {

                        }
                    }
                        break;
					case 404:
					{
                        switch(obj.MsgSubId)
                        {
                            case 433:
                            {
                                var loginlayer = new MyMessageBoxLayer();
                                self.addChild(loginlayer,8);
                                loginlayer.init("充值失败，请检查充值卡号！");					
							}
								break;
							case 409:
							{
                                var loginlayer = new MyMessageBoxLayer();
                                self.addChild(loginlayer,8);
                                loginlayer.init("转账失败，请稍后再试！");	
							}
								break;
							case 432:
							{
                                var pusermoney = obj.usermoney;
								
								if(pusermoney > m_myselfusermoney)
								{	
									var pmoney = pusermoney - m_myselfusermoney;
									m_myselftempusermoney = m_myselftempusermoney + pmoney;
									m_myselfusermoney = pusermoney;
								}
								
                                self.MyUserMoney.setString(m_myselftempusermoney);
								//m_chongzhiLayer.setVisible(false);
							}
								break;
							case 416:
							{
                                var pusermoney = obj.UserMoney;
								
								if(pusermoney < m_myselfusermoney)
								{	
									var pmoney = m_myselfusermoney - pusermoney;
									m_myselftempusermoney = m_myselftempusermoney - pmoney;
									m_myselfusermoney = pusermoney;
								}
								
                                self.MyUserMoney.setString(m_myselftempusermoney);
								//m_CZhuanZhangLayer.setVisible(false);
							}
								break;
							default:
								break;
						}
					}
						break;
                    case 400:
                    {
                        switch(obj.MsgSubId)
                        {
                            case 402:
                            {
                                var loginlayer = new MyMessageBoxLayer();
                                self.addChild(loginlayer,8);
                                loginlayer.init("用户登录失败，请检查用户名和密码！");
                            }
                                break;
                            case 415:
                            {
                                var loginlayer = new MyMessageBoxLayer();
                                self.addChild(loginlayer,8);
                                loginlayer.init("此账号已被封，请联系客服人员！");
                            }
                                break;
                            case 504:
                            {
                                var loginlayer = new MyMessageBoxLayer();
                                self.addChild(loginlayer,8);
                                loginlayer.init("此账号已经在服务器中，不能重复登录！");
                            }
                                break;
                            case 418:
                            {
                                var loginlayer = new MyMessageBoxLayer();
                                self.addChild(loginlayer,8);
                                loginlayer.init("此账号已被玩家锁机，请联系客服人员！");
                            }
                                break;
                            case 401:
                            {
                                isLoginSuccuss = true;

                                myselfUserId = obj.UserId;
                                m_myselfusermoney = obj.money;
                                m_myselftempusermoney = m_myselfusermoney;

                                self.MyUserName.setString(m_userloginname);
                                self.MyUserMoney.setString(m_myselfusermoney);
                                //MyMainLoginlayer.removeAllChildrenWithCleanup(true);
                                MyMainLoginlayer.removeFromParent();

                                self.MyUserName.setVisible(true);
                                self.MyUserMoney.setVisible(true);

                                //获取服务器列表
                                var row1 = {};
                                row1.MsgId = 800;
                                socket.send(JSON.stringify(row1));

                               // if(m_isplaymusic) {
                               //     cc.audioEngine.playMusic(soud_01, true);
                               // }
                            }
                                break;
                            default:
                                break;
                        }
                    }
                        break;
                    case 700:
                    {
                        switch(obj.MsgSubId)
                        {
                            case 701:
                            {
                                var loginlayer = new MyMessageBoxLayer();
                                self.addChild(loginlayer,8);
                                loginlayer.init("注册成功！");

                                m_registerLayer.setVisible(false);
                                MyMainLoginlayer.setVisible(true);
                            }
                                break;
                            case 702:
                            {
                                var loginlayer = new MyMessageBoxLayer();
                                self.addChild(loginlayer,8);
                                loginlayer.init("注册失败，请检查您的用户名和密码！");
                            }
                                break;
                            default:
                                break;
                        }
                    }
                        break;
                    case 800:
                        {
                            var MsgSubId = obj.MsgSubId;

                            if(MsgSubId == 803)
                            {
                                var loginlayer = new MyMessageBoxLayer();
                                self.addChild(loginlayer,8);
                                loginlayer.init("获取服务器失败，请稍后再试！");
                            }
                            else
                            {
                                var RoomCount = obj.RoomCount;

                                console.info("start."+RoomCount);

                                m_isLoginSuccess = true;

                                m_gameserver = "ws://"+obj.Room[0].serverip+":"+obj.Room[0].serverport;

                                //socket.onclose();

                                console.info("start.");

                                gameserversocket = new WebSocket(m_gameserver);

                                gameserversocket.onopen = function(){
                                    gameserversocket.send('100');

                                    last_health_game = new Date();
                                    clearInterval(keepalivetimer_game);
                                    keepalivetimer_game = setInterval( function(){keepalive(gameserversocket,1)},1000);

                                    console.info("connned.");
                                }

                                gameserversocket.onmessage = function(msg) {
                                    var str = "";
                                    str = msg.data;

                                    //console.info(str);

                                    var objgame = eval('(' + str + ')');

                                    switch(objgame.MsgId)
                                    {
                                        case 300:
                                        {
                                            if(objgame.MsgSubId == 301)
                                            {
                                                var row1 = {};
                                                row1.MsgId = 500;
                                                row1.UserName = m_userloginname;
                                                row1.UserPW = m_userloginpassword;
                                                row1.DeviceType=1;
                                                gameserversocket.send(JSON.stringify(row1));
                                            }
                                        }
                                        break;
                                        case 500:
                                        {
                                            switch(objgame.MsgSubId)
                                            {
                                                case 502:
                                                {
                                                    var loginlayer = new MyMessageBoxLayer();
                                                    self.addChild(loginlayer,8);
                                                    loginlayer.init("游戏服务器登录失败，请联系客服人员！");
                                                }
                                                    break;
                                                case 505:
                                                {
                                                    var loginlayer = new MyMessageBoxLayer();
                                                    self.addChild(loginlayer,8);
                                                    loginlayer.init("游戏服务器满员，请联系客服人员！");
                                                }
                                                    break;
                                                case 504:
                                                {
                                                    var loginlayer = new MyMessageBoxLayer();
                                                    self.addChild(loginlayer,8);
                                                    loginlayer.init("您已经在游戏中2，请稍后再试！"+objgame.GameState);
                                                }
                                                    break;
                                                case 501:
                                                {
                                                    if(objgame.ID == myselfUserId){
                                                        var row1 = {};
                                                        row1.MsgId = 900;
                                                        row1.MsgSubId=901;
                                                        row1.RoomIndex = -1;
                                                        row1.ChairIndex = -1;
                                                        row1.EnterPWd = "";
                                                        row1.Enterfirst=0;
                                                        row1.Entersecond=0;
                                                        gameserversocket.send(JSON.stringify(row1));
                                                    }
                                                }
                                                    break;
                                                default:
                                                    break;
                                            }
                                        }
                                            break;
                                        case 900:
                                        {
                                            console.info(objgame.MsgSubId);

                                            if(objgame.MsgSubId == 901) {
                                                switch(objgame.MsgSubId2) {
                                                    case 920: {
                                                        var loginlayer = new MyMessageBoxLayer();
                                                        self.addChild(loginlayer,8);
                                                        loginlayer.init("您已经在游戏中3，请稍后再试！");
                                                    }
                                                    break;
                                                    case 912: {
                                                        var loginlayer = new MyMessageBoxLayer();
                                                        self.addChild(loginlayer,8);
                                                        loginlayer.init("进入房间失败，请稍后再试！");
                                                    }
                                                        break;
                                                    case 914: {
                                                        var loginlayer = new MyMessageBoxLayer();
                                                        self.addChild(loginlayer,8);
                                                        loginlayer.init("金币不够，请先充值！");
                                                    }
                                                        break;
                                                }
                                            }
                                        }
                                            break;
                                        case 1000:
                                        {
                                            console.info(objgame.MsgSubId);

                                            switch(objgame.MsgSubId)
                                            {
                                                case 1001:
                                                case 1007:
                                                {
                                                    gamestate = objgame.gamestate;
                                                    gamepielement = objgame.gamepielement;
                                                    m_GameItemBeiLv = objgame.GamePielement;
                                                    var pjvhao = objgame.jvindex + 1;
                                                    m_colorrecordcount = objgame.colorrecordcount;
                                                    m_cardrecourdcount = objgame.cardrecourdcount;
                                                    m_timexiazhu = objgame.timexiazhu;
                                                    m_timekaipai = objgame.timekaipai;
                                                    m_timejiesuan = objgame.timejiesuan;
                                                   // m_myselfgamegonggaostr = objgame.gamegonggao;

                                                    m_oldJvHao = pjvhao;

                                                    /*self.MyFangKuiPeiLv.setString(m_GameItemBeiLv[0]/10);
                                                    self.MyMeiHuaPeiLv.setString(m_GameItemBeiLv[1]/10);
                                                    self.MyHongTaoPeiLv.setString(m_GameItemBeiLv[2]/10);
                                                    self.MyHeiTaoPeiLv.setString(m_GameItemBeiLv[3]/10);
                                                    self.MyWangPeiLv.setString(m_GameItemBeiLv[4]/10);*/

                                                    var tmpJvCount = m_cardrecourdcount.length + 1;
                                                    var tmpJvString = "第 "+tmpJvCount+" 局";
                                                    self.CurrentGameJvCount.setString(tmpJvString);
                                                    tmpJvString = "第 "+pjvhao+" 轮";
                                                    self.CurrentGameLunCount.setString(tmpJvString);

                                                    self.MyFangKuiPeiLv.setVisible(true);
                                                    self.MyMeiHuaPeiLv.setVisible(true);
                                                    self.MyHongTaoPeiLv.setVisible(true);
                                                    self.MyHeiTaoPeiLv.setVisible(true);
                                                    self.MyWangPeiLv.setVisible(true);

                                                    self.MyColorCountFangkuai.setString(m_colorrecordcount[3]);
                                                    self.MyColorCountMeiHua.setString(m_colorrecordcount[2]);
                                                    self.MyColorCountHongTao.setString(m_colorrecordcount[1]);
                                                    self.MyColorCountHeiTao.setString(m_colorrecordcount[0]);
                                                    self.MyColorCountWang.setString(m_colorrecordcount[4]);

                                                    var index = 0;
                                                    for(i=0;i<m_cardrecourdcount.length;i++){
                                                        if(m_cardrecourdcount[i] == 78)
                                                        {
                                                            m_gamerecordsprites[index].setTextureRect(cc.rect(0,4*22,94,22));
                                                        }
                                                        else if(m_cardrecourdcount[i] == 79)
                                                        {
                                                            m_gamerecordsprites[index].setTextureRect(cc.rect(94,4*22,94,22));
                                                        }
                                                        else {
                                                            var resultcard = m_cardrecourdcount[i].toString(16);

                                                            var cardcolor = 3;
                                                            var cardnum = parseInt("0x"+resultcard[0])-1;

                                                            if(resultcard.length > 1) {
                                                                cardcolor = 3 - Number(resultcard[0]);
                                                                cardnum = parseInt("0x" + resultcard[1]) - 1;
                                                            }

                                                            m_gamerecordsprites[index].setTextureRect(cc.rect(cardnum * 94, cardcolor * 22, 94, 22));
                                                        }
                                                        m_gamerecordsprites[index].setVisible(true);
                                                        index += 1;
                                                    }

                                                    if(gamestate == 2) {
                                                        self.MyTimerStateSprite.setTextureRect(cc.rect(111*2,0,111,31));
                                                        self.MyTimerStateSprite.setVisible(true);
                                                        m_curTime = m_timekaipai;
                                                    }else if(gamestate == 1) {
                                                        self.MyTimerStateSprite.setTextureRect(cc.rect(111,0,111,31));
                                                        self.MyTimerStateSprite.setVisible(true);
                                                        m_curTime = m_timexiazhu;
                                                    }else {
                                                        m_curTime = m_timejiesuan;
                                                        self.MyTimerStateSprite.setTextureRect(cc.rect(0,0,111,31));
                                                        self.MyTimerStateSprite.setVisible(true);
                                                    }

                                                    clearInterval(m_curTimerId);
                                                    Timeralive(false);
                                                    m_curTimerId = setInterval( function(){Timeralive(true)},1000);

                                                    /*if(m_myselfgamegonggaostr.length > 0) {
                                                        var gamegonggaolayer = new MyMessageBoxLayer();
                                                        self.addChild(gamegonggaolayer, 8000);
                                                        gamegonggaolayer.init(m_myselfgamegonggaostr);
                                                    }*/
                                                }
                                                    break;
                                                case 1002:
                                                {
                                                    gamestate = objgame.gamestate;
                                                    var pjvhao = objgame.jvindex + 1;

                                                    for(i=0;i<5;i++)
                                                    {
                                                        m_myselfJettonTotal[i] = 0;
                                                        m_AllJettonTotal[i] = 0;
                                                    }

                                                    if(m_oldJvHao != pjvhao)
                                                    {
                                                        m_oldJvHao=pjvhao;
                                                        m_cardrecourdcount=[];

                                                        for(i=0;i<65;i++)
                                                        {
                                                            m_gamerecordsprites[i].setVisible(false);
                                                        }
                                                    }

                                                    self.MyFangKuiSelfJettonTotal.setString("0");
                                                    self.MyFangKuaiAllJettonTotal.setString("0");
                                                    self.MyMeiHuaSelfJettonTotal.setString("0");
                                                    self.MyMeiHuaAllJettonTotal.setString("0");
                                                    self.MyHongTaoSelfJettonTotal.setString("0");
                                                    self.MyHongTaoAlljettonTotal.setString("0");
                                                    self.MyHeiTaoSelfJettonTotal.setString("0");
                                                    self.MyHeiTaoAllJettonTotal.setString("0");
                                                    self.MyWangSelfJettonTotal.setString("0");
                                                    self.MyWangAllJettonTotal.setString("0");

                                                    var tmpJvCount = m_cardrecourdcount.length + 1;
                                                    var tmpJvString = "第 "+tmpJvCount+" 局";
                                                    self.CurrentGameJvCount.setString(tmpJvString);
                                                    tmpJvString = "第 "+pjvhao+" 轮";
                                                    self.CurrentGameLunCount.setString(tmpJvString);

                                                    if(isLoginSuccuss &&
                                                        gamepielement > 0 && m_myselfusermoney >= gamepielement)
                                                    {
                                                        self.MyFangKuiBtnItem.setEnabled(true);
                                                        self.MyMeiHuaBtnItem.setEnabled(true);
                                                        self.MyHongTaoBtnItem.setEnabled(true);
                                                        self.MyHeiTaoBtnItem.setEnabled(true);
                                                        self.MyWangBtnItem.setEnabled(true);
                                                    }

                                                    m_isClearConntineData=false;
                                                    m_isCurrentJetton = false;

                                                    if(m_tempcolorjettons[0] > 0 ||
                                                        m_tempcolorjettons[1] > 0 ||
                                                        m_tempcolorjettons[2] > 0 ||
                                                        m_tempcolorjettons[3] > 0 ||
                                                        m_tempcolorjettons[4] > 0) {
                                                        self.MyBtnContinueJetton.setEnabled(true);
                                                    }

                                                    self.MyTimerStateSprite.setTextureRect(cc.rect(111,0,111,31));
                                                    self.MyTimerStateSprite.setVisible(true);
                                                    m_curTime = m_timexiazhu;
                                                    clearInterval(m_curTimerId);
                                                    Timeralive(false);
                                                    m_curTimerId = setInterval( function(){Timeralive(true)},1000);

                                                    if(m_isplaymusic) {
                                                        //  cc.audioEngine.playEffect(sound_GameAnteBegin, false);
                                                    }
                                                }
                                                    break;
                                                case 1003:
                                                {
                                                    gamestate = objgame.gamestate;
                                                    var resultcardnum = objgame.resultcard;

                                                    self.MyFangKuiBtnItem.setEnabled(false);
                                                    self.MyMeiHuaBtnItem.setEnabled(false);
                                                    self.MyHongTaoBtnItem.setEnabled(false);
                                                    self.MyHeiTaoBtnItem.setEnabled(false);
                                                    self.MyWangBtnItem.setEnabled(false);
                                                    self.MyBtnContinueJetton.setEnabled(false);
                                                    self.MyBtnClearJetton.setEnabled(false);

                                                    self.MyTimerStateSprite.setTextureRect(cc.rect(111*2,0,111,31));
                                                    self.MyTimerStateSprite.setVisible(true);
                                                    m_curTime = m_timekaipai;
                                                    clearInterval(m_curTimerId);
                                                    Timeralive(false);
                                                    m_curTimerId = setInterval( function(){Timeralive(true)},1000);

                                                    if(m_isCurrentJetton == false)
                                                    {
                                                        for(i=0;i<5;i++) {
                                                            m_tempcolorjettons[i] = 0;
                                                        }
                                                    }

                                                    if(resultcardnum == 78)
                                                    {
                                                        self.MyCardImage.setTexture(s_dxwang);
                                                        self.MyCardImage.setTextureRect(cc.rect(0,0,264,369));

                                                        m_colorrecordcount[4] += 1;

                                                        //if(m_isplaymusic) {
                                                        //     cc.audioEngine.playEffect(sound_Joker1, false);
                                                        //}
                                                    }
                                                    else if(resultcardnum == 79)
                                                    {
                                                        self.MyCardImage.setTexture(s_dxwang);
                                                        self.MyCardImage.setTextureRect(cc.rect(264,0,264,369));

                                                        m_colorrecordcount[4] += 1;

                                                        //if(m_isplaymusic) {
                                                        //     cc.audioEngine.playEffect(sound_Joker2, false);
                                                        // }
                                                    }
                                                    else {
                                                        var resultcard = resultcardnum.toString(16);

                                                        var cardcolor = 3;
                                                        var cardnum = parseInt("0x"+resultcard[0])-1;

                                                        if(resultcard.length > 1) {
                                                            cardcolor = 3 - Number(resultcard[0]);
                                                            cardnum = parseInt("0x" + resultcard[1]) - 1;

                                                            m_colorrecordcount[Number(resultcard[0])] += 1;
                                                        }
                                                        else {
                                                            m_colorrecordcount[0] += 1;
                                                        }

                                                        var psoundstr = "";
                                                        var psoundIndex = cardnum+1;

                                                        if(cardcolor == 3){
                                                            psoundstr = "sounds/Diamond"+psoundIndex+".ogg";
                                                        }
                                                        else if(cardcolor == 2){
                                                            psoundstr = "sounds/Club"+psoundIndex+".ogg";
                                                        }
                                                        else if(cardcolor == 1){
                                                            psoundstr = "sounds/Heart"+psoundIndex+".ogg";
                                                        }
                                                        else if(cardcolor == 0){
                                                            psoundstr = "sounds/Spade"+psoundIndex+".ogg";
                                                        }

                                                        self.MyCardImage.setTexture(s_puke);
                                                        self.MyCardImage.setTextureRect(cc.rect(cardnum * 264, cardcolor * 369, 264, 369));

                                                        // if(m_isplaymusic) {
                                                        //     cc.audioEngine.playEffect(psoundstr, false);
                                                        // }
                                                    }

                                                    self.MyColorCountFangkuai.setString(m_colorrecordcount[3]);
                                                    self.MyColorCountMeiHua.setString(m_colorrecordcount[2]);
                                                    self.MyColorCountHongTao.setString(m_colorrecordcount[1]);
                                                    self.MyColorCountHeiTao.setString(m_colorrecordcount[0]);
                                                    self.MyColorCountWang.setString(m_colorrecordcount[4]);

                                                    m_cardrecourdcount.push(resultcardnum);

                                                    var index = 0;
                                                    for(i=0;i<m_cardrecourdcount.length;i++){
                                                        if(m_cardrecourdcount[i] == 78)
                                                        {
                                                            m_gamerecordsprites[index].setTextureRect(cc.rect(0,4*22,94,22));
                                                        }
                                                        else if(m_cardrecourdcount[i] == 79)
                                                        {
                                                            m_gamerecordsprites[index].setTextureRect(cc.rect(94,4*22,94,22));
                                                        }
                                                        else {
                                                            var resultcard = m_cardrecourdcount[i].toString(16);

                                                            var cardcolor = 3;
                                                            var cardnum = parseInt("0x"+resultcard[0])-1;

                                                            if(resultcard.length > 1) {
                                                                cardcolor = 3 - Number(resultcard[0]);
                                                                cardnum = parseInt("0x" + resultcard[1]) - 1;
                                                            }

                                                            m_gamerecordsprites[index].setTextureRect(cc.rect(cardnum * 94, cardcolor * 22, 94, 22));
                                                        }
                                                        m_gamerecordsprites[index].setVisible(true);
                                                        index += 1;
                                                    }
                                                }
                                                    break;
                                                case 1004:
                                                {
                                                    gamestate = objgame.gamestate;
                                                    var lastresult = objgame.resultnum;
                                                    var pusermoney = objgame.usermoney;

                                                    var pSpriteIndex = Math.floor(Math.random()*10);
                                                    self.MyCardImage.setTexture(s_01);
                                                    self.MyCardImage.setTextureRect(cc.rect(pSpriteIndex*264,0,264,369));

                                                    m_myselfusermoney = pusermoney;
                                                    m_myselftotalresult += lastresult;
                                                    self.MySelfResultTotalLable.setString(m_myselftotalresult);
                                                    m_myselftempusermoney = m_myselfusermoney;
                                                    self.MyUserMoney.setString(m_myselfusermoney);

                                                    self.MyTimerStateSprite.setTextureRect(cc.rect(0,0,111,31));
                                                    self.MyTimerStateSprite.setVisible(true);
                                                    m_curTime = m_timejiesuan;
                                                    clearInterval(m_curTimerId);
                                                    Timeralive(false);
                                                    m_curTimerId = setInterval( function(){Timeralive(true)},1000);

                                                    if(m_isplaymusic) {
                                                        if (lastresult > 0) {
                                                            //   cc.audioEngine.playEffect(sound_GameVt, false);
                                                        }
                                                        else if (lastresult < 0) {
                                                            //   cc.audioEngine.playEffect(sound_GameShfit, false);
                                                        }
                                                        else {
                                                            //  cc.audioEngine.playEffect(sound_GameRecycle, false);
                                                        }
                                                    }
                                                }
                                                    break;
                                                case 1006:
                                                {
                                                    var jettonstate = objgame.gamestate;
                                                    var jettonUserId = objgame.userid;
                                                    var jettonmoney = objgame.jettonmoney;

                                                    if(jettonUserId != myselfUserId) {
                                                        for (i = 0; i < 5; i++) {
                                                            m_AllJettonTotal[i] -= jettonmoney[i];
                                                        }

                                                        self.MyFangKuaiAllJettonTotal.setString(m_AllJettonTotal[0]);
                                                        self.MyMeiHuaAllJettonTotal.setString(m_AllJettonTotal[1]);
                                                        self.MyHongTaoAlljettonTotal.setString(m_AllJettonTotal[2]);
                                                        self.MyHeiTaoAllJettonTotal.setString(m_AllJettonTotal[3]);
                                                        self.MyWangAllJettonTotal.setString(m_AllJettonTotal[4]);
                                                    }
                                                }
                                                    break;
                                                case 1005:
                                                {
                                                    var jettonstate = objgame.gamestate;
                                                    var jettonUserId = objgame.userid;
                                                    var jettonmoney = objgame.jettonmoney;

                                                    if(jettonUserId == myselfUserId)
                                                    {
                                                        if(jettonstate == 0)
                                                        {
                                                            for(i=0;i<5;i++)
                                                            {
                                                                m_myselfJettonTotal[i] -= jettonmoney[i];
                                                                m_AllJettonTotal[i] -= jettonmoney[i];
                                                            }

                                                            self.MyFangKuiSelfJettonTotal.setString(m_myselfJettonTotal[0]);
                                                            self.MyFangKuaiAllJettonTotal.setString(m_AllJettonTotal[0]);
                                                            self.MyMeiHuaSelfJettonTotal.setString(m_myselfJettonTotal[1]);
                                                            self.MyMeiHuaAllJettonTotal.setString(m_AllJettonTotal[1]);
                                                            self.MyHongTaoSelfJettonTotal.setString(m_myselfJettonTotal[2]);
                                                            self.MyHongTaoAlljettonTotal.setString(m_AllJettonTotal[2]);
                                                            self.MyHeiTaoSelfJettonTotal.setString(m_myselfJettonTotal[3]);
                                                            self.MyHeiTaoAllJettonTotal.setString(m_AllJettonTotal[3]);
                                                            self.MyWangSelfJettonTotal.setString(m_myselfJettonTotal[4]);
                                                            self.MyWangAllJettonTotal.setString(m_AllJettonTotal[4]);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if(jettonstate == 1)
                                                        {
                                                            for(i=0;i<5;i++)
                                                            {
                                                                m_AllJettonTotal[i] += jettonmoney[i];
                                                            }

                                                            self.MyFangKuaiAllJettonTotal.setString(m_AllJettonTotal[0]);
                                                            self.MyMeiHuaAllJettonTotal.setString(m_AllJettonTotal[1]);
                                                            self.MyHongTaoAlljettonTotal.setString(m_AllJettonTotal[2]);
                                                            self.MyHeiTaoAllJettonTotal.setString(m_AllJettonTotal[3]);
                                                            self.MyWangAllJettonTotal.setString(m_AllJettonTotal[4]);
                                                        }
                                                    }
                                                }
                                                    break;
                                                default:
                                                    break;
                                            }
                                        }
                                            break;
                                        default:
                                            break;
                                    }
                                }

                                gameserversocket.onclose = function(){
                                    var loginlayer = new MyMessageBoxLayer();
                                    self.addChild(loginlayer,8);
                                    loginlayer.init("游戏服务器连接失败，请稍后再试！");
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }
            }

            socket.onclose = function(){
                self.ConnectServerLabel.removeFromParentAndCleanup(true);
                clearInterval(keepalivetimer);

                var loginlayer = new MyMessageBoxLayer();
                self.addChild(loginlayer,8);
                loginlayer.init("验证服务器连接失败，请稍后再试！");
            }

        } catch(exception){
            var loginlayer = new MyMessageBoxLayer();
            self.addChild(loginlayer,8);
            loginlayer.init(exception);
        }
    }
});

var CWxhhScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new CMainFrameLayer();
        this.addChild(layer);
        layer.init();
    }
});
