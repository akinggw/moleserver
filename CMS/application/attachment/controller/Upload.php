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
// | 附件上传处理类
// +----------------------------------------------------------------------
namespace app\attachment\controller;

use app\admin\service\User as admin_user;
use app\attachment\model\Attachment as Attachment_Model;
use app\common\controller\Base;
use app\member\service\User as home_user;
use think\facade\Hook;

class Upload extends Base
{
    //上传用户
    public $upname = null;
    //上传用户ID
    public $admin_id = 0;
    public $user_id = 0;
    //会员组
    public $groupid = 0;
    //是否后台
    public $isadmin = 0;
    //上传模块
    public $module = 'cms';
    private $uploadUrl = '';
    private $uploadPath = '';
    //编辑器初始配置
    private $confing = array(
        /* 上传图片配置项 */
        "imageActionName" => "uploadimage", /* 执行上传图片的action名称 */
        "imageFieldName" => "upfile", /* 提交的图片表单名称 */
        "imageMaxSize" => 2048000, /* 上传大小限制，单位B */
        "imageAllowFiles" => [".png", ".jpg", ".jpeg", ".gif", ".bmp"], /* 上传图片格式显示 */
        "imageCompressEnable" => true, /* 是否压缩图片,默认是true */
        "imageCompressBorder" => 1600, /* 图片压缩最长边限制 */
        "imageInsertAlign" => "none", /* 插入的图片浮动方式 */
        "imageUrlPrefix" => "", /* 图片访问路径前缀 */
        'imagePathFormat' => '',
        /* 涂鸦图片上传配置项 */
        "scrawlActionName" => "uploadscrawl", /* 执行上传涂鸦的action名称 */
        "scrawlFieldName" => "upfile", /* 提交的图片表单名称 */
        'scrawlPathFormat' => '',
        "scrawlMaxSize" => 2048000, /* 上传大小限制，单位B */
        'scrawlUrlPrefix' => '',
        'scrawlInsertAlign' => 'none',
        /* 截图工具上传 */
        "snapscreenActionName" => "uploadimage", /* 执行上传截图的action名称 */
        'snapscreenPathFormat' => '',
        'snapscreenUrlPrefix' => '',
        'snapscreenInsertAlign' => 'none',
        /* 抓取远程图片配置 */
        'catcherLocalDomain' => array('127.0.0.1', 'localhost', 'img.baidu.com'),
        "catcherActionName" => "catchimage", /* 执行抓取远程图片的action名称 */
        'catcherFieldName' => 'source',
        'catcherPathFormat' => '',
        'catcherUrlPrefix' => '',
        'catcherMaxSize' => 0,
        'catcherAllowFiles' => array('.png', '.jpg', '.jpeg', '.gif', '.bmp'),
        /* 上传视频配置 */
        "videoActionName" => "uploadvideo", /* 执行上传视频的action名称 */
        "videoFieldName" => "upfile", /* 提交的视频表单名称 */
        'videoPathFormat' => '',
        'videoUrlPrefix' => '',
        'videoMaxSize' => 0,
        'videoAllowFiles' => array(".flv", ".swf", ".mkv", ".avi", ".rm", ".rmvb", ".mpeg", ".mpg", ".ogg", ".ogv", ".mov", ".wmv", ".mp4", ".webm", ".mp3", ".wav", ".mid"),
        /* 上传文件配置 */
        "fileActionName" => "uploadfile", /* controller里,执行上传视频的action名称 */
        'fileFieldName' => 'upfile',
        'filePathFormat' => '',
        'fileUrlPrefix' => '',
        'fileMaxSize' => 0,
        'fileAllowFiles' => array(".flv", ".swf"),
        /* 列出指定目录下的图片 */
        "imageManagerActionName" => "listimage", /* 执行图片管理的action名称 */
        'imageManagerListPath' => '',
        'imageManagerListSize' => 20,
        'imageManagerUrlPrefix' => '',
        'imageManagerInsertAlign' => 'none',
        'imageManagerAllowFiles' => array('.png', '.jpg', '.jpeg', '.gif', '.bmp'),
        /* 列出指定目录下的文件 */
        "fileManagerActionName" => "listfile", /* 执行文件管理的action名称 */
        'fileManagerListPath' => '',
        'fileManagerUrlPrefix' => '',
        'fileManagerListSize' => '',
        'fileManagerAllowFiles' => array(".flv", ".swf"),
    );

