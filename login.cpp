#include "login.h"
#include "error.h"

login::login() { head = new nodeLog; }

login::~login() {
    clear();
    delete head;
}

void login::clear() {
    loginID.clear();
    nodeLog *del;
    while (1) {
        if (head->next != nullptr) {
            del = head->next;
            head->next = head->next->next;
            delete del;
        } else { break; }
    }
}

bool login::empty() {
    if (head->next == nullptr) { return true; }
    else { return false; }
}

void login::push(const users &data_) {
    head->next = new nodeLog(data_, head, head->next);//栈顶插入节点
    if (loginID.count(data_.index) == 0) { loginID[data_.index] = 1; }
    else { loginID[data_.index] += 1; }//更新已经登录的账户数据
}

void login::pop() {
    if (head->next == nullptr) { error("Invalid\n"); }
    nodeLog *del = head->next;
    head->next = head->next->next;
    if (loginID.count(del->data.index) == 0) { loginID.erase(del->data.index); }
    else { loginID[del->data.index] -= 1; }//更新已经登录的账户数据
    delete del;

}

login::nodeLog *login::front() {
    if (head->next == nullptr) { error("Invalid\n"); }
    return head->next;
}

bool login::find(const char *userID) {
    if (loginID.count(userID) == 0) { return false; }
    else { return true; }
}