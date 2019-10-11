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
// | 字段模型
// +----------------------------------------------------------------------
namespace app\admin\model;

use think\Db;
use \think\Model;

/**
 * 字段模型
 */
class ModelField extends Model
{

    //生成模型字段缓存
    public function model_field_cache()
    {
        $cache = array();
        $modelList = Db::name("Model")->select();
        foreach ($modelList as $info) {
            $data = Db::name("ModelField")->where(array("modelid" => $info['id'], "status" => 1))->order("listorder ASC")->select();
            $fieldList = array();
            if (!empty($data) && is_array($data)) {
                foreach ($data as $rs) {
                    //扩展配置
                    if (!empty($rs['setting'])) {
                        $rs = array_merge($rs, unserialize($rs['setting']));
                    }
                    $fieldList[$rs['name']] = $rs;
                }
            }
            $cache[$info['id']] = $fieldList;
        }
        cache('ModelField', $cache);
        return $cache;
    }

}
