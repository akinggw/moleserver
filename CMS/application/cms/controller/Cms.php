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
// | cms管理
// +----------------------------------------------------------------------
namespace app\cms\controller;

use app\cms\model\Cms as Cms_Model;
use app\cms\model\Page as Page_Model;
use app\common\controller\Adminbase;
use think\Db;

class Cms extends Adminbase
{
    protected function initialize()
    {
        parent::initialize();
        $this->Cms_Model = new Cms_Model;
    }

    public function index()
    {
        return $this->fetch();
    }

    //栏目信息列表
    public function classlist()
    {
        $catid = $this->request->param('catid/d', 0);
        //当前栏目信息
        $catInfo = getCategory($catid);
        if (empty($catInfo)) {
            $this->error('该栏目不存在！');
        }
        //栏目所属模型
        $modelid = $catInfo['modelid'];
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 1);
            $map = $this->buildparams();

            //检查模型是否被禁用
            if (!getModel($modelid, 'status')) {
                $this->error('模型被禁用！');
            }
            $modelCache = cache("Model");
            $tableName = $modelCache[$modelid]['tablename'];
            $conditions = [
                ['catid', '=', $catid],
                ['status', '<>', -1],
            ];
            $total = Db::name($tableName)->where($map)->where($conditions)->count();
            $list = Db::name($tableName)->page($page, $limit)->where($map)->where($conditions)->order(['listorder', 'id' => 'desc'])->select();
            $_list = [];
            foreach ($list as $k => $v) {
                $v['updatetime'] = date('Y-m-d H:i:s', $v['updatetime']);
                $v['url'] = $this->Cms_Model->buildContentUrl($v['catid'], $v['id']);
                $_list[] = $v;
            }
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);
        }
        /*移动栏目 复制栏目*/
        $tree = new \util\Tree();
        $tree->icon = array('&nbsp;&nbsp;&nbsp;│ ', '&nbsp;&nbsp;&nbsp;├─ ', '&nbsp;&nbsp;&nbsp;└─ ');
        $tree->nbsp = '&nbsp;&nbsp;&nbsp;';
        $categorys = array();
        $result = Db::name('category')->order(array('listorder', 'id' => 'ASC'))->select();
        foreach ($result as $k => $v) {
            if ($v['type'] != 2) {
                continue;
            }
            if ($modelid && $modelid != $v['modelid'] && $v['child'] == 0) {
                continue;
            }
            $v['disabled'] = $v['child'] ? 'disabled' : '';
            $v['selected'] = $k == $catid ? 'selected' : '';
            $categorys[$k] = $v;
        }
        $str = "<option value='\$id' \$selected \$disabled>\$spacer \$catname</option>";
        $tree->init($categorys);
        $string = $tree->get_tree(0, $str);

        $this->assign('string', $string);
        $this->assign('catid', $catid);
        return $this->fetch();
    }

    //回收站
    public function recycle()
    {
        $catid = $this->request->param('catid/d', 0);
        //当前栏目信息
        $catInfo = getCategory($catid);
        if (empty($catInfo)) {
            $this->error('该栏目不存在！');
        }
        //栏目所属模型
        $modelid = $catInfo['modelid'];
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 1);
            $map = $this->buildparams();
            $modelCache = cache("Model");
            $tableName = $modelCache[$modelid]['tablename'];
            $total = Db::name($tableName)->where($map)->where(['catid' => $catid, 'status' => -1])->count();
            $_list = Db::name($tableName)->page($page, $limit)->where($map)->where(['catid' => $catid, 'status' => -1])->order(['listorder', 'id' => 'desc'])->select();
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);
        }
        $this->assign('catid', $catid);
        return $this->fetch();
    }

    //还原回收站
    public function restore()
    {
        $catid = $this->request->param('catid/d', 0);
        //当前栏目信息
        $catInfo = getCategory($catid);
        if (empty($catInfo)) {
            $this->error('该栏目不存在！');
        }
        //栏目所属模型
        $modelid = $catInfo['modelid'];
        $ids = $this->request->param('ids');
        $modelInfo = cache('Model');
        $modelInfo = $modelInfo[$modelid];
        if ($ids) {
            if (!is_array($ids)) {
                $ids = array(0 => $ids);
            }
            Db::name($modelInfo['tablename'])->where('id', 'in', $ids)->setField('status', 1);
        }
        $this->success('还原成功！');

    }

    //移动文章
    public function remove()
    {
        if ($this->request->isPost()) {
            $catid = $this->request->param('catid/d', 0);
            if (!$catid) {
                $this->error("请指定栏目！");
            }
            //需要移动的信息ID集合
            $ids = $this->request->param('ids/s');
            //目标栏目
            $tocatid = $this->request->param('tocatid/d', 0);
            if ($ids) {
                if ($tocatid == $catid) {
                    $this->error('目标栏目和当前栏目是同一个栏目！');
                }
                $modelid = getCategory($tocatid, 'modelid');
                if (!$modelid) {
                    $this->error('该模型不存在！');
                }
                $ids = array_filter(explode('|', $ids), 'intval');
                $tableName = Db::name('model')->where('id', $modelid)->where('status', 1)->value('tablename');
                if (!$tableName) {
                    return $this->error('模型被冻结不可操作~');
                }
                if (Db::name(ucwords($tableName))->where('id', 'in', $ids)->update(['catid' => $tocatid])) {
                    $this->success('栏目修改成功~');
                } else {
                    $this->error('栏目修改失败~');
                }
            } else {
                $this->error('请选择需要移动的信息！');
            }

        }

    }

    //添加栏目
    public function add()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();
            $catid = intval($data['modelField']['catid']);
            if (empty($catid)) {
                $this->error("请指定栏目ID！");
            }
            $category = getCategory($catid);
            if (empty($category)) {
                $this->error('该栏目不存在！');
            }
            if ($category['type'] == 2) {
                $data['modelFieldExt'] = isset($data['modelFieldExt']) ? $data['modelFieldExt'] : [];
                try {
                    $this->Cms_Model->addModelData($data['modelField'], $data['modelFieldExt']);
                } catch (\Exception $ex) {
                    $this->error($ex->getMessage());
                }
            } else if ($category['type'] == 1) {
                $Page_Model = new Page_Model;
                if (!$Page_Model->savePage($data['modelField'])) {
                    $error = $Page_Model->getError();
                    $this->error($error ? $error : '操作失败！');
                }
            }
            $this->success('操作成功！');
        } else {
            $catid = $this->request->param('catid/d', 0);
            $category = getCategory($catid);
            if (empty($category)) {
                $this->error('该栏目不存在！');
            }
            if ($category['type'] == 2) {
                $modelid = $category['modelid'];
                $fieldList = $this->Cms_Model->getFieldList($modelid);
                $this->assign([
                    'catid' => $catid,
                    'fieldList' => $fieldList,
                ]);
                return $this->fetch();
            } else if ($category['type'] == 1) {
                $Page_Model = new Page_Model;
                $info = $Page_Model->getPage($catid);
                $this->assign([
                    'info' => $info,
                    'catid' => $catid,
                ]);
                return $this->fetch('singlepage');
            }

        }
    }

    //编辑信息
    public function edit()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();
            $data['modelFieldExt'] = isset($data['modelFieldExt']) ? $data['modelFieldExt'] : [];
            try {
                $this->Cms_Model->editModelData($data['modelField'], $data['modelFieldExt']);
            } catch (\Exception $ex) {
                $this->error($ex->getMessage());
            }
            $this->success('编辑成功！');

        } else {
            $catid = $this->request->param('catid/d', 0);
            $id = $this->request->param('id/d', 0);
            $category = getCategory($catid);
            if (empty($category)) {
                $this->error('该栏目不存在！');
            }
            if ($category['type'] == 2) {
                $modelid = $category['modelid'];
                $fieldList = $this->Cms_Model->getFieldList($modelid, $id);
                $this->assign([
                    'catid' => $catid,
                    'fieldList' => $fieldList,
                ]);
                return $this->fetch();
            } else {
                return $this->fetch('singlepage');
            }

        }

    }

    //删除
    public function delete()
    {
        $catid = $this->request->param('catid/d', 0);
        $ids = $this->request->param('ids/a', null);
        if (empty($ids) || !$catid) {
            $this->error('参数错误！');
        }
        if (!is_array($ids)) {
            $ids = array(0 => $ids);
        }
        $modelid = getCategory($catid, 'modelid');
        $cmsConfig = cache("Cms_Config");
        try {
            foreach ($ids as $id) {
                $this->Cms_Model->deleteModelData($modelid, $id, $cmsConfig['web_site_recycle']);
            }
        } catch (\Exception $ex) {
            $this->error($ex->getMessage());
        }

        $this->success('删除成功！');
    }

    //清空回收站
    public function destroy()
    {
        $catid = $this->request->param('catid/d', 0);
        $ids = $this->request->param('ids/a', null);
        if (empty($ids) || !$catid) {
            $this->error('参数错误！');
        }
        if (!is_array($ids)) {
            $ids = array(0 => $ids);
        }
        $modelid = getCategory($catid, 'modelid');
        $cmsConfig = cache("Cms_Config");
        try {
            foreach ($ids as $id) {
                $this->Cms_Model->deleteModelData($modelid, $id);
            }
        } catch (\Exception $ex) {
            $this->error($ex->getMessage());
        }

        $this->success('销毁成功！');
    }

    //面板
    public function panl()
    {
        $info['category'] = Db::name('Category')->count();
        $info['model'] = Db::name('Model')->where(['module' => 'cms'])->count();
        $info['tags'] = Db::name('Tags')->count();
        $info['doc'] = 0;
        $modellist = cache('Model');
        foreach ($modellist as $model) {
            if ($model['module'] !== 'cms') {
                continue;
            }
            $tmp = Db::name($model['tablename'])->count();
            $info['doc'] += $tmp;
        }
        $this->assign('info', $info);
        return $this->fetch();
    }

    //显示栏目菜单列表
    public function public_categorys()
    {
        $json = [];
        $categorys = Db::name('Category')->order(array('listorder', 'id' => 'ASC'))->select();
        foreach ($categorys as $rs) {
            //剔除无子栏目外部链接
            if ($rs['type'] == 3 && $rs['child'] == 0) {
                continue;
            }
            $data = array(
                'id' => $rs['id'],
                'parentid' => $rs['parentid'],
                'catname' => $rs['catname'],
                'type' => $rs['type'],
            );
            //终极栏目
            if ($rs['child'] == 0) {
                $data['target'] = 'right';
                $data['url'] = url('cms/cms/classlist', array('catid' => $rs['id']));
            } else {
                $data['isParent'] = true;
            }
            //单页
            if ($rs['type'] == 1) {
                $data['target'] = 'right';
                $data['url'] = url('cms/cms/add', array('catid' => $rs['id']));
            }
            $json[] = $data;
        }
        $this->assign('json', json_encode($json));
        return $this->fetch();
    }

    /**
     * 排序
     */
    public function listorder()
    {
        $catid = $this->request->param('catid/d', 0);
        $id = $this->request->param('id/d', 0);
        $listorder = $this->request->param('value/d', 0);
        $modelid = getCategory($catid, 'modelid');
        $modelCache = cache("Model");
        if (empty($modelCache[$modelid])) {
            return false;
        };
        $tableName = $modelCache[$modelid]['tablename'];
        if (Db::name($tableName)->where('id', $id)->update(['listorder' => $listorder])) {
            $this->success("排序成功！");
        } else {
            $this->error("排序失败！");
        }
    }

    /**
     * 状态
     */
    /*public function setstate()
{
$catid = $this->request->param('catid/d', 0);
$id = $this->request->param('id/d', 0);
$status = $this->request->param('status/d');
$modelid = getCategory($catid, 'modelid');
$modelCache = cache("Model");
if (empty($modelCache[$modelid])) {
return false;
};
$tableName = ucwords($modelCache[$modelid]['tablename']);
if (Db::name($tableName)->where('id', $id)->update(['status' => $status])) {
//更新栏目缓存
cache('Category', null);
getCategory($id, '', true);
$this->success('操作成功！');
} else {
$this->error('操作失败！');
}
}*/

}
