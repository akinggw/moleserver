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
// | 会员支付前台
// +----------------------------------------------------------------------
namespace app\pay\controller;

use app\common\controller\Adminbase;
use app\pay\model\Spend as Spend_Model;

class Spend extends Adminbase
{
    protected function initialize()
    {
        parent::initialize();
        $this->Spend_Model = new Spend_Model;

    }

    public function index()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 1);
            $map = $this->buildparams();
            $total = $this->Spend_Model->where($map)->count();
            $data = $this->Spend_Model->where($map)->page($page, $limit)->order('id', 'desc')->select();
            return json(["code" => 0, "count" => $total, "data" => $data]);
        } else {
            return $this->fetch();
        }
    }

    //删除消费记录
    public function delete()
    {
        $ids = $this->request->param('ids/a');
        empty($ids) && $this->error('参数错误！');
        if (!is_array($ids)) {
            $ids = array($ids);
        }
        $res = $this->Spend_Model->where('id', 'in', $ids)->delete();
        if ($res !== false) {
            $this->success('删除成功！');
        } else {
            $this->error('删除失败！');
        }
    }

}
