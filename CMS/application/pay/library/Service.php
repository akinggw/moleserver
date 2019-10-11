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
// | 订单服务类
// +----------------------------------------------------------------------

namespace app\pay\library;

use Exception;
use think\facade\Session;
//use think\facade\Response;
use Yansongda\Pay\Pay;

class Service
{

    public static function submitOrder($amount, $orderid = null, $type = null, $title = null, $notifyurl = null, $returnurl = null, $method = null)
    {
        if (!is_array($amount)) {
            $params = [
                'amount' => $amount,
                'orderid' => $orderid,
                'type' => $type,
                'title' => $title,
                'notifyurl' => $notifyurl,
                'returnurl' => $returnurl,
                'method' => $method,
            ];
        } else {
            $params = $amount;
        }

        $type = isset($params['type']) && in_array($params['type'], ['alipay', 'wechat']) ? $params['type'] : 'wechat';
        $method = isset($params['method']) ? $params['method'] : 'web';
        $orderid = isset($params['orderid']) ? $params['orderid'] : date("YmdHis") . mt_rand(100000, 999999);
        $openid = isset($params['openid']) ? $params['openid'] : '';
        $title = isset($params['title']) ? $params['title'] : "支付";
        $amount = isset($params['amount']) ? $params['amount'] : 1;
        $html = '';
        $notifyurl = isset($params['notifyurl']) ? $params['notifyurl'] : url('');
        $returnurl = isset($params['returnurl']) ? $params['returnurl'] : url('');

        $request = request();
        $config = Service::getConfig($type);
        $config['notify_url'] = $notifyurl;
        $config['return_url'] = $returnurl;
        if ($type == 'alipay') {
            //创建支付对象
            $pay = Pay::alipay($config);
            //支付宝支付,请根据你的需求,仅选择你所需要的即可
            $params = [
                'out_trade_no' => $orderid, //你的订单号
                'total_amount' => $amount, //单位元
                'subject' => $title,
            ];
            //如果是移动端自动切换为wap
            $method = $request->isMobile() ? 'wap' : $method;
            switch ($method) {
                case 'web':
                    //电脑支付,跳转
                    $html = $pay->web($params)->send();
                    break;
                case 'wap':
                    //手机网页支付,跳转
                    $html = $pay->wap($params)->send();
                    break;
                default:
            }
        } else {
            //如果是移动端自动切换为wap
            $method = $request->isMobile() ? 'wap' : $method;
            //创建支付对象
            $pay = Pay::wechat($config);
            $params = [
                'out_trade_no' => $orderid, //你的订单号
                'body' => $title,
                'total_fee' => $amount * 100, //单位分
            ];
            switch ($method) {
                case 'web':
                    //电脑支付,跳转到自定义展示页面
                    $html = $pay->scan($params);
                    Session::set("wechatorderdata", [
                        'out_trade_no' => $orderid, //你的订单号
                        'body' => $title,
                        'total_fee' => $amount * 100, //单位分
                        "code_url" => $html->code_url,
                        //"code_url" => "weixin://wxpay/bizpayurl?pr=4aXtmGv",
                    ]);
                    $url = url('pay/api/wechat');
                    header("location:{$url}");
                    exit;
                    break;
                case 'wap':
                    //手机网页支付,跳转
                    $html = $pay->wap($params)->send();
                    break;
                default:
            }

        }
        //返回字符串
        $html = is_array($html) ? json_encode($html) : $html;
        return $html;

    }

    /**
     * 验证回调是否成功
     * @param string $type   支付类型
     * @param array  $config 配置信息
     * @return bool|Pay
     */
    public static function checkNotify($type, $config = [])
    {
        $type = strtolower($type);
        if (!in_array($type, ['wechat', 'alipay'])) {
            return false;
        }
        try {
            $pay = Pay::$type(self::getConfig($type));
            $data = $pay->verify();
            if ($type == 'alipay') {
                if (in_array($data['trade_status'], ['TRADE_SUCCESS', 'TRADE_FINISHED'])) {
                    return $pay;
                }
            } else {
                return $pay;
            }
        } catch (Exception $e) {
            return false;
        }

        return false;
    }

    /**
     * 验证返回是否成功
     * @param string $type   支付类型
     * @param array  $config 配置信息
     * @return bool|Pay
     */
    public static function checkReturn($type, $config = [])
    {
        $type = strtolower($type);
        if (!in_array($type, ['wechat', 'alipay'])) {
            return false;
        }
        //微信无需验证
        if ($type == 'wechat') {
            return true;
        }
        try {
            $pay = Pay::$type(self::getConfig($type));
            $get = request()->get('', null, 'trim');
            //去除空值
            $get = array_filter($get, create_function('$v', 'return !empty($v);'));
            $data = $type == 'wechat' ? file_get_contents("php://input") : $get;
            $data = $pay->verify($data);
            if ($data) {
                return $pay;
            }
        } catch (Exception $e) {
            return false;
        }
        return false;
    }

    /**
     * 获取配置
     */
    public static function getConfig($type = 'wechat')
    {
        $config = cache('Pay_Config');
        $config = isset($config[$type]['config']) ? $config[$type]['config'] : $config['wechat']['config'];
        if ($config['mode']) {
            $config['mode'] = 'dev';
        } else {
            unset($config['mode']);
        }
        if ($config['log']) {
            $config['log'] = [
                'file' => \think\facade\Env::get('runtime_path') . 'log/epaylogs/' . $type . '-' . date("Y-m-d") . '.log',
                'level' => 'debug',
            ];
        } else {
            unset($config['log']);
        }
        /*if (isset($config['cert_client']) && substr($config['cert_client'], 0, 6) == '/epay/') {
        $config['cert_client'] = ADDON_PATH . $config['cert_client'];
        }
        if (isset($config['cert_key']) && substr($config['cert_key'], 0, 6) == '/epay/') {
        $config['cert_key'] = ADDON_PATH . $config['cert_key'];
        }

        $config['notify_url'] = empty($config['notify_url']) ? addon_url('epay/api/notifyx', [], false) . '/type/' . $type : $config['notify_url'];
        $config['notify_url'] = !preg_match("/^(http:\/\/|https:\/\/)/i", $config['notify_url']) ? request()->root(true) . $config['notify_url'] : $config['notify_url'];
        $config['return_url'] = empty($config['return_url']) ? addon_url('epay/api/returnx', [], false) . '/type/' . $type : $config['return_url'];
        $config['return_url'] = !preg_match("/^(http:\/\/|https:\/\/)/i", $config['return_url']) ? request()->root(true) . $config['return_url'] : $config['return_url'];*/
        return $config;
    }

}
