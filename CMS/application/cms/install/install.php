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
// | 安装脚本
// +----------------------------------------------------------------------
namespace app\cms\install;

use think\Db;
use util\Sql;
use \sys\InstallBase;

class install extends InstallBase
{
    /**
     * 安装完回调
     * @return boolean
     */
    public function end()
    {
        //填充默认配置
        $Setting = include APP_PATH . 'cms/install/setting.php';
        if (!empty($Setting) && is_array($Setting)) {
            Db::name("Module")->where('module', 'cms')->setField('setting', serialize($Setting));
        }
        //安装演示数据
        if (request()->param('demo') == 1) {
            $sql_file = APP_PATH . "cms/install/demo.sql";
            if (file_exists($sql_file)) {
                $sql_statement = Sql::getSqlFromFile($sql_file);
                if (!empty($sql_statement)) {
                    foreach ($sql_statement as $value) {
                        try {
                            Db::execute($value);
                        } catch (\Exception $e) {
                            $this->error = '导入演示数据失败，请检查demo.sql的语句是否正确';
                            return false;
                        }
                    }
                }
            }
        }
        //复制路由
        $route_file = APP_PATH . str_replace("/", DIRECTORY_SEPARATOR, "cms/install/route_cms.php");
        copy($route_file, ROOT_PATH . 'route' . DIRECTORY_SEPARATOR . 'route_cms.php');
        return true;
    }

}
