<?php
/**
 * 测试数组赋值
 * $arr['goods_id']的值是字符串，$arr['goods_id']['name']赋值后，$arr['goods_id']为t！！
 *
 */
$arr = array('goods_id'=>'1','price'=>20.5);
echo '<pre/>'; var_export($arr);

//此处相当于对$arr['goods_id']赋值test，然后根据字符串便移量取值.
$arr['goods_id']['name'] = 'test';
echo '<pre/>'; var_export($arr);
