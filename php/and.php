<?php
/**
 * php变量是放在变量容器zval中的，zval有个引用计数，unset($a) refcount-1.变量的值还是存在的
 * 参考：
 *    引用计数基本知识：http://php.net/manual/zh/features.gc.refcounting-basics.php
 *    变量的使用：http://php.net/manual/zh/internals2.variables.intro.php
 */
$a = 123;
$b = &$a;
unset($a);

echo $b;
