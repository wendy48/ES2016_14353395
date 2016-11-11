## 一、死锁截图
* 程序设置会运行100次，但是在运行到第77次的时候产生了死锁，截图如下：
![enter description here][1]
## 二、产生死锁的解释
* 由以下代码可以看到，有一个子线程t，当调用t.start()的时候，线程t就被加入了调度队列中，而主线程需要在`while (count-->0)`后才能开始调用`a.methodA(b)`方法，即此时已经消耗了一定的时间片了，接着往下看看类A中的methodA()方法和类B中的methodB()方法在干什么
```
    class Deadlock implements Runnable{
    	A a=new A();
    	B b=new B();
    
    	Deadlock(){
    		Thread t=new Thread(this);
    		int count=20000;
    
    		t.start();
    		while (count-->0);
    		a.methodA(b);
    	}
    
    	public void run(){
    		b.methodB(a);
    	}
    
    	public static void main(String args[]){
    		new Deadlock();
    	}
    }
```
* 可以看到，主线程执行a对象的`methodA(b)`方法，因为之前已经说到主线程的时间片被`while (count-->0)`消耗掉了一部分，所以很有可能当主线程时间片用完时还没有执行完，此时t线程开始运行，进入run函数中执行`b.methodB(a)`方法，但是此时因为关键字synchronized，a.last()无法执行，需要等待主线程执行完`b.last()`才可以，因此线程t被阻塞，然后轮到主线程继续执行，但是同样道理，因为b对象的`methodB()`方法被线程t占用，导致无法执行b.last()方法，线程b被阻塞，就这样，两个线程相互阻塞形成了死锁
```
    class A{
    	synchronized void methodA(B b){
    		b.last();
    	}
    	synchronized void last(){
    		System.out.println("Inside A.last()");
    	}
    }
    
    class B{
    	synchronized void methodB(A a){
    		a.last();
    	}
    	synchronized void last(){
    		System.out.println("Inside B.last()");
    	}
    }
```
## 三、死锁产生的四个必要条件
* 1.互斥
至少有一个资源一次仅能被一个进程使用，如果有另一个进程申请该资源，则必须等到该资源被释放才能使用。
* 2.占有并等待
一个进程必须占有一个资源，并且在等待另一个资源，且所等待的资源被另一进程所占用。
* 3.非抢占
资源无法被抢占，只能等到进程完成任务后自动释放。
* 4.循环等待
有一组等待资源的进程{P0,P1,...,Pn}，P0等待的资源被P1占有，P1等待的资源被P2占有，...，Pn等待的资源被P0占有，形成死循环。
## 四、实验感想
这次试验主要是对死锁产生的理解，其实以前在操作系统的课上我们就已经对死锁的产生有了理解并且通过不同的方式去进行解决，不过有一些知识点也有遗忘了，比如这次让我们写出产生死锁的四个条件，我就一直想不起来非抢占这一重要条件，所以这次试验帮助我们巩固了死锁的知识还是挺好的，毕竟这是很重要的一个知识点。


  [1]: ./images/%E5%81%9C%E6%AD%A2.PNG "停止.PNG"
  [2]: ./images/1478847063163.jpg "1478847063163.jpg"