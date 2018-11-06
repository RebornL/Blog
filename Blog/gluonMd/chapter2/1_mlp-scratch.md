*time: 2017-09-13*

*author: LQB*

## 从零开始实现多层感知机

实现两层以上（包含隐藏层）的网络

```python
"""
实现一个两层网络
"""
import sys
sys.path.append("..")
import utils

# 加载数据
batch_size = 256
train_data, test_data = utils.load_data_fashion_mnist(batch_size)

# 定义模型参数
from mxnet import ndarray as nd
num_inputs = 28*28
num_outputs = 10

num_hiddens = 256
weight_scale = 0.01#控制参数随机初始化的大小
"""
定义多个隐藏层也与此类似
"""
#定义隐藏层的参数
W1 = nd.random_normal(shape=(num_inputs, num_hiddens), scale=weight_scale)
b1 = nd.zeros(num_hiddens)
#输出层
W2 = nd.random_normal(shape=(num_hiddens, num_outputs), scale=weight_scale)
b2 = nd.zeros(num_outputs)

params = [W1, b1, W2, b2]
for param in params:
  param.attach_grad()

# 激活函数
#使用非线性激活函数，可以使得模型拟合非线性函数
def relu(X):
  return nd.maximum(X, 0)

# 模型定义
def net(X):
  X = X.reshape((-1, num_inputs))#将数据整理成num_inputs列，即每一行为一个样本数
  #隐藏层
  h1 = relu(nd.dot(X, W1) + b1)
  #输出层
  # output = relu(nd.dot(h1, W2) + b2)
  output = nd.dot(h1, W2)+b2#输出层去除relu层，精度和loss都改善了很多
  return output

# Softmax交叉熵损失函数
from mxnet import gluon
softmax_cross_entropy = gluon.loss.SoftmaxCrossEntryLoss()

# 训练
from mxnet import autograd as ag
learning_rate = 0.01
eopchs = 5#迭代次数

for epoch in range(epochs):
  train_loss = 0.
  train_acc = 0.
  for data, label in train_data:
    with ag.record():
      output = net(data)
      loss = softmax_cross_entropy(output, label)
      
    loss.backward()
    utils.SGD(params, learning_rate/batch_size)
    
    train_loss = nd.mean(loss).asscalar()
    train_acc += utils.accuracy(output, label)
    
  test_acc = evaluate_accuracy(test_data, net)
  print("Epoch %d. Loss: %f, Train acc %f, Test acc %f" % (
        epoch, train_loss/len(train_data), train_acc/len(train_data), test_acc))

"""
结果（包含参数的修改）
#去除输出层的relu（多加了一个隐藏层）
Epoch 0. Loss: 1.203505, Train acc 0.525931, Test acc 0.773828
Epoch 1. Loss: 0.553093, Train acc 0.791755, Test acc 0.825879
Epoch 2. Loss: 0.463569, Train acc 0.827665, Test acc 0.852344
Epoch 3. Loss: 0.415650, Train acc 0.845767, Test acc 0.859473
Epoch 4. Loss: 0.382934, Train acc 0.856804, Test acc 0.863086

#在隐藏层1后加入一个输出为128的隐藏层
Epoch 0. Loss: 1.685811, Train acc 0.400449, Test acc 0.569238
Epoch 1. Loss: 1.206510, Train acc 0.589406, Test acc 0.591895
Epoch 2. Loss: 1.105020, Train acc 0.636536, Test acc 0.648730
Epoch 3. Loss: 1.044514, Train acc 0.664689, Test acc 0.656250
Epoch 4. Loss: 0.885805, Train acc 0.688309, Test acc 0.704102

weight_scale = 0.1
Epoch 0. Loss: 2.376104, Train acc 0.181006, Test acc 0.198828
Epoch 1. Loss: 2.104059, Train acc 0.197313, Test acc 0.199219
Epoch 2. Loss: 2.098766, Train acc 0.197329, Test acc 0.200098
Epoch 3. Loss: 2.094375, Train acc 0.198111, Test acc 0.200098
Epoch 4. Loss: 2.091576, Train acc 0.198088, Test acc 0.199707

weight_scale = 0.01
Epoch 0. Loss: 1.656948, Train acc 0.416079, Test acc 0.447754
Epoch 1. Loss: 1.534534, Train acc 0.453613, Test acc 0.457813
Epoch 2. Loss: 1.518049, Train acc 0.458367, Test acc 0.460449
Epoch 3. Loss: 1.507120, Train acc 0.461082, Test acc 0.464160
Epoch 4. Loss: 1.495984, Train acc 0.464135, Test acc 0.464746

weight_scale = 0.001
Epoch 0. Loss: 1.446629, Train acc 0.417725, Test acc 0.543652
Epoch 1. Loss: 1.125899, Train acc 0.539489, Test acc 0.565234
Epoch 2. Loss: 1.070151, Train acc 0.560189, Test acc 0.578418
Epoch 3. Loss: 1.038833, Train acc 0.571980, Test acc 0.585938
Epoch 4. Loss: 1.021052, Train acc 0.579172, Test acc 0.591211
"""



















```

