# __测试用例__  
该目录为使用者提供了基础的测试用例，用以测试qspinlock的功能与性能。   
## get_started   
该例提供了基础的spinlock功能的测试模块。   
__测试方法__   
* 在Makefile文件中修改交叉编译器和已经编译好的kernel目录信息。   
* 编译   
    ```
    cd get_started
    make
    ```   
* 将编译好的`demo.ko`和`app`文件加入内核对应的文件系统中   
    ```
    sudo cp demo.ko ${ROOTFSPATH}/home
    sudo cp demo.ko ${ROOTFSPATH}/home
    ```   
* 启动测试设备（如qemu）并安装模块   
    ```
    cd ${ROOTFSPATH}/home
    insmod demo.ko
    ```   
    此时应当在shell中显示信息   
    ```
    [ 1812.212765] demo: loading out-of-tree module taints kernel.
    [ 1812.219582] demo_init -- 71.
    ```   
* 测试过程   
    在后台运行测试程序   
    ```
    ./app&
    ```
    会得到输出   
    ```
    [ 1906.095760] random: crng init done
    [ 1906.136918] demo_open -- 36.
    ```
    此时该进程获取spinlock并于5s后释放   
    在spinlock被获取的5s内再次运行测试程序   
    ```
    ./app
    ```   
    则此时会得到以下信息   
    ```
    open: Device or resource busy
    ```  
    表明设备正忙，在5s后spinlock被释放，测试程序可以再次正常运行，说明spinlock功能正常。