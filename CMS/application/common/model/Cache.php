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
// | 缓存模型
// +----------------------------------------------------------------------
namespace app\common\model;

use \think\Model;

class Cache extends Model
{
    /**
     * 删除指定模块下的全部缓存队列
     * @param type $module 模块名称
     * @return boolean
     */
    public function deleteCacheModule($module)
    {
        if (empty($module)) {
            $this->error = '请指定模块！';
            return false;
        }
        if (self::destroy(['module' => $module, 'system' => 0]) !== false) {
            return true;
        } else {
            $this->error = '删除失败！';
            return false;
        }
    }

    /**
     * 安装模块是，注册缓存
     * @param array $cache 缓存配置
     * @param array $config 模块配置
     * @return boolean
     */
    public function installModuleCache(array $cache, array $config)
    {
        if (empty($cache) || empty($config)) {
            $this->error = '参数不完整！';
            return false;
        }
        $module = $config['module'];
        $data = array();
        foreach ($cache as $key => $rs) {
            $add = array(
                'key' => $key,
                'name' => $rs['name'],
                'module' => isset($rs['module']) ? $rs['module'] : $module,
                'model' => $rs['model'],
                'action' => $rs['action'],
                //'param' => isset($rs['param']) ? $rs['param'] : '',
                'system' => 0,
            );
            $data[] = $add;
        }
        if (!empty($data)) {
            return $this->allowField(true)->saveAll($data) !== false ? true : false;
        }
        return true;
    }

}
