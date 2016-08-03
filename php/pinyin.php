<?php
class Pinyin{
    private $_outEncoding = "GB2312";
    function getFirstChar($str=''){
        if( !$str ) return null;
        $fchar=ord($str{0});
        if($fchar>=ord("A") and $fchar<=ord("z") )return strtoupper($str{0});
        $s= $this->safe_encoding($str);
        $asc=ord($s{0})*256+ord($s{1})-65536;
        echo $asc;
        if($asc>=-20319 and $asc<=-20284)return "A";
        if($asc>=-20283 and $asc<=-19776)return "B";
        if($asc>=-19775 and $asc<=-19219)return "C";
        if($asc>=-19218 and $asc<=-18711)return "D";
        if($asc>=-18710 and $asc<=-18527)return "E";
        if($asc>=-18526 and $asc<=-18240)return "F";
        if($asc>=-18239 and $asc<=-17923)return "G";
        if($asc>=-17922 and $asc<=-17418)return "H";
        if($asc>=-17417 and $asc<=-16475)return "J";
        if($asc>=-16474 and $asc<=-16213)return "K";
        if($asc>=-16212 and $asc<=-15641)return "L";
        if($asc>=-15640 and $asc<=-15166)return "M";
        if($asc>=-15165 and $asc<=-14923)return "N";
        if($asc>=-14922 and $asc<=-14915)return "O";
        if($asc>=-14914 and $asc<=-14631)return "P";
        if($asc>=-14630 and $asc<=-14150)return "Q";
        if($asc>=-14149 and $asc<=-14091)return "R";
        if($asc>=-14090 and $asc<=-13319)return "S";
        if($asc>=-13318 and $asc<=-12839)return "T";
        if($asc>=-12838 and $asc<=-12557)return "W";
        if($asc>=-12556 and $asc<=-11848)return "X";
        if($asc>=-11847 and $asc<=-11056)return "Y";
        if($asc>=-11055 and $asc<=-10247)return "Z";
        return null;
    }

    function safe_encoding($string)  {
        $encoding="UTF-8";
        for($i=0;$i<strlen($string);$i++)  {
            if(ord($string{$i})<128) continue;
            if((ord($string{$i})&224)==224) {//第一个字节判断通过   
                $char=$string{++$i};
                if((ord($char)&128)==128)  {//第二个字节判断通过   
                    $char=$string{++$i};
                    if((ord($char)&128)==128) {
                        $encoding="UTF-8";
                        break;
                    }
                }
            }
            if((ord($string{$i})&192)==192)  {//第一个字节判断通过   
                $char=$string{++$i};
                if((ord($char)&128)==128) {//第二个字节判断通过   
                    $encoding="GB2312";
                    break;
                }
            }
        }
        if(strtoupper($encoding)==strtoupper($this->_outEncoding))   
            return $string;
        else   
            return iconv($encoding,$this->_outEncoding,$string);
    }
}

$str = '泸';
$pinyin = new Pinyin();  
echo $pinyin->getFirstChar($str);
