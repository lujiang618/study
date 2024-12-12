<?php
class IndexAction extends Action {
    public function index(){
        $this->name = 'thinkphp';
        $Data = M('Data'); // 实例化Data数据模型
        $this->data = $Data->select();
        $this->display();
    }
}
