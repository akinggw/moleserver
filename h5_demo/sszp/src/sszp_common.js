var SSZP_CARDTYPE_QINGLONG = 0;                 // 青龙
var SSZP_CARDTYPE_BAIHU = 1;                        // 白虎
var SSZP_CARDTYPE_ZHUQUE = 2;                       // 朱雀
var SSZP_CARDTYPE_XUANWU = 3;                        // 玄武
var SSZP_CARDTYPE_XIAOBAILONG = 4;                  // 小白龙
var SSZP_CARDTYPE_XIAOLAOHU = 5;                   // 小老虎
var SSZP_CARDTYPE_XIAOFENGHUANG = 6;                // 小凤凰
var SSZP_CARDTYPE_XIAOWUGUI = 7;                   // 小乌龟

var STATE_XIAZHU = 0;          // 下注状态
var STATE_GAMING = 1;               // 游戏状态
var STATE_GAMEEND = 2;              // 游戏结束状态

function getCirclePosition3(angle,radius,center,isfan) {
    var pangle = angle*3.14159265359/180;

    var newPos = {x:0,y:0};

    if(isfan) {
        newPos.x = center.x + (radius * Math.sin(pangle));
        newPos.y = center.y + (radius * Math.cos(pangle));
    }
    else {
        newPos.x = center.x + (radius * Math.cos(pangle));
        newPos.y = center.y + (radius * Math.sin(pangle));
    }

    return newPos;
}

function getDistance(pos1,pos2)
{
    return Math.sqrt((pos1.x-pos2.x) * (pos1.x-pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y));
}

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

        this.sprite = new cc.Sprite.create(s_common_pop_bkg_sprite);
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
            s_pop_ok_1_sprite,
            s_pop_ok_2_sprite,
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