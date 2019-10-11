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
// | phpemail类
// +----------------------------------------------------------------------
namespace util;

use PHPMailer\PHPMailer\Exception;
use PHPMailer\PHPMailer\PHPMailer;

class Email
{
    /**
     * 单例对象
     */
    protected static $instance;
    /**
     * phpmailer对象
     */
    protected $mail = [];

    /**
     * 错误内容
     */
    protected $_error = '';

    /**
     * 默认配置
     */
    public $options = [
        'debug' => 0, //关闭调式模式
    ];

    /**
     * 初始化
     * @access public
     * @param array $options 参数
     * @return Email
     */
    public static function instance($options = [])
    {
        if (is_null(self::$instance)) {
            self::$instance = new static($options);
        }

        return self::$instance;
    }

    /**
     * 构造函数
     * @param array $options
     */
    public function __construct($options = [])
    {
        if ($config = \think\facade\Config::get('app.')) {
            $this->options = array_merge($this->options, $config);
        }

        $this->options = array_merge($this->options, $options);
        $securArr = [1 => 'tls', 2 => 'ssl'];
        $this->mail = new PHPMailer(true);
        $this->mail->SMTPDebug = $this->options['debug'];
        $this->mail->isSMTP();
        $this->mail->Host = $this->options['mail_smtp_host']; //SMTP服务器
        // 设置为“需要验证”
        if ($this->options['mail_auth']['key']) {
            $this->mail->SMTPAuth = true;
        } else {
            $this->mail->SMTPAuth = false;
        }
        $this->mail->Username = $this->options['mail_smtp_user']; //SMTP username
        $this->mail->Password = $this->options['mail_smtp_pass']; // SMTP password
        $this->mail->SMTPSecure = isset($securArr[$this->options['mail_verify_type']['key']]) ? $securArr[$this->options['mail_verify_type']['key']] : ''; //支持TLS加密,还接受了ssl
        $this->mail->Port = $this->options['mail_smtp_port']; //TCP端口连接

        //设置发件人
        $this->from($this->options['mail_from']);
    }

    /**
     * 设置邮件主题
     * @param string $subject
     * @return $this
     */
    public function subject($subject)
    {
        $this->options['subject'] = $subject;
        return $this;
    }

    /**
     * 设置发件人
     * @param string $email
     * @param string $name
     * @return $this
     */
    public function from($email, $name = '')
    {
        $this->options['from'] = $email;
        $this->options['from_name'] = $name;
        return $this;
    }

    /**
     * 设置邮件正文
     * @param string $body
     * @param boolean $ishtml
     * @return $this
     */
    public function message($body, $ishtml = true)
    {
        $this->options['body'] = $body;
        $this->options['ishtml'] = $ishtml;
        return $this;
    }

    /**
     * 设置收件人
     * @param string $email
     * @param string $name
     * @return $this
     */
    public function to($email, $name = '')
    {
        $this->options['to'] = $email;
        $this->options['to_name'] = $name;
        return $this;
    }

    //发送邮件
    public function send()
    {
        $result = false;
        switch ($this->options['mail_type']['key']) {
            case 1:
                try
                {
                    //使用phpmailer发送
                    $this->mail->setFrom($this->options['from'], $this->options['from_name']);
                    $this->mail->addAddress($this->options['to'], $this->options['to_name']);
                    $this->mail->Subject = $this->options['subject'];
                    if ($this->options['ishtml']) {
                        $this->mail->isHTML(true);
                        $this->mail->Body = $this->options['body'];
                    } else {
                        $this->mail->AltBody = $this->options['body'];
                    }

                    $result = $this->mail->send();
                } catch (Exception $e) {
                    $this->setError($e->getMessage());
                }
                $this->setError($result ? '' : $this->mail->ErrorInfo);
                break;
            case 2:
                //使用mail方法发送邮件
                $headers = 'MIME-Version: 1.0' . "\r\n";
                $headers .= "Content-type: text/html; charset=" . $this->options['charset'] . "\r\n";
                $headers .= "To: {$this->options['to_name']} <{$this->options['to']}>\r\n"; //收件人
                $headers .= "From: {$this->options['from_name']} <{$this->options['from']}>\r\n"; //发件人
                $result = mail($this->options['to'], $this->options['subject'], $this->options['body'], $headers);
                $this->setError($result ? '' : error_get_last()['message']);
                break;
            default:
                $this->setError('邮件功能已关闭');
                break;
        }
        return $result;
    }

    /**
     * 获取最后产生的错误
     * @return string
     */
    public function getError()
    {
        return $this->_error;
    }

    /**
     * 设置错误
     * @param string $error 信息信息
     */
    protected function setError($error)
    {
        $this->_error = $error;
    }

}