    protected function initialize()
    {
        parent::initialize();
        //检查是否后台登录，后台登录下优先级最高，用于权限判断
        if (admin_user::instance()->isLogin()) {
            $this->isadmin = 1;
            $this->upname = admin_user::instance()->username;
            $this->admin_id = admin_user::instance()->id;
        } elseif (home_user::instance()->isLogin()) {
            $this->upname = home_user::instance()->username;
            $this->user_id = home_user::instance()->id;
            $this->groupid = home_user::instance()->groupid ? home_user::instance()->groupid : 8;
        } else {
            return $this->error('未登录');
        }
        $this->uploadUrl = config('public_url') . 'uploads/';
        $this->uploadPath = config('upload_path');

        //图片上传大小和类型
        $this->confing['imageMaxSize'] = $this->confing['catcherMaxSize'] = config('upload_image_size') * 1024;
        if (!empty(config('upload_image_ext'))) {
            $imageallowext = parse_attr(config('upload_image_ext'));
            foreach ($imageallowext as $k => $rs) {
                $imageallowext[$k] = ".{$rs}";
            }
            $this->confing['imageAllowFiles'] = $imageallowext;
        }

        //附件上传大小和类型
        $this->confing['fileMaxSize'] = $this->confing['videoMaxSize'] = config('upload_file_size') * 1024;
        if (!empty(config('upload_file_ext'))) {
            $fileallowext = parse_attr(config('upload_file_ext'));
            foreach ($fileallowext as $k => $rs) {
                $fileallowext[$k] = ".{$rs}";
            }
            $this->confing['fileAllowFiles'] = $fileallowext;
        }

    }

    public function upload($dir = '', $from = '', $module = '', $thumb = 0, $thumbsize = '', $thumbtype = '', $watermark = 1, $sizelimit = -1, $extlimit = '')
    {
        //验证是否可以上传
        $status = $this->isUpload($module);
        if (true !== $status) {
            return json([
                'code' => -1,
                'info' => $status,
            ]);
        }
        if ($dir == '') {
            return $this->error('没有指定上传目录');
        }
        if ($from == 'ueditor') {
            return $this->ueditor();
        }
        return $this->saveFile($dir, $from, $module, $thumb, $thumbsize, $thumbtype, $watermark, $sizelimit, $extlimit);
    }

    /**
     * 检查是否可以上传
     */
    protected function isUpload($module)
    {
        $module_list = cache('Module');
        if ($module_list[strtolower($module)] || strtolower($module) == 'admin') {
            $this->module = strtolower($module);
        } else {
            return false;
        }
        //如果是前台上传，判断用户组权限
        if ($this->isadmin == 0) {
            $member_group = cache('Member_Group');
            if ((int) $member_group[$this->groupid]['allowattachment'] < 1) {
                return "所在的用户组没有附件上传权限！";
            }
        }
        return true;
    }

