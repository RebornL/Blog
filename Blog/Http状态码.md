### 2XX

请求成功

- 200 正常处理
- 204 成功处理，蛋服务器没有新数据返回，显示页面不更新
- 206 对服务器进行范围请求，只返回一部分数据



### 3XX

表示重定向

- 301 *永久重定向*，请求资源已经分配了新的URI中，URL地址已经改变
- 302 *转发*，请求资源临时分配新的URI中，URL地址没有变
- 303 与302功能一样，但客户端请求方式为GET来获取资源
- 304 *返回未过期的缓存数据*，发送了附带请求，但不符合条件
- 307 与302功能一样，但客户端必须保持请求方法不变向新的地址发出请求



### 4XX

表示客户端出错

- 400 请求报文语法错误
- 401 需要认证身份
- 403 没有访问权限
- 404 服务器没有这个资源



### 5XX

表示服务器出错

- 500 内部资源出错
- 503 服务器繁忙



### HTTP请求报文

- 报文首部

  - 请求行（请求方式 链接 协议）
  - 首部字段

- 空行（CR+LF）

- 报文主体：GET请求，请求内容放在链接中；POST请求，请求内容放在报文主体中

  > 补充：GET产生一个TCP数据包；POST产生两个TCP数据包。
  >
  > 对于GET方式的请求，浏览器会把http header和data一并发送出去，服务器响应200（返回数据）；
  >
  > 而对于POST，浏览器先发送header，服务器响应100 continue，浏览器再发送data，服务器响应200 ok（返回数据）。（[链接](https://zhuanlan.zhihu.com/p/22536382)）

GET请求：

```
GET / HTTP/1.1
Host: www.baidu.com
Connection: keep-alive
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8,en;q=0.6
Cookie: BAIDUID=4082549DEE5E64678FC46642E185D98C:FG=1;

```

POST请求：

```
POST / HTTP/1.1
Host: www.demo.com
Cache-Control: no-cache
Postman-Token: 81d7b315-d4be-8ee8-1237-04f3976de032
Content-Type: application/x-www-form-urlencoded

key=value&testKey=testValue
```



### HTTP响应报文

- 报文首部
  - 响应状态行（协议 HTTP状态码 描述结果）
  - 首部字段
- 空行（CR+LF）
- 响应正文

响应内容：

```
HTTP/1.1 200 OK
Date: Sat, 31 Dec 2005 23:59:59 GMT
Content-Type: text/html;charset=ISO-8859-1
Content-Length: 122

＜html＞
＜head＞
＜title＞Wrox Homepage＜/title＞
＜/head＞
＜body＞
＜!-- body goes here --＞
＜/body＞
＜/html＞
```



### 补充

> Http与Https的区别：

1. HTTP 的URL 以http:// 开头，而HTTPS 的URL 以https:// 开头
2. HTTP 是不安全的，而 HTTPS 是安全的
3. HTTP 标准端口是80 ，而 HTTPS 的标准端口是443
4. 在OSI 网络模型中，HTTP工作于应用层，而HTTPS 的安全传输机制工作在传输层
5. HTTP 无法加密，而HTTPS 对传输的数据进行加密
6. HTTP无需证书，而HTTPS 需要CA机构wosign的颁发的SSL证书



[HTTP面试题](https://mp.weixin.qq.com/s?__biz=MzI4Njg5MDA5NA==&mid=2247483733&idx=1&sn=f9ab8d07d2151bd40cdcd9a290317346&chksm=ebd74054dca0c942a36e6e63c783e9b1f414a16e2c702ae4b371a204960a50c7ae89af207139#rd)

























