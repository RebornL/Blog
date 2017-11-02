*time: 2017-09-17*

*author: LQB*

## 使用gluon进行正则化

- mxnet/gluon的默认初始化是weight = uniform(-0.07, 0.07)

```python
from mxnet import ndarray as nd
from mxnet import autograd as ag
from mxnet import gluon

num_train = 20#训练样本数量
num_test = 100#测试样本数量
num_inputs = 200#输入大小

true_w = nd.ones((num_inputs, 1)) * 0.01
true_b = 0.05

X = nd.random.normal(shape=(sum_train+num_test, num_inputs))
# y = X*true_w + noise
y = nd.dot(X, true_w)
y += 0.01*nd.random.normal(shape=y.shape)

X_train, X_test = X[:num_train, :], X[num_train:, :]
y_train, y_test = y[:num_train], y[num_train]

%matplotlib inline
import matplotlib as mpl
mpl.rcParams['figure.dpi'] = 120
import matplotlib.pyplot as plt

batch_size = 1
dataset_train = gluon.data.ArrayDataset(X_train, y_train)
data_iter_train = gluon.data.DataLoader(dataset_train, batch_size)#训练数据加载器

square_loss = gluon.loss.L2Loss()#平方差损失函数

def test():
  return square_loss(net(X), y).mean().asscalar()

def train(weight_decay):
  learning_rate = 0.005
  epochs = 10
  
  net = gluon.nn.Sequential()
  #定义网络结构
  with net.name_scope():
    net.add(gluon.nn.Dense(1))
    
  net.initialize()#参数随机初始化
  
  #wd参数：通过优化算法的wd参数 (weight decay)实现对模型的正则化。这相当于 L2 范数正则化。
  trainer = gluon.Trainer(net.collect_params(), 'sgd', 'learning_rate': learning_rate, 'wd': weight_decay)
  
  train_loss = []
  test_loss = []
  for e in range(epochs):
    for data, label in data_iter_train:
      with ag.record():
        output = net(data)
        loss = square_loss(output, label)
      loss.backward()
      trainer.step(batch_size)
    train_loss.append(test(net, X_train, y_train))
    test_loss.append(test(net, X_test, y_test))
    
  plt.plot(train_loss)
  plt.plot(test_loss)
  plt.legend(['train','test'])
  plt.show()

  return ('learned w[:10]:', net[0].weight.data()[:,:10], 
          'learend b:', net[0].bias.data())
```

