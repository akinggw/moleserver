layui.define(['table', 'element', 'layer', 'form', 'notice'], function(exports) {
    var element = layui.element,
        table = layui.table,
        layer = layui.layer,
        $ = layui.jquery,
        form = layui.form,
        notice = layui.notice;

    layui.formatDateTime = function(timeStamp) {
        var date = new Date();
        date.setTime(timeStamp * 1000);
        var y = date.getFullYear();
        var m = date.getMonth() + 1;
        m = m < 10 ? ('0' + m) : m;
        var d = date.getDate();
        d = d < 10 ? ('0' + d) : d;
        var h = date.getHours();
        h = h < 10 ? ('0' + h) : h;
        var minute = date.getMinutes();
        var second = date.getSeconds();
        minute = minute < 10 ? ('0' + minute) : minute;
        second = second < 10 ? ('0' + second) : second;
        return y + '-' + m + '-' + d + ' ' + h + ':' + minute + ':' + second;
    };

    var fly = {
        json: function(url, data, success, options) {
            options = options || {};
            return $.ajax({
                type: options.type || 'post',
                dataType: options.dataType || 'json',
                data: data,
                url: url,
                success: function(res) {
                    res = fly.onAjaxResponse(res);
                    if (res.code === 1) {
                        notice.success(res.msg);
                        success && success(res);
                    } else {
                        notice.error(res.msg);
                        options.error && options.error();
                    }
                },
                error: function(e) {
                    notice.error("请求异常，请重试");
                    options.error && options.error(e);
                }
            })
        },
        //服务器响应数据后
        onAjaxResponse: function(response) {
            try {
                var ret = typeof response === 'object' ? response : JSON.parse(response);
                if (!ret.hasOwnProperty('code')) {
                    $.extend(ret, { code: -2, msg: response, data: null });
                }
            } catch (e) {
                var ret = { code: -1, msg: e.message, data: null };
            }
            return ret;
        }
    };

    /**
     * iframe弹窗
     * @href 弹窗地址
     * @title 弹窗标题
     * @lay-data {width: '弹窗宽度', height: '弹窗高度', idSync: '是否同步ID', table: '数据表ID(同步ID时必须)', type: '弹窗类型'}
     */
    $(document).on('click', '.layui-iframe', function() {
        var that = $(this),
            query = '';
        var def = { width: '750px', height: '500px', idSync: false, table: 'dataTable', type: 2, url: !that.attr('data-href') ? that.attr('href') : that.attr('data-href'), title: that.attr('title') };
        var opt = new Function('return ' + that.attr('lay-data'))() || {};

        opt.url = opt.url || def.url;
        opt.title = opt.title || def.title;
        opt.width = opt.width || def.width;
        opt.height = opt.height || def.height;
        opt.type = opt.type || def.type;
        opt.table = opt.table || def.table;
        opt.idSync = opt.idSync || def.idSync;

        if (!opt.url) {
            notice.info('请设置data-href参数');
            return false;
        }

        if (opt.idSync) { // ID 同步
            if ($('.checkbox-ids:checked').length <= 0) {
                var checkStatus = table.checkStatus(opt.table);
                if (checkStatus.data.length <= 0) {
                    notice.info('请选择要操作的数据');
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
     */
    $(document).on('click', '.layui-tr-del', function() {
        var that = $(this),
            href = !that.attr('data-href') ? that.attr('href') : that.attr('data-href');
        layer.confirm('删除之后无法恢复，您确定要删除吗？', { icon: 3, title: '提示信息' }, function(index) {
            if (!href) {
                notice.info('请设置data-href参数');
                return false;
            }
            $.get(href, function(res) {
                res = fly.onAjaxResponse(res);
                if (res.code == 1) {
                    notice.success(res.msg);
                    that.parents('tr').remove();
                } else {
                    notice.error(res.msg);
                }
            });
            layer.close(index);
        });
        return false;
    });

    /**
     * 列表页批量操作按钮组
     * @attr href 操作地址
     * @attr data-table table容器ID
     * @class confirm 类似系统confirm
     * @attr tips confirm提示内容
     */
    $(document).on('click', '.layui-batch-all', function() {
        var that = $(this),
            query = '',
            code = function(that) {
                var href = that.attr('href') ? that.attr('href') : that.attr('data-href');
                var tableObj = that.attr('data-table') ? that.attr('data-table') : 'dataTable';
                if (!href) {
                    notice.info('请设置data-href参数');
                    return false;
                }

                if ($('.checkbox-ids:checked').length <= 0) {
                    var checkStatus = table.checkStatus(tableObj);
                    if (checkStatus.data.length <= 0) {
                        notice.info('请选择要操作的数据');
                        return false;
                    }
                    for (var i in checkStatus.data) {
                        if (i > 0) {
                            query += '&';
                        }
                        query += 'ids[]=' + checkStatus.data[i].id;
                    }
                } else {
                    if (that.parents('form')[0]) {
                        query = that.parents('form').serialize();
                    } else {
                        query = $('#pageListForm').serialize();
                    }
                }

                fly.json(href, query, function(res) {
                    if (res.code === 1) {
                        table.reload('dataTable');
                    }
                });
            };
        if (that.hasClass('confirm')) {
            var tips = that.attr('tips') ? that.attr('tips') : '您确定要执行此操作吗？';
            layer.confirm(tips, { icon: 3, title: '提示信息' }, function(index) {
                code(that);
                layer.close(index);
            });
        } else {
            code(that);
        }
        return false;
    });


    /**
     * 通用状态设置开关
     * @attr data-href 请求地址
     */
    form.on('switch(switchStatus)', function(data) {
        var that = $(this),
            status = 0;
        if (!that.attr('data-href')) {
            notice.info('请设置data-href参数');
            return false;
        }
        if (this.checked) {
            status = 1;
        }
        $.get(that.attr('data-href'), { status: status }, function(res) {
            if (res.code === 1) {
                notice.success(res.msg);
            } else {
                notice.error(res.msg);
                that.trigger('click');
                form.render('checkbox');
            }
        });
    });

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
        that.prop('disabled', true);
        fly.json(_form.attr('action'), _form.serialize(), function(res) {
            if (res.code == 0) {
                that.prop('disabled', true);
                setTimeout(function() {
                    that.prop('disabled', false);
                }, 3000);
            } else {
                setTimeout(function() {
                    that.prop('disabled', false);
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
        }, {
            error: function() {
                that.prop('disabled', true);
                setTimeout(function() {
                    that.prop('disabled', false);
                }, 3000);
            }

        });
        return false;
    });

    //ajax get请求
    $(document).on('click', '.ajax-get', function() {
        var that = $(this),
            href = !that.attr('data-href') ? that.attr('href') : that.attr('data-href'),
            refresh = !that.attr('refresh') ? 'true' : that.attr('refresh');
        if (!href) {
            layer.msg('请设置data-href参数');
            return false;
        }
        if ($(this).hasClass('confirm')) {
            layer.confirm('确认要执行该操作吗?', { icon: 3, title: '提示' }, function(index) {
                $.get(href, {}, function(res) {
                    layer.msg(res.msg, {}, function() {
                        if (refresh == 'true' || refresh == 'yes') {
                            if (typeof(res.url) != 'undefined' && res.url != null && res.url != '') {
                                location.href = res.url;
                            } else {
                                location.reload();
                            }
                        }
                    });
                });
            });
        } else {
            $.get(href, {}, function(res) {
                layer.msg(res.msg, {}, function() {
                    if (refresh == 'true') {
                        if (typeof(res.url) != 'undefined' && res.url != null && res.url != '') {
                            location.href = res.url;
                        } else {
                            location.reload();
                        }
                    }
                });
            });
        };
        return false;
    });

    //通用表单post提交
    $('.ajax-post').on('click', function(e) {
        var target, query, _form,
            target_form = $(this).attr('target-form'),
            that = this,
            nead_confirm = false;

        _form = $('.' + target_form);
        if ($(this).attr('hide-data') === 'true') {
            _form = $('.hide-data');
            query = _form.serialize();
        } else if (_form.get(0) == undefined) {
            return false;
        } else if (_form.get(0).nodeName == 'FORM') {
            if ($(this).hasClass('confirm')) {
                if (!confirm('确认要执行该操作吗?')) {
                    return false;
                }
            }
            if ($(this).attr('url') !== undefined) {
                target = $(this).attr('url');
            } else {
                target = _form.attr("action");
            }
            query = _form.serialize();
        } else if (_form.get(0).nodeName == 'INPUT' || _form.get(0).nodeName == 'SELECT' || _form.get(0).nodeName == 'TEXTAREA') {
            _form.each(function(k, v) {
                if (v.type == 'checkbox' && v.checked == true) {
                    nead_confirm = true;
                }
            })
            if (nead_confirm && $(this).hasClass('confirm')) {
                if (!confirm('确认要执行该操作吗?')) {
                    return false;
                }
            }
            query = _form.serialize();
        } else {
            if ($(this).hasClass('confirm')) {
                if (!confirm('确认要执行该操作吗?')) {
                    return false;
                }
            }
            query = _form.find('input,select,textarea').serialize();
        }

        $.post(target, query).success(function(data) {
            if (data.code == 1) {
                parent.layui.layer.closeAll();
                if (data.url) {
                    layer.msg(data.msg + ' 页面即将自动跳转~');
                } else {
                    layer.msg(data.msg);
                }
                setTimeout(function() {
                    if (data.url) {
                        location.href = data.url;
                    } else {
                        location.reload();
                    }
                }, 1500);
            } else {
                layer.msg(data.msg);
                setTimeout(function() {
                    if (data.url) {
                        location.href = data.url;
                    }
                }, 1500);
            }
        });
        return false;
    });

    exports('common', {});
});