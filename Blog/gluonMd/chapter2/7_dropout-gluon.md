*time: 2017-09-27*

*author: LQB*

## 使用gloun实现丢弃层（dropout）

```python
from mxnet import gluon

net = nn.Sequential()
drop_prob1 = 0.2
drop_prob2 = 0.5

with net.name_scope():
    net.add(nn.Flatten())#对输入层进行处理
    net.add(nn.Dense(256, activation="relu"))
    net.add(nn.Dropout(drop_prob1))
    net.add(nn.Dense(256, activation="relu"))
    net.add(nn.Dropout(drop_prob2))
    net.add(nn.Dense(10))
    
net.initialize()

import sys
sys.path.append("..")
import utils
from mxnet import nd
from mxnet import autograd as ag
from mxnet import gluon

batch_size = 256
train_data, test_data = utils.load_data_fashion_mnist(batch_size)
softmax_cross_entropy = gluon.loss.SoftmaxCrossEntropy()
trainer = gluon.Trainer(net.collect_params(), "sgd", {"learning_rate"： 0.5})

epochs = 5
for epoch in range(5):
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
        
    test_acc = utils.evaluate_accuracy(test_data, net)
    print("Epoch %d. Loss: %f, Train acc %f, Test acc %f" % (
        epoch, train_loss/len(train_data), 
        train_acc/len(train_data), test_acc))
    
```

