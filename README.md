# Bookstore大作业开发文档
程序主要功能系统：```账户系统```、```图书系统```、```日志系统```、```工具系统```
包括
* `main.cpp`
* ```account.h``` ,```account.cpp```
* ```login.h``` ,```login.cpp```
* ```book.h```,```book.cpp```
* ```log.h``` ,```log.cpp```
* ```blockchain.h```(模板类，实现和声明写在同一文件内)
* ```error.h```,```error.cpp```
* ```transfer.h```,`transfer.cpp`

需要存储文件
* ```account_node```,`account_info`
* ```book_ISBN_node```,```book_ISBN_info```
* ```book_name_node```,```book_name_info```
* ```book_author_node```,```book_author_info```
* ```book_keyword_node```,```book_keyword_info```
* ```log```,```finance```

## 账户系统
包括```account.h``` ,```account.cpp```,```login.h``` ,```login.cpp```：

* ```account.h``` 用于实现账户系统的存储、指令操作。
接口:
```cpp
    //构造函数
    account(logShow *logShowPoint_);

//---------------------------------------------------------------

    //su函数

    //登录帐户
    //使登录帐户变为已登录状态，当前帐户变为该帐户
    //如果该帐户不存在则操作失败
    //如果密码错误则操作失败
    //如果当前帐户权限等级高于登录帐户则可以省略密码
    //最低权限要求{0}

    //密码未忽略的情况
    void su(const char *userID, const char *password);

    //密码忽略的情况
    void su(const char *userID);

//---------------------------------------------------------------

    //logout函数

    //注销帐户
    //撤销最后一次成功执行的 su 指令效果
    //如无已登录帐户则操作失败
    //最低权限要求{1}
    void logout();

//---------------------------------------------------------------

    //registering函数

    //注册帐户
    //注册信息为[UserID] [Password] [Username]、权限等级为 {1} 的帐户。
    //如果 [UserID] 与已注册帐户重复则操作失败。
    //最低权限要求{0}
    void registering(const char *userID, const char *password, const char *username);

//---------------------------------------------------------------

    //passwd函数

    //修改密码
    //修改指定帐户的密码
    //如果该帐户不存在则操作失败
    //如果密码错误则操作失败
    //如果当前帐户权限等级为 {7} 则可以省略 [CurrentPassword]
    //最低权限要求{1}

    //CurrentPassword未省略的情况
    void passwd(const char *userID, const char *currentPassword, const char *newPassword);

    //当前帐户权限等级为 {7} ，可以省略 [CurrentPassword]
    void passwd(const char *userID, const char *newPassword);

//---------------------------------------------------------------

    //useradd函数

    //创建帐户
    //如果待创建帐户的权限等级大于等于当前帐户权限等级则操作失败
    //如果 [UserID] 与已注册帐户重复则操作失败
    //最低权限要求{3}
    void useradd(const char *userID, const char *password, int privilege, const char *userName);

//---------------------------------------------------------------

    //deleting函数

    //删除帐户
    //如果待删除帐户不存在则操作失败
    //如果待删除帐户已登录则操作失败
    //最低权限要求{7}
    void deleting(const char *userID);

//---------------------------------------------------------------

    //getLogin函数

    //返回指向本类中的登录栈的指针
    login *getLogin() { return &loginStack; }
```

* ```login.h``` 用于实现登录栈，定义了登录栈类。能够推入```（push）```、弹出```（pop）```栈顶的账户，能够清空```（clear）```登录栈的内容。具体通过链表来实现。
接口：
```cpp 
    //构造函数
    login();

    //析构函数
    ~login();

    //clear函数
    //清空所有登录账户
    void clear();

    //empty函数
    //若无登陆账户，返回true;反之，返回false
    bool empty();

    //push函数
    //将新登录账户推至栈顶
    void push(const users &data_);

    //pop函数
    //弹出栈顶函数
    //若栈是空的，抛出错误
    void pop();

    //front函数
    //返回栈顶的节点
    //若栈是空的，抛出错误
    nodeLog *front();

    //find函数
    //若目标账户已登录，返回true;反之，返回false
    bool find(const char *userID);

    //select函数
    //用于在添加选择
    //若栈空，抛出错误
    void select(const char *ISBN);

    //showSelect函数
    //返回目前栈顶账户选择的ISBN
    //若未选择或栈空，抛出错误
    std::string showSelect();

    //modifySelect函数
    //用于选择后对selectListISBN和节点的selectISBN进行更新
    //若未选择或栈空，抛出错误
    void modifySelect(const char *ISBN);
```

