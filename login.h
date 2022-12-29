//
// Created by irfnfnkemed on 2022/12/22.
//

#ifndef BOOKSTROREOFWGJ_LOGIN_H
#define BOOKSTROREOFWGJ_LOGIN_H

#include <unordered_map>
#include <string>


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
        int toSelectISBN = -1;//指向selectListISBN的key的指针

        nodeLog() {}

        nodeLog(users data_, nodeLog *prev_ = nullptr, nodeLog *next_ = nullptr) {
            data = data_;
            if (prev_ != nullptr) { prev_->next = this; }
            next = next_;
            toSelectISBN = -1;
        }
    };

    std::unordered_map<std::string, int> loginID;//用于记录登录的账户，以及登录次数
    std::unordered_map<int, std::string> selectListISBN;//用于现在被选择的ISBN，以及选择次数
    std::unordered_map<int, int> selectNumISBN;//用于现在被选择的ISBN的选择次数(key与selectListISBN同)
    std::unordered_map<std::string, int> selectConverseISBN;//selectListISBN的反向
    int cnt = 0;//用于selectListISBN的key值

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
};

#endif //BOOKSTROREOFWGJ_LOGIN_H
