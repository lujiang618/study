<?php
$str = '2016-9-10 08:00-12:00';
echo date('m',$str);
echo "\n";
echo date('d',$str);
echo "\n";
echo date('Y',$str);
echo "\n";

echo date('Y-m-d',$str);
echo "\n";
echo strtotime($str);
echo "\n";