存储方式：
* 利用**二进制文件**来存储所有账户的```ID```、```密码```、```用户名```和```权限```。文件的内部使用**块状链表**来作为存储的具体实现。需要两个文件```account_node```和```account_info```来存储信息，前者存放双向链表的节点，后者存放账户的具体信息。
```account_node```中，所有图书的信息按其```ID```值升序排列，每个节点中存有节点中对应那部分账户的信息块在```account_info```中的开始位置、前后节点在```account_node```的开始位置和该节点下账户的```ID```值的范围。每个节点在二进制文件中所占用的字节数应当相同（均为最长情况，若有空余位则用是0补齐），以便于读取。
```account_info```中，每个链表节点下按```ID```值从小到大顺序存储账户的各个信息。每个账户各个信息在二进制文件中所占用的字节数应当相同（均为最长情况，若有空余位则用是0补齐），以方便文件定位指针快速定位所需账户信息。每个节点的的所有数据的最大总和应为```4KB```（若有少量多出则用0补齐）来提高文件读写效率，并据此来确定每个节点的最大容量。每个节点的最小容量取为最大容量的一半。

 指令：```登录帐户```、```删除帐户```、```注册帐户```、```修改密码```、```创建帐户```、```删除帐户```。
* 登录帐户 
  * 调用```account.h```中的```su```函数来登录账户，先通过块状链表的检索找到账户，再将该账户添加到登录栈中。
* 注销帐户
  * 调用```account.h```中的```logout```函数，弹出栈顶的账户。
* 注册帐户
  * 调用```account.h```中的```registering```函数来注册账户，将账户信息通过块状链表的插入添加到相应文件中。
* 修改密码 
  * 调用```account.h```中的```passwd```函数来修改密码，先进行检索，然后修改相应信息，维护块状链表。
* 创建帐户
  * 调用```account.h```中的```useradd```函数来创建账户，与注册类似，只是权限信息不同
* 删除帐户
  * 调用```account.h```中的```deleting```函数来对删除账户，先进行检索，然后将该账户信息从文件中通过块状链表的删除删去。

## 图书系统
包括```book.h``` ,```book.cpp```：

* ```book.h``` 用于实现图书系统的指令操作：
接口:
```cpp
    //构造函数
    account(logShow *logShowPoint_);

//---------------------------------------------------------------

    //su函数

    //登录帐户
    //使登录帐户变为已登录状态，当前帐户变为该帐户
    //如果该帐户不存在则操作失败
    //如果密码错误则操作失败
    //如果当前帐户权限等级高于登录帐户则可以省略密码
    //最低权限要求{0}

    //密码未忽略的情况
    void su(const char *userID, const char *password);

    //密码忽略的情况
    void su(const char *userID);

//---------------------------------------------------------------

    //logout函数

    //注销帐户
    //撤销最后一次成功执行的 su 指令效果
    //如无已登录帐户则操作失败
    //最低权限要求{1}
    void logout();

//---------------------------------------------------------------

    //registering函数

    //注册帐户
    //注册信息为[UserID] [Password] [Username]、权限等级为 {1} 的帐户。
    //如果 [UserID] 与已注册帐户重复则操作失败。
    //最低权限要求{0}
    void registering(const char *userID, const char *password, const char *username);

//---------------------------------------------------------------

    //passwd函数

    //修改密码
    //修改指定帐户的密码
    //如果该帐户不存在则操作失败
    //如果密码错误则操作失败
    //如果当前帐户权限等级为 {7} 则可以省略 [CurrentPassword]
    //最低权限要求{1}

    //CurrentPassword未省略的情况
    void passwd(const char *userID, const char *currentPassword, const char *newPassword);

    //当前帐户权限等级为 {7} ，可以省略 [CurrentPassword]
    void passwd(const char *userID, const char *newPassword);

//---------------------------------------------------------------

    //useradd函数

    //创建帐户
    //如果待创建帐户的权限等级大于等于当前帐户权限等级则操作失败
    //如果 [UserID] 与已注册帐户重复则操作失败
    //最低权限要求{3}
    void useradd(const char *userID, const char *password, int privilege, const char *userName);

//---------------------------------------------------------------

    //deleting函数

    //删除帐户
    //如果待删除帐户不存在则操作失败
    //如果待删除帐户已登录则操作失败
    //最低权限要求{7}
    void deleting(const char *userID);

//---------------------------------------------------------------

    //getLogin函数

    //返回指向本类中的登录栈的指针
    login *getLogin() { return &loginStack; }
```

