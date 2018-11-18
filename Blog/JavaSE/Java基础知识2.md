---
layout:     post
title:      Java基础知识2
subtitle:   
date:       2018-11-10
author:     Reborn
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Java
---

# 基础知识2

## 接口

### Comparable接口

启用对象排序时，是可以实现Comparable接口，里面只有一个需要实现的方法

```java
public interface Comparable<T> {
    int compareTo(T other);
}
```

> compareTo返回值，返回正值（大于0）代表着**当前对象**排在other之后；返回负数（小于0）代表着**当前对象**排在other之前；两个对象相同时，返回0 。

**两个数值的数字比较，建议直接采用包装类的compare方法，可以出NaN和无穷大值。**



### Comparator接口

```java
public interface Comparator<T> {
    int compare(T first, T second);
}
```

使用：

```java
Comparator<String> comp = new LengthComparator<>();
if(comp.compare(word1, word2)>0) {...}
```



#### Comparable接口和Comparator接口的区别：

Comparable接口比较，需要类来亲自实现这个类，才能相互比较；而Comparator接口可以由第三类来实现，需要比较的类不需要实现该接口，只需要作为参数传进去即可。两个各有好处。lambda常用Comparator接口。



### lambda表达式

lambda表达式将函数表达为对象，所有函数式接口都可以用lambda表达式实现。在函数式接口作为参量的地方有特殊的语法，可以将lambda表达式变得更简洁----方法引用和构造函数引用。

#### 方法引用

举例，如Arrays.sort方法对对字符串进行排序

```java
Arrays.sort(strings, (x, y) -> x.compareToIgnoreCase(y));
```

此时也可以用方法引用的方式：

```java
Arrays.sort(strings, String::compareToIgnoreCase);
```

方法引用三种方式：

- 类::实例方法
- 类::静态方法
- 对象::实例方法

#### 构造函数引用

构造函数引用的方法名都是new，如`Employee::new`和`Employee[]::new`。使用：

```java
Employee[] buttons = stream.toArray(Employee[]::new);
```



## Stream

Java8中新增流操作，只需要指定怎么做即可。举例说，要求某个属性的平均值，只需要指定数据源和属性，然后流类库会优化计算。

```java
//统计字符串长度超过12的个数
long count = words.stream().filter(w -> w.length()>12).count();
```



#### Stream和集合的区别：

1. 流不存储元素
2. 流操作不改变源的数据
3. 流操作可能是延时，在需要结果的时候才会计算结果出来







































