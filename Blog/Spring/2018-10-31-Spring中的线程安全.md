---
layout:     post
title:      Spring中线程安全
subtitle:   
date:       2018-10-31
author:     Reborn
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Spring
    - Java Web
---

## Spring中线程安全

- 保证bean是无状态对象，如Service、DAO和Controller这些都是无状态的对象，只是执行某些操作。bean的生命周期：

  1. singleton：默认的scope，每个scope为singleton的bean都会被定义为一个单例对象，该对象的生命周期是与Spring IOC容器一致的（但在第一次被注入时才会创建）。

  2. prototype：bean被定义为在每次注入时都会创建一个新的对象。

  3. request：bean被定义为在每个HTTP请求中创建一个单例对象，也就是说在单个请求中都会复用这一个单例对象。

  4. session：bean被定义为在一个session的生命周期内创建一个单例对象。

  5. application：bean被定义为在ServletContext的生命周期中复用一个单例对象。

  6. websocket：bean被定义为在websocket的生命周期中复用一个单例对象。

- 当bean需要有自己变量时，可以用ThreadLocal将变量变为线程私有，避免多线程共用。最后使用完成，需要手动remove释放内存。

  >ThreadLocal内部含有一个TheadLocalMap类（采用线性探测法实现的HashMap，key为ThreadLocal对象且为WeakReference）
  >
  >如果Thread Local被置为null之后，TheadLocalMap的Entry的key也为null，只要线程不结束，这些无法访问的value就会形成内存泄漏，因此即使时弱引用的key，如果不手动调用ThreadLocalMap的set、getEntry和remove函数去自动清理，仍然会存在泄露问题。**因此每次使用完毕，都需要手动调用ThreadLocal的remove方法清除无用的Entry。**

- 当实例变量需要在多线程中共享时，可以采用Synchronized、CAS、lock等线程同步操作实现线程安全。