<?php
//测试pattern //i
$str = ' http://192.168.51.151/gelu/index.php/waph5/product-1.html';

$status =preg_match('/authorize/i',$str);

if(!$status)
{
echo '111';
}

/**
 * 测试下正则的长度限制
 *
 *
 */

$str = '11111';

if( !preg_match('/^([0-9]){6}$/i',$str) ){
    echo 'error';
}else{
    echo 'right';
}
