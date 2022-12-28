#include <iostream>
#include "blockchain.h"
#include "account.h"
#include "book.h"
#include "translate.h"
#include "log.h"

/*
////用于读取的节点块
//class node {
//public:
//    char low[64];//节点中存储的索引的下限
//    char high[64];//节点中存储的索引的下限
//    int next;//下一个节点的位置（0-based）
//    int prev;//上一个节点的位置（0-based）
//    int number;//节点中存储的对应信息的位置（表第几块）（0-based）
//    int stored;//块内存储的信息条数
//};
//
////每一条信息
//class everyInfo {
//public:
//    char index[64];//信息索引
//    char value[8];//信息值
//};
//
////用于读取的信息
//class info {
//public:
//    everyInfo infoMem[60 * 15];
//};
//
//void aaa(everyInfo &s, const everyInfo &r) {
//    s = r;
//}

int main() {
    int n;
    account a;
    std::string command;
//    blockchain<node, info, everyInfo> a("../file/h", "../file/i", 64, 64 * 25);
    std::cin >> n;
//    char insert[64];
//    char value[8];
//    for (int ii = 1; ii <= n; ++ii) {
//        std::cin >> command;
//        if (command == "insert") {
//            std::cin >> insert >> value;
//            everyInfo q;
//            for (int i = 0; i < 64; ++i) { q.index[i] = insert[i]; }
//            for (int i = 0; i < 8; ++i) { q.value[i] = value[i]; }
//            a.insert(insert, value, q, aaa);
//        } else if (command == "delete") {
//            std::cin >> insert >> value;
//            a.erase(insert, value);
//        } else if (command == "find") {
//            std::cin >> insert;
//            a.print(insert);
//            std::cout << '\n';
//        }
//    }
    char userID[30];
    char password[30];
    char userName[30];
    int privilege;
    char currentPassword[30];
    char newPassword[30];
    for (int ii = 1; ii <= n; ++ii) {
        std::cin >> command;
        if (command == "su1") {
            std::cin >> userID >> password;
            try { a.su(userID, password); }
            catch (...) { std::cout << "su1!!!!" << std::endl; }
        } else if (command == "su2") {
            std::cin >> userID;
            try { a.su(userID); }
            catch (...) { std::cout << "su2!!!!" << std::endl; }
        } else if (command == "logout") {
            try { a.logout(); }
            catch (...) { std::cout << "logout!!!!" << std::endl; }
        } else if (command == "registering") {
            std::cin >> userID >> password >> userName;
            try { a.registering(userID, password, userName); }
            catch (...) { std::cout << "registering!!!!" << std::endl; }
        } else if (command == "passwd1") {
            std::cin >> userID >> currentPassword >> newPassword;
            try { a.passwd(userID, currentPassword, newPassword); }
            catch (...) { std::cout << "passwd1!!!!" << std::endl; }
        } else if (command == "passwd2") {
            std::cin >> userID >> newPassword;
            try { a.passwd(userID, newPassword); }
            catch (...) { std::cout << "passwd2!!!!" << std::endl; }
        } else if (command == "useradd") {
            std::cin >> userID >> password >> privilege >> userName;
            try { a.useradd(userID, password, privilege, userName); }
            catch (...) { std::cout << "useradd!!!!" << std::endl; }
        } else if (command == "deleting") {
            std::cin >> userID;
            try { a.deleting(userID); }
            catch (...) { std::cout << "deleting!!!!" << std::endl; }
        }
    }
    return 0;
        int i;
    while (1) {
        try {
            std::cin >> i;
            if (i == 1) { break; }
            if (i == 2) { s.scan(); }
            if (i == 3) { std::cout << s.nextToken() << std::endl; }
            if (i == 4) { std::cout << toChar_20(s.nextToken()) << std::endl; }
            if (i == 5) { std::cout << toChar_30(s.nextToken()) << std::endl; }
            if (i == 6) { std::cout << toChar_60(s.nextToken()) << std::endl; }
            if (i == 7) { std::cout << toLong_100(s.nextToken()) << std::endl; }
            if (i == 8) { std::cout << toInt(s.nextToken()) << std::endl; }
        } catch (...) { std::cout << "!!!" << std::endl; }
    }

}*/


