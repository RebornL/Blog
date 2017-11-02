*time: 2017-10-13*

*author: LQB*

## 图像增强

在数据不足的情况下，可以通过采用图像增强的技术扩增图片的数量。

```python
%matplotlib inline
import matplotlib.pyplot as plt
from mxnet import image
# 图像展示
img = image.imdecode(open("../img/cat1.jpg", "rb").read())
plt.imshow(img.asnumpy())

# 定义一个辅助函数
def apply(img, aug, n=3):
    _, figs = plt.subplot(n, n, figsize=(8, 8))
    for i in range(n):
        for j in range(n):
            """
            1.将数据转化成float类型进行操作
            2.copy操作，有些aug操作是直接通过改写输入获取性能的提升
            """
            x = img.astype("float32")
            y = aug(x).clip(0, 254)#确保所有的值在0-254之间
            # 显示浮点图像的时候，imshow要求输入在[0, 1]之间
            figs[i][j].imshow(y.asnumpy()/255.0)
            figs[i][j].axes.get_xaxis().set_visible(False)
            figs[i][j].axes.get_yaxis().set_visible(False)
```

数据增强的操作有：水平翻转，颜色变化，裁剪等等

```python
# 水平翻转
aug = image.HorizontalFlipAug(0.5)
apply(img, aug)

# 随机裁剪
aug = image.RandomCropAug([200, 200])

# 随机裁剪，要求保留至少0.1的区域，随机长宽比在.5和2之间。
# 最后将结果resize到200x200
aug = image.RandomSizedCropAug((200, 200), .1, (.5, 2))

# 颜色变化
aug = image.BrightnessJitterAug(.5)
aug = image.HueJitterAug(.5)
```

