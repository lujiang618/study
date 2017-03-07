<?php
/**
 * 插入算法：将要排序的内容分为两部分，有序区间和无序区间，有序区间开始只有一个值，每次从无序区间拿取一个值，到有序区间排序
 *
 */
ini_set('memory_limit','-1');
$arr = [8,1,3,6,9,5,4,2,0,7];
echo '要排序的数组==>';
print_r($arr);

echo "\r\n";

$len = count($arr);

//生序排列
for( $i = 1; $i < $len ;$i++ ){
    if( $arr[$i] < $arr[$i-1] ){
        $tmp = $arr[$i]; //$tmp 叫哨兵。。。。。

        for( $j = $i-1; $tmp < $arr[$j] && $j >=0 ;$j--){
            $arr[$j+1] = $arr[$j];
            $arr[$j] = $tmp;
        }
    }
}

echo '升序排列==>';
print_r($arr);
echo "\r\n";

//降序排列
for ($i = 1;$i < $len ; $i++){
    if( $arr[$i] > $arr[$i-1] ){
        $tmp = $arr[$i];
        for( $j = $i-1; $tmp > $arr[$j] && $j >= 0; $j--){
            $arr[$j+1] = $arr[$j];
            $arr[$j] = $tmp;
        }
    }
}
echo '降序排列==>';
print_r($arr);
echo "\r\n";
