*time: 2017-09-13*

*author: LQB*

## 使用gluon实现softmax多分类器

```python
import sys
sys.path.append("..")
import utils
# 加载数据
batch_size = 256
train_data, test_data = utils.load_data_fashion_mnist(batch_size)

# 初始化模型
from mxnet import gluon

net = gluon.nn.Sequential()
with net.name_scope():
  net.add(gluon.nn.Flatten())#自动reshape输入数据为batch_size x pic_size
  net.add(gluon.nn.Dense(10))#网络结构自动调整
net.initialize()#参数随机初始化

# Softmax交叉熵损失函数
softmax_cross_entropy = gluon.loss.SoftmaxCrossEntropyLoss()

# slover函数
trainer = gluon.Trainer(net.collect_params, 'sgd', {'learning_rate': 0.1})

# 训练
from mxnet import ndarray as nd
from mxnet import autograd as ag

epochs = 5
for epoch in range(epochs):
  train_loss = 0
  train_acc = 0
  for data, label in train_data:
    with ag.record():
      output = net(data)
      loss = softmax_cross_entropy(output, label)
      
    loss.backward()
    trainer.step(batch_size)#调整网络参数
    
    train_loss += nd.mean(loss).asscalar()
    train_acc += utils.accuracy(output, label)
  # 检测每次迭代后，网络模型的精确度  
  test_acc = utils.evaluate_accuracy(test_data, net)
  print("Epoch %d. Loss: %f, Train acc %f, Test acc %f" % (
        epoch, train_loss/len(train_data), train_acc/len(train_data), test_acc))
```

使用gluon的好处，网络结构自动调整，只需要定义层和每层的输出即可。