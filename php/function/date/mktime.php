<?php
/**
 * mktime()不带参数时等同于time()
 */
$datetime = date('Y-m-d',@mktime()); 
echo $datetime;

echo "\r\n";
echo date('Y-m-d',time());
