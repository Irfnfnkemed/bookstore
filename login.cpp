#include "login.h"
#include "error.h"

login::login() {
    head = new nodeLog;
    cnt = 0;
}

login::~login() {
    clear();
    delete head;
}

void login::clear() {
    loginID.clear();
    selectListISBN.clear();
    selectNumISBN.clear();
    selectConverseISBN.clear();
    cnt = 0;
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
    else { ++loginID[data_.index]; }//更新已经登录的账户数据
}

void login::pop() {
    if (head->next == nullptr) { error("Invalid\n"); }
    nodeLog *del = head->next;
    head->next = head->next->next;
    if (loginID[del->data.index] == 1) { loginID.erase(del->data.index); }
    else { --loginID[del->data.index]; }//更新已经登录的账户数据
    if (del->toSelectISBN != -1) {
        //处理选择的ISBN
        if (selectNumISBN[del->toSelectISBN] == 1) {
            selectNumISBN.erase(del->toSelectISBN);
            selectConverseISBN.erase(selectListISBN[del->toSelectISBN]);
            selectListISBN.erase(del->toSelectISBN);
        } else { --selectNumISBN[del->toSelectISBN]; }
    }
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

void login::select(const char *ISBN) {
    if (head->next == nullptr) { error("Invalid\n"); }
    if (head->next->toSelectISBN != -1) {
        //处理之前选择的ISBN
        if (selectNumISBN[head->next->toSelectISBN] == 1) {
            selectNumISBN.erase(head->next->toSelectISBN);
            selectConverseISBN.erase(selectListISBN[head->next->toSelectISBN]);
            selectListISBN.erase(head->next->toSelectISBN);
        } else { --selectNumISBN[head->next->toSelectISBN]; }
    }
    //处理新选择的ISBN
    if (selectConverseISBN.count(ISBN) == 0) {
        selectListISBN[cnt] = ISBN;
        selectNumISBN[cnt] = 1;
        selectConverseISBN[ISBN] = cnt;
        head->next->toSelectISBN = cnt;
        ++cnt;
    } else {
        head->next->toSelectISBN = selectConverseISBN[ISBN];
        ++selectNumISBN[head->next->toSelectISBN];
    }
}

std::string login::showSelect() {
    if (head->next == nullptr) { error("Invalid\n"); }
    if (head->next->toSelectISBN == -1) { error("Invalid\n"); }
    return selectListISBN[head->next->toSelectISBN];
}

void login::modifySelect(const char *ISBN) {
    if (head->next == nullptr) { error("Invalid\n"); }
    if (head->next->toSelectISBN == -1) { error("Invalid\n"); }
    selectListISBN[head->next->toSelectISBN] = ISBN;
    selectConverseISBN.erase(selectListISBN[head->next->toSelectISBN]);
    selectConverseISBN[ISBN] = head->next->toSelectISBN;
}