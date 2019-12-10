var m_gameserversocket = null;
var last_health_game = null;
var health_timeout_game=6000;
var keepalivetimer_game = null;
var m_GameState = 0;
var m_timegamestart = 0;
var m_timexiazhu = 0;
var m_timekaipai = 0;
var m_timejiesuan = 0;

function keepalive_game(ws) {
    var time = new Date();
    var curtime = last_health_game.getTime();

    if(time.getTime() - curtime > health_timeout_game)
    {
        clearInterval(keepalivetimer_game);
    }
    else
    {
        if (ws.bufferedAmount == 0) {
            ws.send('100');

            last_health_game = time;
        }
    }
};

function StartGamingTimer() {
    if(m_GameState == 0) {
        starttimer(m_timexiazhu);
    }
    else if(m_GameState == 1) {
        starttimer(m_timekaipai);
    }
    else if(m_GameState == 2) {
        starttimer(m_timejiesuan);
    }
    else {
        starttimer(m_timegamestart);
    }
}

function OnProcessGameSocketOnOpen() {
    m_gameserversocket.send('100');

    last_health_game = new Date();
    clearInterval(keepalivetimer_game);
    keepalivetimer_game = setInterval( function(){keepalive_game(m_gameserversocket)},1000);
}

function ShowGameStateImage() {
    if(m_GameState == 0) {
        v_GamingState_sprite_one.setVisible(false);
        v_GamingState_sprite_two.setVisible(true);
        v_GamingState_sprite_three.setVisible(false);
    }
    else if(m_GameState == 1) {
        v_GamingState_sprite_one.setVisible(false);
        v_GamingState_sprite_two.setVisible(false);
        v_GamingState_sprite_three.setVisible(true);
    }
    else  {
        v_GamingState_sprite_one.setVisible(true);
        v_GamingState_sprite_two.setVisible(false);
        v_GamingState_sprite_three.setVisible(false);
    }
}

