*time：2017-09-11*

*author：LQB*	

## 实现一个简单的线性回归模型

```python
from mxnet import ndarray as nd
from mxnet import autograd as ag
import random

num_inputs = 2
num_examples = 1000#数据量
#真实的权重和偏置值
true_w = [2, -3,4]
true_b = 4.2

x = nd.random_normal(shape=(num_examples, num_inputs))#随机初始化输入数据
# 构造一个人工数据集
y = true_w[0]*x[:, 0]+true_w[1]*x[:, 1]+true_b
# 添加噪声
y += nd.random_normal(shape=y.shape)

# 随机读取数据
batch_size = 10
def data_iter():
  idx = list(range(num_examples))
  random.shuffle(idx)
  for i in range(0, num_examples, batch_size):
    # 每次随机读取十个数据
    j = nd.array(idx[i:min(i+batch_size, num_examples)])
    yield nd.take(x, j), nd.take(y, j)
    
"""
读取数据方式
for data, label in data_iter():
	print(data, label)
"""

# 随机初始化参数,w,b是全局变量
w = nd.random_normal(shape=(num_inputs, 1))
b = nd.zeros(1,)
params = [w, b]
for param in params:
    #分别给w，b的求导值分配内存空间
    param.attach_grad()
    
# 定义模型
def net(x):
  # 这个函数定义的不够通用
  return nd.dot(x, w)+b#采用线性模型

# 损失函数
def square_loss(yhat, y):
  # 采用平方差距离作为损失函数的判断
  # 注意这里我们把y变形成yhat的形状来避免自动广播
  return (yhat-y.reshape(yhat.shape))**2

# 优化函数
# params: 参数
# lr: 学习率
# 这是常见的梯度下降求值：w = w - lr*dw
def SGD(params, lr):
  for param in params:
    param[:] = param - lr*param.grad
    #param.grad的值由前向函数backward()可得
    

if __name__ == '__main__':
  epochs = 5#迭代次数
  learning_rate = 0.001#学习率
  for epoch in range(epochs):
    total_loss = 0
    # 以batch_size的方式可以加快速度遍历所有的数据
    for data, label in data_iter():
      with ag.record():
        output = net(data)
        loss = square_loss(output, label)
      #将w，b的求导值放置在 已分配好的内存空间 然后通过.grad属性值获取导数值
      loss.backward()#得到dw，db
      SGD(params, learning_rate)
      total_loss += nd.sum(loss).asscalar()
      
    print("Epoch %d, the average loss: %f" % (epoch, total_loss/num_examples))
    
  print(true_w, w.T)
  print(true_b, b)
```

