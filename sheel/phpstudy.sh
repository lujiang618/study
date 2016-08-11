#! /bin/bash
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH

## =========================================================================
##            phpStudy For linux 2014
## =========================================================================
## onekey install Apache/Nginx/Lighttpd + PHP5.2/5.3/5.4/5.5 on Linux 
## 
## For more information please visit http://www.phpstudy.net/
## =========================================================================

# Check if user is root
if [ $(id -u) != "0" ]; then
    echo "Error: You must be root to run this script, please use root to uninstall phpstudy"
    exit 1
fi

if [ -s /phpstudy/server/do_not_delete ];then
serv=$(cat /phpstudy/server/do_not_delete)
else
echo error
exit
fi

if [ -s /phpstudy/server/php/do_not_delete ];then
php=$(cat /phpstudy/server/php/do_not_delete)
fi

SCRIPTNAME=/phpstudy/phpstudy.sh

function fw(){

re=`ps -ef|grep ${1}|grep -v "grep " |wc -l`

if [ $re -gt 0 ]
then
echo ${1}  [start]            ${1}  [已启动]
else 
echo ${1}  [stop]            ${1}  [已停止]
fi
}

do_start() {
/phpstudy/mysql/support-files/mysql.server start

if [ "$serv" = "Nginx" ];then
	if [ "$php" = "2" ];then
	/phpstudy/server/php/sbin/php-fpm start
	 else
	/phpstudy/server/php/sbin/php-fpm
	fi
	/phpstudy/server/nginx/sbin/nginx
elif [ "$serv" = "Apache" ];then
	/phpstudy/server/httpd/bin/apachectl start
elif [ "$serv" = "Lighttpd" ];then
	if [ "$php" = "2" ];then
	/phpstudy/server/php/sbin/php-fpm start
	 else
	/phpstudy/server/php/sbin/php-fpm
	fi
	/phpstudy/server/lighttpd/sbin/lighttpd -f /phpstudy/server/lighttpd/conf/lighttpd.conf

fi
sleep 1s

fw mysqld
if [ "$serv" = "Nginx" ];then
fw php-fpm
fw nginx
elif [ "$serv" = "Apache" ];then
fw httpd
elif [ "$serv" = "Lighttpd" ];then
fw php-fpm
fw lighttpd
fi
}



do_stop() {
killall mysqld

if [ "$serv" = "Nginx" ];then
killall nginx
killall php-fpm

elif [ "$serv" = "Apache" ];then
killall httpd
elif [ "$serv" = "Lighttpd" ];then
killall lighttpd
killall php-fpm
fi

sleep 1s

fw mysqld
if [ "$serv" = "Nginx" ];then
fw nginx
elif [ "$serv" = "Apache" ];then
fw httpd
elif [ "$serv" = "Lighttpd" ];then
fw lighttpd
fi
}

function uninstall_phpstudy(){

read -p "卸载 phpStudy ,请输入 y 确认：
uninstall phpstudy, please input y: " yes

if [ "$yes" = "y" ] ;then
mkdir /phpstudy/backup

killall httpd 
killall nginx
killall lighttpd
killall php-fpm 
killall mysqld
sleep 1s
mv -f /phpstudy/data /phpstudy/backup/data-$(date -d now +%Y%m%d%H%M%S)
chkconfig --del phpstudy 
update-rc.d -f phpstudy remove

rm -Rf /phpstudy/server/
rm -Rf /phpstudy/mysql/
rm -Rf /phpstudy/log/
rm -Rf /etc/my.cnf
rm -Rf  /etc/init.d/phpstudy 
rm -Rf  /phpstudy/phpstudy.sh
rm -Rf  /usr/bin/phpstudy

echo 
echo "phpStudy 卸载完成。";
echo "phpstudy uninstall completed";
echo 
fi
}


