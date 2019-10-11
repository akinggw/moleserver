layui.define(['layer', 'laytpl', 'form', 'element', 'upload', 'util'], function(exports) {
    var $ = layui.jquery,
        layer = layui.layer,
        laytpl = layui.laytpl,
        form = layui.form,
        element = layui.element,
        upload = layui.upload,
        util = layui.util,
        device = layui.device(),
        DISABLED = 'layui-btn-disabled';

    //阻止IE7以下访问
    if (device.ie && device.ie < 10) {
        layer.alert('如果您非得使用 IE 浏览器访问，那么请使用 IE10+');
    }


    layui.focusInsert = function(obj, str) {
        var result, val = obj.value;
        obj.focus();
        if (document.selection) { //ie
            result = document.selection.createRange();
            document.selection.empty();
            result.text = str;
        } else {
            result = [val.substring(0, obj.selectionStart), str, val.substr(obj.selectionEnd)];
            obj.focus();
            obj.value = result.join('');
        }
    };

    //显示当前tab
    if (location.hash) {
        element.tabChange('user', location.hash.replace(/^#/, ''));
    }
    element.on('tab(user)', function() {
        var othis = $(this),
            layid = othis.attr('lay-id');
        if (layid) {
            location.hash = layid;
        }
    });

    //登录
    form.on("submit(login)", function(data) {
        var action = $(data.form).attr('action');
        $.post(action, $(data.form).serialize(), success, "json");
        return false;

        function success(data) {
            if (data.code) {
                layer.msg(data.msg, {
                    offset: '15px',
                    icon: 1,
                    time: 1000
                }, function() {
                    window.location.href = data.url;
                });
            } else {
                layer.msg(data.msg, { icon: 5 });
                //刷新验证码
                $("#verify").click();
            }
        }
    });

    //表单输入效果
    $(".login-main .input-item").click(function(e) {
        e.stopPropagation();
        $(this).addClass("layui-input-focus").find(".layui-input").focus();
    })
    $(".login-main .input-item .layui-input").focus(function() {
        $(this).parent().addClass("layui-input-focus");
    })
    $(".login-main .input-item .layui-input").blur(function() {
        $(this).parent().removeClass("layui-input-focus");
        if ($(this).val() != '') {
            $(this).parent().addClass("layui-input-active");
        } else {
            $(this).parent().removeClass("layui-input-active");
        }
    })

    //刷新验证码
    $("#verify").click(function() {
        var verifyimg = $("#verify").attr("src");
        $("#verify").attr("src", verifyimg.replace(/\?.*$/, '') + '?' + Math.random());
    });

    form.on('radio(type)', function(data) {
        var type = data.value;
        if (type == 'email') {
            $("input[name='mobile']").attr('lay-verify', '');
            $("input[name='email']").attr('lay-verify', 'email|required');

        }
        if (type == 'mobile') {
            $("input[name='email']").attr('lay-verify', '');
            $("input[name='mobile']").attr('lay-verify', 'phone|required');

        }
        $("div.layui-form-item[data-type]").addClass("layui-hide");
        $("div.layui-form-item[data-type='" + type + "']").removeClass("layui-hide");
        $(".btn-captcha").data("url", $(this).data("send-url")).data("type", type);
    });

    //发送验证码
    $(document).on("click", ".btn-captcha", function(e) {
        var that = this;
        var type = $(that).data("type") ? $(that).data("type") : 'mobile';
        var element = $("input[name='" + type + "']");
        var si = {};

        var data = { event: $(that).data("event") };
        data[type] = element.val();

        $(that).attr('disabled', true).text("发送中...");

        $.post($(that).data("url"), data, function(data) {
            if (data.code == 1) {
                var seconds = 120;
                si[type] = setInterval(function() {
                    seconds--;
                    if (seconds <= 0) {
                        clearInterval(si[type]);
                        $(that).removeClass("layui-btn-disabled").text("获取验证码").attr("disabled", false);
                    } else {
                        $(that).addClass("layui-btn-disabled").text(seconds + "秒后可发送");
                    }
                }, 1000);
            } else {
                $(that).removeClass("layui-btn-disabled").text("获取验证码").attr("disabled", false);
                layer.msg(data.msg);
            }

        })
        return false;
    })

    /**
     * 监听表单提交
     * @attr action 请求地址
     * @attr data-form 表单DOM
     */
    form.on('submit(formSubmit)', function(data) {
        var _form = '',
            that = $(this),
            text = that.text(),
            options = { pop: false, refresh: true, jump: false, callback: null };
        if ($(this).attr('data-form')) {
            _form = $(that.attr('data-form'));
        } else {
            _form = that.parents('form');
        }

        if (that.attr('lay-data')) {
            options = new Function('return ' + that.attr('lay-data'))();
        }
        that.prop('disabled', true).text('提交中...');
        $.ajax({
            type: "POST",
            url: _form.attr('action'),
            data: _form.serialize(),
            success: function(res) {
                that.text(res.msg);
                if (res.code == 0) {
                    that.prop('disabled', false).addClass('layui-btn-danger');
                    setTimeout(function() {
                        that.removeClass('layui-btn-danger').text(text);
                    }, 3000);
                } else {
                    setTimeout(function() {
                        that.text(text).prop('disabled', false);
                        if (options.callback) {
                            options.callback(that, res);
                        }
                        if (options.pop == true) {
                            if (options.refresh == true) {
                                parent.location.reload();
                            } else if (options.jump == true && res.url != '') {
                                parent.location.href = res.url;
                            }
                            parent.layui.layer.closeAll();
                        } else if (options.refresh == true) {
                            if (res.url != '') {
                                location.href = res.url;
                            } else {
                                location.reload();
                            }
                        }
                    }, 3000);
                }
            }
        });
        return false;
    });

    //上传图片
    if ($('.upload-img')[0]) {
        layui.use('upload', function(upload) {
            var avatarAdd = $('.avatar-add');
            upload.render({
                elem: '.upload-img',
                url: GV.image_upload_url,
                size: 50,
                before: function() {
                    avatarAdd.find('.loading').show();
                },
                done: function(res) {
                    if (res.code == 0) {
                        $.post(GV.profile_upload_url, {
                            avatar: res.id
                        }, function(res) {
                            //location.reload();
                        });
                    } else {
                        layer.msg(res.info, { icon: 5 });
                    }
                    avatarAdd.find('.loading').hide();
                },
                error: function() {
                    avatarAdd.find('.loading').hide();
                }
            });
        });
    }

    /**
     * iframe弹窗
     * @href 弹窗地址
     * @title 弹窗标题
     * @lay-data {width: '弹窗宽度', height: '弹窗高度', idSync: '是否同步ID', table: '数据表ID(同步ID时必须)', type: '弹窗类型'}
     */
    $(document).on('click', '.layui-iframe', function() {
        var that = $(this),
            query = '';
        var def = { width: '750px', height: '500px', idSync: false, table: 'dataTable', type: 2, url: that.attr('href'), title: that.attr('title') };
        var opt = new Function('return ' + that.attr('lay-data'))() || {};

        opt.url = opt.url || def.url;
        opt.title = opt.title || def.title;
        opt.width = opt.width || def.width;
        opt.height = opt.height || def.height;
        opt.type = opt.type || def.type;
        opt.table = opt.table || def.table;
        opt.idSync = opt.idSync || def.idSync;

        if (!opt.url) {
            layer.msg('请设置href参数');
            return false;
        }

        if (opt.idSync) { // ID 同步
            if ($('.checkbox-ids:checked').length <= 0) {
                var checkStatus = table.checkStatus(opt.table);
                if (checkStatus.data.length <= 0) {
                    layer.msg('请选择要操作的数据');
                    return false;
                }

                for (var i in checkStatus.data) {
                    query += '&id[]=' + checkStatus.data[i].id;
                }
            } else {
                $('.checkbox-ids:checked').each(function() {
                    query += '&id[]=' + $(this).val();
                })
            }
        }

        if (opt.url.indexOf('?') >= 0) {
            opt.url += '&iframe=yes' + query;
        } else {
            opt.url += '?iframe=yes' + query;
        }

        layer.open({ type: opt.type, title: opt.title, content: opt.url, area: [opt.width, opt.height] });
        return false;

    });

    /**
     * 通用表格行数据行删除
     * @attr href或data-href 请求地址
     * @attr refresh 操作完成后是否自动刷新
     */
    $(document).on('click', '.layui-tr-del', function() {
        var that = $(this),
            href = !that.attr('data-href') ? that.attr('href') : that.attr('data-href');
        layer.confirm('删除之后无法恢复，您确定要删除吗？', { icon: 3, title: '提示信息' }, function(index) {
            if (!href) {
                layer.msg('请设置data-href参数');
                return false;
            }
            $.get(href, function(res) {
                if (res.code == 0) {
                    layer.msg(res.msg);
                } else {
                    that.parents('tr').remove();
                }
            });
            layer.close(index);
        });
        return false;
    });

    //加载特定模块
    if (layui.cache.page && layui.cache.page !== 'index') {
        var extend = {};
        extend[layui.cache.page] = layui.cache.page;
        layui.extend(extend);
        layui.use(layui.cache.page);
    }

    //手机设备的简单适配
    var treeMobile = $('.site-tree-mobile'),
        shadeMobile = $('.site-mobile-shade')

    treeMobile.on('click', function() {
        $('body').addClass('site-mobile');
    });

    shadeMobile.on('click', function() {
        $('body').removeClass('site-mobile');
    });
    exports('fly', {});

});