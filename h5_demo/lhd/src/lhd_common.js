var m_MainFrameLayer = null;
var m_MenuItemObjTag = 100;
var m_jettonspritelist = [];
var m_jettonspritelistcount = 0;
var v_gameover_bg_sprite = null;
var v_gameover_bg_lost_sprite = null;
var v_gameover_bg_win_sprite = null;
var v_number_sprite = null;
var v_number_timer_sprite = null;
var v_GamingState_sprite_one = null;
var v_GamingState_sprite_two = null;
var v_GamingState_sprite_three = null;
var v_card_result_sprite_xian = null;
var v_card_result_sprite_zhuang = null;
var v_card_result_sprite_anim = null;
var v_game_record_sprite_list = [];
var v_game_record_sprite_long_count  = null;
var v_game_record_sprite_hu_count = null;
var v_game_record_sprite_hong_count = null;
var v_game_record_long_count = 0;
var v_game_record_hu_count = 0;
var v_game_record_hong_count = 0;
var v_user_name_text = null;
var v_user_money_text = null;
var v_user_game_jushu = null;
var v_user_game_result = null;
var v_game_jetton_total_text = null;
var v_game_jetton_person_text = null;
var v_game_jetton_long_text = null;
var v_game_jetton_hu_text = null;
var v_game_jetton_hong_text = null;
var v_game_jetton_long_total = 0;
var v_game_jetton_hu_total = 0;
var v_game_jetton_hong_total = 0;
var v_game_jetton_total = 0;
var v_game_timer_id = null;
var v_game_timer_current_time = 0;
var v_user_mouse_isprocess = false;
var v_game_waiting_colorlayer = null;
var v_game_waiting_sprite = null;
var v_game_state_timer_id = null;
var v_nomoney_xiazhu_sprite = null;
var v_qingxiazhu_sprite = null;
var v_hong_win_sprite = null;
var v_hu_win_sprite = null;
var v_long_win_sprite = null;
var v_card_one_value = null;
var v_card_two_value = null;
var v_current_gaming_result = 0;
var v_gaming_result_vector = [];
var v_game_user_jetton_total = 0;
var v_game_alluser_jetton_total = 0;
var v_sound_dians_list = [];

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
