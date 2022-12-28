//
// Created by irfnfnkemed on 2022/12/23.
//
#ifndef BOOKSTROREOFWGJ_ACCOUNT_H
#define BOOKSTROREOFWGJ_ACCOUNT_H

#include "login.h"
#include "blockchain.h"
#include <fstream>
#include <string>

class login;

class nodeAccount {
public:
    char low[31];//节点中存储的索引的下限
    char high[31];//节点中存储的索引的下限
    int next;//下一个节点的位置（0-based）
    int prev;//上一个节点的位置（0-based）
    int number;//节点中存储的对应信息的位置（表第几块）（0-based）
    int stored;//块内存储的信息条数
};

class infoAccount {
public:
    users infoMem[2048];
};

class account {
private:
    const std::string fileNodeAccountName = "account_node";
    const std::string fileInfoAccountName = "account_info";
    const size_t sizeIndex = 31;
    const size_t numInfo = 2048;

    //用于账户信息的文件操作
    blockchain<nodeAccount, infoAccount, users> accountStore;

    login loginStack;//用于登录栈
public:

    //构造函数
    account();

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

private:

    //assignUsers函数

    //将一个users用另一个users覆盖
    static void assignUsers(users &newAccount, const users &oldAccount) {
        newAccount = oldAccount;
    }

//---------------------------------------------------------------
    //assignUserID函数

    //用于UserID、Password的正确性检查和赋值
    inline void assignUserID(char *newUserID, const char *userID);

//---------------------------------------------------------------

    //assignUserName函数

    //用于UserName的正确性检查和赋值
    inline void assignUserName(char *newUserName, const char *userName);

//---------------------------------------------------------------

    //judgePrivilege函数

    //用于privilege的合法性检查
    inline bool judgePrivilege(int privilege);

};

#endif //BOOKSTROREOFWGJ_ACCOUNT_H
