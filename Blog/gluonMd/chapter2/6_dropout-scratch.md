*time: 2017-09-26*

*author: LQB*

## 丢弃层

dropout法的作用：可以使得原神经网络不同子集在训练数据上都尽可能表现良好，而丢弃的实质是对输入层和隐藏层的参数进行正则化，防止过拟合。

```python
# 获取数据
import sys
sys.path.append("..")
import utils
batch_size = 256
train_data, test_data = utils.load_data_fashion_mnist(batch_size)

# 定义网络参数
num_inputs = 28*28
num_outputs = 10

num_hidden1 = 256
num_hidden2 = 256
weight_scale = 0.01

W1 = nd.random_normal(shape=(num_inputs, num_hidden1), scale=weight_scale)
b1 = nd.zeros(num_hidden1)

W2 = nd.random_normal(shape=(num_hidden1, num_hidden2), scale=weight_scale)
b2 = nd.zeros(num_hidden2)

W3 = nd.random_normal(shape=(num_hidden2, num_outputs), scale=weight_scale)
b3 = nd.zeros(num_outputs)

params = [W1, b1, W2, b2, W3, b3]

for param in params:
	param.attach_grad()//给各个参数求导变量提供内存空间
  
# 定义模型
drop_prob1 = 0.2
drop_prob2 = 0.5

def net(X):
    X = X.reshape((-1, num_inputs))#变成列向量
    h1 = nd.relu(nd.dot(X, W1)+b1)
    h1 = droput(h1, drop_prob1)
    h2 = nd.relu(nd.dot(h1, W2)+b2)
    h2 = dropout(h2, drop_prob2)
    return nd.dot(h2, W3)+b3
  
# 训练
from mxnet import autograd as ag
from mxnet import gluon
#损失函数
softmax_cross_entropy = gluon.loss.SoftmaxCrossEntropyLoss()

learning_rate = 0.5

for epoch in range(5):
    train_loss = 0.
    train_acc = 0.
    for data, label in train_data:
        with ag.record():
            output = net(data)
            loss = softmax_cross_entropy(output, label)
            
        loss.backward()
        utils.SGD(params, learning_rate/batch_size)
        
        train_loss += nd.mean(loss).asscalar()
        train_acc += utils.accuracy(output, label)
        
    test_acc = utils.evaluate_accuracy(test_data, net)
	print("Epoch %d. Loss: %f, Train acc %f, Test acc %f" % (
        epoch, train_loss/len(train_data), 
        train_acc/len(train_data), test_acc))
```