int main() {
    //std::freopen("./data/complex/testcase3/2.in", "r", stdin);
    //std::freopen("out", "w", stdout);
    account accountSystem;
    log logSystem(accountSystem.getLogin());
    book bookSystem(accountSystem.getLogin(), &logSystem);
    tokenScanner commandScanner;
    std::string tmp[11];
    char tmpChar_20[5][21];
    char tmpChar_30[5][31];
    char tmpChar_60[5][61];
    while (1) {
        commandScanner.scan();
        tmp[0] = commandScanner.nextToken();
        try {
            if (tmp[0] == "") { continue; }
            else if (tmp[0] == "exit" || tmp[0] == "quit") { exit(0); }
            else if (tmp[0] == "su") {
                tmp[1] = commandScanner.nextToken();
                tmp[2] = commandScanner.nextToken();
                if (tmp[1] == "" || commandScanner.nextToken() != "") { error("Invalid\n"); }
                if (tmp[2] != "") {
                    toChar_30(tmp[1], tmpChar_30[0]);
                    toChar_30(tmp[2], tmpChar_30[1]);
                    accountSystem.su(tmpChar_30[0], tmpChar_30[1]);
                } else {
                    toChar_30(tmp[1], tmpChar_30[0]);
                    accountSystem.su(tmpChar_30[0]);
                }
            } else if (tmp[0] == "logout") {
                if (commandScanner.nextToken() != "") { error("Invalid\n"); }
                accountSystem.logout();
            } else if (tmp[0] == "register") {
                tmp[1] = commandScanner.nextToken();
                tmp[2] = commandScanner.nextToken();
                tmp[3] = commandScanner.nextToken();
                if (tmp[3] == "" || commandScanner.nextToken() != "") { error("Invalid\n"); }
                toChar_30(tmp[1], tmpChar_30[0]);
                toChar_30(tmp[2], tmpChar_30[1]);
                toChar_30(tmp[3], tmpChar_30[2]);
                accountSystem.registering(tmpChar_30[0], tmpChar_30[1], tmpChar_30[2]);
            } else if (tmp[0] == "passwd") {
                tmp[1] = commandScanner.nextToken();
                tmp[2] = commandScanner.nextToken();
                tmp[3] = commandScanner.nextToken();
                if (tmp[2] == "" || commandScanner.nextToken() != "") { error("Invalid\n"); }
                if (tmp[3] != "") {
                    toChar_30(tmp[1], tmpChar_30[0]);
                    toChar_30(tmp[2], tmpChar_30[1]);
                    toChar_30(tmp[3], tmpChar_30[2]);
                    accountSystem.passwd(tmpChar_30[0], tmpChar_30[1], tmpChar_30[2]);
                } else {
                    toChar_30(tmp[1], tmpChar_30[0]);
                    toChar_30(tmp[2], tmpChar_30[1]);
                    accountSystem.passwd(tmpChar_30[0], tmpChar_30[1]);
                }
            } else if (tmp[0] == "useradd") {
                tmp[1] = commandScanner.nextToken();
                tmp[2] = commandScanner.nextToken();
                tmp[3] = commandScanner.nextToken();
                tmp[4] = commandScanner.nextToken();
                if (tmp[4] == "" || commandScanner.nextToken() != "") { error("Invalid\n"); }
                toChar_30(tmp[1], tmpChar_30[0]);
                toChar_30(tmp[2], tmpChar_30[1]);
                toChar_30(tmp[4], tmpChar_30[2]);
                accountSystem.useradd(tmpChar_30[0], tmpChar_30[1],
                                      toInt(tmp[3]), tmpChar_30[2]);
            } else if (tmp[0] == "delete") {
                tmp[1] = commandScanner.nextToken();
                if (tmp[1] == "" || commandScanner.nextToken() != "") { error("Invalid\n"); }
                toChar_30(tmp[1], tmpChar_30[0]);
                accountSystem.deleting(tmpChar_30[0]);
            } else if (tmp[0] == "show") {
                tmp[1] = commandScanner.nextToken();
                if (tmp[1] == "") { bookSystem.show(); }
                else if (tmp[1] == "finance") {
                    tmp[2] = commandScanner.nextToken();
                    if (commandScanner.nextToken() != "") { error("Invalid\n"); }
                    if (tmp[2] == "") { logSystem.show(); }
                    else { logSystem.show(toInt(tmp[2])); }
                } else {
                    breakEqual(tmp[1], tmp[2]);
                    if (commandScanner.nextToken() != "") { error("Invalid\n"); }
                    if (tmp[1] == "") {
                        bookSystem.show();
                    } else {
                        if (tmp[1] == "-ISBN") {
                            if (tmp[2] == "") { error("Invalid\n"); }
                            toChar_20(tmp[2], tmpChar_20[0]);
                            bookSystem.showISBN(tmpChar_20[0]);
                        } else {
                            popQuotations(tmp[2]);
                            toChar_60(tmp[2], tmpChar_60[0]);
                            if (tmp[1] == "-name") {
                                bookSystem.showBookName(tmpChar_60[0]);
                            } else if (tmp[1] == "-author") {
                                bookSystem.showAuthor(tmpChar_60[0]);
                            } else if (tmp[1] == "-keyword") {
                                bookSystem.showKeyword(tmpChar_60[0]);
                            } else { error("Invalid\n"); }
                        }
                    }
                }
            } else if (tmp[0] == "buy") {
                tmp[1] = commandScanner.nextToken();
                tmp[2] = commandScanner.nextToken();
                if (tmp[2] == "" || commandScanner.nextToken() != "") { error("Invalid\n"); }
                toChar_20(tmp[1], tmpChar_20[0]);
                bookSystem.buy(tmpChar_20[0], toInt(tmp[2]));
            } else if (tmp[0] == "select") {
                tmp[1] = commandScanner.nextToken();
                if (tmp[1] == "" || commandScanner.nextToken() != "") { error("Invalid\n"); }
                toChar_20(tmp[1], tmpChar_20[0]);
                bookSystem.select(tmpChar_20[0]);
            } else if (tmp[0] == "modify") {
                tmp[1] = commandScanner.nextToken();
                tmp[3] = commandScanner.nextToken();
                tmp[5] = commandScanner.nextToken();
                tmp[7] = commandScanner.nextToken();
                tmp[9] = commandScanner.nextToken();
                if (commandScanner.nextToken() != "") { error("Invalid\n"); }
                bool judge[5] = {false, false, false, false, false};
                long price_100 = 0;
                tmpChar_20[0][0] = tmpChar_60[0][0] =
                tmpChar_60[1][0] = tmpChar_60[2][0] = '\0';
                for (int i = 1; i <= 5; ++i) {
                    if (tmp[2 * i - 1] == "") { break; }
                    breakEqual(tmp[2 * i - 1], tmp[2 * i]);
                    if (tmp[2 * i] == "") { error("Invalid\n"); }
                    if (tmp[2 * i - 1] == "-ISBN") {
                        if (!judge[0]) { judge[0] = true; }
                        else { error("Invalid\n"); }
                        toChar_20(tmp[2 * i], tmpChar_20[0]);
                    } else if (tmp[2 * i - 1] == "-price") {
                        if (!judge[4]) { judge[4] = true; }
                        else { error("Invalid\n"); }
                        price_100 = toLong_100(tmp[2 * i]);
                    } else {
                        popQuotations(tmp[2 * i]);
                        if (tmp[2 * i - 1] == "-name") {
                            if (!judge[1]) { judge[1] = true; }
                            else { error("Invalid\n"); }
                            toChar_60(tmp[2 * i], tmpChar_60[0]);
                        } else if (tmp[2 * i - 1] == "-author") {
                            if (!judge[2]) { judge[2] = true; }
                            else { error("Invalid\n"); }
                            toChar_60(tmp[2 * i], tmpChar_60[1]);
                        } else if (tmp[2 * i - 1] == "-keyword") {
                            if (!judge[3]) { judge[3] = true; }
                            else { error("Invalid\n"); }
                            toChar_60(tmp[2 * i], tmpChar_60[2]);
                        } else { error("Invalid\n"); }
                    }
                }
                bookSystem.modify(judge, tmpChar_20[0], tmpChar_60[0],
                                  tmpChar_60[1], tmpChar_60[2], price_100);
            } else if (tmp[0] == "import") {
                tmp[1] = commandScanner.nextToken();
                tmp[2] = commandScanner.nextToken();
                if (tmp[2] == "" || commandScanner.nextToken() != "") { error("Invalid\n"); }
                bookSystem.import(toInt(tmp[1]), toLong_100(tmp[2]));
            } else { error("Invalid\n"); }
        } catch (errorException &ex) { std::cout << ex.getMessage(); }
    }

    return 0;
}