    /**
     * 保存附件
     * @param string $dir 附件存放的目录
     * @param string $from 来源
     * @param string $module 来自哪个模块
     * @return string|\think\response\Json
     */
    protected function saveFile($dir = '', $from = '', $module = '', $thumb = 0, $thumbsize = '', $thumbtype = '', $watermark = 1, $sizelimit = -1, $extlimit = '')
    {
        if (!function_exists("finfo_open")) {
            switch ($from) {
                case 'ueditor':
                    return json(['state' => '检测到环境未开启php_fileinfo拓展']);
                default:
                    return json([
                        'code' => -1,
                        'info' => '检测到环境未开启php_fileinfo拓展',
                    ]);
            }
        }
        // 附件大小限制
        $size_limit = $dir == 'images' ? config('upload_image_size') : config('upload_file_size');
        if (-1 != $sizelimit) {
            $sizelimit = intval($sizelimit);
            if ($sizelimit >= 0 && (0 == $size_limit || ($size_limit > 0 && $sizelimit > 0 && $size_limit > $sizelimit))) {
                $size_limit = $sizelimit;
            }
        }
        $size_limit = $size_limit * 1024;
        // 附件类型限制
        $ext_limit = $dir == 'images' ? config('upload_image_ext') : config('upload_file_ext');
        $ext_limit = $ext_limit != '' ? parse_attr($ext_limit) : '';

        // 水印参数
        $watermark = $this->request->post('watermark', '');
        // 获取附件数据
        switch ($from) {
            case 'ueditor':
                $file_input_name = 'upfile';
                break;
            default:
                $file_input_name = 'file';
        }
        $file = $this->request->file($file_input_name);
        if ($file == null) {
            switch ($from) {
                case 'ueditor':
                    return json(['state' => '获取不到文件信息']);
                default:
                    return json([
                        'code' => -1,
                        'info' => '获取不到文件信息',
                    ]);
            }
        }

        // 判断附件是否已存在
        if ($file_exists = Attachment_Model::get(['md5' => $file->hash('md5')])) {
            if ($file_exists['driver'] == 'local') {
                $file_path = $this->uploadUrl . $file_exists['path'];
            } else {
                $file_path = $file_exists['path'];
            }
            switch ($from) {
                case 'ueditor':
                    return json([
                        "state" => "SUCCESS", // 上传状态，上传成功时必须返回"SUCCESS"
                        "url" => $file_path, // 返回的地址
                        "title" => $file_exists['name'], // 附件名
                    ]);
                    break;
                default:
                    return json([
                        'code' => 0,
                        'info' => $file_exists['name'] . '上传成功',
                        'class' => 'success',
                        'id' => $file_exists['id'],
                        'path' => $file_path,
                    ]);
            }
        }

        // 判断附件大小是否超过限制
        if ($size_limit > 0 && ($file->getInfo('size') > $size_limit)) {
            switch ($from) {
                case 'ueditor':
                    return json(['state' => '附件过大']);
                    break;
                default:
                    return json([
                        'status' => 0,
                        'info' => '附件过大',
                    ]);
            }
        }
        // 判断附件格式是否符合
        $file_name = $file->getInfo('name');
        $file_ext = strtolower(substr($file_name, strrpos($file_name, '.') + 1));
        $error_msg = '';
        if ($ext_limit == '') {
            $error_msg = '获取文件后缀限制信息失败！';
        }
        try {
            $fileMine = $file->getMime();
        } catch (\Exception $ex) {
            $error_msg = $ex->getMessage();
        }
        if ($fileMine == 'text/x-php' || $fileMine == 'text/html') {
            $error_msg = '禁止上传非法文件！';
        }
        if (preg_grep("/php/i", $ext_limit)) {
            $error_msg = '禁止上传非法文件！';
        }
        if (!preg_grep("/$file_ext/i", $ext_limit)) {
            $error_msg = '附件类型不正确！';
        }

        if (!in_array($file_ext, $ext_limit)) {
            $error_msg = '附件类型不正确！';
        }
        if ($error_msg != '') {
            switch ($from) {
                case 'ueditor':
                    return json(['state' => $error_msg]);
                    break;
                default:
                    return json([
                        'code' => -1,
                        'info' => $error_msg,
                    ]);
            }
        }

        // 附件上传钩子，用于第三方文件上传扩展
        if (config('upload_driver')['key'] != 'local') {
            $hook_result = Hook::listen('uploadAfter', ['file' => $file, 'from' => $from, 'module' => $module], true);
            if (false !== $hook_result) {
                return $hook_result;
            }
        }

        // 移动到框架应用根目录指定目录下
        $info = $file->move($this->uploadPath . DIRECTORY_SEPARATOR . $dir);
        if ($info) {
            // 水印功能
            if ($watermark == '') {
                if ($dir == 'images' && config('upload_thumb_water') == 1 && config('upload_thumb_water_pic') != "") {
                    model('Attachment')->create_water($info->getRealPath(), config('upload_thumb_water_pic'));
                }
            }

            // 获取附件信息
            $file_info = [
                'aid' => $this->admin_id,
                'uid' => $this->user_id,
                'name' => $file->getInfo('name'),
                'mime' => $file->getInfo('type'),
                'path' => $dir . '/' . str_replace('\\', '/', $info->getSaveName()),
                'ext' => $info->getExtension(),
                'size' => $info->getSize(),
                'md5' => $info->hash('md5'),
                'sha1' => $info->hash('sha1'),
                'module' => $module,
            ];
            if ($file_add = Attachment_Model::create($file_info)) {
                switch ($from) {
                    case 'ueditor':
                        return json([
                            "state" => "SUCCESS", // 上传状态，上传成功时必须返回"SUCCESS"
                            "url" => $this->uploadUrl . $file_info['path'], // 返回的地址
                            "title" => $file_info['name'], // 附件名
                        ]);
                        break;
                    default:
                        return json([
                            'code' => 0,
                            'info' => $file_info['name'] . '上传成功',
                            'id' => $file_add['id'],
                            'path' => empty($file_info['thumb']) ? $this->uploadUrl . $file_info['path'] : $this->uploadUrl . $file_info['thumb'],
                        ]);
                }
            } else {
                switch ($from) {
                    case 'ueditor':
                        return json(['state' => '上传失败']);
                        break;
                    default:
                        return json(['code' => 0, 'info' => '上传成功,写入数据库失败']);
                }
            }
        } else {
            switch ($from) {
                case 'ueditor':
                    return json(['state' => '上传失败']);
                    break;
                default:
                    return json(['code' => -1, 'info' => $file->getError()]);
            }

        }
    }

