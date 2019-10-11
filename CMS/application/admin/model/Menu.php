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
// | 后台菜单模型
// +----------------------------------------------------------------------
namespace app\admin\model;

use app\admin\model\AuthRule;
use app\admin\service\User;
use \think\Db;
use \think\Model;

class Menu extends Model
{

    /**
     * 获取菜单
     * @return type
     */
    final public function getMenuList()
    {
        $data = $this->getTree(0);
        return $data;
    }

    /**
     * 取得树形结构的菜单
     * @param type $myid
     * @param type $parent
     * @param type $Level
     * @return type
     */
    final public function getTree($myid, $parent = "", $Level = 1)
    {
        $data = $this->adminMenu($myid);
        $Level++;
        if (is_array($data)) {
            $ret = null;
            foreach ($data as $a) {
                $id = $a['id'];
                $name = $a['app'];
                $controller = $a['controller'];
                $action = $a['action'];
                //附带参数
                $fu = "";
                if ($a['parameter']) {
                    $fu = "?" . $a['parameter'];
                }
                $array = array(
                    "menuid" => $id,
                    "id" => $id . $name,
                    "title" => $a['title'],
                    "icon" => $a['icon'],
                    "parent" => $parent,
                    "url" => url("{$name}/{$controller}/{$action}{$fu}", array("menuid" => $id)),
                );
                $ret[$id . $name] = $array;
                $child = $this->getTree($a['id'], $id, $Level);
                //由于后台管理界面只支持三层，超出的不层级的不显示
                if ($child && $Level <= 3) {
                    $ret[$id . $name]['items'] = $child;
                }
            }
        }
        return $ret;
    }

    /**
     * 按父ID查找菜单子项
     * @param integer $parentid   父菜单ID
     * @param integer $with_self  是否包括他自己
     */
    final public function adminMenu($parentid, $with_self = false)
    {
        $parentid = (int) $parentid;
        $result = $this->where(array('parentid' => $parentid, 'status' => 1))->order('listorder ASC,id ASC')->cache(60)->select()->toArray();
        if (empty($result)) {
            $result = array();
        }
        if ($with_self) {
            $parentInfo = $this->where(array('id' => $parentid))->find();
            $result2[] = $parentInfo ? $parentInfo : array();
            $result = array_merge($result2, $result);
        }
        //是否超级管理员
        if (User::instance()->isAdministrator()) {
            return $result;
        }
        $array = array();
        foreach ($result as $v) {
            $rule = $v['app'] . '/' . $v['controller'] . '/' . $v['action'];
            if ($this->checkRule($rule, [1, 2], null)) {
                $array[] = $v;
            }
        }
        return $array;
    }

    /**
     * 权限检测
     * @param string  $rule    检测的规则
     * @param string  $mode    check模式
     * @return boolean
     */
    final protected function checkRule($rule, $type = AuthRule::RULE_URL, $mode = 'url')
    {
        static $Auth = null;
        if (!$Auth) {
            $Auth = new \libs\Auth();
        }
        if (!$Auth->check($rule, User::instance()->id, $type, $mode)) {
            return false;
        }
        return true;
    }

    /**
     * 返回后台节点数据
     * @param boolean $tree    是否返回多维数组结构(生成菜单时用到),为false返回一维数组(生成权限节点时用到)
     * @retrun array
     *
     * 注意,返回的主菜单节点数组中有'controller'元素,以供区分子节点和主节点
     *
     */
    final public function returnNodes($tree = true)
    {
        static $tree_nodes = array();
        if ($tree && !empty($tree_nodes[(int) $tree])) {
            return $tree_nodes[$tree];
        }
        if ((int) $tree) {
            $list = $this->order('listorder ASC,id ASC')->select()->toArray();
            foreach ($list as $key => $value) {
                $list[$key]['url'] = $value['app'] . '/' . $value['controller'] . '/' . $value['action'];
            }
            $nodes = list_to_tree($list, $pk = 'id', $pid = 'parentid', $child = 'operator', $root = 0);
            foreach ($nodes as $key => $value) {
                if (!empty($value['operator'])) {
                    $nodes[$key]['child'] = $value['operator'];
                    unset($nodes[$key]['operator']);
                }
            }
        } else {
            $nodes = $this->order('listorder ASC,id ASC')->select()->toArray();
            foreach ($nodes as $key => $value) {
                $nodes[$key]['url'] = $value['app'] . '/' . $value['controller'] . '/' . $value['action'];
            }
        }
        $tree_nodes[(int) $tree] = $nodes;
        return $nodes;
    }

