*time: 2017-09-22*

*author: LQB*

## 卷积神经网络----从0开始

卷积神经网络解决两个问题：

- 图片中相近的元素在向量上表示很远，从而在模型上很难体现他们的空间关系
- 大图片的输入，可能会造成模型过大，输入层过大

```python
from mxnet import nd

#输入输出数据格式 batch x channel x height x width
#权重格式 input_filter x output_filter x height x width
w = nd.arange(4).reshape((1,1,2,2))
b = nd.array([1])
data = nd.arange(9).reshape((1,1,3,3))
out = nd.Convolution(data, w, b, kernel=w.shape[2:], num_filter=w.shape[1])
print('input:', data, '\n\nweight:',w,'\n\nbias:',b,'\n\noutput:',out)
print(data.shape, w.shape, out.shape)
#((1L, 1L, 3L, 3L), (1L, 1L, 2L, 2L), (1L, 1L, 2L, 2L))

# 增加stride(跳跃的距离)，pad(补空的大小)
data = nd.arange(9).reshape((1,1,3,3))
out = nd.Convolution(data, w, b, kernel=w.shape[2:], num_filter=w.shape[1], stride=(2,2), pad=(1,1))
print(data.shape, w.shape, out.shape)
#((1L, 1L, 3L, 3L), (1L, 1L, 2L, 2L), (1L, 1L, 2L, 2L))
```

补零做padding， 移动步长stride

```python
data = nd.arange(25).reshape((1,1,5,5))
out = nd.Convolution(data, w, b, kernel=w.shape[2:], num_filter=w.shape[1], stride=(2, 2), pad=(1, 1))
print("input:", data, "\n\nweight:", w, "\n\nbias:", b, "\n\noutput:", output)
print(data.shpae, w.shape, out.shape)
```

示意图：

![img](https://raw.githubusercontent.com/vdumoulin/conv_arithmetic/master/gif/padding_strides.gif)

当输入数据有多个通道的时候，每个通道会有对应的权重，然后会对每个通道做卷积之后在通道之间求和

​				$$conv(data, w, b) = \sum_i conv(data[:,i,:,:], w[0,i,:,:], b)$$

每个通道与对应通道的权重相乘。

```python
w = nd.arange(8).reshape((1, 2, 2, 2))
data = nd.arange(18).reshape((1, 2, 3, 3))

out = nd.Convolution(data, w, b, kernel=w.shape[2:], num_filter=w.shape[0])
print("input:", data, "\n\nweight:", w, "\n\nbias:", b, "\n\noutput:", out)
print(data.shape, w.shape, out.shape)
```



当输入需要多通道时，每个输出通道有对应权重，然后每个通道上做卷积。

​				$$conv(data, w, b)[:,i,:,:] = conv(data, w[i,:,:,:], b[i])$$

```python
w = nd.arange(16).reshape((2, 2, 2, 2))
data = nd.arange(18).reshape((1, 2, 3, 3))
b = nd.array([1, 2])

out = nd.Convolution(data, w, b, kernel=w.shape[2:], num_filter=w.shape[0])
print("input:", data, "\n\nweight:", w, "\n\nbias:", b, "\n\noutput:", out)
```

#### 池化层

池化层可以缓解由于卷积对于位置敏感的问题。每次从一个小窗口取出最大的元素或者取均值作为输出。

```python
data = nd.arange(18).reshape((1,2,3,3))

max_pool = nd.Pooling(data=data, pool_type="max", kernel=(2,2))
ang_pool = nd.Pooling(data=data, pool_type="avg", kernel=(2,2))
print("data:", data, "max pooling:", max_pool, "avg pooling:", avg_pool)
```

### 构建模型

```python
#获取数据
import sys
sys.path.append("..")
from utils import load_data_fashion_mnist
batch_size = 256
train_data, test_data = load_data_fashion_mnist(batch_size)

#定义模型
import mxnet as mx
try:
  	ctx = mx.gpu()
  	_ = nd.zeros((1,), ctx=ctx)
except:
  	ctx = mx.cpu()
  
weight_scale = 0.01
# output channels = 20, kernel = (5,5)
W1 = nd.random_normal(shape=(20, 1, 5, 5), scale=weight_scale, ctx=ctx)
b1 = nd.zeros(W1.shape[0], ctx=ctx)

# output channels = 50, kernel = (3,3)
W2 = nd.random_normal(shape=(50, 20, 3, 3), scale=weight_scale, ctx=ctx)
b2 = nd.zeros(W2.shape[0], ctx=ctx)

# output dim = 128
W3 = nd.random_normal(shape=(1250, 128), scale=weight_scale, ctx=ctx)
b3 = nd.zeros(W3.shape[1], ctx=ctx)

# output dim = 10
W4 = nd.random_normal(shape(W3.shape[1], 10), scale=weight_scale, ctx=ctx)
b4 = nd.zeros(W4.shape[1], ctx=ctx)

params = [W1, b1, W2, b2, W3, b3, W4, b4]
for param in params:
  	param.attach_grad()
  
#卷积模块通常是“卷积层-激活层-池化层”
def net(X, verbose=False):
    X = X.as_in_context(W1.context)
    # 第一层卷积
    h1_conv = nd.Convolution(data=X, weight=W1, bias=b1, kernel=W1.shape[2:], num_filter=W1.shape[0])
    h1_activation = nd.relu(h1_conv)
    h1 = nd.Pooling(data=h1_activation, pool_type="max", kernel=(2, 2), stride=(2, 2))
    # 第二层卷积
    h2_conv = nd.Convolution(data=h1, weight=W2, bias=b2, kernel=W2.shape[2:], num_filter=W2.shape[0])
    h2_activation = nd.relu(h2_conv)
    h2 = nd.Pooling(data=h2_activation, pool_type="max", kernel=(2,2), stride=(2,2))
    h2 = nd.flatten(h2)
    # 第一层全连接
    h3_linear = nd.dot(h2, W3) + b3
    h3 = nd.relu(h3_linear)
    # 第二层全连接
    h4_linear = nd.dot(h3, W4) + b4
	if verbose:
        print('1st conv block:', h1.shape)
        print('2nd conv block:', h2.shape)
        print('1st dense:', h3.shape)
        print('2nd dense:', h4_linear.shape)
        print('output:', h4_linear)
    return h4_linear
  
#训练
from mxnet import autograd as ag
from utils import SGD, accuracy, evaluate_accuracy
from mxnet import gluon

softmax_cross_entropy = gluon.loss.SoftmaxCrossEntropyLoss()

learning_rate = 0.2

epochs = 5
for epoch in range(epochs):
    train_loss = 0.
    train_acc = 0.
    for data, label in train_data:
        label = label.as_in_context(ctx)
        with ag.record():
          	output = net(data)
            loss = softmax_cross_entropy(output, label)
        loss.backward()
        SGD(params, learning_rate/batch_size)
        
        train_loss += nd.mean(loss).asscalar()
        train_acc += accuracy(output, label)
        
    test_acc = evaluate_accuracy(test_data, net, ctx)
    print("Epoch %d. Loss: %f, Train acc %f, Test acc %f" % (
            epoch, train_loss/len(train_data), train_acc/len(train_data), test_acc))
```

