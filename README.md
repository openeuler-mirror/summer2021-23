# Summer2021-No.23 实现RISCV架构下支持qspinlock

#### 介绍
https://gitee.com/openeuler-competition/summer-2021/issues/I3EAV7

#### 软件架构
该仓库基于openeuler内核的`OLK-5.10`分支，通过git安装补丁后可以使原本的版本在RISCV架构下支持`qspinlock`。


#### 安装教程

1.  安装git   
```
sudo apt update
sudo apt install git
```
2.  克隆openeuler kernel
```
git clone https://gitee.com/openeuler/kernel.git
```
3.  克隆本仓库到本地
```
https://gitee.com/openeuler-competition/summer2021-23.git
```   
4.  进入kernel仓库并切换到`OLK-5.10`分支   
```
cd kernel
git checkout remotes/origin/OLK-5.10
```
5.  为kernel打补丁
```
git apply ../summer2021-23/0001-riscv-Convert-custom-spinlock-rwlock-to-generic-qspi.patch --reject
```


#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request
