$(function() {
    $(".search-box .butn").on('click', function() {
        if ($(this).hasClass('hov')) {
            $(this).removeClass('hov');
            $(this).siblings('.share-sub').stop().animate({
                width: 288
            }, 500);

        } else {
            $(this).addClass('hov');
            $(this).siblings('.share-sub').stop().animate({
                width: 0
            }, 500);

        }
    });

    $('.case_list').on('mouseenter','li',function(){
        $(this).addClass('hover');
    }).on('mouseleave','li',function(){
        $(this).removeClass('hover');
    });




})
jQuery(".main_case").slide({titCell:".hd ul",mainCell:".case_list ul",autoPage:true,effect:"left",autoPlay:true,vis:3});
/* 控制左右按钮显示 */
jQuery(".fullSlide").hover(function() { jQuery(this).find(".prev,.next").stop(true, true).fadeTo("show", 0.5) }, function() { jQuery(this).find(".prev,.next").fadeOut() });
/* 调用SuperSlide */
jQuery(".fullSlide").slide({
    titCell: ".hd ul",
    mainCell: ".bd ul",
    effect: "fold",
    autoPlay: true,
    autoPage: true,
    trigger: "click",
    startFun: function(i) {
        var curLi = jQuery(".fullSlide .bd li").eq(i); /* 当前大图的li */
        if (!!curLi.attr("_src")) {
            curLi.css("background-image", curLi.attr("_src")).removeAttr("_src") /* 将_src地址赋予li背景，然后删除_src */
        }
    }
});