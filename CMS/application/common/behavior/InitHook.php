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
// | 注册钩子
// +----------------------------------------------------------------------
namespace app\common\behavior;

use think\Db;
use think\facade\Cache;
use think\facade\Hook;

// 初始化钩子信息
class InitHook
{

    // 行为扩展的执行入口必须是run
    public function run($params)
    {
        $data = Cache::get('Hooks');
        if (empty($data)) {
            //所有模块和插件钩子
            $hooks = Db::name('Hooks')->where('status', 1)->column('name,addons,modules');
            foreach ($hooks as $key => $value) {
                $hooks_class = [];
                //模块
                if ($value['modules']) {
                    $data = Db::name('Module')->whereIn('module', $value['modules'])->where('status', 1)->column('module');
                    if ($data) {
                        foreach ($data as $key => $module) {
                            $hooks_class[] = "\\app\\" . $module . "\\behavior\\Hooks";
                        }
                    }
                }
                //插件
                if ($value['addons']) {
                    $data = Db::name('Addons')->whereIn('name', $value['addons'])->where('status', 1)->column('name');
                    if ($data) {
                        $hooks_class = array_merge($hooks_class, array_map('get_addon_class', $data));
                    }
                }
                Hook::add($value['name'], $hooks_class);
            }
            Cache::set('Hooks', Hook::get());
        } else {
            //批量导入插件
            Hook::import($data, false);
        }
    }

}