    /**
     * 模块安装时进行菜单注册
     * @param array $data 菜单数据
     * @param array $config 模块配置
     * @param type $parentid 父菜单ID
     * @return boolean
     */
    public function installModuleMenu(array $data, array $config, $parentid = 0)
    {
        if (empty($data) || !is_array($data)) {
            $this->error = '没有数据！';
            return false;
        }
        if (empty($config)) {
            $this->error = '模块配置信息为空！';
            return false;
        }
        //默认安装时父级ID
        $defaultMenuParentid = $this->where(array('app' => 'admin', 'controller' => 'module', 'action' => 'list'))->value('id') ?: 45;
        //安装模块名称
        $moduleNama = $config['module'];
        foreach ($data as $rs) {
            if (empty($rs['route'])) {
                $this->error = '菜单信息配置有误，route 不能为空！';
                return false;
            }
            $route = $this->menuRoute($rs['route'], $moduleNama);
            $pid = $parentid ?: ((is_null($rs['parentid']) || !isset($rs['parentid'])) ? (int) $defaultMenuParentid : $rs['parentid']);
            $newData = array_merge(array(
                'title' => $rs['name'],
                'icon' => isset($rs['icon']) ? $rs['icon'] : '',
                'parentid' => $pid,
                'status' => isset($rs['status']) ? $rs['status'] : 0,
                'tip' => isset($rs['remark']) ? $rs['remark'] : '',
                'listorder' => isset($rs['listorder']) ? $rs['listorder'] : 0,
            ), $route);

            $result = self::create($newData);
            if (!$result) {
                return false;
            }
            //是否有子菜单
            if (!empty($rs['child'])) {
                if ($this->installModuleMenu($rs['child'], $config, $result['id']) !== true) {
                    return false;
                }
            }
        }
        //清除缓存
        cache('Menu', null);
        return true;
    }

    /**
     * 把模块安装时，Menu.php中配置的route进行转换
     * @param type $route route内容
     * @param type $moduleNama 安装模块名称
     * @return array
     */
    private function menuRoute($route, $moduleNama)
    {
        $route = explode('/', $route, 3);
        if (count($route) < 3) {
            array_unshift($route, $moduleNama);
        }
        $data = array(
            'app' => $route[0],
            'controller' => $route[1],
            'action' => $route[2],
        );
        return $data;
    }

    /**
     * 添加插件后台管理菜单
     * @param type $info
     * @param type $adminlist
     * @return boolean
     */
    public function addAddonMenu($info, $admin_list = null)
    {
        if (empty($info)) {
            return false;
        }
        //查询出“插件后台列表”菜单ID
        $menuId = self::where(array("app" => "addons", "controller" => "addons", "action" => "addonadmin"))->value('id');
        if (empty($menuId)) {
            return false;
        }
        $data = array(
            //父ID
            "parentid" => $menuId,
            //模块目录名称，也是项目名称
            "app" => "addons",
            //插件名称
            "controller" => $info['name'],
            //方法名称
            "action" => "index",
            //附加参数 例如：a=12&id=777
            "parameter" => "isadmin=1",
            //状态，1是显示，0是不显示
            "status" => 1,
            //名称
            "title" => $info['title'],
            //备注
            "tip" => $info['title'] . "插件管理后台！",
            //排序
            "listorder" => 0,
        );
        //添加插件后台
        $parentid = self::create($data);
        if (!$parentid) {
            return false;
        }
        //显示“插件后台列表”菜单
        self::where(array('id' => $menuId))->update(array('status' => 1));
        //插件具体菜单
        if (!empty($admin_list)) {
            foreach ($admin_list as $key => $menu) {
                //检查参数是否存在
                if (empty($menu['title']) || empty($menu['action'])) {
                    continue;
                }
                //如果是index，跳过，因为已经有了。。。
                if ($menu['action'] == 'index') {
                    continue;
                }
                $data = array(
                    //父ID
                    "parentid" => $parentid->getAttr('parentid'),
                    //模块目录名称，也是项目名称
                    "app" => "addons",
                    //文件名称，比如LinksAction.class.php就填写 Links
                    "controller" => $info['name'],
                    //方法名称
                    "action" => $menu['action'],
                    //附加参数 例如：a=12&id=777
                    "parameter" => 'isadmin=1',
                    //状态，1是显示，0是不显示
                    "status" => (int) $menu['status'],
                    //名称
                    "title" => $menu['title'],
                    //备注
                    "tip" => $menu['tip'] ?: '',
                    //排序
                    "listorder" => (int) $menu['listorder'],
                );
                self::create($data);
            }
        }
        return true;
    }

    /**
     * 删除对应插件菜单和权限
     * @param type $info 插件信息
     * @return boolean
     */
    public function delAddonMenu($info)
    {
        if (empty($info)) {
            return false;
        }
        //查询出“插件后台列表”菜单ID
        $menuId = self::where(array("app" => "Addons", "controller" => "Addons", "action" => "addonadmin"))->value('id');
        if (empty($menuId)) {
            return false;
        }
        //删除对应菜单
        self::where(array('app' => 'Addons', 'controller' => $info['name']))->delete();
        //删除权限
        //M("Access")->where(array("app" => "Addons", 'controller' => $info['name']))->delete();
        //检查“插件后台列表”菜单下还有没有菜单，没有就隐藏
        $count = self::where(array('parentid' => $menuId))->count();
        if (!$count) {
            self::where(array('id' => $menuId))->update(array('status' => 0));
        }
        return true;
    }

    /**
     * 更新缓存
     * @param type $data
     * @return type
     */
    public function menu_cache()
    {
        $data = $this->select()->toArray();
        if (empty($data)) {
            return false;
        }
        $cache = array();
        foreach ($data as $rs) {
            $cache[$rs['id']] = $rs;
        }
        cache('Menu', $cache);
        return $cache;
    }

}
