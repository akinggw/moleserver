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
namespace app\member\uninstall;

use think\Db;
use \sys\UninstallBase;

class Uninstall extends UninstallBase
{
    //固定相关表
    private $modelTabList = array(
        'member',
        'member_group',
        'member_content',
    );

    //卸载
    public function run()
    {
        if (request()->param('clear') == 1) {
            //删除对应模型数据表
            if (!empty($this->modelTabList)) {
                foreach ($this->modelTabList as $tablename) {
                    //删除固定表
                    if (!empty($tablename)) {
                        $tablename = config('database.prefix') . $tablename;
                        Db::execute("DROP TABLE IF EXISTS `{$tablename}`;");
                    }
                }
            }
        }
        //删除路由
        //unlink(ROOT_PATH . 'route' . DIRECTORY_SEPARATOR . 'route_cms.php');
        //隐藏cms的投稿菜单
        if (isModuleInstall('cms')) {
            Db::name('menu')->where(['app' => 'cms', 'controller' => 'publish', 'action' => 'index'])->setField('status', 0);
        }
        return true;
    }

}
