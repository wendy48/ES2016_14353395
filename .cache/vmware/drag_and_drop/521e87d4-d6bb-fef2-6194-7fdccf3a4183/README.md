##Description
The distributed operation layer (DOL) is a software development framework to program parallel applications. The DOL allows to specify applications based on the Kahn process network model of computation and features a simulation engine based on SystemC. Moreover, the DOL provides an XML-based specification format to describe the implementation of a parallel application on a multi-processor systems, including binding and mapping.

##How to install
###1.在ubantu中安装必要的环境
* `$	sudo apt-get update`
* `$	sudo apt-get install ant`
* `$ sudo apt-get install openjdk-7-jdk`
* `$	sudo apt-get install unzip`
###2.下载文件
* `sudo wget http://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.1.tgz`
* `sudo wget http://www.tik.ee.ethz.ch/~shapes/downloads/dol_ethz.zip`
###3.解压文件
* 新建dol的文件夹  `$	mkdir dol`
* 将dolethz.zip解压到 dol文件夹中  `$	unzip dol_ethz.zip -d dol`
* 解压systemc  `$	tar -zxvf systemc-2.3.1.tgz`
###4.编译systemc
* 解压后进入systemc-2.3.1的目录下
      `$	cd systemc-2.3.1`
* 新建一个临时文件夹objdir
      `$	mkdir objdir`
* 进入该文件夹objdir
      `$	cd objdir`
* 运行configure(能根据系统的环境设置一下参数，用于编译)
      `$	../configure CXX=g++ --disable-async-updates`
![image](https://cloud.githubusercontent.com/assets/22701016/19220354/18498242-8e5e-11e6-85bf-650cfcba3094.PNG)
* 编译
 `$	sudo make install`
* 记录当前的工作路径
 `$	pwd`

	![image](https://cloud.githubusercontent.com/assets/22701016/19220352/1846e406-8e5e-11e6-9ffb-485047a3b5cf.PNG)
###5.编译dol  
* 进入刚刚dol的文件夹
`$	cd ../dol`  修改build_zip.xml文件
       
	找到下面这段话，就是说上面编译的systemc位置在哪里，

     `<property name="systemc.inc" value="YYY/include"/>`
     `<property name="systemc.lib" value="YYY/lib-linux/libsystemc.a"/>`

     把YYY改成pwd的结果如下：

    ![image](https://cloud.githubusercontent.com/assets/22701016/19220351/1845cd82-8e5e-11e6-959f-5f68ae254c10.PNG)
* 然后是编译
`$	ant -f build_zip.xml all` 若成功会显示build successful

    ![image](https://cloud.githubusercontent.com/assets/22701016/19220353/18479e6e-8e5e-11e6-86b6-1b99ac465c2d.PNG)

     接着可以试试运行第一个例子

    进入build/bin/mian路径下
`$	cd build/bin/main`

     然后运行第一个例子
`$	ant -f runexample.xml -Dnumber=1` 结果如下：
![image](https://cloud.githubusercontent.com/assets/22701016/19220355/184c3172-8e5e-11e6-98fc-2c54e3b71ac0.PNG)

##Experimental experience
实验中，在安装DOL的过程中由于粗心，在修改build_zip.xml文件的时候把路径写错了，导致运行第一个例子的时候失败了，还好后来根据错误提示找到了原因。使用Git进行版本控制，并将仓库托管在 Github上的过程中，学会使用了markdown这样比较方便美观的编辑器，还学会了如何使用Github创建仓库，并通过远程连接将文件传到远程仓库中，虽然对DOL的整体框架还不是十分了解，但是随着进一步的实验，相信以后会逐渐掌握。