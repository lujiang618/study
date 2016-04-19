<?php
$arr = array('1'=>'waph5','2'=>'微信扫码支付','3'=>'积分显示bug','4'=>'商品批量修改bug','5'=>'更新工作日志');
$temp = '2';

$merge = array_merge($arr,$temp);

var_dump($merge);
