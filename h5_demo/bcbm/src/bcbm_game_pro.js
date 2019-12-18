var m_gameserversocket = null;
var last_health_game = null;
var health_timeout_game=6000;
var keepalivetimer_game = null;
var m_GameState = 0;
var m_timegamestart = 0;
var m_timexiazhu = 0;
var m_timekaipai = 0;
var m_timejiesuan = 0;
var m_timecurlost = 0;
var m_CurrentZhuang = 0;
var m_TotalShangZhuangCount = 0;
var m_CurShangZhuangCount = 0;
var m_ZhuangTotalWinResult = 0;
var m_curGamingResult = null;
var m_usertotalGamingResult = 0;
var m_allusertotalJettons = 0;
var m_meusertotalJettons = 0;

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
                        msglayer.init("您已经在游戏中，请稍后再试！");
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
                    case 913: {
                        var pplayerid = objgame.ID;
                        var pRoomId = objgame.RoomId;
                        var pChairIndex = objgame.ChairIndex;

                        if(m_myselfUserId == pplayerid) {
                            m_myselfUserRoomId = pRoomId;
                            m_myselfUserChairIndex = pChairIndex;
                        }
                    }
                        break;
                    default:
                        break;
                }
            }
            else if(objgame.MsgSubId == 918) {
                if(objgame.ID == m_myselfUserId) {
                    m_myselfusermoney = objgame.Money;
                    m_labelMoney.setString(m_myselfusermoney);
                }
            }
        }
            break;
        case 1000: {
            switch (objgame.MsgSubId) {
                case 1001:
                {
                    m_GameState = objgame.GameState;
                    m_curGamingResult = objgame.curGamingResult;

                    ShowGamingState(m_GameState);
                    EnableMouseProcess(false);
                    StartGamingTimer();

                    StartAnimation(m_curGamingResult);
                }
                    break;
                case 1002:
                {
                    var pplayerId = objgame.playerId;
                    var pCardType = objgame.JettonArea;
                    var pMoney = objgame.JettonMoney;

                    if(m_myselfUserChairIndex == pplayerId) {
                        m_meusertotalJettons = m_meusertotalJettons + pMoney;
                    }
                    else {
                        GamingJetton(pCardType,pMoney,false);
                    }

                    m_allusertotalJettons = m_allusertotalJettons + pMoney;

                    m_labelallJettons.setString(m_allusertotalJettons);
                    m_labelCurJetton.setString(m_meusertotalJettons);
                }
                    break;
                case 1003:
                {
                    m_curGamingResult = objgame.curGamingResult;
                    var pusertotalmoney = objgame.OldTotalResult2;

                    m_usertotalGamingResult = m_usertotalGamingResult + pusertotalmoney;

                    if(pusertotalmoney > 0) {
                        cc.audioEngine.playEffect(v_sound_EndWin, false);
                    }
                    else if(pusertotalmoney < 0) {
                        cc.audioEngine.playEffect(v_sound_EndLost, false);
                    }
                    else {
                        cc.audioEngine.playEffect(v_sound_EndDraw, false);
                    }

                    clearAnimation();
                    addvaluetogamerecord(m_curGamingResult);
                    ShowGamingRecords();
                    showgameresult(pusertotalmoney,true);
                    m_labelTotalResult.setString(m_usertotalGamingResult);
                }
                    break;
                case 1005:
                {
                    m_GameState = objgame.GameState;
                    m_CurrentZhuang = objgame.CurrentZhuang;
                    m_TotalShangZhuangCount = objgame.TotalShangZhuangCount;
                    m_CurShangZhuangCount = objgame.CurShangZhuangCount;
                    m_ZhuangTotalWinResult = objgame.ZhuangTotalWinResult;

                    m_allusertotalJettons = 0;
                    m_meusertotalJettons = 0;

                    m_labelallJettons.setString(m_allusertotalJettons);
                    m_labelCurJetton.setString(m_meusertotalJettons);

                    cc.audioEngine.playEffect(v_sound_GameStart, false);

                    ClearGamingJettons();
                    showgameresult(0,false);
                    ShowGameState(3,true);
                    ShowGamingState(m_GameState);
                    EnableMouseProcess(true);
                    StartGamingTimer();
                }
                    break;
                case 1006:
                case 1004:
                {
                    m_GameState = objgame.GameState;
                    m_timegamestart = objgame.timegamestart;
                    m_timexiazhu = objgame.timexiazhu;
                    m_timekaipai = objgame.timekaipai;
                    m_timejiesuan = objgame.timejiesuan;
                    m_timecurlost = objgame.timecurlost;
                    m_CurrentZhuang = objgame.CurrentZhuang;
                    m_TotalShangZhuangCount = objgame.TotalShangZhuangCount;
                    m_CurShangZhuangCount = objgame.CurShangZhuangCount;
                    m_ZhuangTotalWinResult = objgame.ZhuangTotalWinResult;
                    var GameRecords = objgame.GameRecords;

                    if(GameRecords != null) {
                        for(var value of GameRecords) {
                            addvaluetogamerecord(value.decCardType);
                        }

                        ShowGamingRecords();
                    }

                    ShowGameState(1,true);
                    ShowGamingState(m_GameState);
                    StartGamingTimer();
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
