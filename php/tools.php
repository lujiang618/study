<?php
$data = null;

if( $_POST )
{
    $method = $_POST['type'];
    $content = $_POST['content'];

    switch ($method)
    {
    case 'date':
        $data = $method('Y-m-d H:i:s',$content);
        break;
    case 'json_decode':
        $data = $method($content,true);
        break;
    default:
        $data = $method($content);
        break;
    }
}
?>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<style>
table{width:100%;border-collapse:collapse;border-spacing:0;border-left:1px solid #888;border-top:1px solid #888;background:#efefef;}
th,td{border-right:1px solid #888;border-bottom:1px solid #888;padding:5px 15px;}
th{font-weight:bold;background:#ccc;}
textarea{width:98%;height:300px;}
</style>
</head>
<form action="" method="post">
  <table>
    <col width="40%" />
    <tr>
      <td>
        <select name="type">
          <option value="json_decode">json_decode</option>
          <option value="json_encode">json_encode</option>
          <option value="unserialize">unserialize</option>
          <option value="serialize">serialize</option>
          <option value="urlencode">urlencode</option>
          <option value="urldecode">urldecode</option>
          <option value="date">date</option>
          <option value="strtotime">strtotime</option>
        </select>
        <input type="submit" name="submit" value="提交" />
        <input type="reset" name="reset" value="重置" />
      </td>
      <td rowspan="2">
        <?php echo '<pre/>';var_export($data); ?>
      </td>
    </tr>
    <tr>
      <td>
      <textarea name="content" value='' ></textarea>
      </td>
    </tr>
  </table>
</form>

