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
// | 后台欢迎页
// +----------------------------------------------------------------------
namespace app\admin\controller;

use app\common\controller\Adminbase;
use think\Db;
use app\member\model\Member as Member_Model;
use app\member\model\Userdata as Userdata_Model;
use app\member\model\androiduserinfo as AndroidUserInfo_Mode;
use app\games\model\gameroom as gameroom_Model;
use app\games\model\game as game_Model;
use app\gameserver\model\gamerecords as gamerecords_Model;
use app\gameserver\model\goldoperaterecords as goldoperaterecords_Model;

class Main extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->Member_Model = new Member_Model;
        $this->Userdata_Model = new Userdata_Model;
        $this->AndroidUserInfo_Mode = new AndroidUserInfo_Mode;
        $this->gameroom_Model = new gameroom_Model;
        $this->game_Model = new game_Model;
        $this->gamerecords_Model = new gamerecords_Model;
        $this->goldoperaterecords_Model = new goldoperaterecords_Model;
    }

    //欢迎首页
    public function index()
    {
        $onlineusercount = $this->gameroom_Model->
            field('sum(currealplayercount) as pcurrealplayercount,sum(currobotplayercount) as pcurrobotplayercount')->
            find();

        $this->assign('userInfo', $this->_userinfo);
        $this->assign('sys_info', $this->get_sys_info());
        $this->assign('admincount', $onlineusercount);
       // $this->assign('admincountrobot', $onlineusercount['pcurrobotplayercount']);
        $this->assign('usercount', $this->Member_Model->where('mol_member.gtype != 1')->count());
        $this->assign('machinecount', $this->game_Model->count());
        $this->assign('ordercount', $this->gameroom_Model->count());
        $this->assign('ordersum', $this->AndroidUserInfo_Mode->count());
        $this->assign('cardcount', $this->gamerecords_Model->count());
        $this->assign('cardsum', $this->goldoperaterecords_Model->count());
        return $this->fetch();
    }

    /**
     * 游戏记录
     */
    public function manage()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 15);
            $page = $this->request->param('page/d', 1);

            $_list = $this->gamerecords_Model->page($page, $limit)->
            join('game ge','ge.id = mol_gamerecords.gameid','left')->
            join('member mb','mb.uid = mol_gamerecords.userid','left')->
            field('mol_gamerecords.*,ge.name,mb.username')->
            order(array('mol_gamerecords.collectdate' => 'DESC'))->
            select();

            $total = count($_list);
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);

        }

        return $this->fetch();
    }

    /**
     * 最新在线用户
     */
    public function manageuser()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 15);
            $page = $this->request->param('page/d', 1);

            $_list = $this->Member_Model->page($page, $limit)->
            join('userdata ud','ud.userid = mol_member.uid','left')->
            where('to_days(from_unixtime(lastlogintime, "%Y-%m-%d %H:%i:%s")) = to_days(NOW()) and ud.curgamingstate > 0 and gtype != 1')->
            field('mol_member.*,ud.curgamingstate')->
            order(array('mol_member.lastlogintime' => 'DESC'))->
            withAttr('sex', function ($value, $data) { if($value == 0) return '男'; else return '女';})->
            withAttr('genable', function ($value, $data) { if($value == 0) return '封号'; else return '正常';})->
            withAttr('curgamingstate', function ($value, $data) { if($value == 0) return '正常'; elseif($value == 1) return '准备'; elseif($value == 2) return '游戏中'; elseif($value == 3) return '掉线'; elseif($value == 4) return '排队';})->
            withAttr('createtime', function ($value, $data) {return time_format($value);})->
            withAttr('lastlogintime', function ($value, $data) {return time_format($value);})->
            select();

            $total = count($_list);
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);

        }

        return $this->fetch();
    }

    /**
     * 最新服务器状态
     */
    public function manageserver()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 15);
            $page = $this->request->param('page/d', 1);

            $_list = $this->gameroom_Model->page($page, $limit)->
            join('game ge','ge.id = mol_gameroom.gameid','left')->
            field('mol_gameroom.*,ge.name')->
            order(array('mol_gameroom.createtime' => 'DESC'))->
            select();

            $total = count($_list);
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);

        }

        return $this->fetch();
    }

    //phpinfo信息 按需显示在前台
    public function get_sys_info()
    {
        //$sys_info['os'] = PHP_OS; //操作系统
        $sys_info['ip'] = GetHostByName($_SERVER['SERVER_NAME']); //服务器IP
        $sys_info['web_server'] = $_SERVER['SERVER_SOFTWARE']; //服务器环境
        $sys_info['phpv'] = phpversion(); //php版本
        $sys_info['fileupload'] = @ini_get('file_uploads') ? ini_get('upload_max_filesize') : 'unknown'; //文件上传限制
        //$sys_info['memory_limit'] = ini_get('memory_limit'); //最大占用内存
        //$sys_info['set_time_limit'] = function_exists("set_time_limit") ? true : false; //最大执行时间
        //$sys_info['zlib'] = function_exists('gzclose') ? 'YES' : 'NO'; //Zlib支持
        //$sys_info['safe_mode'] = (boolean) ini_get('safe_mode') ? 'YES' : 'NO'; //安全模式
        //$sys_info['timezone'] = function_exists("date_default_timezone_get") ? date_default_timezone_get() : "no_timezone";
        //$sys_info['curl'] = function_exists('curl_init') ? 'YES' : 'NO'; //Curl支持
        //$sys_info['max_ex_time'] = @ini_get("max_execution_time") . 's';
        $sys_info['domain'] = $_SERVER['HTTP_HOST']; //域名
        //$sys_info['remaining_space'] = round((disk_free_space(".") / (1024 * 1024)), 2) . 'M'; //剩余空间
        //$sys_info['user_ip'] = $_SERVER['REMOTE_ADDR']; //用户IP地址
        $sys_info['beijing_time'] = gmdate("Y年n月j日 H:i:s", time() + 8 * 3600); //北京时间
        $sys_info['time'] = date("Y年n月j日 H:i:s"); //服务器时间
        //$sys_info['web_directory'] = $_SERVER["DOCUMENT_ROOT"]; //网站目录
        $mysqlinfo = Db::query("SELECT VERSION() as version");
        $sys_info['mysql_version'] = $mysqlinfo[0]['version'];
        if (function_exists("gd_info")) {
            //GD库版本
            $gd = gd_info();
            $sys_info['gdinfo'] = $gd['GD Version'];
        } else {
            $sys_info['gdinfo'] = "未知";
        }
        return $sys_info;
    }

}
