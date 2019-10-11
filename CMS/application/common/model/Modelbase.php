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
// | 公共模型
// +----------------------------------------------------------------------
namespace app\common\model;

use think\Db;
use think\facade\Config;
use think\Model;

/**
 * 公共模型
 */
class Modelbase extends Model
{
    /**
     * 删除表
     * @param string $tablename 不带表前缀的表名
     * @return type
     */
    public function drop_table($table)
    {
        $table = Config::get("database.prefix") . strtolower($table);
        return Db::query("DROP TABLE $table");
    }

    /**
     * 检查表是否存在
     * $table 不带表前缀
     */
    public function table_exists($table)
    {
        $table = Config::get("database.prefix") . strtolower($table);
        if (true == Db::query("SHOW TABLES LIKE '{$table}'")) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * 检查字段是否存在
     * $table 不带表前缀
     */
    public function field_exists($table, $field)
    {
        $fields = $this->get_fields($table);
        return array_key_exists($field, $fields);
    }

    /**
     * 获取表字段
     * $table 不带表前缀
     */
    public function get_fields($table)
    {
        $fields = array();
        $table = Config::get("database.prefix") . strtolower($table);
        $data = Db::query("SHOW COLUMNS FROM $table");
        foreach ($data as $v) {
            $fields[$v['Field']] = $v['Type'];
        }
        return $fields;
    }
}
