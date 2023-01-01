#include <iostream>
#include "blockchain.h"
#include "account.h"
#include "book.h"
#include "translate.h"
#include "log.h"

int main() {
    logShow logShowSystem;
    account accountSystem(&logShowSystem);
    log logSystem(accountSystem.getLogin(), &logShowSystem);
    book bookSystem(accountSystem.getLogin(), &logSystem, &logShowSystem);
    tokenScanner commandScanner;
    std::string tmp[11];
    char tmpChar_20[5][21];
    char tmpChar_30[5][31];
    char tmpChar_60[5][61];
    logShowSystem.storeLog("\nLogin the bookstore system.\n------------------------------\n");
    while (1) {
        commandScanner.scan();
        tmp[0] = commandScanner.nextToken();
        try {
            if (tmp[0] == "") { continue; }
            else if (tmp[0] == "exit" || tmp[0] == "quit") {
                if (commandScanner.nextToken() != "") { error("Invalid\n"); }
                logShowSystem.storeLog("------------------------------\nExit the bookstore system.\n\n");
                break;
            } else if (tmp[0] == "su") {
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
                if (tmp[1] == "" || commandScanner.nextToken() != "") { error("Invalid\n"); }
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
            } else if (tmp[0] == "log") {
                if (commandScanner.nextToken() != "") { error("Invalid\n"); }
                if (accountSystem.getLogin()->front()->data.privilege < 7) { error("Invalid\n"); }
                logShowSystem.show();
                logShowSystem.storeLog("Account[" +
                                       std::string(accountSystem.getLogin()->front()->data.index) +
                                       "] show the log.\n");
            } else { error("Invalid\n"); }
        } catch (errorException &ex) { std::cout << ex.getMessage(); }
    }
    return 0;
}