存储方式：

* 利用**二进制文件**来存储所有图书的```ISBN```、```书名```、```作者```、```关键词```、```数量单价```和```交易总额```。文件的内部使用**块状链表**来作为存储的具体实现。需要两个文件```book_ISBN_node```和```book_ISBN_info```来存储信息，前者存放双向链表的节点，后者存放图书的具体信息。
```book_ISBN_node```中，所有图书的信息按其```ISBN```值升序排列，每个节点中存有节点中对应那部分图书的信息块在```book_ISBN_info```中的开始位置、前后节点在```book_ISBN_node```的开始位置和该节点下图书的```ISBN```值的范围。每个节点在二进制文件中所占用的字节数应当相同（均为最长情况，若有空余位则用是0补齐），以便于读取。
```book_ISBN_info```中，每个链表节点下按```ISBN```值从小到大顺序存储图书的各个信息。每本书各个信息在二进制文件中所占用的字节数应当相同（均为最长情况，若有空余位则用是0补齐），以方便文件定位指针快速定位所需图书信息。每个节点的的所有数据的最大总和应为```4KB```（若有少量多出则用0补齐）来提高文件读写效率，并据此来确定每个节点的最大容量。每个节点的最小容量取为最大容量的一半。
类似的，分别再以书名、作者名、关键词为索引，建立储存文档```book_name_node```、```book_name_info```、```book_author_node```、```book_author_info```、```book_keyword_node```、```book_keyword_info```，以便于不同索引类型下的检索等操作。


指令：```检索图书```、```购买图书```、```选择图书```、```修改图书信息```、```图书进货```
* 检索图书 
  * 调用```book.h```中的```show```函数来对图书进行检索，具体通过块状链表的检索(先通过索引值确定目标所在节点，再在节点内进行检索)来实现，只读文件内容。
* 购买图书
  * 调用```book.h```中的```buy```函数来对图书进行购买，先进行检索，然后修改文件中对应的图书数量。若卖完，则删除该图书有关信息。
* 选择图书
  * 调用```book.h```中的```select```函数来对图书进行选择，只读文件内容，先进行检索，然后将选择的图书记录进对应账户对象的登录栈```（login.h）```的相关内容中。
* 修改图书信息 
  * 调用```book.h```中的```modify```函数来修改图书信息，先进行检索，然后修改相应信息，维护块状链表。
* 图书进货
  * 调用```book.h```中的```import```函数来进货，利用块状链表的插入等操作进行维护。
 
## 日志系统
包括```log.h``` ,```log.cpp```：
* ```log.h```用于读写日志记录。每次调用相关接口函数时，利用```log.h```将相应信息代表的01串写入相应文件中。
接口：
```cpp
class log://与交易记录有关
    
   //构造函数
    log(login *loginPoint_, logShow *logShowPoint_);

    //析构函数
    ~log();
    
//-----------------------------------------------

    //show函数

    //输出所有交易之总额
    //最低权限要求 {7}
    void show();

    //输出后count_笔交易之总额
    //最低权限要求 {7}
    void show(int count_);

//-----------------------------------------------

    //add函数

    //储存一笔收入/支出记录，添加到finance文件中
    //flag为true表收入，false表支出
    void add(bool flag, long add);
```
```cpp
class logShow://与日志有关

//构造函数 
    logShow();

    //析构函数
    ~logShow();

//---------------------------------------------------------------------

    //storeLog函数

    //将string类信息存储到log文件中
    void storeLog(std::string info);

    //将char类信息存储到log文件中
    void storeLog(const char *info);

//---------------------------------------------------------------------

    //show函数
    //打印日志
    void show();
```

