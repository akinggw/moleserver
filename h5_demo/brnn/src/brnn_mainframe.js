var m_MainFrameLayer = null;

var CMainFrameLayer = cc.Layer.extend({

    init:function () {
        this._super();

        var size = cc.director.getWinSize();

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

