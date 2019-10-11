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
// | 模型模型
// +----------------------------------------------------------------------
namespace app\formguide\model;

use app\cms\model\Models as Models_Model;
use think\Db;
use think\facade\Config;

class Models extends Models_Model
{
    protected $name = 'model';
    protected $autoWriteTimestamp = true;

    /**
     * 创建模型
     * @param type $data 提交数据
     * @return boolean
     */
    public function addModelFormguide($data, $module = 'formguide')
    {
        if (empty($data)) {
            throw new \Exception('数据不得为空！');
        }
        $data['tablename'] = $data['tablename'] ? 'form_' . $data['tablename'] : '';
        $data['module'] = $module;
        $data['setting'] = serialize($data['setting']);
        //添加模型记录
        if (self::allowField(true)->save($data)) {
            cache("Model", null);
            //创建模型表
            $sql = "CREATE TABLE IF NOT EXISTS `think_form_table` (
                       `id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT,
                       `uid` mediumint(8) unsigned NOT NULL,
                       `username` varchar(20) NOT NULL,
                       `inputtime` int(10) unsigned NOT NULL,
                       `ip` bigint(20) unsigned DEFAULT '0',
                       PRIMARY KEY (`id`)
                    ) ENGINE=MyISAM DEFAULT CHARSET=utf8;";
            //表名替换
            $sql = str_replace("think_form_table", Config::get("database.prefix") . $data['tablename'], $sql);
            Db::execute($sql);
        }
    }

}
