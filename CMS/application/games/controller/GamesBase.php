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
// | 前台会员管理
// +----------------------------------------------------------------------
namespace app\games\controller;

use app\common\controller\Homebase;
use think\facade\Config;

class GamesBase extends HomeBase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
    }

    /**
     * 检测用户是否已经登陆
     */
    final public function check_member()
    {
        if (substr($this->request->module(), 0, 7) == 'public_' || in_array(strtolower($this->request->action()), $this->noNeedLogin)) {
            //所有以public_开头的方法都无需检测是否登陆
            return true;
        }
    }
}