fw_restart() {


if [ "$serv" = "Nginx" ];then
	killall nginx
	/phpstudy/server/nginx/sbin/nginx
	fw nginx

elif [ "$serv" = "Apache" ];then
	killall httpd
	sleep 1s
	/phpstudy/server/httpd/bin/apachectl start
	fw httpd
elif [ "$serv" = "Lighttpd" ];then
	killall lighttpd
	/phpstudy/server/lighttpd/sbin/lighttpd -f /phpstudy/server/lighttpd/conf/lighttpd.conf
	fw lighttpd

fi


}


function vhost_add(){
    #Define domain name
    read -p "(Please input domains such as:www.phpstudy.net):" domains
    if [ "$domains" = "" ]; then
        echo "You need input a domain."
        exit 1
    fi
    domain=`echo $domains | awk '{print $1}'`
   # if [ -f "/phpstudy/server/httpd/conf/vhosts/$domain.conf" ]; then
    #    echo "$domain is exist!"
     #   exit
    #fi
    #Define website dir
    webdir="/phpstudy/www/$domain"
    DocumentRoot="$webdir"
    logsdir="/phpstudy/log/$domain"
   # mkdir -p $DocumentRoot $logsdir
   mkdir -p $DocumentRoot
    chown -R www:www $webdir


echo 
if [ "$serv" = "Apache" ];then
cat >/phpstudy/server/httpd/conf/vhosts/$domain.conf<<EOF
<virtualhost *:80>
ServerName  $domain
ServerAlias  $domains 
DocumentRoot  $DocumentRoot
DirectoryIndex index.php index.html
<Directory $DocumentRoot>
Options +Includes +FollowSymLinks -Indexes
AllowOverride All
Order Deny,Allow
Allow from All
</Directory>
</virtualhost>
EOF
fi

if [ "$serv" = "Nginx" ];then
cat >/phpstudy/server/nginx/conf/vhosts/$domain.conf<<EOF
server {
        listen       80;
        server_name  $domain;
        root   "$DocumentRoot";
        location / {
            index  index.html index.htm index.php;
            #autoindex  on;
        }
        location ~ \.php(.*)$ {
            fastcgi_pass   127.0.0.1:9000;
            fastcgi_index  index.php;
            fastcgi_split_path_info  ^((?U).+\.php)(/?.+)$;
            fastcgi_param  SCRIPT_FILENAME  \$document_root\$fastcgi_script_name;
            fastcgi_param  PATH_INFO  \$fastcgi_path_info;
            fastcgi_param  PATH_TRANSLATED  \$document_root$fastcgi_path_info;
            include        fastcgi_params;
        }
}
EOF
fi

if [ "$serv" = "Lighttpd" ];then
cat >/phpstudy/server/lighttpd/conf/vhosts/$domain.conf<<EOF
\$HTTP["host"] == "$domain" {
    server.document-root = "$DocumentRoot"
    server.port = 80
}
EOF
fi

  
    echo "Successfully create $domain vhost"
    echo "################### information about your website ######################"
    echo "The DocumentRoot:$DocumentRoot"
   echo 
 #/phpstudy/phpstudy restart > /dev/null 2>&1
fw_restart
}

