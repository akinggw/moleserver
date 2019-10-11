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
// | 插件管理
// +----------------------------------------------------------------------
namespace app\addons\controller;

use app\addons\model\Addons as Addons_Model;
use app\common\controller\Adminbase;
use think\Db;
use think\facade\Cache;
use ZipArchive;

class Addons extends Adminbase
{
    //显示插件列表
    public function index()
    {
        if ($this->request->isAjax()) {
            //取得模块目录名称
            $dirs = array_map('basename', glob(ADDON_PATH . '*', GLOB_ONLYDIR));
            if ($dirs === false || !file_exists(ADDON_PATH)) {
                $this->error = '插件目录不可读或者不存在';
                return false;
            }
            // 读取数据库插件表
            $addons = Addons_Model::order('id desc')->column(true, 'name');
            //遍历插件列表
            foreach ($dirs as $value) {
                //是否已经安装过
                if (!isset($addons[$value])) {
                    $class = get_addon_class($value);
                    if (!class_exists($class)) {
                        // 实例化插件失败忽略执行
                        $addons[$value]['uninstall'] = -1;
                        continue;
                    }
                    //获取插件配置
                    $obj = new $class();
                    $addons[$value] = $obj->info;
                    if ($addons[$value]) {
                        $addons[$value]['uninstall'] = 1;
                        unset($addons[$value]['status']);
                        //是否有配置
                        //$config = $obj->getAddonConfig();
                        //$addons[$value]['config'] = $config;
                    }
                }
            }
            int_to_string($addons, array('status' => array(-1 => '损坏', 0 => '禁用', 1 => '启用', null => '未安装')));
            $result = array("code" => 0, "data" => $addons);
            return json($result);
        }
        return $this->fetch();

    }

    //插件钩子列表
    public function hooks()
    {
        if ($this->request->isAjax()) {
            $list = Db::name("hooks")->select();
            int_to_string($list, array(
                'type' => [1 => '视图', 2 => '控制器'],
            ));
            $result = array("code" => 0, "data" => $list);
            return json($result);
        }
        return $this->fetch();
    }

    /**
     * 设置插件页面
     */
    public function config()
    {
        $addonId = $this->request->param('id/d');
        if (empty($addonId)) {
            $this->error('请选择需要操作的插件！');
        }
        //获取插件信息
        $addon = Addons_Model::where(array('id' => $addonId))->find();
        if (!$addon) {
            $this->error('该插件没有安装！');
        }
        $addon = $addon->toArray();
        //实例化插件入口类
        $addon_class = get_addon_class($addon['name']);
        if (!class_exists($addon_class)) {
            trace("插件{$addon['name']}无法实例化,", 'ADDONS', 'ERR');
        }
        $addonObj = new $addon_class();
        $addon['addon_path'] = $addonObj->addon_path;
        $addon['custom_config'] = isset($addonObj->custom_config) ? $addonObj->custom_config : '';
        $db_config = $addon['config'];
        //载入插件配置数组
        $addon['config'] = include $addonObj->config_file;
        if ($db_config) {
            $db_config = json_decode($db_config, true);
            foreach ($addon['config'] as $key => $value) {
                if ($value['type'] != 'group') {
                    $addon['config'][$key]['value'] = isset($db_config[$key]) ? $db_config[$key] : '';
                } else {
                    foreach ($value['options'] as $gourp => $options) {
                        foreach ($options['options'] as $gkey => $value) {
                            $addon['config'][$key]['options'][$gourp]['options'][$gkey]['value'] = $db_config[$gkey];
                        }
                    }
                }
            }
        }
        $this->assign('data', $addon);
        if ($addon['custom_config']) {
            //加载配置文件config.html
            $this->assign('custom_config', $this->view->fetch($addon['addon_path'] . $addon['custom_config']));
        }
        return $this->fetch();
    }

    /**
     * 保存插件设置
     */
    public function saveConfig()
    {
        $id = $this->request->param('id/d');
        //获取插件信息
        $info = Addons_Model::where(array('id' => $id))->find();
        if (empty($info)) {
            $this->error('该插件没有安装！');
        }
        $config = $this->request->param('config/a');
        $flag = Db::name('Addons')->where(['id' => $id])->setField('config', json_encode($config));
        if ($flag !== false) {
            //更新插件缓存
            //$this->addons->addons_cache();
            $this->success('保存成功', url('index'));
        } else {
            $this->error('保存失败');
        }
    }

    //启用插件
    public function enable()
    {
        $id = $this->request->param('id/d');
        //cache('Hooks', null);
        if (Addons_Model::where('id', $id)->update(['status' => 1])) {
            $this->success('启用成功');
        } else {
            $this->error('启用失败');
        }
    }

    //禁用插件
    public function disable()
    {
        $id = $this->request->param('id/d');
        //cache('Hooks', null);
        if (Addons_Model::where('id', $id)->update(['status' => 0])) {
            $this->success('禁用成功');
        } else {
            $this->error('禁用失败');
        }
    }

