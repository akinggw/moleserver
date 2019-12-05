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

function clearalljettonsprites() {
    if(m_MainFrameLayer == null)
        return;

    for(var i=0;i<m_jettonspritelistcount;i++){
        m_MainFrameLayer.removeChild(m_jettonspritelist[i]);
    }

    m_jettonspritelistcount = 0;
    v_game_jetton_long_total = 0;
    v_game_jetton_hu_total = 0;
    v_game_jetton_hong_total = 0;

    v_game_jetton_long_text.setVisible(false);
    v_game_jetton_hu_text.setVisible(false);
    v_game_jetton_hong_text.setVisible(false);
}

function starttimer(timenum) {
    if(v_game_timer_id != null) {
        clearInterval(v_game_timer_id);
    }

    v_game_timer_current_time = timenum;
    v_number_timer_sprite.setString(v_game_timer_current_time);
    v_number_timer_sprite.setVisible(true);

    v_game_timer_id = setInterval( function(){
        v_game_timer_current_time = v_game_timer_current_time - 1;
        if(v_game_timer_current_time < 0) {
            v_number_timer_sprite.setVisible(false);
            clearInterval(v_game_timer_id);
        }

        if(v_game_timer_current_time < 4) {
            cc.audioEngine.playEffect(s_sound_clock, false);
        }

        v_number_timer_sprite.setString(v_game_timer_current_time);
    },1000);
}

function addvaluetovector(value) {
    v_gaming_result_vector.push(value);

    if(v_gaming_result_vector.length > 52) {
        v_gaming_result_vector.splice(0,1);
    }

    if(value == 0) v_game_record_long_count = v_game_record_long_count + 1;
    else if(value == 1) v_game_record_hu_count = v_game_record_hu_count + 1;
    else v_game_record_hong_count = v_game_record_hong_count + 1;

    v_game_record_sprite_long_count.setString(v_game_record_long_count);
    v_game_record_sprite_hu_count.setString(v_game_record_hu_count);
    v_game_record_sprite_hong_count.setString(v_game_record_hong_count);
}

function showusergamingjettons() {
    v_game_jetton_total_text.setString(v_game_alluser_jetton_total);
    v_game_jetton_person_text.setString(v_game_user_jetton_total);
}

function ShowGamingResult() {
    for(var i=0;i<v_gaming_result_vector.length;i++) {
        v_game_record_sprite_list[i].setTextureRect(cc.rect(27*v_gaming_result_vector[i],0,27,28));
        v_game_record_sprite_list[i].setVisible(true);
    }
}

function showcard(type,card,isshow) {
    var tmp = card;
    var color = (tmp & 240) >> 4;
    var value = (tmp & 15);

    cc.audioEngine.playEffect(s_sound_DispatchCard, false);

    var size = cc.director.getWinSize();
    v_card_result_sprite_anim.setPosition(size.width / 2, size.height / 2+130);
    v_card_result_sprite_anim.setVisible(isshow);

    if(type == 1) {
        v_card_result_sprite_xian.setTextureRect(cc.rect(value*55,color*73,55,73));

        if(isshow) {
            var showPosition = cc.callFunc(function(){
                v_card_result_sprite_anim.setVisible(!isshow);
                v_card_result_sprite_xian.setVisible(isshow);

                cc.audioEngine.playEffect(v_sound_dians_list[value], false);

                var p_timer_id = setInterval( function() {
                    clearInterval(p_timer_id);

                    showcard(2, v_card_two_value, true);
                },500);

                },v_card_result_sprite_anim);
            var moveTo = cc.moveTo(0.2, v_card_result_sprite_xian.getPosition());
            var seq = cc.sequence(moveTo, showPosition);
            v_card_result_sprite_anim.runAction(seq);
        }
        else {
            v_card_result_sprite_xian.setVisible(isshow);
        }
    }
    else {
        v_card_result_sprite_zhuang.setTextureRect(cc.rect(value*55,color*73,55,73));

        if(isshow) {
            var showPosition = cc.callFunc(function () {
                v_card_result_sprite_anim.setVisible(!isshow);
                v_card_result_sprite_zhuang.setVisible(isshow);

                cc.audioEngine.playEffect(v_sound_dians_list[value], false);

                var p_timer_id = setInterval( function() {
                    clearInterval(p_timer_id);

                    if (v_current_gaming_result == 0) {
                        ShowGameState(6, true);

                        cc.audioEngine.playEffect(s_sound_DragonY, false);
                    }
                    else if (v_current_gaming_result == 1) {
                        ShowGameState(5, true);

                        cc.audioEngine.playEffect(s_sound_Tigery, false);
                    }
                    else if (v_current_gaming_result == 2) {
                        ShowGameState(4, true);
                    }
                },2000);

            }, v_card_result_sprite_anim);
            var moveTo = cc.moveTo(0.2, v_card_result_sprite_zhuang.getPosition());
            var seq = cc.sequence(moveTo, showPosition);
            v_card_result_sprite_anim.runAction(seq);
        }
        else {
            v_card_result_sprite_zhuang.setVisible(isshow);
        }
    }
}

