<?php
/**
 * 测试array_merge_recursive（）函数的使用
 */

echo '测试没有明确键的arr。';
echo "\r\n";
$arr1 = array(1,2,3);
$arr2 = array(4,5,6);
print_r(array_merge_recursive($arr1,$arr2));

echo '测试有明确key且重复的arr';
echo "\r\n";
$arr3 = array('like'=>'money','work'=>'shopex');
$arr4 = array('work'=>'yunda','like'=>'knowledge');
print_r(array_merge_recursive($arr4,$arr3));

echo '测试下新数组的顺序';
echo "\r\n";
$arr5 = array('1'=>'1','2'=>'2');
$arr6 = array('0'=>'0','4'=>'4');
print_r(array_merge_recursive($arr5,$arr6));