    private function ueditor()
    {
        $action = $this->request->get('action');
        switch ($action) {
            /* 获取配置信息 */
            case 'config':
                $result = $this->confing;
                break;
            /* 上传图片 */
            case 'uploadimage':
                return $this->saveFile('images', 'ueditor');
                break;
            /* 上传涂鸦 */
            case 'uploadscrawl':
                return $this->saveFile('images', 'ueditor_scrawl');
                break;
            /* 上传视频 */
            case 'uploadvideo':
                return $this->saveFile('videos', 'ueditor');
                break;
            /* 上传附件 */
            case 'uploadfile':
                return $this->saveFile('files', 'ueditor');
                break;
            /* 列出图片 */
            case 'listimage':
                return $this->showFileList('listimage');
                break;

            /* 列出附件 */
            case 'listfile':
                return $this->showFileList('listfile');
                break;
            default:
                $result = array(
                    'state' => '请求地址出错',
                );
                break;
        }
        /* 输出结果 */
        if (isset($_GET["callback"])) {
            if (preg_match("/^[\w_]+$/", $_GET["callback"])) {
                return htmlspecialchars($_GET["callback"]) . '(' . $result . ')';
            } else {
                return json(['state' => 'callback参数不合法']);
            }
        } else {
            return json($result);
        }
    }

    /**
     * @param string $type 类型
     * @param $config
     * @return \think\response\Json
     */
    protected function showFileList($type = '')
    {
        /* 获取参数 */
        $size = input('get.size/d', 0);
        $start = input('get.start/d', 0);
        $allowExit = input('get.exit', '');
        if ($size == 0) {
            $size = 20;
        }
        /* 判断类型 */
        switch ($type) {
            /* 列出附件 */
            case 'listfile':
                $allowExit = '' == $allowExit ? config('upload_file_ext') : $allowExit;
                break;
            /* 列出图片 */
            case 'listimage':
            default:
                $allowExit = '' == $allowExit ? config('upload_image_ext') : $allowExit;
        }

        /* 获取附件列表 */
        $filelist = Attachment_Model::order('id desc')->where('ext', 'in', $allowExit)->where('status', 1)->limit($start, $size)->column('id,path,create_time,name,size');
        if (empty($filelist)) {
            return json(array(
                "state" => "没有找到附件",
                "list" => [],
                "start" => $start,
                "total" => 0
            ));
        }
        $uploadUrl = config('public_url');
        $list = [];
        $i = 0;
        foreach ($filelist as $value) {
            $list[$i]['id'] = $value['id'];
            $list[$i]['url'] = $uploadUrl . 'uploads/' . $value['path'];
            $list[$i]['name'] = $value['name'];
            $list[$i]['size'] = format_bytes($value['size']);
            $list[$i]['mtime'] = $value['create_time'];
            $i++;
        }

        /* 返回数据 */
        $result = array(
            "state" => "SUCCESS",
            "list" => $list,
            "start" => $start,
            "total" => Attachment_Model::where('ext', 'in', $allowExit)->count(),
        );
        return json($result);

    }

}
