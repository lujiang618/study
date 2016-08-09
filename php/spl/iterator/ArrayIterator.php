<?php
/**
 * ArrayIterator
 * 可以遍历对象的属性
 */
class array_iterator{
    public $name='gelu';
    public $sex = 'x';
    public $age = 30;
}

$obj = new ArrayIterator( new array_iterator() );

foreach($obj as $key => $val){
    echo $key.'==>'.$val."\n";
    echo $key.'==>'.$obj[$key]."\n";
}
