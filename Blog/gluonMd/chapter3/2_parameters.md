*time: 2017-09-15*

*author: LQB*

## 参数初始化

#### 初始化模型参数

```python
net.initialize()#默认随机初始化参数，之后才可以进行运算
net(x)
```

#### 访问参数

```python
# 第一种方式，按层来访问每一层的权重weight和偏置bias,得到Parameter类，通过data()方法获取值，grad()方法获取梯度
w = net[0].weight
b = net[0].bias
print("name: ", net[0].name, "weight: ", w, "bias: ", b)
print("weight value: ", w.data())
print("weight gradtitude: ", w.grad())
print("bias value: ", b.data())
print("bias gradtitude: ", b.grad())

# 第二种方式，使用collect_params()获取所有层的参数
params = net.collect_params()
print(params)
for param in params:
  print(param, params[param], params[param].data())
```

#### 不同初始化函数进行初始化

```python
from mxnet import init
params.initialize(init=init.Normal(sigma=0.02), force_reinit=True)
print(net[0].weight.data())

# 自定义初始化方法
class MyInit(init.Initializer):
  def __init__(self):
  	super(Myinit, self).__init()
  	self._verbose = True
    
  def _init_weight(self, _, arr):
    # 初始化权重，使用out=arr后我们不需指定形状
    print("init weight", arr.shape)
    nd.random.uniform(low=5, high=10, out=arr)
    
  def _init_bias(self, _, arr):
    print("init bias", arr.shape)
    # 初始化偏移
    arr[:] = 2
    
params.initialize(init=MyInit(), force_init=True)
print(net[0].weight.data(), net[0].bias.data())
"""
# 这里可以发现_init_bias不起作用
init weight (4, 5)
init weight (2, 4)

[[ 6.11160707  8.08467007  6.93244457  9.71874046  9.51299286]
 [ 8.40910149  7.24975014  6.79753971  8.06531715  7.18515968]
 [ 9.51174355  8.48815536  5.49640179  5.30112743  9.8490448 ]
 [ 8.33383369  8.26570034  8.35318947  5.85454798  6.05191278]]
<NDArray 4x5 @cpu(0)>
[ 0.  0.  0.  0.]
<NDArray 4 @cpu(0)>
"""
  
```

#### 延后初始化

延后初始化，在于初始化参数的时候，由于不知道输入数据大小，不知道其形状大小，因此无法输出参数的shape值。

```python
net = get_net()
print(net.collect_params())
net.initialize(init=MyInit())
"""
这个不会有输入值
"""
net(x)
#此时就会有输出打印
"""
('init weight', (4L, 5L))
('init weight', (2L, 4L))
Out[24]:

[[ 714.38696289  674.64227295]
 [ 702.47253418  664.65948486]
 [ 563.41418457  533.46936035]]
<NDArray 3x2 @cpu(0)>
"""
print(net.collect_params())
```

#### 避免延后初始化

创建网络的时候，设定输入大小

```python
net = nn.Sequential()
with net.name_scope():
  net.add(nn.Dense(4, in_units=5, activation='relu'))
  net.add(nn.Dense(2, in_units=4))
  
net.initialize(MyInit())
```

#### 共享参数

指定params参数

```python
net = nn.Sequential()
with net.name_scope():
    net.add(nn.Dense(4, in_units=4, activation="relu"))
    net.add(nn.Dense(4, in_units=4, activation="relu", params=net[-1].params))
    net.add(nn.Dense(2, in_units=4))
    
net.initialize(MyInit())
print(net[0].weight.data())
print(net[1].weight.data())
```