#===============================================================================
#Description:Remove apache virtualhost.
#Usage:vhost_del
#===============================================================================
function vhost_del(){
    read -p "(Please input a domain you want to delete):" vhost_domain
    if [ "$vhost_domain" = "" ]; then
        echo "You need input a domain."
        exit 1
    fi
    echo "---------------------------"
    echo "vhost account = $vhost_domain"
    echo "---------------------------"
    echo ""
    get_char(){
    SAVEDSTTY=`stty -g`
    stty -echo
    stty cbreak
    dd if=/dev/tty bs=1 count=1 2> /dev/null
    stty -raw
    stty echo
    stty $SAVEDSTTY
    }
    echo "Press any key to start delete vhost..."
    echo "or Press Ctrl+c to cancel"
    echo ""
    char=`get_char`
if [ "$serv" = "Apache" ];then
    if [ -f "/phpstudy/server/httpd/conf/vhosts/$vhost_domain.conf" ]; then
        rm -rf /phpstudy/server/httpd/conf/vhosts/$vhost_domain.conf
       # rm -rf /phpstudy/www/$vhost_domain
    
    else
        echo "Error!!No such domain file.Please check your input domain again..."
        exit 1
    fi
fi

if [ "$serv" = "Nginx" ];then
    if [ -f "/phpstudy/server/nginx/conf/vhosts/$vhost_domain.conf" ]; then
        rm -rf /phpstudy/server/nginx/conf/vhosts/$vhost_domain.conf
      #  rm -rf /phpstudy/www/$vhost_domain
    
    else
        echo "Error!!No such domain file.Please check your input domain again..."
        exit 1
    fi
fi

if [ "$serv" = "Lighttpd" ];then
    if [ -f "/phpstudy/server/lighttpd/conf/vhosts/$vhost_domain.conf" ]; then
        rm -rf /phpstudy/server/lighttpd/conf/vhosts/$vhost_domain.conf
       # rm -rf /phpstudy/www/$vhost_domain
 
    else
        echo "Error!!No such domain file.Please check your input domain again..."
        exit 1
    fi
fi

    
    echo "Successfully delete $vhost_domain vhost"
    echo "You need to remove site directory manually!"
fw_restart
#/phpstudy/phpstudy start > /dev/null 2>&1
echo 
}

#===============================================================================
#Description:List apache virtualhost.
#Usage:vhost_list
#===============================================================================
function vhost_list(){
if [ "$serv" = "Apache" ];then
    ls /phpstudy/server/httpd/conf/vhosts/ | cut -f 1,2,3 -d "."
fi
if [ "$serv" = "Nginx" ];then
    ls /phpstudy/server/nginx/conf/vhosts/ | cut -f 1,2,3 -d "."
fi
if [ "$serv" = "Lighttpd" ];then
    ls /phpstudy/server/lighttpd/conf/vhosts/ | cut -f 1,2,3 -d "."
fi
}


#===============================================================================
#Description:add,del,list ftp user.
#Usage:ftp (add|del|list)
#===============================================================================
function ftp(){
if [ ! -s /etc/init.d/pure-ftpd ];then
echo "not install pure-ftpd"
exit
fi
    case "$faction" in
    add)
    read -p "(Please input ftpuser name):" ftpuser
    read -p "(Please input ftpuser password):" ftppwd
    read -p "(Please input ftpuser root directory):" ftproot
    useradd -d $ftproot -c pure-ftpd -s /bin/sh  $ftpuser
    echo $ftpuser:$ftppwd |chpasswd
    if [ -d "$ftproot" ]; then
        chown -R $ftpuser $ftproot
    else
        mkdir -p $ftproot
        chown -R $ftpuser $ftproot
    fi
    echo "Successfully create ftpuser $ftpuser"
    echo "ftp root directory is $ftproot"
    ;;
    del)
    read -p "(Please input the ftpuser you want to delete):" ftpuser
    userdel $ftpuser
    echo "Successfully delete ftpuser $ftpuser"
    ;;
    list)
    printf "FTPUser\t\tRoot Directory\n"
    cat /etc/passwd | grep pure-ftpd | awk 'BEGIN {FS=":"} {print $1"\t\t"$6}'
    ;;
    *)
    echo "Usage:add|del|list"
    exit 1
    esac
}

case "$1" in
 start)

 do_start
 echo 
 ;;
 stop)
 
 do_stop
 echo
 ;;

 restart)

 do_stop
 echo 
 do_start
 echo 
 ;;

 uninstall)
 
uninstall_phpstudy
echo 
 ;;

add)
   vhost_add
    ;;
del)
   vhost_del
    ;;
list)
   vhost_list
    ;;
ftp)
  faction=$2
    ftp
        ;;

 *)
 echo "Usage: $SCRIPTNAME {start|stop|restart|add|del|list|ftp(add,del,list)|uninstall}"  

 exit 3

esac
