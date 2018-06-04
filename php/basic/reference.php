<?php
/**
 * Created by PhpStorm.
 * User: lujiang
 * Date: 2017/8/25
 * Time: 13:41
 */
// eg1:
error_reporting(E_ALL);
ini_set('display_errors',1);
//var_dump(&$a);

//eg2:
$arr = range(1,3);
foreach ($arr as &$val) {

}

foreach($arr as $val) {

}

/************************************************
 *           输出1,2,2                          *
 ************************************************/
var_dump($arr);

//eg3:
$arr = array(1,3);

function test($item,$key,&$arr) {
    unset($arr[$key]);
}

//var_dump(array_walk($arr,'test',&$arr));

//eg4:
$arr = array(&$arr);

var_dump($arr === $arr);

$arr = range(1,3);
foreach($arr as &$v){

}
foreach($arr as $v){

}
var_dump($arr);//[1,2,2]

// 解决一
$arr = range(1,3);
foreach($arr as &$v){

}
unset($v);
foreach($arr as $v){

}
var_dump($arr);//[1,2,3]
// 解决二
$arr = range(1,3);
foreach($arr as &$v){

}
foreach($arr as $v2){

}
var_dump($arr);//[1,2,3]
// 解决三
$arr = range(1,3);
foreach($arr as &$v){

}
foreach($arr as &$v){

}
var_dump($arr);//[1,2,3]