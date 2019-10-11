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
// | 支付模块管理
// +----------------------------------------------------------------------
namespace app\pay\controller;

use app\common\controller\Adminbase;
use app\pay\model\Account as Account_Model;
use app\pay\model\Payment as Payment_Model;
use app\pay\model\Spend as Spend_Model;
use think\Db;

class Payment extends Adminbase
{
    protected function initialize()
    {
        parent::initialize();
        $this->Payment_Model = new Payment_Model;
        $this->Account_Model = new Account_Model;
        $this->Spend_Model = new Spend_Model;

    }

    public function modify_deposit()
    {
        if ($this->request->isAjax()) {
            $data = $this->request->post();
            $result = $this->validate($data, 'Account');
            if (true !== $result) {
                return $this->error($result);
            }
            $userinfo = Db::name('member')->where('username', trim($data['username']))->find();
            if ($userinfo) {
                if ($data['pay_unit']) {
                    //增加
                    $this->Account_Model->_add($data['pay_type'], floatval($data['unit']), 'recharge', $userinfo['id'], $userinfo['username'], $data['usernote'], $this->_userinfo['username']);
                } else {
                    //减少
                    $this->Spend_Model->_spend($data['pay_type'], floatval($data['unit']), $userinfo['id'], $userinfo['username'], '后台充值', $data['usernote']);
                }
                $this->success("充值成功！");
            } else {
                $this->error('用户不存在！');
            }

        } else {
            return $this->fetch();
        }
    }

    //入账列表
    public function pay_list()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 1);
            $map = $this->buildparams();
            $total = $this->Account_Model->where($map)->count();
            $data = $this->Account_Model->where($map)->page($page, $limit)->order('id', 'desc')->select();
            return json(["code" => 0, "count" => $total, "data" => $data]);
        } else {
            return $this->fetch();
        }
    }

    //支付模块列表
    public function index()
    {
        if ($this->request->isAjax()) {
            $data = $this->Payment_Model->select();
            return json(["code" => 0, "data" => $data]);
        } else {
            return $this->fetch();
        }
    }

    //删除入账记录
    public function delete()
    {
        $ids = $this->request->param('ids/a');
        empty($ids) && $this->error('参数错误！');
        if (!is_array($ids)) {
            $ids = array($ids);
        }
        $res = $this->Account_Model->where('id', 'in', $ids)->delete();
        if ($res !== false) {
            $this->success('删除成功！');
        } else {
            $this->error('删除失败！');
        }
    }

    //支付模块配置
    public function edit()
    {
        if ($this->request->isPost()) {
            $data = [];
            $id = $this->request->param('id/d', 0);
            $config = $this->request->param('config/a');
            $data['status'] = $this->request->param('status/d', 0);
            $data['config'] = serialize($config);
            if ($this->Payment_Model->allowField(true)->save($data, ['id' => $id])) {
                cache('Pay_Config', null);
                $this->success("更新成功！", url('index'));
            } else {
                $this->success("更新失败！");
            }
        } else {
            $id = $this->request->param('id/d', 0);
            $info = $this->Payment_Model->where('id', $id)->find();
            $this->assign('info', $info);
            return $this->fetch($info['name']);
        }

    }

}
