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
// | 栏目权限模型
// +----------------------------------------------------------------------
namespace app\cms\model;

use \think\Model;

class CategoryPriv extends Model
{
    /**
     * 更新权限
     * @param  $catid 栏目ID
     * @param  $priv_datas
     * @param  $is_admin 1为管理员
     */
    public function update_priv($catid, $priv_datas, $is_admin = 1)
    {
        //删除旧的
        self::where(array('catid' => $catid, 'is_admin' => $is_admin))->delete();
        if (is_array($priv_datas) && !empty($priv_datas)) {
            foreach ($priv_datas as $r) {
                $r = explode(',', $r);
                //动作
                $action = $r[0];
                //角色或者会员用户组
                $roleid = $r[1];
                self::create(array('catid' => $catid, 'roleid' => $roleid, 'is_admin' => $is_admin, 'action' => $action));
            }
        }
    }

    /**
     * 检查栏目权限
     * @param $privs 权限数据
     * @param $action 动作
     * @param $roleid 角色
     * @param $is_admin 是否为管理组
     */
    public function check_category_priv($privs, $action, $roleid, $is_admin = 1)
    {
        $checked = '';
        foreach ($privs as $priv) {
            if ($priv['is_admin'] == $is_admin && $priv['roleid'] == $roleid && $priv['action'] == $action) {
                $checked = 'checked';
            }

        }
        return $checked;
    }

}
