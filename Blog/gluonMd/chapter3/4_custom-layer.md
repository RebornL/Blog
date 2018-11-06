*time: 2017-09-22*

*author: LQB*

## 设定自定义层

#### 定义一个简单的层

定义一个可以复用的层

```python
from mxnet import nd
from mxnet.gluon import nn

class CenteredLayer(nn.Block):
  def __init__(self, **kwargs):
    super(CenteredLayer, self).__init__(**kwargs)
    
  def forward(self, x):
    return x - x.mean()
  
#简单使用
layer = CenteredLayer()
layer(nn.array([1,2,3,4,5]))
# 输出[-2, -1, 0, 1, 2]

#复杂使用
net = nn.Sequential()
with net.name_scope():
  #定义网路结构
  net.add(nn.Dense(128))
  net.add(nn.Dense(10))
  net.add(CenteredLayer())
  
net.initialize()
y = net(nd.random.uniform(shape=(4, 8)))
y.mean()


```

#### 带模型参数的自定义层

```Python
from mxnet import gluon
# 1.直接创建Parameter
# 定义一个参数名为exciting_parameter_yay，大小为3x3
my_param = gluon.Parameter("exciting_parameter_yay", shape(3, 3))
my_param.initialize()#随机初始化
print(my_param.data())

# 2.通过ParameterDict类型的参数params定义
pd = gluon.ParameterDict(prefix="block1_")
pd.get("exciting_parameter_yay", shape=(3,3))
pd
"""
block1_ (
  Parameter block1_exciting_parameter_yay (shape=(3, 3), dtype=<type 'numpy.float32'>)
)
"""

# 实现Dense层
class MyDense(nn.Block):
  def __init__(self, units, in_units, **kwargs):
    super(MyDense, self).__init__(**kwargs)
    with self.name_scope():
      self.weight = self.params.get("weight", shape=(in_units, units))
      self.bias = self.params.get('bias', shape=(units,)) 
      
  def forward(self, x):
        linear = nd.dot(x, self.weight.data()) + self.bias.data()
        return nd.relu(linear)
      
#使用
net = nn.Sequential()
with net.name_scope():
  net.add(MyDense(32, in_units=64))#指定in_units,避免延迟初始化
  net.add(MyDense(2, in_units=32))
  
net.initialize()
net(nd.random.uniform(shape=(2, 64)))
```



对比Dense的实现，查看Dense是如何实现避免延迟初始化的？？？