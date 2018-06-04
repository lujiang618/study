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
    public function printTrace(){
        echo '<br/>';
        echo 'Class First!';
        echo '<br/>';
        $trace = debug_backtrace(DEBUG_BACKTRACE_IGNORE_ARGS);
        var_dump($trace[1]);
    }
}