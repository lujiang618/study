<?php
$arr = array('openid'=>1);
$str = 'openidddd';

echo 'start';
if( !empty($str['openid']) ){
    echo 'str true';
}
echo "\n";

if( isset($str['openid']) ){
    echo 'isset true';
}
