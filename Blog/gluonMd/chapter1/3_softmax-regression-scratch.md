*time: 2017-09-11*

*author: LQB*

## 从零开始实现softmax多分类器

```python
from mxnet import gluon
from mxnet import ndarray as nd

def transform(data, label):
  # 这里进行数据的转换，不太理解为什么要除以255
  return data.astype('float32')/255, label.astype('float32')
# 下载fashionMnist数据集
mnist_train = gluon.data.vision.FashionMNIST(train=True, transform=transform)
mnist_test = gluon.data.vision.FashionMNIST(train=False, transform=transform)

# 读取数据
batch_size = 256
train_data = gluon.data.DataLoader(mnist_train, batch_size, shuffle=True)
test_data = gluon.data.DataLoader(mnist_test, batch_size, shuffle=False)

# 初始化参数
num_inputs = 28*28#像素点的个数，这里可以想象成将图片上所有的像素点排列成一个列向量作为输入
num_outputs = 10

W = nd.random_normal(shape=(num_inputs, num_outputs))
b = nd.random_normal(shape=num_outputs)#10x1的矩阵
params = [W, b]

for param in params:
  # 给求导的值分配内存空间
  param.attach_grad()
  
# 建立模型
# softmax 将值进行归一化处理，使得前向计算的值接近标签值
def softmax(X):
  exp = nd.exp(X)
  # 将exp的每一行加起来，构成(nrows, 1)的矩阵
  partition = nd.sum(exp.reshape(-1, num_inputs), keepdims=True)
  return exp/partition

def net(X):
  # X.reshape之后的矩阵每一行为一个训练数据
  # 这里可以结合andrew ng讲过每个神经元都会进行两步操作，线性运算和激活函数的操作
  return softmax(nd.dot(X.reshape(-1, num_inputs), W)+b)

# 交叉熵损失函数
"""
https://zhuanlan.zhihu.com/p/27223959
"""
def cross_entropy(yhat, y):
  # 公式写成这样y[0]*log(yhat[0])+...+y[n]*log(yhat[n])+...
  return -nd.pick(nd.log(yhat), y)

# 计算精度
def accuracy(output, label):
  # 这个操作还不太理解
  # update（2017-09-13）：output.argmax(axis=1)会得到256x1的矩阵，每一行的值为其最大概率值的index值，然后与label对比，1为符合，0为不符合， 进而得到256x1的矩阵，最后nd.mean计算1的个数所占的比例就是精度值
  return nd.mean(output.argmax(axis=1)==label).asscalar()
def evaluate_accuracy(data_iterator, net):
  acc = 0.
  for data, label in data_iterator:
    output = net(data)
    acc += accuracy(output, label)
  return acc / len(data_iterator)


# 训练
import sys
sys.path.append('..')
from utils import SGD
from mxnet import autograd

learning_rate = .01

for epoch in range(5):
    train_loss = 0.
    train_acc = 0.
    for data, label in train_data:
        with autograd.record():
            output = net(data)
            loss = cross_entropy(output, label)
        loss.backward()
        # 将梯度做平均，这样学习率会对batch size不那么敏感
        SGD(params, learning_rate/batch_size)

        train_loss += nd.mean(loss).asscalar()
        train_acc += accuracy(output, label)
	# 每次迭代完测试网络的准确性
    test_acc = evaluate_accuracy(test_data, net)
    print("Epoch %d. Loss: %f, Train acc %f, Test acc %f" % (
        epoch, train_loss/len(train_data), train_acc/len(train_data), test_acc))
    
    
# 预测
import matplotlib.pyplot as plt

def show_images(images):
    n = images.shape[0]
    _, figs = plt.subplots(1, n, figsize=(15, 15))
    for i in range(n):
        figs[i].imshow(images[i].reshape((28, 28)).asnumpy())
        figs[i].axes.get_xaxis().set_visible(False)
        figs[i].axes.get_yaxis().set_visible(False)
    plt.show()

def get_text_labels(label):
    text_labels = [
        't-shirt', 'trouser', 'pullover', 'dress,', 'coat',
        'sandal', 'shirt', 'sneaker', 'bag', 'ankle boot'
    ]
    return [text_labels[int(i)] for i in label]
  
data, label = mnist_test[0:9]
# show_images(data)
print('true labels')
print(get_text_labels(label))

predicted_labels = net(data).argmax(axis=1)
print('predicted labels')
print(get_text_labels(predicted_labels.asnumpy()))
```

注意事项：可能是对于NDArray操作不熟悉，有些函数及方法的具体实现的原理不了解，暂时看不懂，之后需要重新更新