function EnableMouseProcess(isenable) {
    v_user_mouse_isprocess = isenable;
}

function ShowGameState(type,isshow) {
    if(type == 1) {
        v_game_waiting_sprite.setVisible(isshow);
    }
    else if(type == 2) {
        v_nomoney_xiazhu_sprite.setVisible(isshow);
    }
    else if(type == 3) {
        v_qingxiazhu_sprite.setVisible(isshow);
    }
    else if(type == 4) {
        v_hong_win_sprite.setVisible(isshow);
    }
    else if(type == 5) {
        v_hu_win_sprite.setVisible(isshow);
    }
    else if(type == 6) {
        v_long_win_sprite.setVisible(isshow);
    }

    if(isshow) {
        v_game_state_timer_id = setInterval( function(){
            clearInterval(v_game_state_timer_id);

            if(type == 1) {
                v_game_waiting_sprite.setVisible(!isshow);
            }
            else if(type == 2) {
                v_nomoney_xiazhu_sprite.setVisible(!isshow);
            }
            else if(type == 3) {
                v_qingxiazhu_sprite.setVisible(!isshow);
            }
            else if(type == 4) {
                v_hong_win_sprite.setVisible(!isshow);
            }
            else if(type == 5) {
                v_hu_win_sprite.setVisible(!isshow);
            }
            else if(type == 6) {
                v_long_win_sprite.setVisible(!isshow);
            }
        },2000);
    }
}

