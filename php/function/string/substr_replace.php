<?php
$id_card = '371322198706186915';
echo strlen($id_card);
echo "\r\n";
echo $c=str_pad('',13,"*");
echo "\r\n";
echo $s = substr_replace($id_card,$c,3,13);
echo "\r\n";
echo strlen($s);
echo "\r\n";
echo substr_replace('l618','ujiang',1,0);
