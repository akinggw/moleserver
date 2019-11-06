<?php
return [
    [
        //父菜单ID，NULL或者不写系统默认，0为顶级菜单
        "parentid" => 0,
        //地址，[模块/]控制器/方法
        "route" => "member/member/index1",
        //类型，1：权限认证+菜单，0：只作为菜单
        "type" => 0,
        //状态，1是显示，0不显示（需要参数的，建议不显示，例如编辑,删除等操作）
        "status" => 1,
        //名称
        "name" => "玩家",
        //图标
        "icon" => "icon-people",
        //备注
        "remark" => "",
        //排序
        "listorder" => 4,
        //子菜单列表
        "child" => [
            [
                "route" => "member/member/index",
                "type" => 1,
                "status" => 1,
                "name" => "玩家管理",
                "icon" => "icon-people",
                "child" => [
                    [
                        "route" => "member/member/manage",
                        "type" => 1,
                        "status" => 1,
                        "name" => "玩家列表",
                        "icon" => "icon-huiyuan",
                    ],
                ],
            ],
            [
                "route" => "member/robotmanage/index",
                "type" => 1,
                "status" => 1,
                "name" => "机器人管理",
                "icon" => "icon-huiyuan2",
                "child" => [
                    [
                        "route" => "member/robotmanage/manage",
                        "type" => 1,
                        "status" => 1,
                        "name" => "机器人列表",
                        "icon" => "icon-huiyuan",
                    ],
                    [
                        "route" => "member/robottimemanage/manage",
                        "type" => 1,
                        "status" => 1,
                        "name" => "机器人上岗时间",
                        "icon" => "icon-huiyuan",
                    ],
                    [
                        "route" => "member/robotchatmsg/manage",
                        "type" => 1,
                        "status" => 1,
                        "name" => "机器人聊天发言",
                        "icon" => "icon-huiyuan",
                    ],
                    [
                        "route" => "member/robotcontrol/manage",
                        "type" => 1,
                        "status" => 1,
                        "name" => "机器人控制",
                        "icon" => "icon-huiyuan",
                    ],
                ],
            ],
            [
                "route" => "member/agentuser/index",
                "type" => 1,
                "status" => 1,
                "name" => "代理管理",
                "icon" => "icon-people",
                "child" => [
                    [
                        "route" => "member/agentuser/manage",
                        "type" => 1,
                        "status" => 1,
                        "name" => "代理列表",
                        "icon" => "icon-huiyuan",
                    ],
                    [
                        "route" => "member/userchongzhi/manage",
                        "type" => 1,
                        "status" => 1,
                        "name" => "充值记录",
                        "icon" => "icon-huiyuan",
                    ],
                ],
            ],
        ],
    ],
];
