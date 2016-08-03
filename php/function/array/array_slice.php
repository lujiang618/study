<?php
$arr = array(0,1,2,3,4);

echo "\n";
print_r($arr);

$slice = array_slice($arr,1,2);

echo "\n";
print_r($slice);

$slice1 = array_slice($arr,-2,1);

echo "\n";
print_r($slice1);

$slice2 = array_slice($arr,-4,-2);

echo "\n";
print_r($slice2);

$slice3 = array_slice($arr,1,-2);

echo "\n";
print_r($slice3);
