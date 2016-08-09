<?php
$num = 5;
$location = 'tree';

$format = 'There are %d monkeys in the %s';
echo sprintf($format,$num,$location);

echo "\n";

$format = 'The %s contains %d monkeys';
echo sprintf($format,$num,$location);

echo "\n";
#可以用n$指定参数的顺序
$format = 'The %2$s contains %1$d monkeys';
echo sprintf($format,$num,$location);

echo "\n";
#参数可以使用多次
$format = 'The %2$s contains %1$d monkeys.
           That \'s a nice %2$s full of %1$d monkeys.';
echo sprintf($format,$num,$location);

echo "\n";
#填充字符串'.s
echo sprintf("%'.9d\n",123);
echo sprintf("%'.09d\n",123);

echo "\n";
$format = 'The %2$s contains %1$04d monkeys';
echo sprintf($format,$num,$location);
