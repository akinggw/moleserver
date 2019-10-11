<?php
return [
    [
        //父菜单ID，NULL或者不写系统默认，0为顶级菜单
        "parentid" => 45,
        //地址，[模块/]控制器/方法
        "route" => "pay/payment/pay_list",
        //类型，1：权限认证+菜单，0：只作为菜单
        "type" => 0,
        //状态，1是显示，0不显示（需要参数的，建议不显示，例如编辑,删除等操作）
        "status" => 1,
        //名称
        "name" => "支付管理",
        //图标
        "icon" => "icon-yue",
        //备注
        "remark" => "",
        //排序
        "listorder" => 3,
        //子菜单列表
        "child" => [
            [
                "route" => "pay/payment/index",
                "type" => 1,
                "status" => 0,
                "name" => "支付模块",
                "child" => [
                    [
                        "route" => "pay/payment/edit",
                        "type" => 1,
                        "status" => 0,
                        "name" => "支付配置",
                    ],
                ],
            ],
        ],
    ],
];
