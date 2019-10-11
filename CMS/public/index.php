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
// | [ 应用入口文件 ]
// +----------------------------------------------------------------------

namespace think;

if (version_compare(PHP_VERSION, '5.6.0', '<')) {
    header("Content-type: text/html; charset=utf-8");
    die('PHP 5.6.0 及以上版本系统才可运行~ ');
}

define('IF_PUBLIC', true);
define('ROOT_PATH', __DIR__ . DIRECTORY_SEPARATOR . '..' . DIRECTORY_SEPARATOR);
define('APP_PATH', ROOT_PATH . 'application' . DIRECTORY_SEPARATOR);
define('ADDON_PATH', ROOT_PATH . 'addons' . DIRECTORY_SEPARATOR);
define('ROOT_URL', rtrim(dirname($_SERVER["SCRIPT_NAME"]), '\\/') . '/');
define('TEMPLATE_PATH', ROOT_PATH . 'public' . DIRECTORY_SEPARATOR . 'templates' . DIRECTORY_SEPARATOR);

// 加载基础文件
require ROOT_PATH . 'thinkphp' . DIRECTORY_SEPARATOR . 'base.php';
// 执行应用并响应
Container::get('app')->run()->send();

/*如果你的服务器不支持域名绑定目录
1.请将index.php放置根目录
2.注释上面代码define('IF_PUBLIC', true);
3.改成define('ROOT_PATH', __DIR__ . DIRECTORY_SEPARATOR);
 */
