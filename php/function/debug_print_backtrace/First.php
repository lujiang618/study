<?php
namespace debug;
/**
 * Created by PhpStorm.
 * User: lujiang
 * Date: 2017/8/28
 * Time: 15:16
 */
class First
{
    public function printTrace($trace){
        echo '<br/>';
        echo 'Class A';
        echo '<br/>';
        echo 'Trace info:'.$trace;
    }
}