var CMainFrameLayer = cc.Layer.extend({
    bgsprite:null,
    v_timer_bg_sprite:null,
    s_100_btn:null,
    s_500_btn:null,
    s_1000_btn:null,
    s_5000_btn:null,
    s_10000_btn:null,
    s_50000_btn:null,

    init:function () {
        this._super();

        var size = cc.director.getWinSize();

        this.bgsprite = new cc.Sprite(s_bgsprite);
        this.bgsprite.setAnchorPoint(0.5, 0.5);
        this.bgsprite.setPosition(size.width / 2, size.height / 2);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(this.bgsprite, 0);

        this.v_timer_bg_sprite = new cc.Sprite(s_timer_bg_sprite);
        //this.v_timer_bg_sprite.setAnchorPoint(0.5, 0.5);
        this.v_timer_bg_sprite.setScale(1.2);
        this.v_timer_bg_sprite.setPosition(size.width/2+110,size.height/2-145);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(this.v_timer_bg_sprite, 0);

        v_GamingState_sprite_one = new cc.Sprite(s_GamingState_sprite1);
        //this.v_timer_bg_sprite.setAnchorPoint(0.5, 0.5);
        v_GamingState_sprite_one.setScaleX(0.65);
        v_GamingState_sprite_one.setScaleY(0.9);
        v_GamingState_sprite_one.setPosition(size.width/2+109,size.height/2-200);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_GamingState_sprite_one, 0);

        v_GamingState_sprite_two = new cc.Sprite(s_GamingState_sprite2);
        //this.v_timer_bg_sprite.setAnchorPoint(0.5, 0.5);
        v_GamingState_sprite_two.setScaleX(0.65);
        v_GamingState_sprite_two.setScaleY(0.9);
        v_GamingState_sprite_two.setPosition(size.width/2+109,size.height/2-200);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_GamingState_sprite_two, 0);

        v_GamingState_sprite_three = new cc.Sprite(s_GamingState_sprite3);
        //this.v_timer_bg_sprite.setAnchorPoint(0.5, 0.5);
        v_GamingState_sprite_three.setScaleX(0.65);
        v_GamingState_sprite_three.setScaleY(0.9);
        v_GamingState_sprite_three.setPosition(size.width/2+109,size.height/2-200);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_GamingState_sprite_three, 0);

        for(var i=0;i<52;i++) {
            var posX = parseInt(i / 4);
            var posY = parseInt(i % 4);

            v_game_record_sprite_list[i] = new cc.Sprite(s_gamerecord_sprite);
            v_game_record_sprite_list[i].setAnchorPoint(0, 1.0);
            v_game_record_sprite_list[i].setScale(0.77);
            v_game_record_sprite_list[i].setTextureRect(cc.rect(0,0,27,28));
            v_game_record_sprite_list[i].setPosition(size.width/2-350+posX*30,size.height/2-183+posY*23);
            v_game_record_sprite_list[i].setVisible(false);
            this.addChild(v_game_record_sprite_list[i], 0);
        }

        v_game_record_sprite_long_count = new cc.LabelTTF.create("0", "黑体", 13);
        // position the label on the center of the screen
        v_game_record_sprite_long_count.setPosition(size.width / 2-108, size.height/2-220);
        v_game_record_sprite_long_count.setColor(cc.color(255,255,0));
        v_game_record_sprite_long_count.setAnchorPoint(0.5, 0.5);
        // add the label as a child to this layer
        this.addChild(v_game_record_sprite_long_count,0);

        v_game_record_sprite_hu_count = new cc.LabelTTF.create("0", "黑体", 13);
        // position the label on the center of the screen
        v_game_record_sprite_hu_count.setPosition(size.width / 2-43, size.height/2-220);
        v_game_record_sprite_hu_count.setColor(cc.color(255,255,0));
        v_game_record_sprite_hu_count.setAnchorPoint(0.5, 0.5);
        // add the label as a child to this layer
        this.addChild(v_game_record_sprite_hu_count,0);

        v_game_record_sprite_hong_count = new cc.LabelTTF.create("0", "黑体", 13);
        // position the label on the center of the screen
        v_game_record_sprite_hong_count.setPosition(size.width / 2+22, size.height/2-220);
        v_game_record_sprite_hong_count.setColor(cc.color(255,255,0));
        v_game_record_sprite_hong_count.setAnchorPoint(0.5, 0.5);
        // add the label as a child to this layer
        this.addChild(v_game_record_sprite_hong_count,0);

        v_user_name_text = new cc.LabelTTF.create("test", "黑体", 12);
        // position the label on the center of the screen
        v_user_name_text.setPosition(size.width / 2-245, size.height/2+214);
        v_user_name_text.setColor(cc.color(255,255,255));
        v_user_name_text.setAnchorPoint(0.5, 0.5);
        // add the label as a child to this layer
        this.addChild(v_user_name_text,0);

        v_user_money_text = new cc.LabelTTF.create("0", "黑体", 12);
        // position the label on the center of the screen
        v_user_money_text.setPosition(size.width / 2-245, size.height/2+197);
        v_user_money_text.setColor(cc.color(255,255,255));
        v_user_money_text.setAnchorPoint(0.5, 0.5);
        // add the label as a child to this layer
        this.addChild(v_user_money_text,0);

        v_user_game_jushu = new cc.LabelTTF.create("0", "黑体", 12);
        // position the label on the center of the screen
        v_user_game_jushu.setPosition(size.width / 2+300, size.height/2+214);
        v_user_game_jushu.setColor(cc.color(255,255,255));
        v_user_game_jushu.setAnchorPoint(0.5, 0.5);
        // add the label as a child to this layer
        this.addChild(v_user_game_jushu,0);

        v_user_game_result = new cc.LabelTTF.create("0", "黑体", 12);
        // position the label on the center of the screen
        v_user_game_result.setPosition(size.width / 2+300, size.height/2+199);
        v_user_game_result.setColor(cc.color(255,255,255));
        v_user_game_result.setAnchorPoint(0.5, 0.5);
        // add the label as a child to this layer
        this.addChild(v_user_game_result,0);

        v_game_jetton_total_text = new cc.LabelTTF.create("0", "黑体", 22);
        // position the label on the center of the screen
        v_game_jetton_total_text.setPosition(size.width / 2-100, size.height/2-97);
        v_game_jetton_total_text.setColor(cc.color(255,255,0));
        v_game_jetton_total_text.setAnchorPoint(0, 0.5);
        // add the label as a child to this layer
        this.addChild(v_game_jetton_total_text,0);

        v_game_jetton_person_text = new cc.LabelTTF.create("0", "黑体", 22);
        // position the label on the center of the screen
        v_game_jetton_person_text.setPosition(size.width / 2+90, size.height/2-97);
        v_game_jetton_person_text.setColor(cc.color(255,255,0));
        v_game_jetton_person_text.setAnchorPoint(0, 0.5);
        // add the label as a child to this layer
        this.addChild(v_game_jetton_person_text,0);

        v_card_result_sprite_xian = new cc.Sprite(s_card_sprite);
        v_card_result_sprite_xian.setAnchorPoint(0.5, 0.5);
        v_card_result_sprite_xian.setScale(1.25);
        v_card_result_sprite_xian.setPosition(size.width / 2-116, size.height / 2-30);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_card_result_sprite_xian, 9000);

        v_card_result_sprite_zhuang = new cc.Sprite(s_card_sprite);
        v_card_result_sprite_zhuang.setAnchorPoint(0.5, 0.5);
        v_card_result_sprite_zhuang.setScale(1.25);
        v_card_result_sprite_zhuang.setPosition(size.width / 2+110, size.height / 2-30);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_card_result_sprite_zhuang, 9000);

        v_card_result_sprite_anim = new cc.Sprite(s_card_sprite);
        v_card_result_sprite_anim.setAnchorPoint(0.5, 0.5);
        v_card_result_sprite_anim.setScale(1.25);
        v_card_result_sprite_anim.setPosition(size.width / 2, size.height / 2+130);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        v_card_result_sprite_anim.setTextureRect(cc.rect(2*55,4*73,55,73));
        this.addChild(v_card_result_sprite_anim, 9000);

        v_gameover_bg_sprite = new cc.Sprite(s_gameover_bg_sprite);
        v_gameover_bg_sprite.setAnchorPoint(0.5, 0.5);
        v_gameover_bg_sprite.setPosition(size.width / 2, size.height / 2+20);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_gameover_bg_sprite, 9000);

        v_gameover_bg_lost_sprite = new cc.Sprite(s_gameover_bg_lost_sprite);
        v_gameover_bg_lost_sprite.setAnchorPoint(0.5, 0.5);
        v_gameover_bg_lost_sprite.setPosition(size.width / 2, size.height / 2+120);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_gameover_bg_lost_sprite, 9000);

        v_gameover_bg_win_sprite = new cc.Sprite(s_gameover_bg_win_sprite);
        v_gameover_bg_win_sprite.setAnchorPoint(0.5, 0.5);
        v_gameover_bg_win_sprite.setPosition(size.width / 2, size.height / 2+162);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_gameover_bg_win_sprite, 9000);

        // ;表示减 < 表示加
        v_number_sprite = new cc.LabelAtlas("<123223432",s_number_sprite,11,17,"0");
        v_number_sprite.setAnchorPoint(0.5, 0.5);
        v_number_sprite.setPosition(size.width / 2+20, size.height / 2-7);
        this.addChild(v_number_sprite, 9000);

        v_number_timer_sprite = new cc.LabelAtlas("6",s_timer_num_sprite,19,22,"0");
        v_number_timer_sprite.setAnchorPoint(0.5, 0.5);
        v_number_timer_sprite.setPosition(size.width/2+110,size.height/2-152);
        this.addChild(v_number_timer_sprite, 10);

        v_game_jetton_long_text = new cc.LabelAtlas("1000",s_timer_num_sprite,19,22,"0");
        v_game_jetton_long_text.setAnchorPoint(0.5, 0.5);
        v_game_jetton_long_text.setPosition(size.width/2-205,size.height/2-15);
        this.addChild(v_game_jetton_long_text, 3000);

        v_game_jetton_hu_text = new cc.LabelAtlas("2000",s_timer_num_sprite,19,22,"0");
        v_game_jetton_hu_text.setAnchorPoint(0.5, 0.5);
        v_game_jetton_hu_text.setPosition(size.width/2+190,size.height/2-15);
        this.addChild(v_game_jetton_hu_text, 3000);

        v_game_jetton_hong_text = new cc.LabelAtlas("3000",s_timer_num_sprite,19,22,"0");
        v_game_jetton_hong_text.setAnchorPoint(0.5, 0.5);
        v_game_jetton_hong_text.setPosition(size.width/2-10,size.height/2+140);
        this.addChild(v_game_jetton_hong_text, 3000);

        v_game_waiting_colorlayer = new cc.LayerColor(new cc.color(0, 0, 0, 200), size.width, size.height);
        v_game_waiting_colorlayer.setAnchorPoint(0.5, 0.5);
        v_game_waiting_colorlayer.setPosition(0,0);
        this.addChild(v_game_waiting_colorlayer, 19000);

        v_game_waiting_sprite = new cc.Sprite(s_game_waiting_sprite);
        v_game_waiting_sprite.setAnchorPoint(0.5, 0.5);
        //s_game_waiting_sprite.setScale(1.25);
        v_game_waiting_sprite.setPosition(size.width / 2, size.height / 2);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_game_waiting_sprite, 19000);

        v_nomoney_xiazhu_sprite = new cc.Sprite(s_nomoney_xiazhu_sprite);
        v_nomoney_xiazhu_sprite.setAnchorPoint(0.5, 0.5);
        //s_game_waiting_sprite.setScale(1.25);
        v_nomoney_xiazhu_sprite.setPosition(size.width / 2, size.height / 2);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_nomoney_xiazhu_sprite, 19000);

        v_qingxiazhu_sprite = new cc.Sprite(s_qingxiazhu_sprite);
        v_qingxiazhu_sprite.setAnchorPoint(0.5, 0.5);
        //s_game_waiting_sprite.setScale(1.25);
        v_qingxiazhu_sprite.setPosition(size.width / 2, size.height / 2);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_qingxiazhu_sprite, 19000);

        v_hong_win_sprite = new cc.Sprite(s_hong_win_sprite);
        v_hong_win_sprite.setAnchorPoint(0.5, 0.5);
        //s_game_waiting_sprite.setScale(1.25);
        v_hong_win_sprite.setPosition(size.width / 2, size.height / 2);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_hong_win_sprite, 19000);

        v_hu_win_sprite = new cc.Sprite(s_hu_win_sprite);
        v_hu_win_sprite.setAnchorPoint(0.5, 0.5);
        //s_game_waiting_sprite.setScale(1.25);
        v_hu_win_sprite.setPosition(size.width / 2, size.height / 2);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_hu_win_sprite, 19000);

        v_long_win_sprite = new cc.Sprite(s_long_win_sprite);
        v_long_win_sprite.setAnchorPoint(0.5, 0.5);
        //s_game_waiting_sprite.setScale(1.25);
        v_long_win_sprite.setPosition(size.width / 2, size.height / 2);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(v_long_win_sprite, 19000);

        v_sound_dians_list[0] = s_sound_1_dian;
        v_sound_dians_list[1] = s_sound_2_dian;
        v_sound_dians_list[2] = s_sound_3_dian;
        v_sound_dians_list[3] = s_sound_4_dian;
        v_sound_dians_list[4] = s_sound_5_dian;
        v_sound_dians_list[5] = s_sound_6_dian;
        v_sound_dians_list[6] = s_sound_7_dian;
        v_sound_dians_list[7] = s_sound_8_dian;
        v_sound_dians_list[8] = s_sound_9_dian;
        v_sound_dians_list[9] = s_sound_10_dian;
        v_sound_dians_list[10] = s_sound_11_dian;
        v_sound_dians_list[11] = s_sound_12_dian;
        v_sound_dians_list[12] = s_sound_13_dian;

        v_gameover_bg_sprite.setVisible(false);
        v_gameover_bg_lost_sprite.setVisible(false);
        v_gameover_bg_win_sprite.setVisible(false);
        v_number_sprite.setVisible(false);
        v_game_waiting_colorlayer.setVisible(false);
        v_GamingState_sprite_two.setVisible(false);
        v_GamingState_sprite_three.setVisible(false);
        v_card_result_sprite_xian.setTextureRect(cc.rect(0,0,55,73));
        v_card_result_sprite_zhuang.setTextureRect(cc.rect(0,0,55,73));
        v_card_result_sprite_xian.setVisible(false);
        v_card_result_sprite_zhuang.setVisible(false);
        v_card_result_sprite_anim.setVisible(false);
        v_game_jetton_long_text.setVisible(false);
        v_game_jetton_hu_text.setVisible(false);
        v_game_jetton_hong_text.setVisible(false);
        v_nomoney_xiazhu_sprite.setVisible(false);
        v_game_waiting_sprite.setVisible(false);
        v_qingxiazhu_sprite.setVisible(false);
        v_hong_win_sprite.setVisible(false);
        v_hu_win_sprite.setVisible(false);
        v_long_win_sprite.setVisible(false);

        this.s_100_btn = new cc.MenuItemImage.create(
            s_100_1_sprite,
            s_100_2_sprite,
            s_100_3_sprite,
            MenuItemProcess,this);
        this.s_500_btn = new cc.MenuItemImage.create(
            s_500_1_sprite,
            s_500_2_sprite,
            s_500_3_sprite,
            MenuItemProcess,this);
        this.s_1000_btn = new cc.MenuItemImage.create(
            s_1000_1_sprite,
            s_1000_2_sprite,
            s_1000_3_sprite,
            MenuItemProcess,this);
        this.s_5000_btn = new cc.MenuItemImage.create(
            s_5000_1_sprite,
            s_5000_2_sprite,
            s_5000_3_sprite,
            MenuItemProcess,this);
        this.s_10000_btn = new cc.MenuItemImage.create(
            s_10000_1_sprite,
            s_10000_2_sprite,
            s_10000_3_sprite,
            MenuItemProcess,this);
        this.s_50000_btn = new cc.MenuItemImage.create(
            s_50000_1_sprite,
            s_50000_2_sprite,
            s_50000_3_sprite,
            MenuItemProcess,this);

        var menu = new cc.Menu.create(this.s_100_btn,this.s_500_btn,this.s_1000_btn,this.s_5000_btn,this.s_10000_btn,this.s_50000_btn);
        menu.setPosition(0, 0);
        this.addChild(menu, 1);

        this.s_100_btn.setTag(100);
        this.s_100_btn.setScale(1.2);
        this.s_100_btn.setPosition(size.width / 2+205, size.height / 2-141);
        this.s_500_btn.setTag(500);
        this.s_500_btn.setScale(1.2);
        this.s_500_btn.setPosition(size.width / 2+272, size.height / 2-141);
        this.s_1000_btn.setTag(1000);
        this.s_1000_btn.setScale(1.2);
        this.s_1000_btn.setPosition(size.width / 2+339, size.height / 2-141);
        this.s_5000_btn.setTag(5000);
        this.s_5000_btn.setScale(1.2);
        this.s_5000_btn.setPosition(size.width / 2+205, size.height / 2-195);
        this.s_10000_btn.setTag(10000);
        this.s_10000_btn.setScale(1.2);
        this.s_10000_btn.setPosition(size.width / 2+272, size.height / 2-195);
        this.s_50000_btn.setTag(50000);
        this.s_50000_btn.setScale(1.2);
        this.s_50000_btn.setPosition(size.width / 2+339, size.height / 2-195);

        cc.audioEngine.playMusic(s_sound_BGM_Room_4, true);

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

