# java基础知识1

## java新的基础知识
### for each语句
```java
char[] s = {'a', 'b', 'c'};
for(char ele: s) {
    System.out.println(ele);
}
```

### 数组拷贝
将一个数组变量拷贝给另一个数组变量，这两个变量指向同一个数组。
若想要将一个数组的所有值拷贝到新的数组中去，可以采用Array的copyOf方法。
```java
int[] luckyNumbers = smallPrimes;
luckyNumbers[5] = 12;

int[] copiedLuckyNumbers = Array.copyOf(luckyNumbers, luckyNumbers.length);
```