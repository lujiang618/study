<?php
/**
 * 冒泡排序:通过相邻元素之间的比较和交换，使关键字较小的元素逐渐从底部移向顶部。
 *
 */

$arr = [ 5,3,1,4,6,2,8,7,9,0];
$len = count($arr);

echo '升序排列===>';
echo "\r\n";

function bubble($arr,$len){
    $tmp = 0;

    for( $i = 1; $i<$len ; $i++){ // 只要进行len-1次排序，因为是用最后的元素和前一元素比较
        for( $j = $len - 1; $j >= $i; $j--){
            if( $arr[$j] < $arr[$j-1] ){
                $tmp = $arr[$j];
                $arr[$j] = $arr[$j-1];
                $arr[$j-1] = $tmp;
            }
        }
    }
    return $arr;
}

$arr = bubble($arr,$len);
print_r($arr);
echo "\r\n";

function rbubble($arr,$n){
    $tmp = 0;

    for( $i = 1; $i<$n; $i++){
        for( $j = $n-1; $j >= $i; $j--){
            if( $arr[$j] > $arr[$j-1] ){
                $tmp = $arr[$j];
                $arr[$j] = $arr[$j-1];
                $arr[$j-1] = $tmp;
            }
        }
    }
    return $arr;
}

echo '降序排列==>';
$arr = rbubble($arr,$len);
print_r($arr);
echo "\r\n";

echo '测试排序的躺数：';
$arr =  [36,28,45,13,67,36,18,56];
$len = count($arr);
/**
for($i = 1; $i<$len; $i++){
    $flag = 0;

    for( $j = $len - 1; $j >=$i; $j--){
        if( $arr[$j] > $arr[$j-1] ) {
            $tmp = $arr[$j];
            $arr[$j] = $arr[$j-1];
            $arr[$j-1] = $tmp;
            $flag = 1;
        }
    }
    echo $i;
    print_r($arr);
    echo "\r\n";
    if( !$flag ) return;
}
 */
echo '双向冒泡===>';

function t_bubble($arr,$n){
    $i = 1;
    $j = 0;
    $noswap = 1;
    $tmp = 0;

    while($noswap){
        $noswap = 0;
        //自底向上比较
        for( $j = $n-$i ; $j >= $i; $j--){
            if( $arr[$j] < $arr[$j-1] ){
                $tmp = $arr[$j];
                $arr[$j] = $arr[$j-1];
                $arr[$j-1] = $tmp;
                $noswap = 1;
            }
        }

        //自上向下比较
        for( $j = $i ; $j < $n-$i;$j++){
            if( $arr[$j] > $arr[$j+1] ){
                $tmp = $arr[$j];
                $arr[$j] = $arr[$j+1];
                $arr[$j+1] = $tmp;
                $noswap = 1;
            }
        }

        $i++;
    }
    return $arr;
}

$arr = t_bubble($arr,$len);
print_r($arr);
echo "\r\n";

