# 实验名称
SM4 speedup

# 实验简介
1、使用多线程对SM4进行加速；

2、实现T-table，将查S盒与线性L合并；

3、在T表基础上，使用SIMD指令集进一步优化。

# 实验完成人
权周雨 

学号：202000460021 

git账户名称：baekhunee

# T-table
本次实验中SM4根据算法流程一步步实现即可。SM4的密钥扩展方案与加密过程中均涉及查S盒与线性变换L，使用T表优化即将二者合并，将原来4个8进8出的S盒替换为一个32进32出的新表。

## 运行截图
![image](https://user-images.githubusercontent.com/105578152/180714289-082d841a-fc1a-41b8-8cd4-56492c8932a8.png)
