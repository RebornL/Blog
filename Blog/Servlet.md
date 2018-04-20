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

+ 通过设置response.setCharacterEncoding("UTF-8"); response.setContentType("text/html;charset=utf-8");使得页面中文不为乱码



#### servlet应用

文件下载，生成随机数图片，数据压缩上传等等，详情见这个[链接](https://mp.weixin.qq.com/s?__biz=MzI4Njg5MDA5NA==&mid=2247484081&idx=2&sn=06d118f2976a1f2cf98e5fca47af252a&chksm=ebd743b0dca0caa6895f25f0eaaac66bec5f5f8141e1a869bfc8562f8c4e933ef8c1fe807dfd#rd)



#### HttpServletRequest

#### HttpServletResponse



### 会话

#### Cookie

由于Http协议是无状态协议，不会保存数据，因此每次建立链接，服务器都会重开新的线程进行处理。为了让服务器能够识别客户端，W3C提出了Cookie用来保存客户端信息。此时，Cookie就相当于通行证，服务器端能够读取Cookie信息来识别客户端。



Cookie的流程：浏览器访问服务器，**如果服务器需要记录该用户的状态，就使用response向浏览器发送一个Cookie，浏览器会把Cookie保存起来。当浏览器再次访问服务器的时候，浏览器会把请求的网址连同Cookie一同交给服务器**。



request可以通过getCookie获取Cookie对象，response可以通过addCookie在其响应头添加set-cookie头字段。

Cookie特点：

- 不可跨域名性
- 保存中文，需要使用URLEncoder.encode进行编码，读取时需要使用URLEncoder.decode进行解码。
- setMaxAge设置过期时间，setDomain设置可获取Cookie的域名，设置Cookie的secure属性为true则只允许在HTTPS和SSL等安全协议中传输Cookie



#### Session

session是另外一种记录浏览状态的机制，与Cookie的区别，Session存储在服务器端，Cookie存储在客户端；Session可以存储对象，Cookie只能存储字段。



Session比Cookie更为好用，在服务器端可以作为一种域对象，让servlet相互通信。

- setMaxInactiveInterval()方法设置有效期
- addAttribute()，给session添加属性
- 通过JSESSIONID来识别不同客户端，JSESSIONID存储在Cookies中，当Cookie被禁止时，通过**URL重写**来解决JSESSIONID存储
  - **encodeURL(String url)**
  - **encodeRedirectURL(String url)**

#### 转发和重定向

### 过滤器

在web请求到达servlet之前，会首先经过过滤器（Filter）进行处理；servlet处理完请求也需要经过过滤器再返回给服务器。



过滤器的作用：过滤敏感字符串，避免中文乱码，权限验证等等，限制用户访问某些资源。