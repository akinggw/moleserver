/**
 * Layui图标选择器
 * @author wujiawei0926@yeah.net
 * @version 1.1
 */

layui.define(['laypage', 'form'], function(exports) {
    "use strict";

    var IconPicker = function() {
            this.v = '1.1';
        },
        _MOD = 'iconPicker',
        _this = this,
        $ = layui.jquery,
        laypage = layui.laypage,
        form = layui.form,
        BODY = 'body',
        TIPS = '请选择图标';

    /**
     * 渲染组件
     */
    IconPicker.prototype.render = function(options) {
        var opts = options,
            // DOM选择器
            elem = opts.elem,
            // 数据类型：fontClass/unicode
            type = opts.type == null ? 'fontClass' : opts.type,
            // 是否分页：true/false
            page = opts.page == null ? true : opts.page,
            // 每页显示数量
            limit = opts.limit == null ? 12 : opts.limit,
            // 是否开启搜索：true/false
            search = opts.search == null ? true : opts.search,
            // 每个图标格子的宽度：'43px'或'20%'
            cellWidth = opts.cellWidth,
            // 点击回调
            click = opts.click,
            // 渲染成功后的回调
            success = opts.success,
            // json数据
            data = {},
            // 唯一标识
            tmp = new Date().getTime(),
            // 是否使用的class数据
            isFontClass = opts.type === 'fontClass',
            // 初始化时input的值
            ORIGINAL_ELEM_VALUE = $(elem).val(),
            TITLE = 'layui-select-title',
            TITLE_ID = 'layui-select-title-' + tmp,
            ICON_BODY = 'layui-iconpicker-' + tmp,
            PICKER_BODY = 'layui-iconpicker-body-' + tmp,
            PAGE_ID = 'layui-iconpicker-page-' + tmp,
            LIST_BOX = 'layui-iconpicker-list-box',
            selected = 'layui-form-selected',
            unselect = 'layui-unselect';

        var a = {
            init: function() {
                data = common.getData[type]();

                a.hideElem().createSelect().createBody().toggleSelect();
                a.preventEvent().inputListen();
                common.loadCss();

                if (success) {
                    success(this.successHandle());
                }

                return a;
            },
            successHandle: function() {
                var d = {
                    options: opts,
                    data: data,
                    id: tmp,
                    elem: $('#' + ICON_BODY)
                };
                return d;
            },
            /**
             * 隐藏elem
             */
            hideElem: function() {
                $(elem).hide();
                return a;
            },
            /**
             * 绘制select下拉选择框
             */
            createSelect: function() {
                var oriIcon = '<i class="iconfont">';

                // 默认图标
                if (ORIGINAL_ELEM_VALUE === '') {
                    if (isFontClass) {
                        ORIGINAL_ELEM_VALUE = '';//默认值
                    } else {
                        ORIGINAL_ELEM_VALUE = '&#xe617;';
                    }
                }

                if (isFontClass) {
                    oriIcon = '<i class="iconfont ' + ORIGINAL_ELEM_VALUE + '">';
                } else {
                    oriIcon += ORIGINAL_ELEM_VALUE;
                }
                oriIcon += '</i>';

                var selectHtml = '<div class="layui-iconpicker layui-unselect layui-form-select" id="' + ICON_BODY + '">' +
                    '<div class="' + TITLE + '" id="' + TITLE_ID + '">' +
                    '<div class="layui-iconpicker-item">' +
                    '<span class="layui-iconpicker-icon layui-unselect">' +
                    oriIcon +
                    '</span>' +
                    '<i class="layui-edge"></i>' +
                    '</div>' +
                    '</div>' +
                    '<div class="layui-anim layui-anim-upbit" style="">' +
                    '123' +
                    '</div>';
                $(elem).after(selectHtml);
                return a;
            },
            /**
             * 展开/折叠下拉框
             */
            toggleSelect: function() {
                var item = '#' + TITLE_ID + ' .layui-iconpicker-item,#' + TITLE_ID + ' .layui-iconpicker-item .layui-edge';
                a.event('click', item, function(e) {
                    var $icon = $('#' + ICON_BODY);
                    if ($icon.hasClass(selected)) {
                        $icon.removeClass(selected).addClass(unselect);
                    } else {
                        // 隐藏其他picker
                        $('.layui-form-select').removeClass(selected);
                        // 显示当前picker
                        $icon.addClass(selected).removeClass(unselect);
                    }
                    e.stopPropagation();
                });
                return a;
            },
            /**
             * 绘制主体部分
             */
            createBody: function() {
                // 获取数据
                var searchHtml = '';

                if (search) {
                    searchHtml = '<div class="layui-iconpicker-search">' +
                        '<input class="layui-input">' +
                        '<i class="iconfont icon-search"></i>' +
                        '</div>';
                }

                // 组合dom
                var bodyHtml = '<div class="layui-iconpicker-body" id="' + PICKER_BODY + '">' +
                    searchHtml +
                    '<div class="' + LIST_BOX + '"></div> ' +
                    '</div>';
                $('#' + ICON_BODY).find('.layui-anim').eq(0).html(bodyHtml);
                a.search().createList().check().page();

                return a;
            },
            /**
             * 绘制图标列表
             * @param text 模糊查询关键字
             * @returns {string}
             */
            createList: function(text) {
                var d = data,
                    l = d.length,
                    pageHtml = '',
                    listHtml = $('<div class="layui-iconpicker-list">') //'<div class="layui-iconpicker-list">';

                // 计算分页数据
                var _limit = limit, // 每页显示数量
                    _pages = l % _limit === 0 ? l / _limit : parseInt(l / _limit + 1), // 总计多少页
                    _id = PAGE_ID;

                // 图标列表
                var icons = [];

                for (var i = 0; i < l; i++) {
                    var obj = d[i];

                    // 判断是否模糊查询
                    if (text && obj.indexOf(text) === -1) {
                        continue;
                    }

                    // 是否自定义格子宽度
                    var style = '';
                    if (cellWidth !== null) {
                        style += ' style="width:' + cellWidth + '"';
                    }

                    // 每个图标dom
                    var icon = '<div class="layui-iconpicker-icon-item" title="' + obj + '" ' + style + '>';
                    if (isFontClass) {
                        icon += '<i class="iconfont ' + obj + '"></i>';
                    } else {
                        icon += '<i class="iconfont">' + obj.replace('amp;', '') + '</i>';
                    }
                    icon += '</div>';

                    icons.push(icon);
                }

                // 查询出图标后再分页
                l = icons.length;
                _pages = l % _limit === 0 ? l / _limit : parseInt(l / _limit + 1);
                for (var i = 0; i < _pages; i++) {
                    // 按limit分块
                    var lm = $('<div class="layui-iconpicker-icon-limit" id="layui-iconpicker-icon-limit-' + tmp + (i + 1) + '">');

                    for (var j = i * _limit; j < (i + 1) * _limit && j < l; j++) {
                        lm.append(icons[j]);
                    }

                    listHtml.append(lm);
                }

                // 无数据
                if (l === 0) {
                    listHtml.append('<p class="layui-iconpicker-tips">无数据</p>');
                }

                // 判断是否分页
                if (page) {
                    $('#' + PICKER_BODY).addClass('layui-iconpicker-body-page');
                    pageHtml = '<div class="layui-iconpicker-page" id="' + PAGE_ID + '">' +
                        '<div class="layui-iconpicker-page-count">' +
                        '<span id="' + PAGE_ID + '-current">1</span>/' +
                        '<span id="' + PAGE_ID + '-pages">' + _pages + '</span>' +
                        ' (<span id="' + PAGE_ID + '-length">' + l + '</span>)' +
                        '</div>' +
                        '<div class="layui-iconpicker-page-operate">' +
                        '<i class="iconfont icon-icon-test" id="' + PAGE_ID + '-prev" data-index="0" prev></i> ' +
                        '<i class="iconfont icon-icon-test1" id="' + PAGE_ID + '-next" data-index="2" next></i> ' +
                        '</div>' +
                        '</div>';
                }


                $('#' + ICON_BODY).find('.layui-anim').find('.' + LIST_BOX).html('').append(listHtml).append(pageHtml);
                return a;
            },
            // 阻止Layui的一些默认事件
            preventEvent: function() {
                var item = '#' + ICON_BODY + ' .layui-anim';
                a.event('click', item, function(e) {
                    e.stopPropagation();
                });
                return a;
            },
            // 分页
            page: function() {
                var icon = '#' + PAGE_ID + ' .layui-iconpicker-page-operate .iconfont';

                $(icon).unbind('click');
                a.event('click', icon, function(e) {
                    var elem = e.currentTarget,
                        total = parseInt($('#' + PAGE_ID + '-pages').html()),
                        isPrev = $(elem).attr('prev') !== undefined,
                        // 按钮上标的页码
                        index = parseInt($(elem).attr('data-index')),
                        $cur = $('#' + PAGE_ID + '-current'),
                        // 点击时正在显示的页码
                        current = parseInt($cur.html());

                    // 分页数据
                    if (isPrev && current > 1) {
                        current = current - 1;
                        $(icon + '[prev]').attr('data-index', current);
                    } else if (!isPrev && current < total) {
                        current = current + 1;
                        $(icon + '[next]').attr('data-index', current);
                    }
                    $cur.html(current);

                    // 图标数据
                    $('#' + ICON_BODY + ' .layui-iconpicker-icon-limit').hide();
                    $('#layui-iconpicker-icon-limit-' + tmp + current).show();
                    e.stopPropagation();
                });
                return a;
            },
            /**
             * 搜索
             */
            search: function() {
                var item = '#' + PICKER_BODY + ' .layui-iconpicker-search .layui-input';
                a.event('input propertychange', item, function(e) {
                    var elem = e.target,
                        t = $(elem).val();
                    a.createList(t);
                });
                return a;
            },
            /**
             * 点击选中图标
             */
            check: function() {
                var item = '#' + PICKER_BODY + ' .layui-iconpicker-icon-item';
                a.event('click', item, function(e) {
                    var el = $(e.currentTarget).find('.iconfont'),
                        icon = '';
                    if (isFontClass) {
                        var clsArr = el.attr('class').split(/[\s\n]/),
                            cls = clsArr[1],
                            icon = cls;
                        $('#' + TITLE_ID).find('.layui-iconpicker-item .iconfont').html('').attr('class', clsArr.join(' '));
                    } else {
                        var cls = el.html(),
                            icon = cls;
                        $('#' + TITLE_ID).find('.layui-iconpicker-item .iconfont').html(icon);
                    }

                    $('#' + ICON_BODY).removeClass(selected).addClass(unselect);
                    $(elem).val(icon).attr('value', icon);
                    // 回调
                    if (click) {
                        click({
                            icon: icon
                        });
                    }

                });
                return a;
            },
            // 监听原始input数值改变
            inputListen: function() {
                var el = $(elem);
                a.event('change', elem, function() {
                    var value = el.val();
                })
                // el.change(function(){

                // });
                return a;
            },
            event: function(evt, el, fn) {
                $(BODY).on(evt, el, fn);
            }
        };

        var common = {
            /**
             * 加载样式表
             */
            loadCss: function() {
                var css = '.layui-iconpicker {max-width: 280px;}.layui-iconpicker .layui-anim{display:none;position:absolute;left:0;top:42px;padding:5px 0;z-index:899;min-width:100%;border:1px solid #d2d2d2;max-height:300px;overflow-y:auto;background-color:#fff;border-radius:2px;box-shadow:0 2px 4px rgba(0,0,0,.12);box-sizing:border-box;}.layui-iconpicker-item{border:1px solid #e6e6e6;width:90px;height:38px;border-radius:4px;cursor:pointer;position:relative;}.layui-iconpicker-icon{border-right:1px solid #e6e6e6;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;display:block;width:60px;height:100%;float:left;text-align:center;background:#fff;transition:all .3s;}.layui-iconpicker-icon i{line-height:38px;font-size:18px;}.layui-iconpicker-item > .layui-edge{left:70px;}.layui-iconpicker-item:hover{border-color:#D2D2D2!important;}.layui-iconpicker-item:hover .layui-iconpicker-icon{border-color:#D2D2D2!important;}.layui-iconpicker.layui-form-selected .layui-anim{display:block;}.layui-iconpicker-body{padding:6px;}.layui-iconpicker .layui-iconpicker-list{background-color:#fff;border:1px solid #ccc;border-radius:4px;}.layui-iconpicker .layui-iconpicker-icon-item{display:inline-block;width:21.1%;line-height:36px;text-align:center;cursor:pointer;vertical-align:top;height:36px;margin:4px;border:1px solid #ddd;border-radius:2px;transition:300ms;}.layui-iconpicker .layui-iconpicker-icon-item i.iconfont{font-size:17px;}.layui-iconpicker .layui-iconpicker-icon-item:hover{background-color:#eee;border-color:#ccc;-webkit-box-shadow:0 0 2px #aaa,0 0 2px #fff inset;-moz-box-shadow:0 0 2px #aaa,0 0 2px #fff inset;box-shadow:0 0 2px #aaa,0 0 2px #fff inset;text-shadow:0 0 1px #fff;}.layui-iconpicker-search{position:relative;margin:0 0 6px 0;border:1px solid #e6e6e6;border-radius:2px;transition:300ms;}.layui-iconpicker-search:hover{border-color:#D2D2D2!important;}.layui-iconpicker-search .layui-input{cursor:text;display:inline-block;width:86%;border:none;padding-right:0;margin-top:1px;}.layui-iconpicker-search .iconfont{position:absolute;top:11px;right:4%;}.layui-iconpicker-tips{text-align:center;padding:8px 0;cursor:not-allowed;}.layui-iconpicker-page{margin-top:6px;margin-bottom:-6px;font-size:12px;padding:0 2px;}.layui-iconpicker-page-count{display:inline-block;}.layui-iconpicker-page-operate{display:inline-block;float:right;cursor:default;}.layui-iconpicker-page-operate .iconfont{font-size:12px;cursor:pointer;}.layui-iconpicker-body-page .layui-iconpicker-icon-limit{display:none;}.layui-iconpicker-body-page .layui-iconpicker-icon-limit:first-child{display:block;}';
                var $style = $('head').find('style[iconpicker]');
                if ($style.length === 0) {
                    $('head').append('<style rel="stylesheet" iconpicker>' + css + '</style>');
                }
            },
            /**
             * 获取数据
             */
            getData: {
                fontClass: function() {
                    var arr = ["icon-clothesfill","icon-clothes","icon-rankfill","icon-rank","icon-peoplefill","icon-people","icon-hotfill","icon-hot","icon-dibu","icon-dingbu","icon-beifenruanjian","icon-zhifubao-copy-copy","icon-qq","icon-tubiaoqiehuan","icon-tiyanjiankong","icon-shujutu","icon-baidu","icon-weixin","icon-shuaxin","icon-xingneng","icon-yonghu","icon-oschina","icon-bangzhushouce","icon-male","icon-female","icon-guanggao","icon-caigou-xianxing","icon-caigou","icon-lianjie","icon-mokuaishezhi","icon-yue","icon-guanliyuan","icon-liebiaosousuo","icon-richangqingli","icon-zidongxiufu","icon-jifen","icon-tuichu","icon-huiyuan","icon-rizhi","icon-enterprise","icon-shenhe","icon-tiaoshi","icon-shiyongwendang","icon-global","icon-taobao","icon-yidong","icon-cloud-upload","icon-cloud","icon-cloud-download","icon-cloud-sync","icon-plugins-","icon-neirongguanli","icon-apartment","icon-hangweifenxi","icon-bug-fill","icon-bug","icon-weibo","icon-accessory","icon-add","icon-addition_fill","icon-addition","icon-addpeople_fill","icon-addpeople","icon-browse_fill","icon-browse","icon-brush","icon-brush_fill","icon-close1","icon-collection_fill","icon-collection","icon-coordinates_fill","icon-coordinates","icon-createtask_fill","icon-createtask","icon-delete_fill","icon-delete","icon-emoji_fill","icon-emoji","icon-empty","icon-empty_fill","icon-enterinto","icon-enterinto_fill","icon-feedback_fill","icon-feedback","icon-flag_fill","icon-flag","icon-flashlight","icon-flashlight_fill","icon-fullscreen","icon-group","icon-group_fill","icon-homepage_fill","icon-homepage","icon-interactive_fill","icon-interactive","icon-label","icon-label_fill","icon-like_fill","icon-like","icon-lock_fill","icon-lock","icon-mail","icon-mail_fill","icon-manage_fill","icon-manage","icon-mine","icon-mine_fill","icon-more","icon-narrow","icon-offline_fill","icon-offline","icon-other","icon-picture_fill","icon-picture","icon-playon_fill","icon-playon","icon-praise_fill","icon-praise","icon-prompt_fill","icon-prompt","icon-qrcode_fill","icon-qrcode","icon-right","icon-select_fill","icon-select","icon-send","icon-setup_fill","icon-setup","icon-share_fill","icon-share","icon-stealth_fill","icon-stealth","icon-success_fill","icon-success","icon-switch","icon-systemprompt_fill","icon-systemprompt","icon-tailor","icon-time_fill","icon-time","icon-trash","icon-trash_fill","icon-undo","icon-unlock_fill","icon-unlock","icon-warning_fill","icon-warning","icon-workbench_fill","icon-workbench","icon-search","icon-searchfill","icon-packup","icon-unfold","icon-supply","icon-tools","icon-huiyuan2","icon-tongji"];
                    return arr;
                },
                unicode: function() {
                    return ["&amp;#xe6c9;", "&amp;#xe67b;", "&amp;#xe67a;", "&amp;#xe678;", "&amp;#xe679;", "&amp;#xe677;", "&amp;#xe676;", "&amp;#xe675;", "&amp;#xe673;", "&amp;#xe66f;", "&amp;#xe9aa;", "&amp;#xe672;", "&amp;#xe66b;", "&amp;#xe668;", "&amp;#xe6b1;", "&amp;#xe702;", "&amp;#xe66e;", "&amp;#xe68e;", "&amp;#xe674;", "&amp;#xe669;", "&amp;#xe666;", "&amp;#xe66c;", "&amp;#xe66a;", "&amp;#xe667;", "&amp;#xe7ae;", "&amp;#xe665;", "&amp;#xe664;", "&amp;#xe716;", "&amp;#xe656;", "&amp;#xe653;", "&amp;#xe663;", "&amp;#xe6c6;", "&amp;#xe6c5;", "&amp;#xe662;", "&amp;#xe661;", "&amp;#xe660;", "&amp;#xe65d;", "&amp;#xe65f;", "&amp;#xe671;", "&amp;#xe65e;", "&amp;#xe659;", "&amp;#xe735;", "&amp;#xe756;", "&amp;#xe65c;", "&amp;#xe715;", "&amp;#xe705;", "&amp;#xe6b2;", "&amp;#xe6af;", "&amp;#xe69c;", "&amp;#xe698;", "&amp;#xe657;", "&amp;#xe65b;", "&amp;#xe65a;", "&amp;#xe681;", "&amp;#xe67c;", "&amp;#xe601;", "&amp;#xe857;", "&amp;#xe655;", "&amp;#xe770;", "&amp;#xe670;", "&amp;#xe63d;", "&amp;#xe63e;", "&amp;#xe654;", "&amp;#xe652;", "&amp;#xe651;", "&amp;#xe6fc;", "&amp;#xe6ed;", "&amp;#xe688;", "&amp;#xe645;", "&amp;#xe64f;", "&amp;#xe64e;", "&amp;#xe64b;", "&amp;#xe62b;", "&amp;#xe64d;", "&amp;#xe64a;", "&amp;#xe64c;", "&amp;#xe650;", "&amp;#xe649;", "&amp;#xe648;", "&amp;#xe647;", "&amp;#xe646;", "&amp;#xe644;", "&amp;#xe62a;", "&amp;#xe643;", "&amp;#xe63f;", "&amp;#xe642;", "&amp;#xe641;", "&amp;#xe640;", "&amp;#xe63c;", "&amp;#xe63b;", "&amp;#xe63a;", "&amp;#xe639;", "&amp;#xe638;", "&amp;#xe637;", "&amp;#xe636;", "&amp;#xe635;", "&amp;#xe634;", "&amp;#xe633;", "&amp;#xe632;", "&amp;#xe631;", "&amp;#xe630;", "&amp;#xe62f;", "&amp;#xe62e;", "&amp;#xe62d;", "&amp;#xe62c;", "&amp;#xe629;", "&amp;#xe628;", "&amp;#xe625;", "&amp;#xe623;", "&amp;#xe621;", "&amp;#xe620;", "&amp;#xe61f;", "&amp;#xe61c;", "&amp;#xe60b;", "&amp;#xe619;", "&amp;#xe61a;", "&amp;#xe603;", "&amp;#xe602;", "&amp;#xe617;", "&amp;#xe615;", "&amp;#xe614;", "&amp;#xe613;", "&amp;#xe612;", "&amp;#xe611;", "&amp;#xe60f;", "&amp;#xe60e;", "&amp;#xe60d;", "&amp;#xe60c;", "&amp;#xe60a;", "&amp;#xe609;", "&amp;#xe605;", "&amp;#xe607;", "&amp;#xe606;", "&amp;#xe604;", "&amp;#xe600;", "&amp;#xe658;", "&amp;#x1007;", "&amp;#x1006;", "&amp;#x1005;", "&amp;#xe608;"];
                }
            }
        };

        a.init();
        return new IconPicker();
    };

    /**
     * 选中图标
     * @param filter lay-filter
     * @param iconName 图标名称，自动识别fontClass/unicode
     */
    IconPicker.prototype.checkIcon = function(filter, iconName) {
        var el = $('*[lay-filter=' + filter + ']'),
            p = el.next().find('.layui-iconpicker-item .iconfont'),
            c = iconName;

        if (c.indexOf('#xe') > 0) {
            p.html(c);
        } else {
            p.html('').attr('class', 'iconfont ' + c);
        }
        el.attr('value', c).val(c);
    };

    var iconPicker = new IconPicker();
    exports(_MOD, iconPicker);
});