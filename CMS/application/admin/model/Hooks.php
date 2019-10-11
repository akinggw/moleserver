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
// | 钩子模型
// +----------------------------------------------------------------------
namespace app\admin\model;

use think\Model;

/**
 * 钩子模型
 */

class Hooks extends Model
{
    protected $autoWriteTimestamp = true;

    /**
     * 更新插件里的所有钩子对应的插件
     */
    public function updateHooks($addons_name)
    {
        $addons_class = get_addon_class($addons_name); //获取插件名
        if (!class_exists($addons_class)) {
            $this->error = "未实现{$addons_name}插件的入口文件";
            return false;
        }
        //获取这个插件总的方法列表，数组
        $methods = get_class_methods($addons_class);
        $hooks = $this->column('name');
        $common = array_intersect($hooks, $methods);
        if (!empty($common)) {
            foreach ($common as $hook) {
                $flag = $this->updateAddons($hook, array($addons_name), 'addons');
                if (false === $flag) {
                    $this->removeHooks($addons_name);
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * 更新插件里的所有钩子对应的模块
     */
    public function updateModules($module, $hooksRule)
    {
        if (empty($module) || !is_array($hooksRule)) {
            $this->error = '参数不正确！';
            return false;
        }
        //判断钩子是否存在 不存在就新增
        foreach ($hooksRule as $key => $value) {
            //检查是否有同样的行为
            if (empty($this->where(['name' => $key])->find())) {
                $description = $value['description'];
                $type = (int) $value['type'];
                $this->save([
                    'name' => $key,
                    'description' => $description ?: "模块{$module}中的行为！",
                    'type' => $type ?: 1,
                    'status' => 1,
                    'system' => 0,
                ]);
            }
        }
        $hooks = $this->column('name');
        $common = array_intersect($hooks, array_keys($hooksRule));
        if (!empty($common)) {
            foreach ($common as $hook) {
                $flag = $this->updateAddons($hook, array($module), 'modules');
                if (false === $flag) {
                    $this->removeHooks($module);
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * 更新单个钩子处的插件
     * @param  [type] 钩子名称   [description]
     * @param  [type] 插件名称 [description]
     * @return [type]              [description]
     */
    public function updateAddons($hook_name, $name, $type)
    {
        $o_addons = $this->where(['name' => $hook_name])->value($type);
        if ($o_addons) {
            $o_addons = str2arr($o_addons);
        }
        if ($o_addons) {
            $addons = array_merge($o_addons, $name);
            $addons = array_unique($addons);
        } else {
            $addons = $name;
        }
        $flag = $this->where(['name' => $hook_name])->setField($type, arr2str($addons));
        if (false === $flag) {
            $this->where(['name' => $hook_name])->setField($type, arr2str($o_addons));
        }

        return $flag;
    }

    /**
     * 去除插件所有钩子里对应的插件数据
     */
    public function removeHooks($addons_name)
    {
        $addons_class = get_addon_class($addons_name);
        if (!class_exists($addons_class)) {
            return false;
        }
        //获取这个插件总的方法列表，数组
        $methods = get_class_methods($addons_class);
        $hooks = $this->column('name');
        $common = array_intersect($hooks, $methods);
        if ($common) {
            foreach ($common as $hook) {
                $flag = $this->removeAddons($hook, array($addons_name), 'addons');
                if (false === $flag) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * 去除插件所有钩子里对应的模块数据
     */
    public function removeModules($module, $hooksRule)
    {
        if (empty($module) || !is_array($hooksRule)) {
            $this->error = '参数不正确！';
            return false;
        }
        //TODO 删除钩子 暂时先不删除 后期看是否加上

        $hooks = $this->column('name');
        $common = array_intersect($hooks, array_keys($hooksRule));
        if ($common) {
            foreach ($common as $hook) {
                $flag = $this->removeAddons($hook, array($module), 'modules');
                if (false === $flag) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * 去除单个钩子里对应的插件数据
     * $hook_name 钩子名称
     * $addons_name 插件名称
     */
    public function removeAddons($hook_name, $name, $type)
    {
        $o_addons = $this->where(['name' => $hook_name])->value($type);
        $o_addons = str2arr($o_addons);
        if ($o_addons) {
            $addons = array_diff($o_addons, $name);
        } else {
            return true;
        }
        $flag = $this->where(['name' => $hook_name])->setField($type, arr2str($addons));
        if (false === $flag) {
            $this->where(['name' => $hook_name])->setField($type, arr2str($o_addons));
        }
        return $flag;
    }

    /**
     * 卸载模块时删除对应模块安装时创建的规则！
     * @param type $module 模块标识
     * @return boolean
     */
    public function moduleHooksUninstall($module)
    {
        return true;

    }

}
