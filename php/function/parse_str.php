<?php
$str = 'a=2&b=3&c=4';
parse_str($str);

echo $a;

echo "\r\n";

parse_str($str,$data);
var_dump($data);
