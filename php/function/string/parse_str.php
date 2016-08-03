<?php
$str = 'a=1+b=2+c=3';
parse_str($str,$data);
print_r($data);
