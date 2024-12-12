<?php
/**
 * Created by PhpStorm.
 * User: shinho
 * Date: 2018/1/16
 * Time: 10:29
 */

namespace code\tools;


class Logs
{
    // 默认的日志文件名称
    public static $defaultLogName = 'app_info';

    // 默认后缀
    public static $suffix = '.log';

    // 文件路径
    public static $filePath = '@logs';

    /**
     * 记录日志
     * @author helei
     *
     * @param string|array $content 写入的日志内容，可为字符串或者数组
     * @param string $name 日志名称  默认为：app_info
     * @param array $backTrace 回溯信息，调用过程的字符串。
     * @param bool   $clean
     *
     * @return bool
     */

    public static function write($content, string $name = '', array $backTrace = [], bool $clean = false)
    {
        // 定义在配置文件，如果变量存在并 false，不做 log
        if (!empty(Yii::$app->params['log_to_file'])
            &&  isset(Yii::$app->params['log_to_file'][$name])
            && Yii::$app->params['log_to_file'][$name] === false) {
            return false;
        }

        $prefix = date('Ym') . '_';
        if (empty($name)) {
            $name = $prefix . static::$defaultLogName;
        } else {
            $name = $prefix . $name;
        }
        $fileName = static::$filePath . '/' . $name . static::$suffix;
        $fileName = Yii::getAlias($fileName);

        if ($clean && is_string($content)) {
            static::writeFile($fileName, "{$content}\n");
            return true;
        }

        // 取得 call stack
        $trace = debug_backtrace();
        $file     = '';
        $class    = '';
        $function = '';
        $line     = '';
        if (count($trace) > 1) {
            $file     = $trace[0]['file'    ];
            $class    = $trace[1]['class'   ];
            $function = $trace[1]['function'];
            $line     = $trace[0]['line'    ];
        }

        if (is_array($content)) {
            $content = var_export($content, true);
        }
        // 用PHP_EOL来做換行，提高可移植性
        $content =
            'DateTime : ' . date('Y-m-d H:i:s') . PHP_EOL .
            "    File : $file" . PHP_EOL .
            "   Class : $class" . PHP_EOL .
            "Function : $function" . PHP_EOL .
            "    Line : $line" . PHP_EOL .
            " Content : $content" . PHP_EOL;

        if($backTrace) {
            $backTrace = self::formatBackTrace($backTrace);
            $content .=
                "   Trace : ↓↓↓↓↓↓↓↓↓".PHP_EOL."$backTrace" . PHP_EOL. PHP_EOL;
        }

        // 写入文件
        static::writeFile($fileName, $content);
        return true;
    }


    /**
     * 格式化跟踪信息
     *
     * @param array $backTrace
     *
     * @return string
     */
    private static function formatBackTrace(array $backTrace) :string
    {
        if (empty($backTrace)) {
            return '';
        }

        $debugTrace = '';
        foreach ($backTrace as $key => $trace) {
            if (!isset($trace['file'])) {
                //使用反射API获取函数声明的文件和行数
                $reflection_function = new \ReflectionClass($trace['class']);
                $trace['file']       = $reflection_function->getFileName();
                $trace['line']       = $reflection_function->getStartLine();
            }

            $debugTrace .= '#'.$key.' '.$trace['file'].'('.$trace['line'].'): ';

            if (isset($trace['class'])) {
                $debugTrace .= $trace['class'].'->';
            }

            $debugTrace .= $trace['function'].'(';

            if (isset($trace['args']) && is_array($trace['args'])) {
                $args       = self::formatTraceArgs($trace['args']);
                $debugTrace .= $args;
            }
            $debugTrace .= ')'.PHP_EOL;
        }

        return $debugTrace;
    }

    /**
     * @param string $fileName
     * @param string $content
     * @param string $method  打开模式结尾用b，用于处理不同系统的换行
     * ```php
     *  'a'	写入方式打开   'a+'	读写方式打开
     *  'w'	写入方式打开   'w+'	读写方式打开
     *  'r'	只读方式打开   'r+'	读写方式打开
     * ```
     * @return bool
     * @author helei
     */
    private static function writeFile($fileName, $content, $method = 'ab')
    {
        $handle = @fopen($fileName, $method);
        if (! $handle) {
            return false;
        }

        @fwrite($handle, $content);
        @flock($handle, LOCK_UN);
        @fclose($handle);
        @chmod( $fileName , 0777);

        return true;
    }

    /**
     * 格式化trace函数的参数信息
     *
     * @param array $args
     *
     * @return string
     */
    private static function formatTraceArgs(array $args) :string
    {
        if (empty($args)) {
            return '';
        }

        $argsStr = '';
        foreach ($args as $arg) {
            if (is_object($arg)) {
                $argsStr .= 'object,';
                continue;
            }

            if (is_array($arg)) {
                $argsStr .= 'array,';
                continue;
            }

            if (is_bool($arg)) {
                $argsStr .= $arg ? 'true,' : 'false,';
                continue;
            }
            $argsStr .= $arg.',';
        }
        $argsStr = substr($argsStr, 0, -1);

        return $argsStr;
    }
}