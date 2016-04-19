<?php if (!defined('THINK_PATH')) exit();?><html>
  <head>
    <title>hello, 2016-02-25, 从今天，就开始学习<?php echo ($name); ?>了</title>
  </head>
  <body>
    hello, <?php echo ($name); ?>! <br/>
    <?php if(is_array($data)): $i = 0; $__LIST__ = $data;if( count($__LIST__)==0 ) : echo "" ;else: foreach($__LIST__ as $key=>$vo): $mod = ($i % 2 );++$i; echo ($vo["id"]); ?>--<?php echo ($vo["data"]); ?><br/><?php endforeach; endif; else: echo "" ;endif; ?>
  </body>
</html>