//
// Created by irfnfnkemed on 2022/12/24.
//

#ifndef BOOKSTROREOFWGJ_BOOK_H
#define BOOKSTROREOFWGJ_BOOK_H

#include "account.h"
#include "error.h"
#include <iomanip>
#include <vector>

const size_t sizeIndexISBN = 20;
const size_t sizeIndexBookName = 60;
const size_t sizeIndexAuthor = 60;
const size_t sizeIndexKeyword = 60;
const size_t numInfoISBN = 50;
const size_t numInfoBookName = 50;
const size_t numInfoAuthor = 50;
const size_t numInfoKeyword = 50;

//四种存储模式下的信息、节点
class bookISBN {
public:
    char index[20];//以ISBN为索引,ISBN不会重复，无需引入value
    char bookName[60];//书名
    char author[60];//作者名
    char keyword[60];//未切片的关键词
    long price_100;//100*图书单价
    int stock;//库存数量
};

class bookBookName {
public:
    char index[60];//以书名为索引
    char value[20];//以ISBN为value
    char author[60];//作者名
    char keyword[60];//未切片的关键词
    long price_100;//100*图书单价
    int stock;//库存数量
};

class bookAuthor {
public:
    char index[60];//以作者名为索引
    char value[20];//以ISBN为value
    char bookName[60];//书名
    char keyword[60];//未切片的关键词
    long price_100;//100*图书单价
    int stock;//库存数量
};

class bookKeyword {
public:
    char index[60];//以某一关键词为索引
    char value[20];//以ISBN为value
    char bookName[60];//书名
    char author[60];//作者名
    char keyword[60];//未切片的关键词
    long price_100;//100*图书单价
    int stock;//库存数量
};

class nodeISBN {
public:
    char low[20];//节点中存储的索引的下限
    char high[20];//节点中存储的索引的下限
    int next;//下一个节点的位置（0-based）
    int prev;//上一个节点的位置（0-based）
    int number;//节点中存储的对应信息的位置（表第几块）（0-based）
    int stored;//块内存储的信息条数
};

class nodeOther {
public:
    char low[60];//节点中存储的索引的下限
    char high[60];//节点中存储的索引的下限
    int next;//下一个节点的位置（0-based）
    int prev;//上一个节点的位置（0-based）
    int number;//节点中存储的对应信息的位置（表第几块）（0-based）
    int stored;//块内存储的信息条数
};

class infoISBN {
public:
    bookISBN infoMem[50];
};

class infoBookName {
public:
    bookBookName infoMem[50];
};

class infoAuthor {
public:
    bookAuthor infoMem[50];
};

class infoKeyword {
public:
    bookKeyword infoMem[50];
};

class book {
private:
    const std::string nodeNameISBN = "book_ISBN_node";
    const std::string infoNameISBN = "book_ISBN_info";
    const std::string nodeNameBookName = "book_name_node";
    const std::string infoNameBookName = "book_name_info";
    const std::string nodeNameAuthor = "book_author_node";
    const std::string infoNameAuthor = "book_author_info";
    const std::string nodeNameKeyword = "book_keyword_node";
    const std::string infoNameKeyword = "book_keyword_info";
    //用于不同存储模式下图书信息的文件操作
    blockchain<nodeISBN, infoISBN, bookISBN> ISBNStore;
    blockchain<nodeOther, infoBookName, bookBookName> BookNameStore;
    blockchain<nodeOther, infoAuthor, bookAuthor> AuthorStore;
    blockchain<nodeOther, infoKeyword, bookKeyword> KeywordStore;
    //指向相关联的登录栈的指针
    login *loginPoint;
public:

    //构造函数
    book(login *loginPoint_);

//--------------------------------------------------------------------

    //show系列函数
    //最低权限要求{1}

    //show函数
    //输出所有书籍
    void show();

    //showISBN函数
    //输出所有ISBN符合的书籍
    void showISBN(const char *ISBN);

    //showBookName函数
    //输出所有bookName符合的书籍
    void showBookName(const char *bookName);

    //showAuthor函数
    //输出所有bookAuthor符合的书籍
    void showAuthor(const char *author);

    //showKeyword函数
    //输出所有bookKeyword符合的书籍
    void showKeyword(const char *keyword);

//--------------------------------------------------------------------

    //buy函数

    //购买指定数量的指定图书（由于本系统不包括支付系统，故仅需减少库存），以浮点数输出购买图书所需的总金额
    //没有符合条件的图书则操作失败
    //购买数量为非正整数则操作失败
    //最低权限要求{1}
    void buy(const char *ISBN, int quantity);

//--------------------------------------------------------------------

    //select函数

    //以当前帐户选中指定图书（帐户登出后无需保存选中图书情况）。
    //没有符合条件的图书则创建仅拥有 [ISBN] 信息的新图书
    //退出系统视为取消选中图书
    //最低权限要求{3}
    void select(const char *ISBN);

//--------------------------------------------------------------------

    //modify函数

