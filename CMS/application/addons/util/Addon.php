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
// | 插件基类 插件需要继承此类
// +----------------------------------------------------------------------
namespace app\addons\util;

use think\Db;
use think\facade\Config;
use think\View;

abstract class Addon
{
    /**
     * 视图实例对象
     * @var view
     * @access protected
     */
    protected $view = null;
    /**
     * $info = array(
     *  'name'=>'Editor',
     *  'title'=>'编辑器',
     *  'description'=>'用于增强整站长文本的输入和显示',
     *  'status'=>1,
     *  'author'=>'thinkphp',
     *  'version'=>'0.1'
     *  )
     */
    public $info = [];
    public $addon_path = '';
    public $config_file = '';
    public $custom_config = '';
    public $admin_list = [];
    public $custom_adminlist = '';
    public $access_url = [];

    public function __construct()
    {
        // 获取当前插件目录
        $this->addon_path = ADDON_PATH . $this->getName() . DIRECTORY_SEPARATOR;
        // 读取当前插件配置信息
        if (is_file($this->addon_path . 'config.php')) {
            $this->config_file = $this->addon_path . 'config.php';
        }
        // 初始化视图模型
        $config['view_path'] = $this->addon_path;
        $config = array_merge(Config::get('template.'), $config);
        $this->view = new View();
        $this->view = $this->view->init($config);
        //加载插件函数文件
        if (file_exists($this->addon_path . 'common.php')) {
            include_once $this->addon_path . 'common.php';
        }
    }

    /**
     * 模板变量赋值
     * @access protected
     * @param mixed $name 要显示的模板变量
     * @param mixed $value 变量的值
     * @return void
     */
    final protected function assign($name, $value = '')
    {
        $this->view->assign($name, $value);
    }

    /**
     * 加载模板和页面输出 可以返回输出内容
     * @access public
     * @param string $template 模板文件名或者内容
     * @param array $vars 模板输出变量
     * @param array $replace 替换内容
     * @param array $config 模板参数
     */
    public function fetch($template = '', $vars = [], $replace = [], $config = [])
    {
        if (!is_file($template)) {
            $template = '/' . $template;
        }
        // 关闭模板布局
        $this->view->engine->layout(false);

        echo $this->view->fetch($template, $vars, $replace, $config);
    }
    /**
     * 渲染内容输出
     * @access public
     * @param string $content 内容
     * @param array $vars 模板输出变量
     * @param array $replace 替换内容
     * @param array $config 模板参数
     * @return mixed
     */
    public function display($content, $vars = [], $replace = [], $config = [])
    {
        // 关闭模板布局
        $this->view->engine->layout(false);

        echo $this->view->display($content, $vars, $replace, $config);
    }
    /**
     * 渲染内容输出
     * @access public
     * @param string $content 内容
     * @param array $vars 模板输出变量
     * @return mixed
     */
    public function show($content, $vars = [])
    {
        // 关闭模板布局
        $this->view->engine->layout(false);

        echo $this->view->fetch($content, $vars, [], [], true);
    }
    /**
     * @title 获取当前模块名
     * @return string
     */
    final public function getName()
    {
        $data = explode('\\', get_class($this));
        return strtolower(array_pop($data));
    }
    /**
     * @title 获取当前模块名
     * @return string
     */
    final public function checkInfo()
    {
        $info_check_keys = ['name', 'title', 'description', 'status', 'author', 'version'];
        foreach ($info_check_keys as $value) {
            if (!array_key_exists($value, $this->info)) {
                return false;
            }

        }
        return true;
    }

    /**
     * @title 获取插件的配置数组
     * @param string $name 可选模块名
     * @return array|mixed|null
     */
    final public function getAddonConfig($name = '')
    {
        static $_config = [];
        if (empty($name)) {
            $name = $this->getName();
        }
        if (isset($_config[$name])) {
            return $_config[$name];
        }
        $map['name'] = $name;
        $map['status'] = 1;
        $config = Db::name('Addons')->where($map)->value('config');
        if ($config) {
            $config = json_decode($config, true);
        } else {
            if (is_file($this->config_file)) {
                $temp_arr = include $this->config_file;
                foreach ($temp_arr as $key => $value) {
                    if ($value['type'] == 'group') {
                        foreach ($value['options'] as $gkey => $gvalue) {
                            foreach ($gvalue['options'] as $ikey => $ivalue) {
                                $config[$ikey] = $ivalue['value'];
                            }
                        }
                    } else {
                        $config[$key] = $temp_arr[$key]['value'];
                    }
                }
                unset($temp_arr);
            }
        }
        $_config[$name] = $config;
        return $config;
    }

    //必须实现安装
    abstract public function install();

    //必须卸载插件方法
    abstract public function uninstall();
}
