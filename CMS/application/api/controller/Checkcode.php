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
// | 全局验证码处理类
// +----------------------------------------------------------------------
namespace app\api\controller;

use app\common\controller\Base;
use think\captcha\Captcha;

class Checkcode extends Base
{
    /**
     * 获取验证码
     */
    public function getVerify()
    {
        $captcha = [];
        //设置长度
        $codelen = $this->request->param('length', 4);
        if ($codelen) {
            if ($codelen > 8 || $codelen < 2) {
                $codelen = 4;
            }
            $captcha['length'] = $codelen;
        }
        //设置验证码字体大小
        $fontsize = $this->request->param('font_size', 15);
        if ($fontsize) {
            $captcha['fontSize'] = $fontsize;
        }
        //设置验证码图片宽度
        $width = $this->request->param('imageW', 40);
        if ($width) {
            $captcha['imageW'] = $width;
        }
        //设置验证码图片高度
        $height = $this->request->param('imageH', 110);
        if ($height) {
            $captcha['imageH'] = $height;
        }
        //设置背景颜色
        /*$background = $this->request->param('background');
        if ($background) {
        $checkcode->background = $background;
        }
        //设置字体颜色
        $fontcolor = $this->request->param('font_color');
        if ($fontcolor) {
        $checkcode->fontcolor = $fontcolor;
        }*/
        $captcha = new Captcha($captcha);
        return $captcha->entry();
    }

}