存储方式：
* 利用两个**二进制文件**```log```,```finance```来存储相关信息。
对```log```，存放所有操作的信息记录。
对```finance```，存放有关所有交易的信息记录。对有交易的信息，只记录每次交易的收入和支出。

指令：```财务记录查询```、```生成日志```

* 财务记录查询：
  * 调用```log.h```中的`log`类的```show```函数来查询财务记录。通过读取```finance```文件的信息，输出相应内容。
* 生成日志：
  * 调用```log.h```中的`logShow`类的```show```函数来生成日志。通过读取```log```文件的信息打印日志。

## 工具系统

包括```blockchain.h```,```error.h```,```error.cpp```,```transfer.h```,`transfer.cpp`：
* ```blockchain.h```用于处理文件的块状链表结构，用模板类实现。主要包括检索（`find`）、插入（`insert`）、删除（`erase`）、修改（`change`）、清空（`clear`）等接口函数。
* ```error.h```用于接受并处理抛出的错误。
* `transfer.h`用于读入命令并切片，以及识别实现字符串、数字等之间的相互转换。
接口：
```cpp
//blockchain.h:

    //构造函数

    blockchain(std::string fileNodeName_, std::string fileInfoName_,
               size_t sizeIndex_, size_t numInfo_);

//------------------------------------------------------------------

    //析构函数
    //功能：由于删除时会在文件中留下无效块，析构时需删掉这些无效块，将文件重新储存
    ~blockchain();

//------------------------------------------------------------------

    //find函数：

    //功能：利用索引值 或 索引值和具体的值 查找目标是否存在（查找）
    //     同时，还兼有寻找某条要插入信息的插入位置的作用（定位）
    //若找到符合要求的，返回true; 反之，返回false
    //不论如何，都找到要插入这条信息的位置，使节点文件指针指向该节点开头
    //sto表示要信息若要插入，插入信息块已储存信息条数（0-based）
    //pos表示信息若要插入，应插在第pos（0-based）条前面

    //功能：利用索引值查找目标是否存在，并寻找某条要插入信息的插入位置
    //参数：字符串类的索引值，引用整数类的已储存信息条数和要插入位置
    bool find(const char *index, int &sto, int &pos);

    //功能：利用索引值和具体的值查找目标是否存在，并寻找某条要插入信息的插入位置的作用
    //参数：字符串类的索引值，引用整数类的已储存信息条数和要插入位置
    bool find(const char *index, const char *value, int &sto, int &pos);

    //功能：利用索引值和具体的值查找目标是否存在
    //参数：字符串类的索引值
    bool find(const char *index);

    //功能：利用索引值和具体的值查找目标是否存在
    //参数：字符串类的索引值
    bool find(const char *index, const char *value);

//------------------------------------------------------------------

    //print函数：

    //功能：按具体信息值从小到大输出所有索引值为index的信息
    void print(const char *index, void(*printFunc)(const everyInfo &));

    //功能：输出所有储存的信息
    void print(void(*printFunc)(const everyInfo &));

//------------------------------------------------------------------

    //get函数：

    //功能：按信息所在位置pos返回当前所在信息
    //需要现利用find函数，确认所get的信息存在以及定位
    //参数：信息所在位置pos
    everyInfo get(int pos);

//------------------------------------------------------------------

    //modify函数：

    //功能：修改当前位置pos处的信息
    //需要现利用find函数，确认所modify的信息存在以及定位
    //用于不改变索引值和排列顺序的修改，即不会改变顺序、节点上下限
    //参数：信息所在位置pos，修改后的新的信息newInfo，指向修改用的函数的指针modifyFunc
    void modify(int pos, const everyInfo &newInfo,
                void (*modifyFunc)(everyInfo &, const everyInfo &));

//------------------------------------------------------------------

    //insert函数:

    //功能：根据索引值 或 索引值和具体信息值 插入信息
    //插入后，将节点文件指针、信息文件指针指向开头
    //索引值可以重复，但同一索引下，信息具体值不可重复
    //若插入成功，返回true;反之（已有重复信息），返回false

    //功能：根据索引值插入信息
    //参数：字符串类的索引值，具体信息值，要插入的信息，指向插入信息函数的函数指针
    bool insert(const char *index, const everyInfo &toInsert,
                void (*insertFunc)(everyInfo &, const everyInfo &));

    //功能：根据索引值和具体信息值插入信息
    //参数：字符串类的索引值，具体信息值，要插入的信息，指向插入信息函数的函数指针
    bool insert(const char *index, const char *value, const everyInfo &toInsert,
                void (*insertFunc)(everyInfo &, const everyInfo &));

//------------------------------------------------------------------

    //erase函数：

    //功能：利用索引值（适用于索引值不会重复、具体信息值未知的情况）或
    //     利用索引值和具体信息值（适用于索引值会重复、具体信息值已知的情况）删除信息
    //执行后，将节点文件指针、信息文件指针指向开头
    //若删除成功，返回true; 反之，删除失败（没有对应目标），返回false

    //功能：利用索引值（适用于索引值不会重复、具体信息值未知的情况）删除信息
    //参数：字符串类的索引值
    bool erase(const char *index);

    //功能：利用索引值和具体信息值（适用于索引值会重复、具体信息值已知的情况）删除信息
    //参数：字符串类的索引值，具体信息值
    bool erase(const char *index, const char *value);

//------------------------------------------------------------------

    //empty函数
    //功能：用于判断文件是否为空
    //若空，返回true; 反之，返回false
    bool empty();

//------------------------------------------------------------------

    //insertFind函数
    //功能：用于在已找到的位置的信息中（调用前需定位好）插入信息，在insert函数中使用
    //参数：目标信息块储存信息条数（0-based），目标信息块位置（0-based），
    //     要插入的信息，指向插入信息函数的函数指针
    void insertFind(int sto, int pos, const everyInfo &toInsert,
                    void (*insertFunc)(everyInfo &, const everyInfo &));

//------------------------------------------------------------------

    //eraseDelete函数
    //功能：用于删除已经找到位置的信息（调用前需定位好），在erase函数中使用
    //参数：整数类的要删去信息的块的储存信息条数sto，整数类的要删去信息的位置pos
    void eraseDelete(int sto, int pos);
```
```cpp
//error.h:

    //抛出错误
     void error(std::string message);
```
```cpp
//translate.h

//去除字符串两端的引号
//若字符串两端无引号，报错
//若字符串只有两引号，报错
//若字符串除了两端外还有引号，报错
void popQuotations(std::string &token);

//找到第一个等号，并以此为界将字符串分成两半
//若无等号出现，报错
void breakEqual(std::string &token, std::string &tokenNew);

//将string转换为char[21]
//若string为空，报错
//若string长度超过20，报错
void toChar_20(std::string token, char *toChar);

//将string转换为char[31]
//若string为空，报错
//若string长度超过30，报错
void toChar_30(std::string token, char *toChar);

//将string转换为char[61]
//若string为空，报错
//若string长度超过60，报错
void toChar_60(std::string token, char *toChar);

//将string转换为对应的浮点数的100倍（非负）
//若string为空，报错
//若string长度超过13，报错
//若格式错误，报错
//若有前导零，报错
//输入精度为两位
long toLong_100(std::string token);

//将string转换为对应的int（非负）
//若string为空，报错
//若string长度超过10，报错
//若格式错误，报错
//若有前导零，报错
//若对应数值大于int存储的最大值，报错
int toInt(std::string token);

class tokenScanner:

    //读入一行命令语句
    void scan();

    //以空格为分隔符，获得下一个片段
    //若无下一个片段，返回""
    std::string nextToken();
```
## 其他

主程序```main```函数，处理对应指令。
