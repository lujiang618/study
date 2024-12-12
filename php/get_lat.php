<?php
$getIp='27.115.50.210';
echo 'IP:',$getIp;
echo '<br/>';
$content = file_get_contents("http://api.map.baidu.com/location/ip?ak=7IZ6fgGEGohCrRKUE9Rj4TSQ&ip={$getIp}&coor=bd09ll");
$json = json_decode($content);

echo 'log:',$json->{'content'}->{'point'}->{'x'};//按层级关系提取经度数据
echo '<br/>';
echo 'lat:',$json->{'content'}->{'point'}->{'y'};//按层级关系提取纬度数据
echo '<br/>';
print $json->{'content'}->{'address'};//按层级关系提取address数据
?>
