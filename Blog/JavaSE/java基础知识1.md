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

### Collections补充

#### CopyOnWriteArrayList

内部有两个数组构成，一个用于读，一个用于写，读写分离，互不影响。写操作在复制的数组上，读操作在原始数组上操作。*读操作需要加锁，防止并发写入时导致写入数据丢失*。写操作成功之后会把原始数组指向新的复制数组。

```java
public boolean add(E e) {
    final ReentrantLock lock = this.lock;
    lock.lock();
    try	{
        Object[] elements = getArray();
        int len = elements.length;
        Object[] newElements = Arrays.copyOf(Elements, len+1);
        newElements[len] = e;
        setArray(newElement);
        return true;
    } finally {
        lock.unlock();
    }
}
final void setArray(Object[] a) {
    array = a;
}
```

```java
@SuppressWarnings("unchecked")
private E get(Object[] a, int index) {
    return (E)a[index];
}
```



#### LinkedHashMap

```java
public class LinkedHashMap<K,V> extends HashMap<K,V> implements Map<K,V>
```

继承自HashMap，和HashMap一样具有快速查找特性。内部维护一个双向链表，用来维护插入顺序或LRU顺序，根据accessOrder的值确定，默认为false，维护插入顺序。

```java
/**
 * The head (eldest) of the doubly linked list.
 */
transient LinkedHashMap.Entry<K,V> head;

/**
 * The tail (youngest) of the doubly linked list.
 */
transient LinkedHashMap.Entry<K,V> tail;

final boolean accessOrder;
```

LinkedHashMap有两个重要的操作函数，用于put和get函数中

```java
/**afterNodeAccess函数：
accessorder为true时，当一个节点被访问后，会将该节点移到链表尾部，指定为LRU顺序，保证链表尾部为最近访问的节点。
*/
void afterNodeAccess(Node<K,V> p) { }
/**afterNodeInsertion
put操作之后，若removeEldestEntry()方法返回true时会移出最晚的节点，即链表首部节点。
*/
void afterNodeInsertion(boolean evict) { }
```

关于removeEldestEntry()默认为返回false，若要返回true则需要继承LinkedHashMap重写该方法。这在实现LRU的缓存很常用，移出不常用的缓存。

##### LRU缓存实现

```java
class LRUCache<K, V> extends LinkedHashMap<K, V> {
    private static final int MAX_ENTRIES = 3;//最大缓存空间
    
    protected boolean removeEldestEntry(Map.Entry eldest) {
        return size() > MAX_ENTRIES;
    }
    
    LRUCache() {
        super(MAX_ENTRIES, 0.75f, true);
    }
}
```