    /**
     * 安装插件
     */
    public function install()
    {
        $addonName = $this->request->param('addon_name');
        if (empty($addonName)) {
            $this->error('请选择需要安装的插件！');
        }
        //检查插件是否安装
        if ($this->isInstall($addonName)) {
            $this->error('该插件已经安装，无需重复安装！');
        }
        $class = get_addon_class($addonName);
        if (!class_exists($class)) {
            $this->error('获取插件对象出错！');
        }
        $addonObj = new $class();
        //获取插件信息
        $info = $addonObj->info;
        if (empty($info)) {
            $this->error('插件信息获取失败！');
        }
        //开始安装
        $install = $addonObj->install();
        if ($install !== true) {
            if (method_exists($addonObj, 'getError')) {
                $this->error($addonObj->getError() ?: '执行插件预安装操作失败！');
            } else {
                $this->error('执行插件预安装操作失败！');
            }
        }
        $info['config'] = json_encode($addonObj->getAddonConfig());
        //添加插件安装记录
        $res = Addons_Model::create($info, true);
        if (!$res) {
            $this->error('写入插件数据失败！');
        }
        // 复制静态资源
        $sourceAssetsDir = self::getSourceAssetsDir($addonName);
        $destAssetsDir = self::getDestAssetsDir($addonName);
        if (is_dir($sourceAssetsDir)) {
            \util\File::copy_dir($sourceAssetsDir, $destAssetsDir);
        }
        //如果插件有自己的后台
        if (isset($info['has_adminlist']) && $info['has_adminlist']) {
            $admin_list = $addonObj->admin_list;
            //添加菜单
            model('admin/Menu')->addAddonMenu($info, $admin_list);
        }
        //更新插件行为实现
        $hooks_update = model('admin/Hooks')->updateHooks($addonName);
        if (!$hooks_update) {
            $this->where("name='{$addon_name}'")->delete();
            $this->error('更新钩子处插件失败,请卸载后尝试重新安装！');
        }
        Cache::set('Hooks', null);
        $this->success('插件安装成功！清除浏览器缓存和框架缓存后生效！', url('Addons/index'));
    }

    /**
     * 卸载插件
     */
    public function uninstall()
    {
        $addonId = $this->request->param('id/d');
        if (empty($addonId)) {
            $this->error('请选择需要卸载的插件！');
        }
        //获取插件信息
        $info = Addons_Model::where(array('id' => $addonId))->find();
        $class = get_addon_class($info['name']);
        if (empty($info) || !class_exists($class)) {
            $this->error('该插件不存在！');
        }
        //插件标识
        $addonName = $info['name'];
        //检查插件是否安装
        if ($this->isInstall($addonName) == false) {
            $this->error('该插件未安装，无需卸载！');
        }
        //卸载插件
        $addonObj = new $class();
        $uninstall = $addonObj->uninstall();
        if ($uninstall !== true) {
            if (method_exists($addonObj, 'getError')) {
                $this->error($addonObj->getError() ? $addonObj->getError() : '执行插件预卸载操作失败！');
            } else {
                $this->error('执行插件预卸载操作失败！');
            }
        }
        if (false !== Addons_Model::destroy($addonId)) {
            //删除插件后台菜单
            if (isset($info['has_adminlist']) && $info['has_adminlist']) {
                model('admin/Menu')->delAddonMenu($info);
            }
            // 移除插件基础资源目录
            $destAssetsDir = self::getDestAssetsDir($addonName);
            if (is_dir($destAssetsDir)) {
                \util\File::del_dir($destAssetsDir);
            }
            $hooks_update = model('admin/Hooks')->removeHooks($addonName);
            if ($hooks_update === false) {
                $this->error = '卸载插件所挂载的钩子数据失败！';
            }
            Cache::set('Hooks', null);
            $this->success('插件卸载成功！清除浏览器缓存和框架缓存后生效！', url('Addons/index'));
        } else {
            $this->error('插件卸载失败！');
        }
    }

    //本地安装
    public function local()
    {
        if ($this->request->isPost()) {
            $files = $this->request->file('file');
            if ($files == null) {
                $this->error("请选择上传文件！");
            }
            if (strtolower(substr($files->getInfo('name'), -3, 3)) != 'zip') {
                $this->error("上传的文件格式有误！");
            }
            //插件名称
            $addonName = pathinfo($files->getInfo('name'));
            $addonName = $addonName['filename'];
            //检查插件目录是否存在
            if (file_exists(ADDON_PATH . $addonName)) {
                $this->error('该插件目录已经存在！');
            }

            //上传临时文件地址
            $filename = $files->getInfo('tmp_name');
            $zip = new \util\PclZip($filename);
            $status = $zip->extract(PCLZIP_OPT_PATH, ADDON_PATH . $addonName);
            if ($status) {
                $this->success('插件解压成功，可以进入插件管理进行安装！', url('index'));
            } else {
                $this->error('插件解压失败！');
            }
        }
    }

    /**
     * 检查插件是否已经安装
     * @param type $name 插件标识
     * @return boolean
     */
    public function isInstall($name)
    {
        if (empty($name)) {
            return false;
        }
        $count = Addons_Model::where(array('name' => $name))->find();
        return $count ? true : false;
    }

    /**
     * 获取插件源资源文件夹
     * @param   string $name 插件名称
     * @return  string
     */
    protected static function getSourceAssetsDir($name)
    {
        return ADDON_PATH . $name . DIRECTORY_SEPARATOR . 'public' . DIRECTORY_SEPARATOR;
    }

    /**
     * 获取插件目标资源文件夹
     * @param   string $name 插件名称
     * @return  string
     */
    protected static function getDestAssetsDir($name)
    {
        $assetsDir = ROOT_PATH . str_replace("/", DIRECTORY_SEPARATOR, "public/static/addons/{$name}/");
        if (!is_dir($assetsDir)) {
            mkdir($assetsDir, 0755, true);
        }
        return $assetsDir;
    }

}
