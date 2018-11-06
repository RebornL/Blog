*time: 2017-09-11*

*author: LQB*

## 使用gluon实现线性回归

实现流程与单纯使用ndarray和autograd一致

```python
from mxnet import ndarray as nd
from mxnet import autograd as ag
from mxnet import gluon

num_inputs = 2
num_examples = 1000

true_w = [2, -3.4]
true_b = 4.2

X = nd.random_normal(shape=(num_examples, num_inputs))
y = true_w[0] * X[:, 0] + true_w[1] * X[:, 1] + true_b
y += .01 * nd.random_normal(shape=y.shape)

# 数据读取
batch_size = 10
dataset = gluon.data.ArrayDataset(X, y)
#DataLoader跟yield的迭代一样的作用，选出一定量的图片
data_iter = gluon.data.DataLoader(dataset, batch_size, shuffle=True)#shuffle参数是是否随机
"""
数据读取
for data, label in data_iter:
	print(data, label)
"""

# 定义模型
"""
使用gluon的Sequential串联网络层
"""
net = gluon.nn.Squential()
net.add(gluon.nn.Dense(1))#参数为输出节点的个数

# 初始化参数，默认随机初始化
net.initialize()

# 损失函数
square_loss = gluon.loss.L2Loss()

# solver
trainer = gluon.Trainer(net.collect_params(), 'sgd', {'learning_rate': 0.1})

# 训练
epochs = 5
batch_size = 10
for epoch in range(epochs):
  total_loss = 0
  for data, label in data_iter:
    with ag.record():
      output = net(data)
      loss = square_loss(output, label)
    loss.backward()#将w，b的求导值放置在 已分配好的内存空间 然后通过.grad属性值获取导数值
    trainer.step(batch_size)#更新参数
    total_loss += nd.sum(loss).asscalar()
    
  print("Epoch %d, the average loss: %f" % (epoch, total_loss/num_examples))
  
# 参数对比,首先要获取相应层
# 权重
dense = net[0]
w = dense.weight.data()
b = dense.bias.data()

print(true_w, w)
print(true_b, b)

```

