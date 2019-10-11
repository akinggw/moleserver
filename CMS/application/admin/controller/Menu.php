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
// | 后台菜单管理
// +----------------------------------------------------------------------
namespace app\admin\controller;

use app\admin\model\Menu as Menu_Model;
use app\common\controller\Adminbase;
use think\Db;

class Menu extends Adminbase
{

    //后台菜单首页
    public function index()
    {
        if ($this->request->isAjax()) {
            $tree = new \util\Tree();
            $tree->icon = array('', '', '');
            $tree->nbsp = '';
            $result = Menu_Model::order(array('listorder', 'id' => 'DESC'))->select()->toArray();

            $tree->init($result);
            $_list = $tree->getTreeList($tree->getTreeArray(0), 'title');
            $total = count($_list);
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);
        }
        return $this->fetch();

    }

    //添加后台菜单
    public function add()
    {
        if ($this->request->isPost()) {
            $data = $this->request->param();
            if (!isset($data['status'])) {
                $data['status'] = 0;
            } else {
                $data['status'] = 1;
            }

            $result = $this->validate($data, 'Menu.add');
            if (true !== $result) {
                return $this->error($result);
            }
            if (Menu_Model::create($data)) {
                $this->success("添加成功！", url("index"));
            } else {
                $this->error('添加失败！');
            }
        } else {
            $tree = new \util\Tree();
            $parentid = $this->request->param('parentid/d', '');
            $result = Menu_Model::order(array('listorder', 'id' => 'DESC'))->select()->toArray();
            $array = array();
            foreach ($result as $r) {
                $r['selected'] = $r['id'] == $parentid ? 'selected' : '';
                $array[] = $r;
            }
            $str = "<option value='\$id' \$selected>\$spacer \$title</option>";
            $tree->init($array);
            $select_categorys = $tree->get_tree(0, $str);
            $this->assign("select_categorys", $select_categorys);
            return $this->fetch();
        }
    }

    /**
     *编辑后台菜单
     */
    public function edit()
    {
        if ($this->request->isPost()) {
            $data = $this->request->param();
            if (!isset($data['status'])) {
                $data['status'] = 0;
            } else {
                $data['status'] = 1;
            }
            $result = $this->validate($data, 'Menu.edit');
            if (true !== $result) {
                return $this->error($result);
            }
            if (Menu_Model::update($data)) {
                $this->success("编辑成功！", url("index"));
            } else {
                $this->error('编辑失败！');
            }
        } else {
            $tree = new \util\Tree();
            $id = $this->request->param('id/d', '');
            $rs = Menu_Model::where(["id" => $id])->find();
            $result = Menu_Model::order(array('listorder', 'id' => 'DESC'))->select()->toArray();
            $array = array();
            foreach ($result as $r) {
                $r['selected'] = $r['id'] == $rs['parentid'] ? 'selected' : '';
                $array[] = $r;
            }
            $str = "<option value='\$id' \$selected>\$spacer \$title</option>";
            $tree->init($array);
            $select_categorys = $tree->get_tree(0, $str);
            $this->assign("data", $rs);
            $this->assign("select_categorys", $select_categorys);
            return $this->fetch();
        }

    }

    /**
     * 菜单删除
     */
    public function delete()
    {
        $id = $this->request->param('id/d');
        if (empty($id)) {
            $this->error('ID错误');
        }
        $result = Menu_Model::where(["parentid" => $id])->find();
        if ($result) {
            $this->error("含有子菜单，无法删除！");
        }
        if (Menu_Model::destroy($id) !== false) {
            $this->success("删除菜单成功！");
        } else {
            $this->error("删除失败！");
        }
    }

    /**
     * 菜单排序
     */
    public function listorder()
    {
        $id = $this->request->param('id/d', 0);
        $listorder = $this->request->param('value/d', 0);
        $rs = Menu_Model::update(['id' => $id, 'listorder' => $listorder]);
        if ($rs) {
            $this->success("菜单排序成功！");
        } else {
            $this->error("菜单排序失败！");
        }
    }

    /**
     * 菜单状态
     */
    public function setstate()
    {
        $id = $this->request->param('id/d');
        empty($id) && $this->error('参数不能为空！');
        $status = $this->request->param('status/d');
        if (Menu_Model::update(['status' => $status], ['id' => $id])) {
            $this->success('操作成功！');
        } else {
            $this->error('操作失败！');
        }
    }

}
