var STATE_XIAZHU = 0;          // 下注状态
var STATE_GAMING = 1;               // 游戏状态
var STATE_GAMEEND = 2;              // 游戏结束状态

var IDD_MESSAGE_ROOM_FAPAI = 1001;        // 发牌消息
var IDD_MESSAGE_ROOM_XIAZHU = 1002;       // 下注消息
var IDD_MESSAGE_ROOM_GAMEEND = 1003;       // 结算消息
var IDD_MESSAGE_ROOM_ENTERGAME = 1004;       // 进入游戏消息
var IDD_MESSAGE_ROOM_SHANGZHUANG = 1005;        // 上庄
var IDD_MESSAGE_ROOM_XIAZHUANG = 1006;       // 下庄
var IDD_MESSAGE_ROOM_REENTERGAME = 1007;       // 重进房间消息
var IDD_MESSAGE_ROOM_STARTXIAZHU = 1008;       // 开始下注消息
var IDD_MESSAGE_ROOM_GETZHUANGLIST = 1009;      // 得到装列表
var IDD_MESSAGE_ROOM_SUERCONTROL = 1010;      // 控制出牌
var IDD_MESSAGE_ROOM_CLEARJETTONS = 1011;      // 清除下注

var CT_ERROR			=	0;		//错误类型
var CT_POINT			=	1;		//点数
var CT_SPECIAL_NIU1		=	3;		//牛1
var CT_SPECIAL_NIU2		=	4;		//牛2
var CT_SPECIAL_NIU3		=	5;		//牛3
var CT_SPECIAL_NIU4		=	6;		//牛4
var CT_SPECIAL_NIU5		=	7;		//牛5
var CT_SPECIAL_NIU6		=	8;		//牛6
var CT_SPECIAL_NIU7		=	9;		//牛7
var CT_SPECIAL_NIU8		=	10;		//牛9
var CT_SPECIAL_NIU9		=	11;		//牛9
var CT_SPECIAL_NIUNIUXW	=	12;		//小牛
var CT_SPECIAL_NIUNIUDW	=	13;		//大牛
var CT_SPECIAL_NIUNIU	=	14;		//牛牛
var CT_SPECIAL_NIUYING	=	15;		//银牛
var CT_SPECIAL_NIUKING	=	16;		//金牛
var CT_SPECIAL_BOMEBOME	=	17;		//炸弹

function getcardtypename(ptype)
{
    switch(ptype)
    {
        case CT_POINT:
        {
            return "无牛";
        }
            break;
        case CT_SPECIAL_NIU1:
        {
            return "牛一";
        }
            break;
        case CT_SPECIAL_NIU2:
        {
            return "牛二";
        }
            break;
        case CT_SPECIAL_NIU3:
        {
            return "牛三";
        }
            break;
        case CT_SPECIAL_NIU4:
        {
            return "牛四";
        }
            break;
        case CT_SPECIAL_NIU5:
        {
            return "牛五";
        }
            break;
        case CT_SPECIAL_NIU6:
        {
            return "牛六";
        }
            break;
        case CT_SPECIAL_NIU7:
        {
            return "牛七";
        }
            break;
        case CT_SPECIAL_NIU8:
        {
            return "牛八";
        }
            break;
        case CT_SPECIAL_NIU9:
        {
            return "牛九";
        }
            break;
        case CT_SPECIAL_NIUNIUXW:
        {
            return "小牛";
        }
            break;
        case CT_SPECIAL_NIUNIUDW:
        {
            return "大牛";
        }
            break;
        case CT_SPECIAL_NIUNIU:
        {
            return "牛牛";
        }
            break;
        case CT_SPECIAL_NIUYING:
        {
            return "银牛";
        }
            break;
        case CT_SPECIAL_NIUKING:
        {
            return "金牛";
        }
            break;
        case CT_SPECIAL_BOMEBOME:
        {
            return "炸弹";
        }
            break;
        default:
            break;
    }

    return "";
}

function playcardtypesound(ptype)
{
    switch(ptype)
    {
        case CT_POINT:
        {
            cc.audioEngine.playEffect(v_game_sound_m_no_ox, false);
        }
            break;
        case CT_SPECIAL_NIU1:
        {
            cc.audioEngine.playEffect(v_game_sound_m_ox_1, false);
        }
            break;
        case CT_SPECIAL_NIU2:
        {
            cc.audioEngine.playEffect(v_game_sound_m_ox_2, false);
        }
            break;
        case CT_SPECIAL_NIU3:
        {
            cc.audioEngine.playEffect(v_game_sound_m_ox_3, false);
        }
            break;
        case CT_SPECIAL_NIU4:
        {
            cc.audioEngine.playEffect(v_game_sound_m_ox_4, false);
        }
            break;
        case CT_SPECIAL_NIU5:
        {
            cc.audioEngine.playEffect(v_game_sound_m_ox_5, false);
        }
            break;
        case CT_SPECIAL_NIU6:
        {
            cc.audioEngine.playEffect(v_game_sound_m_ox_6, false);
        }
            break;
        case CT_SPECIAL_NIU7:
        {
            cc.audioEngine.playEffect(v_game_sound_m_ox_7, false);
        }
            break;
        case CT_SPECIAL_NIU8:
        {
            cc.audioEngine.playEffect(v_game_sound_m_ox_8, false);
        }
            break;
        case CT_SPECIAL_NIU9:
        {
            cc.audioEngine.playEffect(v_game_sound_m_ox_9, false);
        }
            break;
        case CT_SPECIAL_NIUNIUXW:
        {
            cc.audioEngine.playEffect(v_game_sound_m_small_five_ox, false);
        }
            break;
        case CT_SPECIAL_NIUNIUDW:
        {
            cc.audioEngine.playEffect(v_game_sound_m_five_ox, false);
        }
            break;
        case CT_SPECIAL_NIUNIU:
        {
            cc.audioEngine.playEffect(v_game_sound_m_five_ox, false);
        }
            break;
        case CT_SPECIAL_NIUYING:
        {
            cc.audioEngine.playEffect(v_game_sound_m_five_ox, false);
        }
            break;
        case CT_SPECIAL_NIUKING:
        {
            cc.audioEngine.playEffect(v_game_sound_m_five_ox, false);
        }
            break;
        case CT_SPECIAL_BOMEBOME:
        {
            cc.audioEngine.playEffect(v_game_sound_m_za_4, false);
        }
            break;
        default:
            break;
    }
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