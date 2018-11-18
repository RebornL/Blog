---
layout:     post
title:      Ubuntu下MariaDB配置
subtitle:   
date:       2018-03-03
author:     Reborn
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Java
---

# MariaDB安装及远程登录设置

## 安装

在debian下，

```shell
sudo apt install mariadb-server
```

然后进行安全设置，如果在安装中途没有出现设置root密码时，在此过程进行设置

```shell
sudo mysql_secure_installation
```

设置密码之后会出现以下选项

> Remove anonymous users? [Y/n] // 删除匿名用户？生产环境建议删除
>
> Disallow root login remotely? [Y/n] // 禁止 root 用户远程登录？生产环境建议禁止
>
> Remove test database and access to it? [Y/n] // 删除测试数据库以及对应的权限？生产环境建议删除
>
> Reload privilege tables now? [Y/n] // 重新载入权限表？这里选择 Y，上面的配置才能立即生效

## 远程登录设置

进入MariaDB终端操作，输入以下命令

```shell
# 给用户最大权限,给root用户最大权限
grant all privileges on *.* to 'root'@'%' identified by 'password';
# 刷新权限表
flush privileges;

exit;
```

然后打开编辑/etc/mysql/mariadb.conf.d/50-server.cnf配置文件，讲bind-address这一行注释掉

```shell
vim /etc/mysql/mariadb.conf.d/50-server.cnf

# 重启MariaDB即可
systemctl restart mysql
```



![MariaDB1](../img/MariaDB1.png)



### 参考链接

- https://www.jianshu.com/p/16682746137b
- https://www.centos.bz/2017/07/centos7mariadb-allow-specify-ip-remote-access/

