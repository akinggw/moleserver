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
// | 全局缓存类
// +----------------------------------------------------------------------
namespace libs;

use think\Db;
use think\facade\Cache;

class Cache_factory
{
    protected static $instance = null;

    /**
     * @param 缓存实例化
     * @return static
     */
    public static function instance($options = [])
    {
        if (is_null(self::$instance)) {
            self::$instance = new self($options);
        }
        return self::$instance;
    }

    /**
     * 获取缓存
     * @param type $name 缓存名称
     * @return null
     */
    public function get($name)
    {
        $cache = Cache::get($name);
        if (!empty($cache)) {
            return $cache;
        } else {
            //尝试生成缓存
            return $this->runUpdate($name);
        }
        return null;
    }

    /**
     * 写入缓存
     * @param string $name 缓存变量名
     * @param type $value 存储数据
     * @param type $expire 有效时间（秒）
     * @return boolean
     */
    public function set($name, $value, $expire = null)
    {
        return Cache::set($name, $value, $expire);
    }

    /**
     * 删除缓存
     * @param string $name 缓存变量名
     * @return boolean
     */
    public function remove($name)
    {
        return Cache::rm($name, null);
    }

    /**
     * 更新缓存
     * @param type $name 缓存key
     * @return boolean
     */
    public function runUpdate($name)
    {
        if (empty($name)) {
            return false;
        }
        //查询缓存key
        $cacheList = Db::name('cache')->where(array('key' => $name))->order(array('id' => 'DESC'))->select();
        if (empty($cacheList)) {
            return false;
        }
        foreach ($cacheList as $config) {
            if (empty($config)) {
                $this->error = '没有可需要更新的缓存信息！';
                return false;
            }
            $mo = '';
            if (empty($config['module'])) {
                $mo = "common/{$config['model']}";
            } else {
                $mo = "{$config['module']}/{$config['model']}";
            }
            $model = model($mo);
            if ($config['action']) {
                $action = $config['action'];
                $model->$action(); //执行方法
            }
        }
        //再次加载
        return Cache::get($name);
    }

}
