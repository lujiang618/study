<?php
/**
 * memcache 入门
 * 2017年03月06日10:50:42
 *
 */
$mem = new Memcache;
$host = '127.0.0.1';
$port = '11211';
$mem->connect($host,$port);
$version = $mem->getVersion();
echo 'Memcached Server version:'.$version."\r\n";
