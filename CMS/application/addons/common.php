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
// | 插件函数文件
// +----------------------------------------------------------------------
/**
 * 检查插件控制器是否存在某操作
 * @param string $name 插件名
 * @param string $controller 控制器
 * @param string $action 动作
 * @author 蔡伟明 <314013107@qq.com>
 * @return bool
 */
function plugin_action_exists($name = '', $controller = '', $action = '')
{
    if (strpos($name, '/')) {
        list($name, $controller, $action) = explode('/', $name);
    }
    return method_exists("addons\\{$name}\\controller\\{$controller}", $action);
}
