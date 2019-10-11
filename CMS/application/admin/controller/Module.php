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
// | 模型管理
// +----------------------------------------------------------------------
namespace app\admin\controller;

//use app\admin\model\Module as ModuleModel;
use app\common\controller\Adminbase;
use sys\Module as ModuleService;
use think\Controller;
use think\Db;

class Module extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->ModuleService = new ModuleService();
    }

    //本地模块列表
    public function index()
    {
        if ($this->request->isAjax()) {
            $list = $this->ModuleService->getAll();
            $result = array("code" => 0, "data" => $list);
            return json($result);
        }
        return $this->fetch();
    }

    //模块安装
    public function install()
    {
        if ($this->request->isPost()) {
            $module = $this->request->param('module');
            if (empty($module)) {
                $this->error('请选择需要安装的模块！');
            }
            if ($this->ModuleService->install($module)) {
                $this->success('模块安装成功！一键清理缓存后生效！', url('admin/Module/index'));
            } else {
                $error = $this->ModuleService->getError();
                $this->error($error ? $error : '模块安装失败！');
            }
        } else {
            $module = $this->request->param('module', '');
            if (empty($module)) {
                $this->error('请选择需要安装的模块！');
            }
            $config = $this->ModuleService->getInfoFromFile($module);
            //版本检查
            if ($config['adaptation']) {
                if (version_compare(config('version.yzncms_version'), $config['adaptation'], '>=') == false) {
                    $version_check = '<i class="iconfont icon-delete text-danger"></i>';
                } else {
                    $version_check = '<i class="iconfont icon-success text-success"></i>';
                }
            }
            $need_module = [];
            $need_plugin = [];
            $table_check = [];
            // 检查模块依赖
            if (isset($config['need_module']) && !empty($config['need_module'])) {
                $need_module = $this->checkDependence('module', $config['need_module']);
            }
            // 检查插件依赖
            if (isset($config['need_plugin']) && !empty($config['need_plugin'])) {
                $need_plugin = $this->checkDependence('plugin', $config['need_plugin']);
            }
            // 检查目录权限
            // 检查数据表
            if (isset($config['tables']) && !empty($config['tables'])) {
                foreach ($config['tables'] as $table) {
                    if (Db::query("SHOW TABLES LIKE '" . config('database.prefix') . "{$table}'")) {
                        $table_check[] = [
                            'table' => config('database.prefix') . "{$table}",
                            'result' => '<span class="text-danger">存在同名</span>',
                        ];
                    } else {
                        $table_check[] = [
                            'table' => config('database.prefix') . "{$table}",
                            'result' => '<i class="iconfont icon-success text-success"></i>',
                        ];
                    }
                }

            }
            $this->assign('need_module', $need_module);
            $this->assign('need_plugin', $need_plugin);
            $this->assign('version_check', $version_check);
            $this->assign('table_check', $table_check);
            $this->assign('config', $config);
            return $this->fetch();

        }
    }

    //模块卸载
    public function uninstall()
    {
        if ($this->request->isPost()) {
            $module = $this->request->param('module');
            if (empty($module)) {
                $this->error('请选择需要安装的模块！');
            }
            if ($this->ModuleService->uninstall($module)) {
                $this->success("模块卸载成功！一键清理缓存后生效！", url("admin/Module/index"));
            } else {
                $error = $this->ModuleService->getError();
                $this->error($error ? $error : "模块卸载失败！", url("admin/Module/index"));
            }
        } else {
            $module = $this->request->param('module', '');
            if (empty($module)) {
                $this->error('请选择需要安装的模块！');
            }
            $config = $this->ModuleService->getInfoFromFile($module);
            $this->assign('config', $config);
            return $this->fetch();

        }
    }

    /**
     * 检查依赖
     * @param string $type 类型：module/plugin
     * @param array $data 检查数据
     * @return array
     */
    private function checkDependence($type = '', $data = [])
    {
        $need = [];
        foreach ($data as $key => $value) {
            if (!isset($value[2])) {
                $value[2] = '=';
            }
            // 当前版本
            if ($type == 'module') {
                $curr_version = Db::name('Module')->where('module', $value[0])->value('version');
            } else {
                $curr_version = Db::name('Addons')->where('name', $value[0])->value('version');
            }
            $result = version_compare($curr_version, $value[1], $value[2]);
            $need[$key] = [
                $type => $value[0],
                'version' => $curr_version ? $curr_version : '未安装',
                'version_need' => $value[2] . $value[1],
                'result' => $result ? '<i class="iconfont icon-success text-success"></i>' : '<i class="iconfont icon-delete text-danger"></i>',
            ];
        }

        return $need;
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
            $moduleName = pathinfo($files->getInfo('name'));
            $moduleName = $moduleName['filename'];
            //检查插件目录是否存在
            if (file_exists(APP_PATH . $moduleName)) {
                $this->error('该模块目录已经存在！');
            }

            //上传临时文件地址
            $filename = $files->getInfo('tmp_name');
            $zip = new \util\PclZip($filename);
            $status = $zip->extract(PCLZIP_OPT_PATH, APP_PATH . $moduleName);
            if ($status) {
                $this->success('模块解压成功，可以进入模块管理进行安装！', url('index'));
            } else {
                $this->error('模块解压失败！');
            }
        }
    }

}