var CLunPanScene = cc.Scene.extend({
    onEnter:function () {
        this._super();

        m_MainFrameLayer = new CMainFrameLayer();
        this.addChild(m_MainFrameLayer);
        m_MainFrameLayer.init();
    }
});

function MenuItemProcess(obj) {
    if(v_user_mouse_isprocess == false)
        return;

    m_MenuItemObjTag = obj.getTag();
};

function GamingJetton(areaIndex,m_MenuItemObjTag) {
    var size = cc.director.getWinSize();

    var tmpDecPos = [];

    tmpDecPos[0] = {x:size.width/2-210, y:size.height/2-40};
    tmpDecPos[1] = {x:size.width/2+170, y:size.height/2-40};
    tmpDecPos[2] = {x:size.width/2, y:size.height/2+100};

    var tmpPos = getCirclePosition3(Math.round(Math.random()*360),Math.round(Math.random()*50),tmpDecPos[areaIndex],false);

    var jettonsprite = new cc.Sprite(s_jetton_sprite);
    jettonsprite.setAnchorPoint(0, 0);
    jettonsprite.setScale(0.5);

    if(m_MenuItemObjTag == 100) jettonsprite.setTextureRect(cc.rect(0,0,52,55));
    else if(m_MenuItemObjTag == 500) jettonsprite.setTextureRect(cc.rect(52.0,0.0,52.0,55.0));
    else if(m_MenuItemObjTag == 1000) jettonsprite.setTextureRect(cc.rect(52.0*2.0,0.0,52.0,55.0));
    else if(m_MenuItemObjTag == 5000) jettonsprite.setTextureRect(cc.rect(52.0*3.0,0.0,52.0,55.0));
    else if(m_MenuItemObjTag == 10000) jettonsprite.setTextureRect(cc.rect(52.0*4.0,0.0,52.0,55.0));
    else if(m_MenuItemObjTag == 50000) jettonsprite.setTextureRect(cc.rect(52.0*5.0,0.0,52.0,55.0));

    jettonsprite.setPosition(tmpPos);
    m_MainFrameLayer.addChild(jettonsprite, 1000);

    m_jettonspritelist[m_jettonspritelistcount]=jettonsprite;
    m_jettonspritelistcount=m_jettonspritelistcount+1;

    if(areaIndex == 0) {
        v_game_jetton_long_total = v_game_jetton_long_total + m_MenuItemObjTag;
        v_game_jetton_long_text.setVisible(true);
        v_game_jetton_long_text.setString(v_game_jetton_long_total);
    }
    else if(areaIndex == 1) {
        v_game_jetton_hu_total = v_game_jetton_hu_total + m_MenuItemObjTag;
        v_game_jetton_hu_text.setVisible(true);
        v_game_jetton_hu_text.setString(v_game_jetton_hu_total);
    }
    else {
        v_game_jetton_hong_total = v_game_jetton_hong_total + m_MenuItemObjTag;
        v_game_jetton_hong_text.setVisible(true);
        v_game_jetton_hong_text.setString(v_game_jetton_hong_total);
    }
}

