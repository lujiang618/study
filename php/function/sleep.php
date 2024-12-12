<?php
/**
 * 测试sleep函数
 * sleep的参数为小数时无效，如果参数是小数时要用usleep
 */
echo 'start...';
echo "\r\n";
$rs = sleep(1);
echo 'sleep 1s ,rs = '.$rs;
echo "\r\n";
$rs = sleep(0.5);
echo 'sleep 0.5s, rs = '.$rs;
echo "\r\n";
$rs = usleep(500000);
echo 'usleep 0.5s, rs = '.$rs;
echo "\r\n";
echo 'end';
echo "\r\n";
