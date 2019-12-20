 //desktop
var isMobile=navigator.userAgent.match(/(iPad)|(iPhone)|(iPod)|(android)|(webOS)/i) !=null;

var isiPad = navigator.userAgent.match(/iPad/i) != null; 

var isiPhone = navigator.userAgent.match(/iPhone|iPod/i) != null;  

//baidu
var isBaidu = navigator.userAgent.match('baidu') != null;

//QQ browser
var isQQ = navigator.userAgent.match('MQQBrowser') != null;

//Saferi Browser
var isFirefox = navigator.userAgent.match('FxiOS') != null;

// UC Browser
var  isUC = navigator.userAgent.indexOf("UCBrowser") != -1;

// Chrome 1+
var isChrome = navigator.userAgent.match('CriOS') != null;

//xiaomi
var isXiaomi = navigator.userAgent.match('XiaoMi') != null;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!!!!!

//isSafari 变量在CocosCreator源码中用到不能随便删除！！！！！！！！！！

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!!!!!
// Safari 3.0+ "[object HTMLElementConstructor]"
var isSafari = navigator.userAgent.match('Safari') && !isBaidu && !isFirefox && !isQQ && !isChrome && !isUC && !isXiaomi;

var isAndroid = /android/i.test(navigator.userAgent || navigator.vendor || window.opera);

var iOS = /iPad|iPhone|iPod/.test(navigator.userAgent) && !window.MSStream;


var maxPorHet = 0;
var maxLanHet = 0;

/**
 * [requestFullScreen 进入全屏]
 */
function requestFullScreen() {
    var de = document.documentElement;
    if (de.requestFullscreen) {
        de.requestFullscreen();
    } else if (de.mozRequestFullScreen) {
        de.mozRequestFullScreen();
    }else if(de.webkitRequestFullScreen){
            de.webkitRequestFullScreen();
    }else if(de.msRequestFullscreen){
       de.msRequestFullscreen();
    }

   orientationswipe.style.display = "none";
}

/**
 * [isFullscreenEnabled 判断全屏模式是否是可用]
 * @return [支持则返回true,不支持返回false]
 */
function isFullscreenEnabled(){
    return document.fullscreenEnabled       ||
           document.mozFullScreenEnabled    || false;
}

var bodyTag = document.getElementsByTagName('body')[0];
var orientationswipe = null;
function displaySwipe() {
    orientationswipe.style.display = "block";
    var isFullScreen =  window.navigator.standalone | false;
    if(!isFullScreen){
        bodyTag.style.height = '150%';
    }
}

function setOrientationSize(maxHet) {
    if(window.innerHeight >= maxHet){

        if(window.innerHeight > maxHet*1.01) {
            maxHet = window.innerHeight;
        }
        if(isSafari) {
            setTimeout(function(){
                if(window.scrollY > 0) {
                    window.scrollTo(0,0);
                }
            },200);
        } else {
            bodyTag.style.height = '100%';
            if(window.scrollY > 0) {
                setTimeout(function(){window.scrollBy(0,-10);},1000);
            }
        }
        // cmkj.EventCtrl.sendDataEvent("test", window.innerHeight + " v2 " + maxHet + " " + maxPorHet + " " + maxLanHet);
        
    }else {
        displaySwipe();
    }

    return maxHet;
}


function setOnOriention() {
    if(isSafari) {
        setTimeout(function(){
            if(window.scrollY > 0) {
                window.scrollTo(0,0);
            }
        },50);
    }
    setTimeout(function(){
        if(window.orientation == 0 || window.orientation == 180){
            maxPorHet = setOrientationSize(maxPorHet);
        }
        if(window.orientation == 90 || window.orientation == -90){
            maxLanHet = setOrientationSize(maxLanHet);
        }
    }, 500)
}

//safari第一次会自动滑动一下。
var isFirstScroll = true;
function setOnScroll() {
    //if(isFirstScroll) {
    //    isFirstScroll = null;
    //    return;
    //}
    if(window.orientation == 0 || window.orientation == 180){
        maxPorHet = setOrientationSize(maxPorHet);
    }
    if(window.orientation == 90 || window.orientation == -90){
        maxLanHet = setOrientationSize(maxLanHet);
    }
}

function setOnResize() {
    setTimeout(function(){
        if(window.orientation == 0 || window.orientation == 180){
            maxPorHet = setOrientationSize(maxPorHet);
        }
        if(window.orientation == 90 || window.orientation == -90){
            maxLanHet = setOrientationSize(maxLanHet);
        }
    }, 500)
}

window.addEventListener('load', function() {
    orientationswipe = document.getElementById("orientationswipe");
    if(window.orientation == 0 || window.orientation == 180){
        displaySwipe();
        maxPorHet = window.innerHeight;
    }
    if(window.orientation == 90 || window.orientation == -90){
        displaySwipe();
        maxLanHet = window.innerHeight;
    }

    window.addEventListener('orientationchange', function () {
        setOnOriention();
    });
    if(window.addEventListener){
        window.addEventListener('scroll', setOnScroll, false);
        window.addEventListener("resize", setOnResize, false);
    } else {
        window.attachEvent('onscroll', setOnScroll);
        window.attachEvent('onresize', setOnResize);
    }

    if(isFullscreenEnabled()) {
        orientationswipe.style.display = "none";
    } else {
        orientationswipe.style.backgroundImage = "url(swipe2.gif)";
    }
}, false);