function MenuItemMousePro(touch,event) {
    if(v_user_mouse_isprocess == false)
        return;

    //获取当前事件触发对象
    var target = event.getCurrentTarget();

    //将触发点转换为基于当前事件触发对象的本地坐标
    var posInNode = target.convertToNodeSpace(touch.getLocation());

    //获取当前事件触发对象的大小
    var size = target.getContentSize();

    //区域（一般为需要拖动对象的大小范围）
    var rect = cc.rect(0,0,size.width,size.height);

    //当前触摸点是否在rect范围内
    var boolRet =cc.rectContainsPoint(rect,posInNode);

    if(m_MenuItemObjTag == 0)
        return boolRet;

    if(m_myselfusermoney - m_MenuItemObjTag < 0) {
        ShowGameState(2,true);
        return;
    }

    var tmpDecPos = [];

    tmpDecPos[0] = {x:size.width/2-210, y:size.height/2-40};
    tmpDecPos[1] = {x:size.width/2+170, y:size.height/2-40};
    tmpDecPos[2] = {x:size.width/2, y:size.height/2+100};

    for(var index=0;index<3;index++) {
        if(getDistance(posInNode,tmpDecPos[index]) < 60) {

            GamingJetton(index,m_MenuItemObjTag);

            var row1 = {};
            row1.MsgId = 1000;
            row1.MsgSubId = 1002;
            row1.CardType = index;
            row1.JettonMoney = m_MenuItemObjTag;

            m_gameserversocket.send(JSON.stringify(row1));

            m_myselfusermoney = m_myselfusermoney - m_MenuItemObjTag;
            v_user_money_text.setString(m_myselfusermoney);

            cc.audioEngine.playEffect(s_sound_AddGold, false);

            break;
        }
    }

    //在则让事件继续往下走；不在则将事件吞噬，下面移动和结束将不会调用。
    return boolRet;
};




