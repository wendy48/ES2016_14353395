##一、修改完成的.dot截图
###1.example1
* .dot截图
![image](https://github.com/wendy48/ES2016_14353395/blob/master/image/dol1.PNG?raw=true)
* .dot截图
![dol2][6]
##二、关于修改的解释
###1.example1
* 实验要求输出三次方数，而原本的程序中对输入端信号i进行的是平方处理，然后写到输出端，因此我们只需要将平方处理改成三次方即可
* 修改src文件夹下的square.c文件，在原本定义平方进程的函数square_fire中将i的乘方改为i的立方
![enter description here][1]
* 运行结果如下，每一行输出结果分别为0~19的整数的三次方
![enter description here][2]
###2.example2
* 实验要求将square模块修改为两个，原本程序中的架构中包含了三个square模块，这是因为对square模块进行了迭代，而且迭代次数定义为3，因此我们只需要将迭代次数修改为2即可
* 修改example2.xml文件，将定义模块迭代数的value值更改为2
![enter description here][4]
* 运行结果如下，每一行输出结果分别为0~19经过两次平方操作的结果，也就是四次方
![enter description here][5]

##三、实验感想
这次的实验主要是对dol实例的分析和修改，其实修改的内容很容易理解也很简单，关键的部分在于要理解整个程序的架构及为什么能够得到这样的输出结果，而且在这次的实验中了解到了dot图，觉得能够很清晰直观的描述出整个程序的运行架构和过程。


  [1]: https://github.com/wendy48/ES2016_14353395/blob/master/image/dol1.PNG?raw=true
  [2]: https://github.com/wendy48/ES2016_14353395/blob/master/image/%E6%8D%95%E8%8E%B71.PNG?raw=true
  [3]: https://github.com/wendy48/ES2016_14353395/blob/master/image/dol1.PNG?raw=true
  [4]: https://github.com/wendy48/ES2016_14353395/blob/master/image/ex2.PNG?raw=true
  [5]: https://github.com/wendy48/ES2016_14353395/blob/master/image/%E6%8D%95%E8%8E%B7.PNG?raw=true
  [6]: https://github.com/wendy48/ES2016_14353395/blob/master/image/dol2.PNG?raw=true
