<?php

function get_first_dir($url){
    $domain = parse_url($url);
    $path = $domain['path'];
    // path = /webpos/index.php/wap/
    $sitename = substr($path,1,strpos($path,'/index.php')-1);
    return $sitename;
}


$url = 'http://192.168.51.151/webpos/index.php/wap/';

echo get_first_dir($url);
