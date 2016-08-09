<?php
/**
 * mktime()不带参数时等同于time()
 */
$datetime = date('Y-m-d',@mktime()); 
echo $datetime;

echo "\r\n";
echo date('Y-m-d',time());

//测试下非法的日期格式
$y = 2099;
$m = 13;
$d = 30;

echo "\r\n";
echo mktime('0','0','0',$m,$d,$y);
