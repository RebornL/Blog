### Servlet

servlet是用Java编写的服务器端程序，主要功能在于交互式地浏览和修改数据，生成动态Web内容。因此servlet最大作用便是处理浏览器带来的HTTP请求，并返回一个响应给浏览器，从而实现浏览器和服务器的交互。

#### servlet生命周期

1. Tomcat加载servlet实例
2. **初始化**，调用init()方法，该方法只会在servlet第一次被加载的时候使用
3. **处理服务，响应请求**，调用servlet的service()方法，根据请求方法的不同调用不同的方法。
4. **销毁**，servlet长期不活动或者tomcat删除servlet，tomcat关闭时，会调用servlet的destory方法，释放资源。
5. 销毁完，等待垃圾回收。

#### servlet细节

+ servlet的url匹配支持通配符，而且具有优先级关系。


+ servlet都是单例。

+ ServletConfig对象可以获取web.xml中对应servlet中配置的初始化参数。

  + ```
    <servlet>
        <servlet-name>Demo1</servlet-name>
        <servlet-class>zhongfucheng.web.Demo1</servlet-class>
        <init-param>
        <param-name>name</param-name>
        <param-value>zhongfucheng</param-value>
        </init-param>
    </servlet>
    <servlet-mapping>
        <servlet-name>Demo1</servlet-name>
        <url-pattern>/Demo1</url-pattern>
    </servlet-mapping>
    ```

  + ```
    //在doGet和doPost方法中
    ServletConfig servletConfig = this.getServletConfig();
    //获取在xml中配置参数
    String name = servletConfig.getInitParameter("name");
    ```

  + **每个servlet都有各自对应的ServletConfig对象**。

+ ServletContext对象，则是代表当前的web容器对象，即所有servlet共享同一个ServletContext对象，这与一个servlet对应一个ServletConfig对象不同。因此可以用ServletContext对象读取整个web应用的参数信息以及站点的资源文件。此外，ServletContext也可以作为一个域对象，存储着不同servlet之间沟通的信息。

  ```
  # web.xml配置站点的参数信息
  <context-param>
  	<param-name>name</param-name>
  	<param-value>zhongfucheng</param-value>
  </context-param>

  //Demo1.java
  ServletContext servletContext = this.getServletContext();
  String name = servletConfig.getAttribute("name");
  ```

  ```java
  //获取站点资源文件
  //获取到ServletContext对象
  ServletContext servletContext = this.getServletContext();

  //调用ServletContext方法获取到读取文件的流
  InputStream inputStream = servletContext.getResourceAsStream("/WEB-INF/classes/zhongfucheng/web/1.png");
  ```

  不同servlet之间传递消息

  ```java
  //Demo2.java
  ServletContext servletContext = this.getServletContext();
  servletContext.setAttribute("MyName", "Demo2");
  ```

  ```java
  //Demo3.java
  ServletContext servletContext = this.getServletContext();
  String value = servletContext.getAttribute("MyName");//value: Demo2
  ```

  ​