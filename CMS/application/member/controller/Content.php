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
// | 会员投稿管理
// +----------------------------------------------------------------------
namespace app\member\controller;

use app\cms\model\Category as Category_Model;
use app\cms\model\Cms as Cms_Model;
use app\member\model\MemberContent as Member_Content_Model;
use think\Db;

class Content extends MemberBase
{
    protected function initialize()
    {
        parent::initialize();
        $this->Cms_Model = new Cms_Model;
        //$this->Member_Content_Model = new Member_Content_Model;
    }

    public function publish()
    {
        $groupinfo = $this->_check_group_auth($this->userinfo['groupid']);
        //判断每日投稿数
        $allowpostnum = Member_Content_Model::where('uid', $this->userid)->whereTime('create_time', 'd')->count();
        if ($groupinfo['allowpostnum'] > 0 && $allowpostnum >= $groupinfo['allowpostnum']) {
            $this->error("今日投稿数量已达上限！");
        }
        if ($this->request->isPost()) {
            $data = $this->request->post(false);
            $data['modelField']['uid'] = $this->userid;
            $data['modelField']['username'] = $this->userinfo['username'];
            $catid = intval($data['modelField']['catid']);
            if (empty($catid)) {
                $this->error("请指定栏目ID！");
            }
            $catidPrv = Db::name('category_priv')->where(array("catid" => $catid, "roleid" => $this->userinfo['groupid'], "is_admin" => 0, "action" => "add"))->find();
            if (empty($catidPrv)) {
                $this->error("您没有该栏目投稿权限！");
            }
            $category = Db::name('Category')->find($catid);
            if (empty($category)) {
                $this->error('该栏目不存在！');
            }
            //判断会员组投稿是否需要审核
            if ($groupinfo['allowpostverify']) {
                $data['modelField']['status'] = 1;
            } else {
                $data['modelField']['status'] = 0;
            }
            if ($category['type'] == 2) {
                $data['modelFieldExt'] = isset($data['modelFieldExt']) ? $data['modelFieldExt'] : [];
                try {
                    $id = $this->Cms_Model->addModelData($data['modelField'], $data['modelFieldExt']);
                } catch (\Exception $ex) {
                    $this->error($ex->getMessage());
                }
            }
            //添加投稿记录
            if ($id) {
                Member_Content_Model::create([
                    'catid' => $catid,
                    'content_id' => $id,
                    'uid' => $data['modelField']['uid'],
                    'username' => $data['modelField']['username'],
                    'create_time' => time(),
                    'status' => $data['modelField']['status'],
                ]);
            }
            if ($data['modelField']['status'] == 1) {
                $this->success('操作成功，内容已通过审核！', url('published'));
            } else {
                $this->success('操作成功，等待管理员审核！', url('published'));
            }
        } else {
            $step = $this->request->param('step/d', 1);
            if ($step == 1) {
                return $this->fetch('/declaration');
            }
            $catid = $this->request->param('catid/d', 0);
            $tree = new \util\Tree();
            $str = "<option value='\$catidurl' \$selected \$disabled>\$spacer \$catname</option>";
            $array = cache("Category");
            foreach ($array as $k => $v) {
                $array[$k] = $v = Db::name('Category')->find($v['id']);
                if ($v['id'] == $catid) {
                    $array[$k]['selected'] = "selected";
                }
                //含子栏目和单页不可以发表
                if ($v['child'] == 1 || $v['type'] == 1) {
                    $array[$k]['disabled'] = "disabled";
                    $array[$k]['catidurl'] = url('publish', array('step' => 2));
                } else {
                    $array[$k]['disabled'] = "";
                    $array[$k]['catidurl'] = url('publish', array('step' => 2, 'catid' => $v['id']));
                }
            }
            $tree->init($array);
            $categoryselect = $tree->get_tree(0, $str, 0);
            //如果有选择栏目的情况下
            if ($catid) {
                $category = Db::name('Category')->find($catid);
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
                }
            }
            $this->assign("categoryselect", $categoryselect);
            return $this->fetch('/publish');
        }

    }

    /**
     * 编辑内容
     */
    public function edit()
    {
        $groupinfo = $this->_check_group_auth($this->userinfo['groupid']);
        if ($this->request->isPost()) {
            $data = $this->request->post(false);

            $catid = intval($data['modelField']['catid']);
            if (empty($catid)) {
                $this->error("请指定栏目ID！");
            }
            $category = Db::name('Category')->find($catid);
            if (empty($category)) {
                $this->error('该栏目不存在！');
            }
            $catidPrv = Db::name('category_priv')->where(array("catid" => $catid, "roleid" => $this->userinfo['groupid'], "is_admin" => 0, "action" => "add"))->find();
            if (empty($catidPrv)) {
                $this->error("您没有该栏目投稿权限！");
            }

            //判断会员组投稿是否需要审核
            if ($groupinfo['allowpostverify']) {
                $data['status'] = 1;
            } else {
                $data['status'] = 0;
            }

            if ($category['type'] == 2) {
                $data['modelFieldExt'] = isset($data['modelFieldExt']) ? $data['modelFieldExt'] : [];
                try {
                    $this->Cms_Model->editModelData($data['modelField'], $data['modelFieldExt']);
                } catch (\Exception $ex) {
                    $this->error($ex->getMessage());
                }
            }
            if ($data['status'] == 1) {
                $this->success('编辑成功，内容已通过审核！', url('published'));
            } else {
                $this->success('编辑成功，等待管理员审核！', url('published'));
            }

        } else {
            $id = $this->request->param('id/d', 0);
            $info = Member_Content_Model::where(array('uid' => $this->userid, 'id' => $id))->find();
            if (empty($info)) {
                $this->error('稿件不存在！');
            }
            $catid = $info['catid'];
            //取得栏目数据
            $category = Category_Model::getCategory($catid);
            if (empty($category)) {
                $this->error('该栏目不存在！', url('publish', array('step' => 2)));
            }
            //模型ID
            $modelid = (int) $category['modelid'];
            //判断是否终极栏目
            if ($category['child'] || $category['type'] == 0) {
                $this->error("该栏目不允许投稿！", url('publish', array('step' => 2)));
            }
            $fieldList = $this->Cms_Model->getFieldList($modelid, $info['content_id']);
            $this->assign([
                'catid' => $catid,
                'fieldList' => $fieldList,
            ]);
            return $this->fetch('/edit');
        }

    }

    public function published()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 10);
            $type = $this->request->param('type/s', "");
            $where = array('uid' => $this->userid);
            if ('check' == $type) {
                $where['status'] = 1;
            }
            if ('checking' == $type) {
                $where['status'] = 0;
            }
            $total = Member_Content_Model::where($where)->count();
            $_list = Member_Content_Model::where($where)->page($page, $limit)->order(array("id" => "DESC"))->select();
            foreach ($_list as $k => $v) {
                $modelid = Category_Model::getCategory($v['catid'], 'modelid');
                $tablename = ucwords(getModel($modelid, 'tablename'));
                $info = Db::name($tablename)->where(array("id" => $v['content_id'], "sysadd" => 0))->find();
                if ($info) {
                    $_list[$k]['url'] = $this->Cms_Model->buildContentUrl($v['catid'], $v['content_id']);
                    $_list[$k]['title'] = $info['title'];
                    $_list[$k]['catname'] = Category_Model::getCategory($v['catid'], 'catname');
                }
            }
            $result = array("code" => 0, "count" => $total, "data" => $_list);

            return json($result);
        }
        return $this->fetch('/published');
    }

    //删除
    public function del()
    {
        $id = $this->request->param('id/d', 0);
        if (empty($id)) {
            $this->error('请指定需要删除的信息！');
        }
        //信息
        $info = Member_Content_Model::where('id', $id)->find();
        //只能删除自己的 且 未通过审核的
        if ($info && $info['uid'] == $this->userid && $info['status'] != 1) {
            //取得栏目信息
            $category = Category_Model::getCategory($info['catid']);
            if (!$category) {
                $this->success('栏目不存在！');
            }
            try {
                $this->Cms_Model->deleteModelData($category['modelid'], $info['content_id']);
            } catch (\Exception $ex) {
                $this->error($ex->getMessage());
            }
            $this->success('删除成功！');
        } else {
            $this->error('对不起，你无权删除！');
        }
    }

    //检查会员组权限
    private function _check_group_auth($groupid)
    {
        $grouplist = cache("Member_Group"); //会员模型
        if (!$grouplist[$groupid]['allowpost']) {
            $this->error('你没有权限投稿，请升级会员组！');
        }
        return $grouplist[$groupid];
    }

}
