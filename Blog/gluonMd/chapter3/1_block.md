*time: 2017-09-14*

*author: LQB*

## 创建神经网络

nn.Block可以简单理解为构造神经网络的部件，可以通过集成它构造所需要的网络。

它的三个作用：

1. 存储参数
2. 描述forward()怎么运行
3. 自动求导

##### 简单使用

```python
class MLP(nn.Block):
  	def __init__(self, **kwargs):
      super(MLP, self).__init__(**kwargs)#提供了prefix（指定名字）和params（指定模型参数）两个参数
      with self.name_scope():
        # 定义两层网络
        self.dense0 = nn.Dense(256)
        self.dense1 = nn.Dense(10)
        
	def forward(self, x):
      return self.dense1(nd.relu(self.dense0(x)))
    
# 使用
net = MLP()
print(net)
"""
MLP(
  (dense1): Dense(10, linear)
  (dense0): Dense(256, linear)
)
"""
x = nd.random.uniform(shape=(4, 20))
y = net(x)

print('default prefix:', net.dense0.name)
# ('default prefix:', 'mlp0_dense0')
net3 = MLP(prefix='another_mlp_')
print('customized prefix:', net3.dense0.name)
# ('customized prefix:', 'another_mlp_dense0')
```

#### Sequential简单实现

```python
class Sequential(nn.Block):
  def __init__(self, **kwargs):
    super(Sequential, self).__init__(**kwargs)
    
  def add(self, block):
    # 添加层数
    self._childer.append(block)
    
  def forward(self, x):
    for block in self._children:
      x = block(x)
    return x
  
# 使用
net4 = Sequential()
with net4.name_scope():
  net4.add(nn.Dense(256, activation='relu'))
  net4.add(nn.Dense(10))
  
print(net4)
```

nn.Block定义网络更为灵活

```python
class FancyMLP(nn.Block):
  def __init__(self, **kwargs):
    super(FancyMLP, self).__init__(**kwargs)
    with self.name_scope():
      self.dense = nn.Dense(256)#定义了一层
      self.weight = nn.random_uniform(shape=(256, 20))#手动初始化和创建了权重weight
      
  def forward(self, x):
    x = nd.relu(self.dense(x))
    x = nd.relu(nd.dot(x, self.weight) + 1)
    x = nd.relu(self.dense(x))
    return x
```

#### nn.Block和nn.Sequential嵌套使用

```python
class RecMLP(nn.Block):
    def __init__(self, **kwargs):
        super(RecMLP, self).__init__(**kwargs)
        self.net = nn.Sequential()
        with self.name_scope():
            self.net.add(nn.Dense(256, activation="relu"))
            self.net.add(nn.Dense(128, activation="relu"))
            self.dense = nn.Dense(64)
            
    def forward(self, x):
        return nd.relu(self.dense(self.net(x)))

rec_mlp = nn.Sequential()
rec_mlp.add(RecMLP())
rec_mlp.add(nn.Dense(10))
print(rec_mlp)
"""
Sequential(
  (0): RecMLP(
    (dense): Dense(64, linear)
    (net): Sequential(
      (0): Dense(256, Activation(relu))
      (1): Dense(128, Activation(relu))
    )
  )
  (1): Dense(10, linear)
)
"""
```

