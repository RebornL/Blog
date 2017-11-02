*time: 2017-09-14*

*author: LQB*

## 欠拟合和过拟合

#### 训练误差与泛化误差

训练误差：机器学习在训练集上表现的误差

泛化误差：则是指机器学习在任意集上表现的误差



#### 欠拟合与过拟合

欠拟合：是指机器学习无法在训练集上得到较低的训练误差

过拟合：是指训练误差远远小于测试集上表现的误差



#### 影响因素

- 模型选择

  更好的模型可以达到更低的训练误差（结合Andrew的课程中讲到，选择更好的模型，可以降低bias误差，防止由于high bias带来的欠拟合）

- 训练集的大小

  训练集过小，容易导致过拟合，因此训练集不宜过小。（统计学理论，泛化误差不会随样本的增多而表大）



#### 示例

三阶拟合，线性拟合（欠拟合），数据量过少（过拟合）

```python
from mxnet import ndarray as nd
from mxnet import autograd
from mxnet import gluon

num_train = 1000
num_test = 1000
true_w = [1.2, -3.4, 5.6]
true_b = 5.0

x = nd.random_normal(shape=(num_train + num_test, 1))
X = nd.concat(x, nd.power(x, 2), nd.power(x, 3))
y = true_w[0] * X[:, 0] + true_w[1] * X[:, 1] + true_w[2] * X[:, 2] + true_b
y += .1 * nd.random_normal(shape=y.shape)
y_train, y_test = y[:num_train], y[num_train:]

square_loss = gluon.loss.L2Loss()

def getNetAndIter(X_train, y_train, batch_size):
    dataset_train = gluon.data.ArrayDataset(X_train, y_train)
    data_iter_train = gluon.data.DataLoader(dataset_train, batch_size, shuffle=True)
    net = gluon.nn.Sequential()
    net.add(gluon.nn.Dense(1))
    return net, data_iter_train
  
def train(net, data_iter_train, lr, cur_loss, epochs, verbose_epoch, batch_size):
    net.collect_params().initialize(force_reinit=True)
    trainer = gluon.Trainer(net.collect_params(), 'sgd', {'learning_rate': lr})
    for epoch in range(epochs):
        total_loss = 0
        for data, label in data_iter_train:
            with autograd.record():
                output = net(data)
                loss = cur_loss(output, label)
            loss.backward()
            trainer.step(batch_size)
            total_loss += nd.sum(loss).asscalar()
        if epoch >= verbose_epoch:
            print("Epoch %d, train loss: %f" % (epoch, total_loss / y_train.shape[0]))
    return net
  
def test(X_test, y_test, net, cur_loss):
    loss_test = nd.sum(cur_loss(net(X_test), y_test)).asscalar() / \
                y_test.shape[0]
    print("Test loss: %f" % loss_test)
    print("True params: ", true_w, true_b)
    print("Learned params: ", net[0].weight.data(), net[0].bias.data())
    
def learn(X_train, X_test, y_train, y_test, lr, cur_loss):
    epochs = 50
    verbose_epoch = 45
    batch_size = min(10, X_train.shape[0])
    net, data_iter_train = getNetAndIter(X_train, y_train, batch_size)
    net_trained = train(net, data_iter_train, lr, cur_loss, epochs, verbose_epoch, batch_size)
    test(X_test, y_test, net_trained, cur_loss)
    
# 三阶多项式拟合
X_train_ord3, X_test_ord3 = X[:num_train, :], X[num_train:, :]
# print(X_train_ord3, X_test_ord3)
learning_rate = 0.01
learn(X_train_ord3, X_test_ord3, y_train, y_test, learning_rate, square_loss)

# 线性拟合（欠拟合）
x_train_ord1, x_test_ord1 = x[:num_train, :], x[num_train:, :]

learning_rate = 0.01
learn(x_train_ord1, x_test_ord1, y_train, y_test, learning_rate, square_loss)

# 训练量不足（过拟合）
y_train, y_test = y[0:2], y[num_train:]
X_train_ord3, X_test_ord3 = X[0:2, :], X[num_train:, :]

learning_rate = 0.01
learn(X_train_ord3, X_test_ord3, y_train, y_test, learning_rate, square_loss)
```

