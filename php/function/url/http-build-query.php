<?php
$arr = array(1,2,3);

$query_string = http_build_query($arr);
echo $query_string;
echo "\r\n";

$query_string1 = http_build_query($arr,'l');
echo $query_string1;
echo "\r\n";