    //如未选中图书则操作失败
    //有重复附加参数为非法指令
    //附加参数内容为空则操作失败
    //不允许将 ISBN 改为原有的 ISBN
    //最低权限要求{3}
    //数组中前之后依次表示ISBN，bookName，author，keyword，true表要修改，false表不修改
    void modify(bool token[4], const char *modifyString[4], long modifyPrice_100);

//--------------------------------------------------------------------

    //import函数

    //以指定交易总额购入指定数量的选中图书，增加其库存数
    //如未选中图书则操作失败
    //购入数量为非正整数则操作失败
    //交易总额为非正数则操作失败
    //最低权限要求{3}
    void import(int quantity, long totalCost_100);

private:

    //print系列函数

    //printISBN函数
    //输出bookISBN信息
    static void printISBN(const bookISBN &toPrint) {
        std::cout << toPrint.index << '\t' << toPrint.bookName << '\t'
                  << toPrint.author << '\t' << toPrint.keyword << '\t'
                  << std::fixed << std::setprecision(2)
                  << double(toPrint.price_100) / 100 << '\t'
                  << toPrint.stock << '\n';
    }

    //printBookName函数
    //输出bookBookName信息
    static void printBookName(const bookBookName &toPrint) {
        std::cout << toPrint.value << '\t' << toPrint.index << '\t'
                  << toPrint.author << '\t' << toPrint.keyword << '\t'
                  << std::fixed << std::setprecision(2)
                  << double(toPrint.price_100) / 100 << '\t'
                  << toPrint.stock << '\n';
    }

    //printAuthor函数
    //输出bookAuthor信息
    static void printAuthor(const bookAuthor &toPrint) {
        std::cout << toPrint.value << '\t' << toPrint.bookName << '\t'
                  << toPrint.index << '\t' << toPrint.keyword << '\t'
                  << std::fixed << std::setprecision(2)
                  << double(toPrint.price_100) / 100 << '\t'
                  << toPrint.stock << '\n';
    }

    //printKeyword函数
    //输出bookKeyword信息
    static void printKeyword(const bookKeyword &toPrint) {
        std::cout << toPrint.value << '\t' << toPrint.bookName << '\t'
                  << toPrint.author << '\t' << toPrint.keyword << '\t'
                  << std::fixed << std::setprecision(2)
                  << double(toPrint.price_100) / 100 << '\t'
                  << toPrint.stock << '\n';
    }

//--------------------------------------------------------------------

    //parser函数
    //用于拆解keyword字符串
    //若发现不合法内容，抛出错误
    //返回存储拆解后各个keyword的vector
    std::vector<char *> parser(const char *keyword);

//--------------------------------------------------------------------

    //assign系列函数
    //用于各种信息间的赋值

    inline static void assignISBN(bookISBN &newAssign, const bookISBN &toAssign) {
        newAssign = toAssign;
    }

    inline static void assignBookName(bookBookName &newAssign, const bookBookName &toAssign) {
        newAssign = toAssign;
    }

    inline static void assignAuthor(bookAuthor &newAssign, const bookAuthor &toAssign) {
        newAssign = toAssign;
    }

    inline static void assignKeyword(bookKeyword &newAssign, const bookKeyword &toAssign) {
        newAssign = toAssign;
    }

    //bookName，author，keyword的赋值与正确性检查
    inline void assignString(char *newString, const char *toString, size_t size) {
        for (int i = 0; i < size; ++i) {
            if (toString[i] == '\0') {
                newString[i] = '\0';
                break;
            } else if (int(toString[i]) >= 33 && int(toString[i]) <= 126 &&
                       toString[i] != '\"') { newString[i] = toString[i]; }
            else { error("Invalid\n"); }//非法，抛出异常
        }
    }

    //ISBN的赋值与正确性检查
    inline void assignStringISBN(char *newString, const char *toString) {
        for (int i = 0; i < sizeIndexISBN; ++i) {
            if (toString[i] == '\0') {
                newString[i] = '\0';
                break;
            } else if (int(toString[i]) >= 33 && int(toString[i]) <= 126) {
                newString[i] = toString[i];
            } else { error("Invalid\n"); }//非法，抛出异常
        }
    }

    void assingISBNtoBookName(bookBookName &newAssign, const bookISBN &toAssign);

    void assingISBNtoAuthor(bookAuthor &newAssign, const bookISBN &toAssign);

    void assingISBNtoKeyword(const char *key, bookKeyword &newAssign, const bookISBN &toAssign);

    //bookName，author，keyword的正确性检查
    inline void judgeString(const char *toString, size_t size) {
        for (int i = 0; i < size; ++i) {
            if (toString[i] == '\0') { break; }
            else if (!(int(toString[i]) >= 33 && int(toString[i]) <= 126 && toString[i] != '\"')) {
                error("Invalid\n");
            }//非法，抛出异常
        }
    }

    //ISBN的正确性检查
    inline void judgeStringISBN(const char *toString) {
        for (int i = 0; i < sizeIndexISBN; ++i) {
            if (toString[i] == '\0') { break; }
            else if (!(int(toString[i]) >= 33 && int(toString[i]) <= 126)) {
                error("Invalid\n");
            }//非法，抛出异常
        }
    }
};

#endif //BOOKSTROREOFWGJ_BOOK_H
