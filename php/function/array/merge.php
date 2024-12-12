<?php
$arr = array('1'=>'waph5','2'=>'微信扫码支付','3'=>'积分显示bug','4'=>'商品批量修改bug','5'=>'更新工作日志');
$temp = '2';

$merge = array_merge($arr,$temp);

var_dump($merge);

echo '<br/>';

$arr1 = [0,1,2,3];
$arr2 = ['0'=>0,1,2,'5'=>3];

var_dump(array_merge($arr1,$arr2));

echo '<br/>';

var_dump($arr1+$arr2);

echo '测试array_merge()和+的效率';
echo '<br/>';

$execTime = 100000;
$time = time();

for ($i = 0; $i < $execTime; $i++) {
    array_merge($arr1,$arr2);
}

echo '用时：' .(time() - $time);
echo '<br/>';

$time = time();

for ($i = 0; $i < $execTime; $i++) {
    $arr1+$arr2;
}

echo '用时：' .(time() - $time);
echo '<br/>';

/**
测试array_merge()和+的效率
用时：26
用时：1
 */