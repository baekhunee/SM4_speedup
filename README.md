# 实验名称
SM4 speedup

# 实验简介
1、使用多线程对SM4进行加速；

2、实现T-table，利用T表进行加速；

3、在T表基础上，使用SIMD指令集进一步优化。

# 实验完成人
权周雨 

学号：202000460021 

git账户名称：baekhunee

# 实现T-table

本次实验中SM4根据算法流程一步步实现即可，其中密钥扩展算法与加密算法中均有先查S表再进行移位的操作，使用T-table进行优化。思路即将查表与线性变换合并，将原来4个8进8出的S盒变为32进32出的T表，测试之后发现最佳加速效果为1倍左右。

## 运行截图

![image](https://user-images.githubusercontent.com/105578152/180712097-5615c433-5ac8-4bba-a87b-078df23547d4.png)

# 多线程优化

SM4_multiplethreads.cpp中使用多线程对SM4进行加速优化，并比较单线程，四线程与八线程的运行时间。

## 运行截图（运行1000次）
![image](https://user-images.githubusercontent.com/105578152/180711049-9c7df480-3c1b-4d43-bcae-75cfdd6cd019.png)

# 使用SIMD指令集进行优化

在T表的基础上，使用SIMD指令集进行优化，提高指令的并行性，提升效果较为明显。

## 运行截图（运行1000次）
![image](https://user-images.githubusercontent.com/105578152/180712904-19bd2629-ec35-4a40-a108-d11317c2331b.png)
