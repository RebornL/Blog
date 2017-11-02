*time: 2017-10-13*

*author: LQB*

## 使用gluon实现卷积神经网络(CNN)

```python
from mxnet.gluon import nn
# 使用Sequential来定义神经网络
net = nn.Sequential()
"""
定义的网络结构：
卷积层+池化层-->卷基层+池化层-->全连接层（128输出）-->全连接层(10输出)
"""
with net.name_scope():
    net.add(
    	nn.Conv2D(channels=20, kernel_size=5, activation="relu"),
      	nn.MaxPool2D(pool_size=2, strides=2),
        nn.Conv2D(channels=50, kernel_size=3, activation="relu"),
        nn.MaxPool2D(pool_size=2, strides=2),
        nn.Flatten(),
        nn.Dense(128, activation="relu"),
        nn.Dense(10)
    )
    
from mxnet import gluon
import sys
sys.path.append("..")
import utils

# 初始化
ctx = utils.try_gpu()
net.initialize(ctx=ctx)
print(ctx)
# 获取数据
batch_size = 256
train_data, test_data = utils.load_data_fashion_mnist(batch_size)

# 训练
loss = gluon.loss.SoftmaxCrossEntropyLoss()
trainer = gluon.Trainer(net.collect_params(), "sgd", {"learning_rate": 0.5})
utils.train(train_data, test_data, net, loss, trainer, ctx, num_epochs=5)
```

