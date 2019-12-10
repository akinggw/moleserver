cc.game.onStart = function(){
   // if(!cc.sys.isNative && document.getElementById("cocosLoading")) //If referenced loading.js, please remove it
   //     document.body.removeChild(document.getElementById("cocosLoading"));

    var designSize = cc.size(800,480);
    var screenSize = cc.view.getFrameSize();

    if(!cc.sys.isNative && screenSize.height < 800){
        designSize = cc.size(480,320);
        cc.loader.resPath = "res";
    }else{
        cc.loader.resPath = "res";
    }

    cc.view.adjustViewPort(true);
    cc.view.enableAutoFullScreen(true);
    cc.view.setDesignResolutionSize(designSize.width, designSize.height, cc.ResolutionPolicy.SHOW_ALL);
    //cc.view.enableRetina(false);
    cc.view.resizeWithBrowserSize(true);

    //load resources
    cc.LoaderScene.preload(g_resources, function () {
        cc.director.runScene(new CSszpScene());
    }, this);
};
cc.game.run();

/*cc.game.onStart = function(){
    cc.view.enableRetina(false);
    cc.view.adjustViewPort(true);
    cc.view.setDesignResolutionSize(480,720,cc.ResolutionPolicy.SHOW_ALL);
    cc.view.resizeWithBrowserSize(true);
    cc.director.setProjection(cc.Director.PROJECTION_2D);

    if (cc.sys.isNative) {
        var searchPaths = jsb.fileUtils.getSearchPaths();
        searchPaths.push('script');
        if (cc.sys.os == cc.sys.OS_IOS || cc.sys.os == cc.sys.OS_OSX) {
            searchPaths.push("res");
            searchPaths.push("src");
        }
        jsb.fileUtils.setSearchPaths(searchPaths);
    }
    //load resources
    cc.LoaderScene.preload(g_mainmenu, function () {
        cc.director.runScene(SysMenu.scene());
    }, this);
};

cc.game.run();*/