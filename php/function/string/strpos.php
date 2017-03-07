<?php
$str = '1111000000';
if(strpos($str,'10000000')){
echo 'has';
}else{
echo 'none';
}

$str = '购物车(废弃)';
$str1 = '购物车(弃)';

echo "\n\r";
if(strpos($str1,'废弃')){
    echo "111\n";
}
