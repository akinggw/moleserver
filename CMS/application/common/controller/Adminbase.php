<?php
// +----------------------------------------------------------------------
// | Yzncms [ 御宅男工作室 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2018 http://yzncms.com All rights reserved.
// +----------------------------------------------------------------------
// | Licensed ( http://www.apache.org/licenses/LICENSE-2.0 )
// +----------------------------------------------------------------------
// | Author: 御宅男 <530765310@qq.com>
// +----------------------------------------------------------------------

// +----------------------------------------------------------------------
// | 后台控制模块
// +----------------------------------------------------------------------
namespace app\common\controller;

use app\admin\service\User;

//定义是后台
define('IN_ADMIN', true);

class Adminbase extends Base
{
    public $_userinfo; //当前登录账号信息
    //初始化
    protected function initialize()
    {
        parent::initialize();

        //过滤不需要登陆的行为
        $allowUrl = [
            'admin/index/login',
            'admin/index/logout',
        ];

        $rule = strtolower($this->request->module() . '/' . $this->request->controller() . '/' . $this->request->action());

        if (!in_array($rule, $allowUrl)) {

            if (defined('UID')) {
                return;
            }
            define('UID', (int) User::instance()->isLogin());
            // 是否是超级管理员
            define('IS_ROOT', User::instance()->isAdministrator());

            if (!IS_ROOT && config('admin_allow_ip')) {
                // 检查IP地址访问
                $arr = explode(',', config('admin_allow_ip'));
                foreach ($arr as $val) {
                    //是否是IP段
                    if (strpos($val, '*')) {
                        if (strpos($this->request->ip(), str_replace('.*', '', $val)) !== false) {
                            $this->error('403:你在IP禁止段内,禁止访问！');
                        }
                    } else {
                        //不是IP段,用绝对匹配
                        if ($this->request->ip() == $val) {
                            $this->error('403:IP地址绝对匹配,禁止访问！');
                        }

                    }
                }
            }
            if (false == $this->competence()) {
                //跳转到登录界面
                $this->error('请先登陆', url('admin/index/login'));
            } else {
                //是否超级管理员
                if (!IS_ROOT) {
                    //检测访问权限
                    if (!$this->checkRule($rule, [1, 2])) {
                        $this->error('未授权访问!');
                    }
                }
            }
        }
    }

    /**
     * 验证登录
     * @return boolean
     */
    private function competence()
    {
        //检查是否登录
        /*$uid = (int) User::instance()->isLogin();
        if (empty($uid)) {
        return false;
        }*/
        //获取当前登录用户信息
        $this->_userinfo = $userInfo = User::instance()->getInfo();
        if (empty($userInfo)) {
            User::instance()->logout();
            return false;
        }
        //是否锁定
        if (!$userInfo['status']) {
            User::instance()->logout();
            $this->error('您的帐号已经被锁定！', url('admin/index/login'));
            return false;
        }
        return $userInfo;

    }

    /**
     * 操作错误跳转的快捷方法
     */
    final public function error($msg = '', $url = null, $data = '', $wait = 3, array $header = [])
    {
        model('admin/Adminlog')->record($msg, 0);
        parent::error($msg, $url, $data, $wait, $header);
    }

    /**
     * 操作成功跳转的快捷方法
     */
    final public function success($msg = '', $url = null, $data = '', $wait = 3, array $header = [])
    {
        model('admin/Adminlog')->record($msg, 1);
        parent::success($msg, $url, $data, $wait, $header);
    }

    /**
     * 权限检测
     * @param string  $rule    检测的规则
     * @param string  $mode    check模式
     * @return boolean
     */
    final protected function checkRule($rule, $type = AuthRule::RULE_URL, $mode = 'url')
    {
        static $Auth = null;
        if (!$Auth) {
            $Auth = new \libs\Auth();
        }
        if (!$Auth->check($rule, UID, $type, $mode)) {
            return false;
        }
        return true;
    }

    /**
     * 生成查询所需要的条件,排序方式
     */
    protected function buildparams()
    {
        $search_field = $this->request->param('search_field/s', '', 'trim');
        $keyword = $this->request->param('keyword/s', '', 'trim');

        $filter_time = $this->request->param('filter_time/s', '', 'trim');
        $filter_time_range = $this->request->param('filter_time_range/s', '', 'trim');

        $map = [];
        // 关键词搜索
        if ($search_field != '' && $keyword !== '') {
            $map[] = [$search_field, 'like', "%$keyword%"];
        }

        //时间范围搜索
        if ($filter_time && $filter_time_range) {
            $filter_time_range = str_replace(' - ', ',', $filter_time_range);
            $arr = explode(',', $filter_time_range);
            !empty($arr[0]) ? $arr[0] : date("Y-m-d", strtotime("-1 day"));
            !empty($arr[1]) ? $arr[1] : date('Y-m-d', time());
            $map[] = [$filter_time, 'between time', [$arr[0] . ' 00:00:00', $arr[1] . ' 23:59:59']];
        }
        return $map;
    }

}
