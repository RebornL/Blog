*time: 2017-09-14*

*author: LQB*

## 使用gluon定义多层感知机

与前一章的区别在与定义网络的时候更为简洁方便

```python

import sys
from mxnet import gluon
from mxnet import ndarray as nd
from mxnet import autograd as ag
sys.path.append("..")
import utils

# 加载数据
batch_size = 256
train_data, test_data = utils.load_data_fashion_mnist(batch_size)

# 定义网络
num_hiddens = 256
net = gluon.nn.Sequantial()
with net.name_scope():
  net.add(gluon.nn.Flatten())
  net.add(gluon.nn.Dense(num_hidden))# 定义隐藏层
  net.add(gluon.nn.Dense(10))# 定义输入层
net.initialize()

# 损失函数
softmax_cross_entropy = gluon.loss.SoftmaxCrossEntryLoss()

# solver
trainer = gluon.Trainer(net.collect_params(), "sgd", {'learning_rate': 0.5})

# 训练
epochs = 5#迭代次数
for epoch in eopchs:
  train_loss = 0.
  train_acc = 0.
  for data, label in train_data:
    with ag.record():
      output = net(data)
      loss = softmax_cross_entropy(output, label)
    loss.backward()
    trainer.step(batch_size)
  	
    train_loss += nd.mean(loss).asscalar()
    train_acc += utils.accuracy(output, label)
    
  test_acc = utils.evaluate(test_data, net)
  print("Epoch %d. Loss: %f, Train acc %f, Test acc %f" % (epoch, train_loss/len(train_data), train_acc/len(train_data), test_acc))


"""
# 一个隐藏层的训练结果
Epoch 0. Loss: 0.728401, Train acc 0.732607, Test acc 0.809863
Epoch 1. Loss: 0.471546, Train acc 0.826662, Test acc 0.841211
Epoch 2. Loss: 0.413458, Train acc 0.848443, Test acc 0.861914
Epoch 3. Loss: 0.381483, Train acc 0.859885, Test acc 0.869922
Epoch 4. Loss: 0.358197, Train acc 0.869526, Test acc 0.879199
    
# 两个隐藏层的训练结果
Epoch 0. Loss: 0.921268, Train acc 0.663104, Test acc 0.809277
Epoch 1. Loss: 0.498535, Train acc 0.812771, Test acc 0.848047
Epoch 2. Loss: 0.427818, Train acc 0.842132, Test acc 0.856934
Epoch 3. Loss: 0.387092, Train acc 0.856117, Test acc 0.861426
Epoch 4. Loss: 0.366855, Train acc 0.863326, Test acc 0.873633
"""



























```

