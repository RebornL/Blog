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

### 不规则数组
```java
/*
1
1 1
1 2 1
1 3 3 1
...
*/
//Java可以实现以上的数组
int[][] odds = new int[NMAX+1][]
for(int n = 0; n <= NMAX; n++) {
    odd[n] = new int[n+1];//分配了列数
}
//然后赋值
for(int n = 0; n < odds.length; n++) {
    for(int k = 0; k < odds[k].length; k++) {
        odds[n][k] = lotteryOdds;
    }
}
```