<?php
/**
 * 测试php的$_SERVER全局变量
 * PATH_INFO
 */

/*
 * $url是浏览器地址栏不包括域名的字符串
 * 输出结果： $_SERVER["PATH_INFO"] :/test/php/basic/server.php
 */
$url = $_SERVER['PHP_SELF'];
echo '$_SERVER["PATH_INFO"] :'.$url;

