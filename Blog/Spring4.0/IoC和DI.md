###IoC的概念和DI的概念
IoC：Inverse of Convert，控制反轉，比較難理解的概念，以後補充
DI：Dependency Injection，依賴注入，讓調用類對某一接口實現的依賴關係由第三方（容器和協作類）注入，以移除調用類對某一接口實現類的依賴。

###IoC的三種類型
- 構造函數注入
- 屬性注入
- 接口注入

####構造函數注入
這種利用構造器的初始化，注入接口的實現類。這種方式有點像面向對象的組合使用。
```java
//MoAttack.java
public class MoAttack {

    private GeLi geli;

    //通過構造函數參數注入革離的飾演者
    public MoAttack(GeLi geli) {
        this.geli = geli;
    }

    public void cityGateAsk() {
        geli.responseAsk("墨者革離");
    }
}

//Director.java
//導演安排角色扮演者
public class Director {
    public void direct() {
        //指定角色飾演者
        GeLi geli = new LiuDeHua();

        //將具體飾演者到劇本中
        MoAttack moAttack = new MoAttack(geli);
        moAttack.cityGateAsk();
    }
}
```

####屬性注入
通過setter方法，在有需要的時候再注入接口實現類。
```java
//MoAttack.java
public class MoAttack {
    private GeLi geli;

    //屬性注入
    public void setGeLi(GeLi geli) {
        this.geli = geli;
    }

    public void cityGateAsk() {
        geli.responseAsk("墨者革離");
    }
}

//Director.java
public class Director {
    public void direct() {
        Geli geli = new LiuDeHua();

        MoAttack moAttack = new MoAttack();
        moAttack.setGeli(geli);
        moAttack.cityGateAsk();
    }
}
```

####接口注入
將調用類的所有依賴注入的方法抽取到一個接口，調用類實現該接口進行依賴注入。

實現的效果，其實與屬性注入並無區別，反而顯得比屬性注入稍麻煩。
```java
//interface
public interface ActorArrangeable {
    void injectGeLi(GeLi geli);
}

//MoAttack.java
public class MoAttack implements ActorArrangeable {
    private GeLi geli;

    public void injectGeLi(Geli geli) {
        this.geli = geli;
    }

    public void cityGateAsk() {
        geli.responseAsk("墨者革離");
    }
}

//Director.java
public class Director {
    public void direct() {
        MoAttack moAttack = new MoAttack();
        GeLi geli = new LiuDeHua();
        moAttack.injectGeLi(geli);
        moAttack.cityGateAsk();
    }
}
```

###Spring容器完成依賴關係的注入
Spring通過配置文件或註解描述類和類之間的依賴關係，自動完成類的初始化和依賴注入工作，其底層工作原理是java的反射機制。
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w4.org/2001/XMLSchema-instance"
    xmlns:p="http://www.springframework.org/schema/p"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
      http://www.springframeworl.org/schema/beans/spring-beans-4.0.xsd">
    <!-- 實現類的實例化 -->
    <bean id="geli" class="LiuDeHua"/>
    <bean id="moAttack" class="com.smart.ioc.MoAttack"
            p:geli-ref="geli"/><!--通過geli-ref建立依賴關係-->
</beans>
```
最後可以通過new XmlBeanFactory("beans.xml")等方式啓動容器，Spring會根據配置文件的描述信息，自動實例化Bean並完成依賴關係的裝配，從容器中即可返回準備就緒的Bean實例，後續可以直接使用。


### java反射機制
```java
//示例
//Car.java
package com.smart.reflect;

public class Car {
    private String brand;
    private String color;
    private int maxSpeed;

    public Car() {}

    public Car(String brand, String color, int maxSpeed) {
        this.brand = brand;
        this.color = color;
        this.maxSpeed = maxSpeed;
    }

    public void introduce() {
        System.out.println("brand: "+brand+" color: "+color+" maxSpeed: "+maxSpeed);
    }

  	//getter 和 setter方法
  	...
}

//ReflectTest.java
package com.smart.reflect;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;

public class ReflectTest {
    public static Car initByDefaultConst() throws Throwable {
        //通過類加載器獲取Car類對象
        ClassLoader loader = Thread.currentThread().getContextClassLoader();
        Class clazz = loader.loadClass("com.smart.reflect.Car");

        //獲取類的默認構造器對象並通過它實例化Car
        Constructor cons = clazz.getDeclaredConstructor((Class[])null);
        Car car = (Car)cons.newInstance();

        //通過反射方法設置屬性
        Method setBrand = clazz.getMethod("setBrand", String.class);
        setBrand.invoke(car, "bif");
        Method setColor = clazz.getMethod("setColor", String.class);
        setColor.invoke(car, "black");
        Method setMaxSpeed = clazz.getMethod("setMaxSpeed", int.class);
        setMaxSpeed.invoke(car, 200);

        return car;
    }

    public static void main(String[] args) throws Throwable {
        Car car = initByDefaultConst();
        car.introduce();
    }
}
```
#####ClassLoader
類加載器的作用是尋找類的字節碼，構造出類在jvm中對象的組建。

类装载器把一个类装入JVM中，需要经过以下步骤：

1. 装载：查找和导入Class文件
2. 链接：校验、准备和解析步骤
   - 校验：检查载入Class文件数据的正确性
   - 准备：给类的静态变量分配存储空间
   - 解析：将符号引用转换成直接引用
3. 初始化：对类的静态变量、静态代码块执行初始化工作

JVM运行时会产生3个ClassLoader：*根装载器*, *ExtClassLoader*, *APPClassLoader*。

- 根装载器：采用C++语言编写，在java中不可见，负责装载JRE的核心类库
- ExtClassLoader：为ClassLoader的子类，负责装载JRE扩展目录ext中JAR类包
- AppClassLoader：为ClassLoader的子类，负责装载Classpath路径下的类包

（这三类装载其存在父子层级关系，根装载器为ExtClassLoader的父装载器，ExtClassLoader为APPClassLoader的父装载器）

```java
public class ClassLoaderTest {
    public static void main(String[] args) {
        ClassLoader loader = Thread.currentThread().getContextClassLoader();
        System.out.println("current loader: "+ loader);
        System.out.println("parent loader: "+ loader.getParent());
        System.out.println("grandparent loader: "+loader.getParent().getParent());
    }
}
/*
current loader: sun.misc.Launcher$AppClassLoader@18b4aac2
parent loader: sun.misc.Launcher$ExtClassLoader@677327b6
grandparent loader: null
*/
```

Class类没有public的构造方法，Class对象是在装载类是有JVM通过调用类装载器中的defineClass()方法自动构造。

#### 反射类

- Constructor：类的构造函数反射类，通过Class对象getConstructors()获取所有构造函数反射对象数组，getConstructor(Class... parameterTypes)获取特定参数的构造函数反射对象。构造器对象的newInstance()方法可以创建一个实例对象。
- Method：类方法的反射类，Class对象getDeclaredMethods()获取所有方法反射类对象数组Method[]。再通过invoke方法进行调用。
- Field：类的成员变量的反射类，Class对象getDeclaredFields方法获取类的成员变量反射对象数组。通过Field的set方法对成员变量进行值设置。