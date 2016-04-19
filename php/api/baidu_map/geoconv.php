<?php
$url = 'http://api.map.baidu.com/geoconv/v1/?ak=qjM4dD0Qu2Li8ycGBQGflLMOh4YFHhsK&coords=121.1490628619,31.1132288816&from=3&to5&output=json';
$result = json_decode(file_get_contents($url),1);
echo '<pre/>';
var_dump($result);
