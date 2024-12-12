<?php
/**
 * Created by PhpStorm.
 * User: shinho
 * Date: 2018/2/3
 * Time: 17:11
 */

$arr1[] = [
    "ship_day"  => "2018-02-05",
    "ship_time" => [
        "18:00-21:00",
    ],
];

$arr1[] = [
    "ship_day"  => "2018-02-06",
    "ship_time" => [
        "18:00-21:00",
    ],
];
var_export($arr1);

echo '<br/>';

var_export(array_unique($arr1));


var_dump(array_diff($arr1,$arr2));


$arr3 = [
    [
        "ship_day"  => "2018-02-04",
        "ship_time" => [
            "全天",
            "18:00-21:00",

        ],
    ],
    [
        "ship_day"  => "2018-02-05",
        "ship_time" => [
            "18:00-21:00",
        ],
    ],
];

var_dump(array_diff($arr2,$arr3));


$arr4 = ['2018-02-03','2018-02-04'];
$arr5 = ['2018-02-03'];

var_dump(array_diff($arr4,$arr5));

var_dump(array_diff($arr5,$arr4));