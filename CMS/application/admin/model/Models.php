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
namespace app\admin\model;

use think\Db;
use \think\Model;

/**
 * 模型
 */
class Models extends Model
{
    protected $name = 'model';

    /**
     * 根据模型类型取得数据用于缓存
     * @param type $type
     * @return type
     */
    public function getModelAll($type = null, $module = null)
    {
        $where = array('status' => 1);
        if (!is_null($type)) {
            $where['type'] = $type;
        }
        if (!is_null($module)) {
            $where['module'] = $module;
        }
        $data = Db::name('Model')->where($where)->select();
        $Cache = array();
        foreach ($data as $v) {
            $Cache[$v['id']] = $v;
        }
        return $Cache;
    }

    /**
     * 生成模型缓存，以模型ID为下标的数组
     * @return boolean
     */
    public function model_cache()
    {
        $data = $this->getModelAll();
        cache('Model', $data);
        return $data;
    }

}
