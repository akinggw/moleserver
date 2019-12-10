var v_account_socket = null;
//var host = "ws://182.61.5.226:6789";
var host = "ws://127.0.0.1:3333";
var last_health_account = null;
var health_timeout_account=6000;
var keepalivetimer_account = null;
var v_MainLoginlayer = null;
var v_RegisterLayer = null;
var m_userloginname_str = null;
var m_userloginpassword_str = null;
var m_myselfUserId = 0;
var m_myselfusermoney = 0;

function keepalive_account(ws) {
    var time = new Date();
    var curtime = last_health_account.getTime();

    if(time.getTime() - curtime > health_timeout_account)
    {
        clearInterval(keepalivetimer_account);
    }
    else
    {
        if (ws.bufferedAmount == 0) {
            ws.send('100');

            last_health_account = time;
        }
    }
};

var CRegisterLayer = cc.LayerColor.extend({
    _boxusername: null,
    sprite:null,
    _boxuserpassword:null,

    init: function () {
        this._super();

        //this.setColor(cc.color(0,0,0));
        this.setOpacity(200);

        var size = cc.director.getWinSize();

        this.sprite = new cc.Sprite(s_login_bg_sprite);
        this.sprite.setAnchorPoint(0.5, 0.5);
        this.sprite.setPosition(size.width / 2, size.height / 2);
        //this.sprite.setScale(0.65);
        this.addChild(this.sprite, 0);

        this._boxusername = new cc.EditBox.create(cc.size(170, 50), new cc.Scale9Sprite.create(), new cc.Scale9Sprite.create());
        this._boxusername.setPlaceholderFontColor(cc.color(0, 0, 0));
        this._boxusername.setPlaceHolder("请输入用户名");
        this._boxusername.setPosition(size.width/2+220, size.height/2+90);
        this._boxusername.setFontColor(cc.color(0, 0, 0));
        this._boxusername.setDelegate(this);
        this.addChild(this._boxusername);

        this._boxuserpassword = new cc.EditBox.create(cc.size(170, 50), new cc.Scale9Sprite.create(),new cc.Scale9Sprite.create());
        this._boxuserpassword.setPlaceholderFontColor(cc.color(0, 0, 0));
        this._boxuserpassword.setPlaceHolder("请输入用户密码");
        this._boxuserpassword.setInputFlag(cc.EDITBOX_INPUT_FLAG_PASSWORD);
        this._boxuserpassword.setPosition(size.width/2+220, size.height/2+23);
        this._boxuserpassword.setFontColor(cc.color(0, 0, 0));
        this._boxuserpassword.setDelegate(this);
        this.addChild(this._boxuserpassword);

        var RegisterBtnItem = new cc.MenuItemImage.create(
            s_reg_reg_1_sprite,
            s_reg_reg_1_sprite,
            function () {
                if(this._boxusername.getString() == "")
                {
                    var msglayer = new MessageBoxLayer();
                    this.addChild(msglayer,800);
                    msglayer.init("用户名不能为空，请重新输入！");
                    return;
                }
                if(this._boxuserpassword.getString() == "")
                {
                    var msglayer = new MessageBoxLayer();
                    this.addChild(msglayer,800);
                    msglayer.init("用户密码不能为空，请重新输入！");
                    return;
                }

                var row1 = {};
                row1.MsgId = 700;
                row1.UserName = this._boxusername.getString();
                row1.UserPW = hex_md5(this._boxuserpassword.getString());
                //row1.Email = "test@126.com";
                //row1.Sex = 0;
                //row1.RealName = row1.UserName;
                //row1.Telephone = '';
                //row1.AvatorIndex = '';
                //row1.Referrer = '';
                //row1.cardnumber = '';

                v_account_socket.send(JSON.stringify(row1));
            },this);
        RegisterBtnItem.setAnchorPoint(0.5, 0.5);
        RegisterBtnItem.setScale(0.9);

        var RegisterBtnCloseItem = new cc.MenuItemImage.create(
            s_btnloginclose_1_sprite,
            s_btnloginclose_2_sprite,
            function () {
                this.setVisible(false);
                v_MainLoginlayer.setVisible(true);
            },this);
        RegisterBtnCloseItem.setAnchorPoint(0.5, 0.5);
        RegisterBtnCloseItem.setScale(0.9);

        var menu = new cc.Menu.create(RegisterBtnItem,RegisterBtnCloseItem);
        menu.setPosition(0, 0);
        this.addChild(menu, 1);
        RegisterBtnItem.setPosition(size.width/2+213,size.height/2-50);
        RegisterBtnCloseItem.setPosition(size.width/2+365,size.height/2+210);
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

        this.sprite = new cc.Sprite(s_login_bg_sprite);
        this.sprite.setAnchorPoint(0.5, 0.5);
        this.sprite.setPosition(size.width / 2, size.height / 2);
        //this.sprite.setScale(0.65);
        this.addChild(this.sprite, 0);

        this._boxusername = new cc.EditBox.create(cc.size(170, 50), new cc.Scale9Sprite.create(), new cc.Scale9Sprite.create());
        this._boxusername.setPlaceholderFontColor(cc.color(0, 0, 0));
        this._boxusername.setPlaceHolder("请输入用户名");
        this._boxusername.setPosition(size.width/2+220, size.height/2+90);
        this._boxusername.setFontColor(cc.color(0, 0, 0));
        this._boxusername.setDelegate(this);
        this.addChild(this._boxusername,0);

        this._boxuserpassword = new cc.EditBox.create(cc.size(170, 50), new cc.Scale9Sprite.create(),new cc.Scale9Sprite.create());
        this._boxuserpassword.setPlaceholderFontColor(cc.color(0, 0, 0));
        this._boxuserpassword.setPlaceHolder("请输入用户密码");
        this._boxuserpassword.setInputFlag(cc.EDITBOX_INPUT_FLAG_PASSWORD);
        this._boxuserpassword.setPosition(size.width/2+220, size.height/2+23);
        this._boxuserpassword.setFontColor(cc.color(0, 0, 0));
        this._boxuserpassword.setDelegate(this);
        this.addChild(this._boxuserpassword,0);

        var LoginBtnItem = new cc.MenuItemImage.create(
            s_login_login_1_sprite,
            s_login_login_2_sprite,
            function () {
                if(this._boxusername.getString() == "")
                {
                    var msglayer = new MessageBoxLayer();
                    this.addChild(msglayer,800);
                    msglayer.init("用户名不能为空，请重新输入！");
                    return;
                }
                if(this._boxuserpassword.getString() == "")
                {
                    var msglayer = new MessageBoxLayer();
                    this.addChild(msglayer,800);
                    msglayer.init("用户密码不能为空，请重新输入！");
                    return;
                }

                m_userloginname_str = this._boxusername.getString();
                m_userloginpassword_str = hex_md5(this._boxuserpassword.getString());

                var row1 = {};
                row1.MsgId = 400;
                row1.username = m_userloginname_str;
                row1.userpwd = m_userloginpassword_str;
                row1.machinecode = 'html5';

                v_account_socket.send(JSON.stringify(row1));
            },this);
        LoginBtnItem.setAnchorPoint(0.5, 0.5);
        LoginBtnItem.setScale(0.9);

        var RegisterBtnItem = new cc.MenuItemImage.create(
            s_login_register_1_sprite,
            s_login_register_2_sprite,
            function () {
                this.setVisible(false);
                v_RegisterLayer.setVisible(true);
            },this);
        RegisterBtnItem.setAnchorPoint(0.5, 0.5);
        RegisterBtnItem.setScale(0.9);

        var menu = new cc.Menu.create(LoginBtnItem,RegisterBtnItem);
        menu.setPosition(0, 0);
        this.addChild(menu, 1);
        LoginBtnItem.setPosition(size.width/2+146,size.height/2-50);
        RegisterBtnItem.setPosition(size.width/2+282,size.height/2-50);
    }
});

