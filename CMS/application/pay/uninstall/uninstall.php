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
// | 卸载脚本
// +----------------------------------------------------------------------
namespace app\pay\uninstall;

use think\Db;
use \sys\UninstallBase;

class Uninstall extends UninstallBase
{
    //固定相关表
    private $modelTabList = array(
        'pay_account',
        'pay_payment',
        'pay_spend',
    );

    //卸载
    public function run()
    {
        if (request()->param('clear') == 1) {
            //删除固定表
            if (!empty($this->modelTabList)) {
                foreach ($this->modelTabList as $tablename) {
                    if (!empty($tablename)) {
                        $tablename = config('database.prefix') . $tablename;
                        Db::execute("DROP TABLE IF EXISTS `{$tablename}`;");
                    }
                }
            }
        }
        return true;
    }

}
