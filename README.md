# Bookstore大作业开发文档
程序主要功能系统：```账户系统```、```图书系统```、```日志系统```、```工具系统```
包括
* `main.cpp`
* ```account.h``` ,```account.cpp```
* ```login.h``` ,```login.cpp```
* ```book.h```,```book.cpp```
* ```log.h``` ,```log.cpp```
* ```blockchain.h```,`blockchain.cpp`
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
包括```account.h``` ,```account.cpp```，```login.h``` ,```login.cpp```：

* ```account.h``` 用于实现账户系统的指令操作：
有接口```su```、```logout```、```register```、```passwd```、```useradd```、```delete ```。

* ```login.h``` 用于实现登录栈，定义了登录栈类。能够推入```（push）```、弹出```（pop）```栈顶的账户，能够清空```（clear）```登录栈的内容。具体通过链表来实现。

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
  * 调用```account.h```中的```register```函数来注册账户，将账户信息通过块状链表的插入添加到相应文件中。
* 修改密码 
  * 调用```account.h```中的```passwd```函数来修改密码，先进行检索，然后修改相应信息，维护块状链表。
* 创建帐户
  * 调用```account.h```中的```useradd```函数来创建账户，与注册类似，只是权限信息不同
* 删除帐户
  * 调用```account.h```中的```delete```函数来对删除账户，先进行检索，然后将该账户信息从文件中通过块状链表的删除删去。

## 图书系统
包括```book.h``` ,```book.cpp```：

* ```book.h``` 用于实现图书系统的指令操作：
有接口```show```、```buy```、```select```、```modify```、```import```

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
* ```log.h```用于读写日志记录。每次调用```account.h```、```book.h``` 的接口函数时，利用```log.h```将相应信息代表的01串写入相应文件中。

存储方式：
* 利用两个**二进制文件**```log```,```finance```来存储相关信息。
对```log```，存放所有操作的信息记录，每种类型的信息都有特定的01串前缀标识符来标记类型。
对```finance```，存放有关所有交易的信息记录和总的交易笔数。总的交易笔数置于文件开头，长度应为最大长度，有多余的用0补齐。对有交易的信息，只记录每次交易的收入和支出。

指令：```财务记录查询```、```生成日志```

* 财务记录查询：
  * 调用```log.h```中的```showFinance```函数来查询财务记录。通过读取```finance```文件的信息，输出相应内容。
* 生成日志：
  * 调用```log.h```中的```log```函数来生成日志。通过读取```log```文件的信息打印日志。

## 工具系统

包括```blockchain.h```,```blockchain.cpp```,```error.h```,```error.cpp```,```transfer.h```,`transfer.cpp`：
* ```blockchain.h```用于处理文件的块状链表结构。包括检索（`find`）、插入（`insert`）、删除（`erase`）、修改（`change`）、清空（`clear`）等接口函数。
* ```error.h```用于接受并处理抛出的错误。
* `transfer.h`用于识别01串标识符、实现字符串、数字、01串等之间的相互转换。

## 其他

主程序为带参数的```main```函数，以便于从命令行中读取相应命令并执行对应操作。
