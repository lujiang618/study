<?php
namespace debug;
include 'First.php';

/**
 * Created by PhpStorm.
 * User: lujiang
 * Date: 2017/8/28
 * Time: 15:17
 */
class Second
{
     public static function execTrace(){
         echo 'Class Second';
         echo '<br/>';
         $first = new First();
         $first->printTrace(debug_print_backtrace());
         echo '<br/>';

     }
}