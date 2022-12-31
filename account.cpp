//
// Created by irfnfnkemed on 2022/12/23.
//

#include "account.h"
#include "error.h"


account::account(logShow *logShowPoint_) :
        accountStore(fileNodeAccountName, fileInfoAccountName, sizeIndex, numInfo) {
    logShowPoint = logShowPoint_;
    if (accountStore.empty()) {
        users initAccount;//创建店长账户
        char initIndex[5] = {'r', 'o', 'o', 't', '\0'};
        char initPassword[5] = {'s', 'j', 't', 'u', '\0'};
        for (int i = 0; i < 5; ++i) {
            initAccount.index[i] = initIndex[i];
            initAccount.password[i] = initPassword[i];
        }
        initAccount.privilege = 7;
        accountStore.insert(initAccount.index, initAccount, assignUsers);//将店长账户存储
    }
}

void account::su(const char *userID, const char *password) {
    int sto, pos;
    users suAccount;
    if (accountStore.find(userID, sto, pos)) {//账户存在
        suAccount = accountStore.get(pos);//找到信息
        if (strcmp(password, suAccount.password) != 0) { error("Invalid\n"); }//密码错误，操作失败
        loginStack.push(suAccount);//添加到登录栈中
        logShowPoint->storeLog("Account[" + std::string(userID) + "] su the system.\n");//添加日志
    } else { error("Invalid\n"); }//此账户不存在，操作失败
}

void account::su(const char *userID) {
    if (loginStack.empty()) { error("Invalid\n"); }//无密码情况下，须有账号登录，而此时无账户登录，操作失败
    int sto, pos;
    users suAccount;
    if (accountStore.find(userID, sto, pos)) {//账户存在
        suAccount = accountStore.get(pos);//找到信息
        if (loginStack.front()->data.privilege <= suAccount.privilege) {
            error("Invalid\n");//权限不足，操作失败
        }
        loginStack.push(suAccount);//添加到登录栈中
        logShowPoint->storeLog("Account[" + std::string(userID) + "] su the system.\n");//添加日志
    } else { error("Invalid\n"); }//此账户不存在，操作失败
}

void account::logout() {
    if (loginStack.empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginStack.front()->data.privilege < 1) { error("Invalid\n"); }//权限不足，操作失败
    std::string tmp1 = loginStack.front()->data.index, tmp2 = "Account ";
    logShowPoint->storeLog("Account[" + std::string(loginStack.front()->data.index) +
                           "] logout the system.\n");//添加日志
    loginStack.pop();
}

void account::registering(const char *userID, const char *password, const char *userName) {
    users newAccount;
    try {//新账户信息
        assignUserID(newAccount.index, userID);
        assignUserID(newAccount.password, password);
        assignUserName(newAccount.userName, userName);
        newAccount.privilege = 1;
    } catch (...) { throw; }//新账户信息有非法内容，操作失败
    if (!accountStore.insert(userID, newAccount, assignUsers)) {
        error("Invalid\n");//插入失败，表明userID重复，操作失败
    }
    //添加日志
    logShowPoint->storeLog("register new account[" + std::string(userID) + "] with ( ");
    logShowPoint->storeLog("password[" + std::string(password) + "] name[");
    logShowPoint->storeLog(std::string(userName) + "] ).\n");
}

void account::passwd(const char *userID, const char *currentPassword, const char *newPassword) {
    if (loginStack.empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginStack.front()->data.privilege < 1) { error("Invalid\n"); }//权限不足，操作失败
    users passwdAccount;
    int sto, pos;
    if (accountStore.find(userID, sto, pos)) {//账户存在
        passwdAccount = accountStore.get(pos);//找到信息
        if (strcmp(passwdAccount.password, currentPassword) == 0) {
            try { assignUserID(passwdAccount.password, newPassword); }
            catch (...) { throw; }//newPassword含有不为数字、字母、下划线字符，操作失败
            accountStore.modify(pos, passwdAccount, assignUsers);
            //添加日志
            logShowPoint->storeLog("Account[" + std::string(loginStack.front()->data.index) +
                                   "] passwd account[");
            logShowPoint->storeLog(std::string(userID) + "] with new password[");
            logShowPoint->storeLog(std::string(newPassword) + "].\n");
        } else { error("Invalid\n"); }//密码错误，操作失败
    } else { error("Invalid\n"); }//此账户不存在，操作失败
}

