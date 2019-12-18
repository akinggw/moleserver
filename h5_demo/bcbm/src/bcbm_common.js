var IDD_MESSAGE_ROOM_FAPAI      = 1001;         // 发牌消息
var IDD_MESSAGE_ROOM_XIAZHU     = 1002;         // 下注消息
var IDD_MESSAGE_ROOM_GAMEEND    = 1003;        // 结算消息
var IDD_MESSAGE_ROOM_ENTERGAME  = 1004;        // 进入游戏消息
var IDD_MESSAGE_ROOM_STARTXIAZHU  = 1005;        // 开始下注消息
var IDD_MESSAGE_ROOM_REENTERGAME = 1006;        // 重回房间
var IDD_MESSAGE_ROOM_SHANGZHUANG = 1007;        // 上庄
var IDD_MESSAGE_ROOM_XIAZHUANG   = 1008;         // 下庄
var IDD_MESSAGE_ROOM_GETZHUANGLIST = 1009;      // 得到装列表
var IDD_MESSAGE_ROOM_SUERCONTROL = 1010;       // 控制出牌
var IDD_MESSAGE_ROOM_CLEARJETTON = 1011;        // 清除下注

var STATE_XIAZHU = 0;          // 下注状态
var STATE_GAMING = 1;             // 游戏状态
var STATE_GAMEEND = 2;          // 游戏结束状态
var STATE_NULL = 3;

var CARTYPE_FALALI = 0;                  // 法拉利
var CARTYPE_LANBOJILI = 1;                 // 兰博基尼
var CARTYPE_MASAILADI = 2;                  // 玛莎拉蒂
var CARTYPE_BAOSHIJIE = 3;                  // 保时捷
var CARTYPE_BENCI = 4;                     // 奔驰
var CARTYPE_BAOMA = 5;                       // 宝马
var CARTYPE_AODI = 6;                       // 奥迪
var CARTYPE_DAZONG = 7;                      // 大众

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

var MessageBoxLayer = cc.LayerColor.extend({
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

        this.sprite = new cc.Sprite.create(v_sprite_common_pop_bkg);
        this.sprite.setAnchorPoint(0.5, 0.5);
        this.sprite.setPosition(size.width / 2, size.height / 2-20);
        this.sprite.setScale(0.75);
        this.addChild(this.sprite);

        var str2 = splitStr(msg, 300, "黑体", 16).join('\n');

        this.helloLabel = new cc.LabelTTF.create(str2, "黑体", 16);
        // position the label on the center of the screen
        this.helloLabel.setPosition(size.width / 2, size.height/2);
        this.helloLabel.setColor(cc.color(0,0,0));
        // add the label as a child to this layer
        this.addChild(this.helloLabel);

        var closeItem = new cc.MenuItemImage.create(
            v_sprite_pop_ok_1,
            v_sprite_pop_ok_2,
            function () {
                this.removeFromParentAndCleanup(true);
            },this);
        closeItem.setAnchorPoint(0.5, 0.5);
        //closeItem.setScale(0.8);

        var menu = new cc.Menu.create(closeItem);
        menu.setPosition(0, 0);
        this.addChild(menu, 1);
        closeItem.setPosition(size.width/2,size.height/2-60);
    }
});