function OnProcessGameSocketOnMessage(msg) {
    var str = "";
    str = msg.data;

    console.info(str);

    var objgame = eval('(' + str + ')');

    switch(objgame.MsgId) {
        case 300: {
            if (objgame.MsgSubId == 301) {
                var row1 = {};
                row1.MsgId = 500;
                row1.UserName = m_userloginname_str;
                row1.UserPW = m_userloginpassword_str;
                row1.DeviceType = 1;
                m_gameserversocket.send(JSON.stringify(row1));
            }
        }
            break;
        case 500:
        {
            switch(objgame.MsgSubId)
            {
                case 502:
                {
                    var msglayer = new MessageBoxLayer();
                    m_MainFrameLayer.addChild(msglayer,800);
                    msglayer.init("游戏服务器登录失败，请联系客服人员！");
                }
                    break;
                case 505:
                {
                    var msglayer = new MessageBoxLayer();
                    m_MainFrameLayer.addChild(msglayer,800);
                    msglayer.init("游戏服务器满员，请联系客服人员！");
                }
                    break;
                case 504:
                {
                    var msglayer = new MessageBoxLayer();
                    m_MainFrameLayer.addChild(msglayer,800);
                    msglayer.init("您已经在游戏中2，请稍后再试！"+objgame.GameState);
                }
                    break;
                case 501:
                {
                    if(objgame.ID == m_myselfUserId){
                        var row1 = {};
                        row1.MsgId = 900;
                        row1.MsgSubId=901;
                        row1.RoomIndex = -1;
                        row1.ChairIndex = -1;
                        row1.EnterPWd = "";
                        row1.Enterfirst=0;
                        row1.Entersecond=0;
                        m_gameserversocket.send(JSON.stringify(row1));
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
            if(objgame.MsgSubId == 901) {
                switch(objgame.MsgSubId2) {
                    case 920: {
                        var msglayer = new MessageBoxLayer();
                        m_MainFrameLayer.addChild(msglayer,800);
                        msglayer.init("您已经在游戏中3，请稍后再试！");
                    }
                        break;
                    case 912: {
                        var msglayer = new MessageBoxLayer();
                        m_MainFrameLayer.addChild(msglayer,800);
                        msglayer.init("进入房间失败，请稍后再试！");
                    }
                        break;
                    case 914: {
                        var msglayer = new MessageBoxLayer();
                        m_MainFrameLayer.addChild(msglayer,800);
                        msglayer.init("金币不够，请先充值！");
                    }
                        break;
                    default:
                        break;
                }
            }
            else if(objgame.MsgSubId == 908) {
                if(objgame.ID == m_myselfUserId) {
                    m_myselfusermoney = objgame.Money;
                    v_user_money_text.setString(m_myselfusermoney);
                }
            }
        }
            break;
        case 1000: {
            switch (objgame.MsgSubId) {
                case 1001: {
                    m_GameState = objgame.GameState;
                    v_current_gaming_result = objgame.curGamingResult;
                    v_card_one_value = objgame.cardone;
                    v_card_two_value = objgame.cardtwo;

                    cc.audioEngine.playEffect(s_sound_tingzhixiazhu, false);

                    showcard(1,v_card_one_value,true);
                    EnableMouseProcess(false);
                    ShowGameStateImage();
                    StartGamingTimer();
                }
                    break;
                case 1002: {
                    var pplayerid = objgame.playerid;
                    var pCarType = objgame.CarType;
                    var pMoney = objgame.Money;

                    if(m_myselfUserId == pplayerid) {
                        v_game_user_jetton_total = v_game_user_jetton_total + pMoney;
                    }
                    else {
                        GamingJetton(pCarType,pMoney);
                    }

                    v_game_alluser_jetton_total = v_game_alluser_jetton_total + pMoney;

                    showusergamingjettons();
                }
                    break;
                case 1003: {
                    m_GameState = objgame.GameState;
                    var usergamingresult = objgame.Result;

                    v_game_jetton_total = v_game_jetton_total + usergamingresult;

                    addvaluetovector(v_current_gaming_result);

                    v_card_result_sprite_xian.setVisible(false);
                    v_card_result_sprite_zhuang.setVisible(false);

                    if(usergamingresult > 0) {
                        cc.audioEngine.playEffect(s_sound_EndWin, false);
                    }
                    else if(usergamingresult < 0) {
                        cc.audioEngine.playEffect(s_sound_EndLost, false);
                    }
                    else {
                        cc.audioEngine.playEffect(s_sound_EndDraw, false);
                    }

                    showgameresult(usergamingresult,true);
                    ShowGamingResult();
                    ShowGameStateImage();
                    v_user_game_result.setString(v_game_jetton_total);
                    StartGamingTimer();
                }
                    break;
                case 1004: {
                    OnProcessGameUserEnterRoom(objgame);
                }
                    break;
                case 1005: {
                    m_GameState = objgame.GameState;
                    var curStrJvHao = objgame.curStrJvHao;

                    v_user_game_jushu.setString(curStrJvHao);
                    v_game_waiting_sprite.setVisible(false);
                    v_game_waiting_colorlayer.setVisible(false);

                    v_game_user_jetton_total = 0;
                    v_game_alluser_jetton_total = 0;
                    clearalljettonsprites();
                    showgameresult(0,false);
                    showusergamingjettons();
                    ShowGameStateImage();
                    EnableMouseProcess(true);
                    ShowGameState(3,true);
                    StartGamingTimer();

                    cc.audioEngine.playEffect(s_sound_GameStart, false);
                }
                    break;
                case 1006: {
                    OnProcessGameUserEnterRoom(objgame);

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

function OnProcessGameSocketOnClose() {
    var msglayer = new MessageBoxLayer();
    m_MainFrameLayer.addChild(msglayer,18000);
    msglayer.init("游戏服务器连接失败，请稍后再试！");
}

function OnProcessGameUserEnterRoom(objgame) {
    m_GameState = objgame.GameState;
    m_timegamestart = objgame.timegamestart;
    m_timexiazhu = objgame.timexiazhu;
    m_timekaipai = objgame.timekaipai;
    m_timejiesuan = objgame.timejiesuan;
    var curStrJvHao = objgame.curStrJvHao;
    var gamerecords = objgame.gamerecords;

    for(var value of gamerecords) {
        var pcardtype = value.cardtype;

        addvaluetovector(pcardtype);
    }

    ShowGamingResult();

    v_game_jetton_total = 0;

    v_user_game_jushu.setString(curStrJvHao);
    v_user_game_result.setString(v_game_jetton_total);
    v_game_waiting_sprite.setVisible(true);
    v_game_waiting_colorlayer.setVisible(true);

    ShowGameStateImage();
    StartGamingTimer();
}