function OnProcessAccountSocketOnOpen() {
    v_account_socket.send('100');

    last_health_account = new Date();
    clearInterval(keepalivetimer_account);
    keepalivetimer_account = setInterval( function(){keepalive_account(v_account_socket)},1000);

    m_userloginname_str = Cookies.get("username");
    m_userloginpassword_str = Cookies.get("userpassword");

    v_MainLoginlayer = new CLoginLayer();
    m_MainFrameLayer.addChild(v_MainLoginlayer,20000);
    v_MainLoginlayer.init();

    v_RegisterLayer = new CRegisterLayer();
    m_MainFrameLayer.addChild(v_RegisterLayer,20000);
    v_RegisterLayer.init();
    v_RegisterLayer.setVisible(false);

    if(m_userloginname_str != null && m_userloginpassword_str != null) {
        v_MainLoginlayer.setVisible(false);

        var row1 = {};
        row1.MsgId = 400;
        row1.username = m_userloginname_str;
        row1.userpwd = m_userloginpassword_str;
        row1.machinecode = 'html5';

        v_account_socket.send(JSON.stringify(row1));
    }
}

function OnProcessAccountSocketOnMessage(msg) {
    var str = "";
    str = msg.data;

    console.info(str);

    var obj = eval('(' + str + ')');

    switch(obj.MsgId) {
        case 300: {
            if (obj.MsgSubId == 301) {

            }
        }
            break;
        case 700:
        {
            switch(obj.MsgSubId)
            {
                case 701:
                {
                    var msglayer = new MessageBoxLayer();
                    v_RegisterLayer.addChild(msglayer,800);
                    msglayer.init("注册成功！");

                    v_RegisterLayer.setVisible(false);
                    v_MainLoginlayer.setVisible(true);
                }
                    break;
                case 702:
                {
                    var msglayer = new MessageBoxLayer();
                    v_RegisterLayer.addChild(msglayer,800);
                    msglayer.init("注册失败，请检查您的用户名和密码！");
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
                    var msglayer = new MessageBoxLayer();
                    v_MainLoginlayer.addChild(msglayer,800);
                    msglayer.init("用户登录失败，请检查用户名和密码！");
                }
                    break;
                case 415:
                {
                    var msglayer = new MessageBoxLayer();
                    v_MainLoginlayer.addChild(msglayer,800);
                    msglayer.init("此账号已被封，请联系客服人员！");
                }
                    break;
                case 504:
                {
                    var msglayer = new MessageBoxLayer();
                    v_MainLoginlayer.addChild(msglayer,800);
                    msglayer.init("此账号已经在服务器中，不能重复登录！");
                }
                    break;
                case 418:
                {
                    var msglayer = new MessageBoxLayer();
                    v_MainLoginlayer.addChild(msglayer,800);
                    msglayer.init("此账号已被玩家锁机，请联系客服人员！");
                }
                    break;
                case 401:
                {
                    m_myselfUserId = obj.UserId;
                    m_myselfusermoney = obj.money;

                    v_MainLoginlayer.removeFromParent();

                    m_labelName.setString(m_userloginname_str);
                    m_labelMoney.setString(m_myselfusermoney);

                    Cookies.set("username",m_userloginname_str);
                    Cookies.set("userpassword",m_userloginpassword_str);

                    //获取服务器列表
                    var row1 = {};
                    row1.MsgId = 800;
                    v_account_socket.send(JSON.stringify(row1));
                }
                    break;
                default:
                    break;
            }
        }
            break;
        case 800: {
            var MsgSubId = obj.MsgSubId;

            if (MsgSubId == 803) {
                var msglayer = new MessageBoxLayer();
                v_MainLoginlayer.addChild(msglayer, 800);
                msglayer.init("获取服务器失败，请稍后再试！");
            }
            else {
                var RoomCount = obj.RoomCount;

                var m_gameserver = "ws://" + obj.Room[2].serverip + ":" + obj.Room[2].serverport;

                try{
                    m_gameserversocket = new WebSocket(m_gameserver);

                    m_gameserversocket.onopen = OnProcessGameSocketOnOpen

                    m_gameserversocket.onmessage = OnProcessGameSocketOnMessage

                    m_gameserversocket.onclose = OnProcessGameSocketOnClose
                } catch(exception){
                    var exceptionlayer = new MessageBoxLayer();
                    m_MainFrameLayer.addChild(exceptionlayer,800);
                    exceptionlayer.init(exception);
                }
            }
        }
            break;
        default:
            break;
    }
}

function OnProcessAccountSocketOnClose() {
    var msglayer = new MessageBoxLayer();
    m_MainFrameLayer.addChild(msglayer,18000);
    msglayer.init("账号服务器连接失败，请稍后再试！");
}