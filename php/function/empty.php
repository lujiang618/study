<?php
/**
 * Created by PhpStorm.
 * User: shinho
 * Date: 2018/1/28
 * Time: 14:18
 */

$str = '0.000';

if (empty($str)) {
    echo 'empty';
} else
{
    echo 'not empty';
}

echo '<br/>';

if (empty((int)$str)) {
    echo 'empty';
} else
{
    echo 'not empty';
}

echo '<br/>';

$arr = [];

$a = $arr['test'];

echo $a;