void account::passwd(const char *userID, const char *newPassword) {
    if (loginStack.empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginStack.front()->data.privilege < 7) { error("Invalid\n"); }//权限不足，操作失败
    users passwdAccount;
    int sto, pos;
    if (accountStore.find(userID, sto, pos)) {//账户存在
        passwdAccount = accountStore.get(pos);//找到信息
        try { assignUserID(passwdAccount.password, newPassword); }
        catch (...) { throw; }//newPassword含有不为数字、字母、下划线字符，操作失败
        accountStore.modify(pos, passwdAccount, assignUsers);
        //添加日志
        logShowPoint->storeLog("Account[" + std::string(loginStack.front()->data.index) +
                               "] passwd account[");
        logShowPoint->storeLog(std::string(userID) + "] with new password[");
        logShowPoint->storeLog(std::string(newPassword) + "].\n");
    } else { error("Invalid\n"); }//此账户不存在，操作失败
}

void account::useradd(const char *userID, const char *password,
                      int privilege, const char *userName) {
    if (loginStack.empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginStack.front()->data.privilege < 3 || !judgePrivilege(privilege) ||
        loginStack.front()->data.privilege <= privilege) { error("Invalid\n"); }//权限不足或权限不合法，操作失败
    users addAccount;
    try {//新账户信息
        assignUserID(addAccount.index, userID);
        assignUserID(addAccount.password, password);
        assignUserName(addAccount.userName, userName);
        addAccount.privilege = privilege;
    } catch (...) { throw; }//新账户信息有非法内容，操作失败
    if (!accountStore.insert(userID, addAccount, assignUsers)) {
        error("Invalid\n");//插入失败，表userID重复，操作失败
    }
    //添加日志
    logShowPoint->storeLog("Account[" + std::string(loginStack.front()->data.index) +
                           "] add new account[");
    logShowPoint->storeLog(std::string(userID) + "] with ( password[");
    logShowPoint->storeLog(std::string(password) + "] privilege[");
    logShowPoint->storeLog(std::to_string(privilege) + "] name[" +
                           std::string(userName) + "] ).\n");
}

void account::deleting(const char *userID) {
    if (loginStack.empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginStack.front()->data.privilege < 7) { error("Invalid\n"); }//权限不足，操作失败
    if (loginStack.find(userID)) { error("Invalid\n"); }//删除账户已登录，操作失败
    if (!accountStore.erase(userID)) { error("Invalid\n"); }//删除失败，表userID不存在，操作失败
    //添加日志
    logShowPoint->storeLog("Account[" + std::string(loginStack.front()->data.index) +
                           "] delete account[");
    logShowPoint->storeLog(std::string(userID) + "].\n");
}

inline void account::assignUserID(char *newUserID, const char *userID) {
    for (int i = 0; i < sizeIndex; ++i) {
        if (userID[i] != '\0') {
            if ((userID[i] >= '0' && userID[i] <= '9') || (userID[i] >= 'a' && userID[i] <= 'z') ||
                (userID[i] >= 'A' && userID[i] <= 'Z') || userID[i] == '_') {
                newUserID[i] = userID[i];
            } else { error("Invalid\n"); }//不为数字、字母、下划线，操作失败
        } else {
            newUserID[i] = '\0';
            break;
        }
    }
}

inline void account::assignUserName(char *newUserName, const char *userName) {
    for (int i = 0; i < sizeIndex; ++i) {
        if (userName[i] != '\0') {
            if (int(userName[i]) >= 33 && int(userName[i]) <= 126) {
                newUserName[i] = userName[i];
            } else { error("Invalid\n"); }//不为可见ASCII字符，操作失败
        } else {
            newUserName[i] = '\0';
            break;
        }
    }
}

inline bool account::judgePrivilege(int privilege) {
    if (privilege == 1 || privilege == 3 || privilege == 7) { return true; }
    else { return false; }
}