<?php
$url = 'http://api.map.baidu.com/geocoder?location=39.990912172420714,116.32715863448607&coor';
$result = file_get_contents($url);
$p = xml_parser_create();
xml_parse_into_struct($p,$result,$value);
xml_parser_free($p);
foreach($value as $val){
    switch($val['tag']){
        case 'PROVINCE':
            $province = $val['value'];
            break;
        case 'CITY':
            $city = $val['value'];
            break;
        case 'DISTRICT':
            $district = $val['value'];
            break;
        case 'STREET':
            $street = $val['value'];
            break;
        default:
            break;
    }
}
echo $province.'.'.$city.'.'.$district.'.'.$street;
