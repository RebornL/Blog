# MySQL入门笔记

---------------------------------------------------------------

**Date: 20180226 **

**Author: Reborn**

---------------------------------

[TOC]

## 检索数据

### select语句

从pro_table选取pro_name这一列：

```mysql
SELECT pro_name FROM pro_table;
```

输出结果：![select1](select1.png)

选取多列：

```mysql
SELECT pro_id, pro_name, pro_price FROM pro_table;
```

输出结果：![select2](select2.png)

选取所有列：

```mysql
SELECT * FROM pro_table;
```

### DISTINCT关键字

DISTINCT关键字用来返回不同，不允许重复



选取出产品的厂商：

```mysql
SELECT DISTINCT vent_id FROM pro_table;
```

输出结果：![distinct1](distinct1.png)

## 过滤数据





























