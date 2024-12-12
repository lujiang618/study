<?php
$url = 'ssl://140.207.69.102';
$fp = fsockopen($url,443);

if( $fp ){
    echo 'ok';
}else{
    echo 'fail';
}
