# Summer2021-No.23 实现RISCV架构下支持qspinlock

#### Description
https://gitee.com/openeuler-competition/summer-2021/issues/I3EAV7

#### Software Architecture
This repository bases on the `OLK-5.10` branch of openeuler kernel. Applying the patch to the original version can make it support `qspinlock` in arch RISCV.

#### Installation

1.  Install git   
```
sudo apt update
sudo apt install git
```
2.  Clone openeuler kernel   
```
git clone https://gitee.com/openeuler/kernel.git
```
3.  Clone this repository   
```
https://gitee.com/openeuler-competition/summer2021-23.git
```
4.  Enter the kernel repository and switch to `OLK-5.10` branch
```
cd kernel
git checkout remotes/origin/OLK-5.10
```
5.  Apply the patch for the kernel   
```
git apply ../summer2021-23/0001-riscv-Convert-custom-spinlock-rwlock-to-generic-qspi.patch --reject
```


#### Contribution

1.  Fork the repository
2.  Create Feat_xxx branch
3.  Commit your code
4.  Create Pull Request


