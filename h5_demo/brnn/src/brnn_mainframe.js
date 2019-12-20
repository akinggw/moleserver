var m_MainFrameLayer = null;

var CMainFrameLayer = cc.Layer.extend({
    bgsprite:null,

    init:function () {
        this._super();

        var size = cc.director.getWinSize();

        this.bgsprite = new cc.Sprite(v_game_sprite_game_bg);
        this.bgsprite.setAnchorPoint(0.5, 0.5);
        this.bgsprite.setPosition(size.width / 2, size.height / 2);
        //this.bgsprite.setScale(size.height / this.sprite.getContentSize().height);
        this.addChild(this.bgsprite, 0);
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

