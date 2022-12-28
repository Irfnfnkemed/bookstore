//
// Created by irfnfnkemed on 2022/12/22.
//

#ifndef BOOKSTROREOFWGJ_LOGIN_H
#define BOOKSTROREOFWGJ_LOGIN_H

#include <unordered_map>

class users {
public:
    char index[31];//用户的userID
    char password[31];
    char userName[31];
    int privilege;
};

//每次登录账户对应的节点
class login {
private:
    struct nodeLog {
        users data;
        nodeLog *next = nullptr;
        char selectISBN[61];//表示选中的图书

        nodeLog() {}

        nodeLog(users data_, nodeLog *prev_ = nullptr, nodeLog *next_ = nullptr) {
            data = data_;
            if (prev_ != nullptr) { prev_->next = this; }
            next = next_;
            selectISBN[0] = '\0';
        }
    };

    std::unordered_map<const char *, int> loginID;//用于记录登录的账户，以及登录次数

public:
    nodeLog *head = nullptr;

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
};

#endif //BOOKSTROREOFWGJ_LOGIN_H
