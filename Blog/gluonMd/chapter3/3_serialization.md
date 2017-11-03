*time: 2017-09-22*

*author: LQB*

## 序列化---读写模型

#### 读写NDArray

通过nd自带的save，load操作保存在内存中的参数到文件中和加载存储在磁盘上的参数文件。

```python
from mxnet import nd
# 1.存储参数
x = nd.ones(3)
y = nd.zeros(4)
filename = "test1.params"
nd.save(filename, [x, y])
# 2.加载数据
a, b = nd.load(filename)
print(a, b)

# 另一种方式
filename = "test2.params"
mydict = {"x":x, "y":y}
nd.save(filename, mydict)

c = nd.load(filename)
print(c['x'], c['y'])
```

#### 读写Gluon模型的参数

```python
from mxnet.gluon import nn

def get_net():
  net = nn.Sequential()
  with nn.name_scope():
    net.add(nn.Dense(10, activation='relu'))
    net.add(nn.Dense(2))
    
  return net

net = get_net()
net.initialize()
x = nd.random.uniform(shape=(2, 10))
print(net(x))

# 保存模型参数
filename = 'test3.params'
net.save_params(filename)
c = nd.load(filename)
print(c)
"""
{'dense1_weight': 
[[-0.04774426 -0.02267893 -0.05454748  0.02446532  0.02188614 -0.02559176
  -0.05065439  0.03896836 -0.04247847  0.06293995]
 [-0.01837847  0.02275376  0.04493906 -0.06809997 -0.05640582  0.01719845
   0.04731229  0.02431235 -0.05654623  0.06607229]]
<NDArray 2x10 @cpu(0)>, 
'dense0_weight': 
[[ 0.0408415   0.04370362  0.00404529 -0.0028032   0.00952624 -0.01501013
   0.05958354  0.04705103 -0.06005495 -0.02276454]
 [-0.0578019   0.02074406 -0.06716943 -0.01844618  0.04656678  0.06400172
   0.03894195 -0.05035089  0.0518017   0.05181222]
 [ 0.06700657 -0.00369488  0.0418822   0.0421275  -0.00539289  0.00286685
   0.03927409  0.02504314 -0.05344158  0.03088857]
 [ 0.01958894  0.01148278 -0.04993054  0.00523225  0.06225365  0.03620619
   0.00305876 -0.05517294 -0.01194733 -0.00369594]
 [-0.03296221 -0.04391347  0.03839272  0.03316854 -0.00613896 -0.03968295
   0.00958075 -0.05106945 -0.06736943 -0.02462026]
 [ 0.01646897 -0.04904552  0.0156934  -0.03887501  0.01637076 -0.01589154
   0.06212472  0.05636378  0.02545484 -0.007007  ]
 [-0.0196689   0.01582889 -0.00881553  0.0563288   0.02766836 -0.05610075
  -0.06156844  0.06577327  0.02334734  0.0214396 ]
 [ 0.0238893  -0.04607266 -0.04054644 -0.0198587  -0.05195032  0.03509606
  -0.02584003  0.01509629 -0.01908049 -0.02449339]
 [ 0.00982755 -0.06462044 -0.00859579  0.01879837  0.06837233  0.06425289
  -0.05571373  0.02139065 -0.04075725  0.01890824]
 [-0.04741667  0.06934194  0.02143516  0.01145905 -0.03453917 -0.0119884
  -0.00471649 -0.00354235 -0.03578042  0.01729142]]
<NDArray 10x10 @cpu(0)>, 
'dense1_bias': 
[ 0.  0.]
<NDArray 2 @cpu(0)>, 
'dense0_bias': 
[ 0.  0.  0.  0.  0.  0.  0.  0.  0.  0.]
<NDArray 10 @cpu(0)>}
"""

# 使用保存在硬盘中的参数
import mxnet as mx
net2 = get_net()
net2.load_params(filename, mx.cpu())
print(net(x))
```
