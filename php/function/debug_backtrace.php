<?php
function one(){
    $date = debug_backtrace();
    echo '<pre/>';var_dump($data);
}

function two(){
    one();
}

function three(){
    two();
}

three();
