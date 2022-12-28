//
// Created by irfnfnkemed on 2022/12/24.
//

#include "book.h"

book::book(login *loginPoint_, log *logPoint_) :
        ISBNStore(nodeNameISBN, infoNameISBN, sizeIndexISBN, numInfoISBN),
        BookNameStore(nodeNameBookName, infoNameBookName, sizeIndexBookName, numInfoBookName),
        AuthorStore(nodeNameAuthor, infoNameAuthor, sizeIndexAuthor, numInfoAuthor),
        KeywordStore(nodeNameKeyword, infoNameKeyword, sizeIndexKeyword, numInfoKeyword) {
    loginPoint = loginPoint_;
    logPoint = logPoint_;
}

void book::show() {
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 1) { error("Invalid\n"); }//权限不足，操作失败
    ISBNStore.print(printISBN);
}

void book::showISBN(const char *ISBN) {
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 1) { error("Invalid\n"); }//权限不足，操作失败
    judgeStringISBN(ISBN);
    ISBNStore.print(ISBN, printISBN);
}

void book::showBookName(const char *bookName) {
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 1) { error("Invalid\n"); }//权限不足，操作失败
    judgeString(bookName, sizeIndexBookName);
    BookNameStore.print(bookName, printBookName);
}

void book::showAuthor(const char *author) {
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 1) { error("Invalid\n"); }//权限不足，操作失败
    judgeString(author, sizeIndexAuthor);
    AuthorStore.print(author, printAuthor);
}

void book::showKeyword(const char *keyword) {
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 1) { error("Invalid\n"); }//权限不足，操作失败
    std::vector<char *> tmp = parser(keyword);
    if (tmp.size() != 1) { error("Invalid\n"); }//不为一个关键词，操作失败
    KeywordStore.print(keyword, printKeyword);
}

void book::buy(const char *ISBN, int quantity) {
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 1) { error("Invalid\n"); }//权限不足，操作失败
    if (quantity == 0) { error("Invalid\n"); }//不合法，操作失败
    int stoISBN, posISBN, stoBookName, posBookName, stoAuthor, posAuthor, stoKeyword, posKeyword;
    if (ISBNStore.find(ISBN, stoISBN, posISBN)) {
        bookISBN buyISBN = ISBNStore.get(posISBN);//找到目标信息
        if (buyISBN.stock < quantity) { error("Invalid\n"); }//本书不够，操作失败
        std::cout << std::fixed << std::setprecision(2)
                  << double(buyISBN.price_100) * quantity / 100 << std::endl;//输出总金额
        buyISBN.stock -= quantity;
        ISBNStore.modify(posISBN, buyISBN, assignISBN);
        //在另外几种存储方式中找到目标信息(若存在),并处理
        if (buyISBN.bookName[0] != '\0') {
            BookNameStore.find(buyISBN.bookName, ISBN, stoBookName, posBookName);
            bookBookName buyBookName = BookNameStore.get(posBookName);
            buyBookName.stock -= quantity;
            BookNameStore.modify(posBookName, buyBookName, assignBookName);
        }
        if (buyISBN.author[0] != '\0') {
            AuthorStore.find(buyISBN.author, ISBN, stoAuthor, posAuthor);
            bookAuthor buyAuthor = AuthorStore.get(posAuthor);
            buyAuthor.stock -= quantity;
            AuthorStore.modify(posAuthor, buyAuthor, assignAuthor);
        }
        if (buyISBN.keyword[0] != '\0') {
            //切片，并处理
            bookKeyword buyKeyword;
            std::vector<char *> tmp = parser(buyISBN.keyword);
            for (int i = 0; i < tmp.size(); ++i) {
                KeywordStore.find(tmp[i], ISBN, stoKeyword, posKeyword);
                if (i == 0) {
                    buyKeyword = KeywordStore.get(posKeyword);
                    buyKeyword.stock -= quantity;
                } else { assignString(buyKeyword.index, tmp[i], sizeIndexKeyword); }
                KeywordStore.modify(posKeyword, buyKeyword, assignKeyword);
            }
        }
        logPoint->add(true, buyISBN.price_100 * quantity);//添加交易记录
    } else { error("Invalid\n"); }//目标书籍不存在，操作失败
}

void book::select(const char *ISBN) {
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 3) { error("Invalid\n"); }//权限不足，操作失败
    try { assignStringISBN(loginPoint->front()->selectISBN, ISBN); }//记录被选择的ISBN
    catch (...) { throw; }//若ISBN不合法，抛出错误
    int sto, pos;
    if (!ISBNStore.find(ISBN, sto, pos)) {
        bookISBN toInsert;
        assignStringISBN(toInsert.index, ISBN);
        toInsert.bookName[0] = toInsert.author[0] = toInsert.keyword[0] = '\0';
        toInsert.price_100 = toInsert.stock = 0;
        ISBNStore.insertFind(sto, pos, toInsert, assignISBN);
    }
}

void book::modify(bool token[4], const char *modifyString_0, const char *modifyString_1,
                  const char *modifyString_2, const char *modifyString_3, long modifyPrice_100) {
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 3) { error("Invalid\n"); }//权限不足，操作失败
    if (loginPoint->front()->selectISBN[0] == '\0') { error("Invalid\n"); }//未选择，操作失败
    bookISBN modISBN;
    bookBookName modBookName;
    bookAuthor modAuthor;
    bookKeyword modKeyword;
    int stoISBN, posISBN, stoBookName, posBookName, stoAuthor, posAuthor, stoKeyword, posKeyword;
    std::vector<char *> tmp;//切片用
    if (token[0] && strncmp(modifyString_0, loginPoint->front()->selectISBN, sizeIndexISBN) == 0) {
        error("Invalid\n"); //将 ISBN 改为原有的 ISBN，操作失败
    }
    if (token[0] && ISBNStore.find(modifyString_0)) { error("Invalid\n"); }//IBSN重复，操作失败
    try {//确保正确性
        if (token[0]) {
            assignStringISBN(modISBN.index, modifyString_0);
        }
        if (token[1]) {
            assignString(modISBN.bookName, modifyString_1, sizeIndexBookName);
        }
        if (token[2]) {
            assignString(modISBN.author, modifyString_2, sizeIndexAuthor);
        }
        if (token[3]) {
            tmp = parser(modifyString_3);
            assignString(modISBN.keyword, modifyString_3, sizeIndexKeyword);
        }
        if (token[4]) { modISBN.price_100 = modifyPrice_100; }
    } catch (...) { throw; }//有非法修改，操作失败
    ISBNStore.find(loginPoint->front()->selectISBN, stoISBN, posISBN);
    modISBN = ISBNStore.get(posISBN);//找到目标信息
    ISBNStore.eraseDelete(stoISBN, posISBN);//删去信息
    //若原图书有具体信息，需先删去其他存储模式中的信息
    if (modISBN.bookName[0] != '\0') {
        BookNameStore.find(modISBN.bookName, modISBN.index, stoBookName, posBookName);
        BookNameStore.eraseDelete(stoBookName, posBookName);//删去信息
    }
    if (modISBN.author[0] != '\0') {
        AuthorStore.find(modISBN.author, modISBN.index, stoAuthor, posAuthor);
        AuthorStore.eraseDelete(stoAuthor, posAuthor);//删去信息
    }
    if (modISBN.keyword[0] != '\0') {
        tmp = parser(modISBN.keyword);
        for (int i = 0; i < tmp.size(); ++i) {
            KeywordStore.find(tmp[i], modISBN.index, stoKeyword, posKeyword);
            KeywordStore.eraseDelete(stoKeyword, posKeyword);
        }
    }
    //修改信息
    try {
        if (token[0]) {
            assignStringISBN(modISBN.index, modifyString_0);
        }
        if (token[1]) {
            assignString(modISBN.bookName, modifyString_1, sizeIndexBookName);
        }
        if (token[2]) {
            assignString(modISBN.author, modifyString_2, sizeIndexAuthor);
        }
        if (token[3]) {
            tmp = parser(modifyString_3);
            assignString(modISBN.keyword, modifyString_3, sizeIndexKeyword);
        }
        if (token[4]) { modISBN.price_100 = modifyPrice_100; }
    } catch (...) { throw; }//有非法修改，操作失败
    //重新插入信息(若有对应值)
    ISBNStore.insert(modISBN.index, modISBN, assignISBN);
    if (modISBN.bookName[0] != '\0') {
        assingISBNtoBookName(modBookName, modISBN);
        BookNameStore.insert(modBookName.index, modBookName.value, modBookName, assignBookName);
    }
    if (modISBN.author[0] != '\0') {
        assingISBNtoAuthor(modAuthor, modISBN);
        AuthorStore.insert(modAuthor.index, modAuthor.value, modAuthor, assignAuthor);
    }
    if (modISBN.keyword[0] != '\0') {
        for (int i = 0; i < tmp.size(); ++i) {
            if (i == 0) { assingISBNtoKeyword(tmp[0], modKeyword, modISBN); }
            else { assignString(modKeyword.index, tmp[i], sizeIndexKeyword); }
            KeywordStore.insert(modKeyword.index, modKeyword.value, modKeyword, assignKeyword);
        }
    }
}

void book::import(int quantity, long totalCost_100) {
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 3) { error("Invalid\n"); }//权限不足，操作失败
    if (loginPoint->front()->selectISBN[0] == '\0') { error("Invalid\n"); }//未选择，操作失败
    int stoISBN, posISBN, stoBookName, posBookName, stoAuthor, posAuthor, stoKeyword, posKeyword;
    bookISBN impISBN;
    bookBookName impBookName;
    bookAuthor impAuthor;
    bookKeyword impKeyword;
    std::vector<char *> tmp;
    ISBNStore.find(loginPoint->front()->selectISBN, stoISBN, posISBN);
    impISBN = ISBNStore.get(posISBN);//找到目标信息
    //若原书籍有信息，只需修改即可
    impISBN.stock += quantity;
    ISBNStore.modify(posISBN, impISBN, assignISBN);
    //在另外几种存储方式中找到目标信息,并处理
    if (impISBN.bookName[0] != '\0') {
        BookNameStore.find(impISBN.bookName, impISBN.index, stoBookName, posBookName);
        impBookName = BookNameStore.get(posBookName);
        impBookName.stock += quantity;
        BookNameStore.modify(posBookName, impBookName, assignBookName);
    }
    if (impISBN.author[0] != '\0') {
        AuthorStore.find(impISBN.author, impISBN.index, stoAuthor, posAuthor);
        impAuthor = AuthorStore.get(posAuthor);
        impAuthor.stock += quantity;
        AuthorStore.modify(posAuthor, impAuthor, assignAuthor);
    }
    if (impISBN.keyword[0] != '\0') {
        //切片，并处理
        tmp = parser(impISBN.keyword);
        for (int i = 0; i < tmp.size(); ++i) {
            KeywordStore.find(tmp[i], impISBN.index, stoKeyword, posKeyword);
            if (i == 0) {
                impKeyword = KeywordStore.get(posKeyword);
                impKeyword.stock += quantity;
            } else { assignString(impKeyword.index, tmp[i], sizeIndexKeyword); }
            KeywordStore.modify(posKeyword, impKeyword, assignKeyword);
        }
    }
    logPoint->add(false, totalCost_100);//添加交易记录
}

char tmp[sizeIndexKeyword][sizeIndexKeyword];

std::vector<char *> book::parser(const char *keyword) {
    std::vector<char *> keywords;
    int j = 0, k = 0;//j表示当前切片字符串的下标，k表示当前切片字符串的序数
    for (int i = 0; i < sizeIndexKeyword; ++i) {
        if (keyword[i] == '\0' && j != 0) {//防止形如 '...|' 的情况
            tmp[k][j] = '\0';//结束一段切片
            for (int t = 0; t < k; ++t) {//若有重复字段，报错
                if (strcmp(tmp[k], keywords[t]) == 0) { error("Invalid\n"); }
            }
            keywords.push_back(tmp[k]);
            return keywords;
        } else if (keyword[i] == '|' && j != 0) {//防止形如 '...||...' '|...' 的情况
            tmp[k][j] = '\0';//结束一段切片
            for (int t = 0; t < k; ++t) {//若有重复字段，报错
                if (strcmp(tmp[k], keywords[t]) == 0) { error("Invalid\n"); }
            }
            keywords.push_back(tmp[k]);//增加新的切片
            j = 0;
            ++k;
        } else if (int(keyword[i]) >= 33 && int(keyword[i]) <= 126 &&
                   keyword[i] != '\"' && keyword[i] != '|' && keyword[i] != '\0') {
            tmp[k][j] = keyword[i];
            ++j;
        } else if (keyword[i] == '\0' && k == 0 && j == 0) {// '' 的情况
            return keywords;
        } else { error("Invalid\n"); }//非法，抛出异常
    }
}

void book::assingISBNtoBookName(bookBookName &newAssign, const bookISBN &toAssign) {
    assignString(newAssign.index, toAssign.bookName, sizeIndexBookName);
    assignStringISBN(newAssign.value, toAssign.index);
    assignString(newAssign.author, toAssign.author, sizeIndexAuthor);
    assignString(newAssign.keyword, toAssign.keyword, sizeIndexKeyword);
    newAssign.price_100 = toAssign.price_100;
    newAssign.stock = toAssign.stock;
}

void book::assingISBNtoAuthor(bookAuthor &newAssign, const bookISBN &toAssign) {
    assignString(newAssign.index, toAssign.author, sizeIndexAuthor);
    assignStringISBN(newAssign.value, toAssign.index);
    assignString(newAssign.bookName, toAssign.bookName, sizeIndexBookName);
    assignString(newAssign.keyword, toAssign.keyword, sizeIndexKeyword);
    newAssign.price_100 = toAssign.price_100;
    newAssign.stock = toAssign.stock;
}

void book::assingISBNtoKeyword
        (const char *key, bookKeyword &newAssign, const bookISBN &toAssign) {
    assignString(newAssign.index, key, sizeIndexKeyword);
    assignStringISBN(newAssign.value, toAssign.index);
    assignString(newAssign.bookName, toAssign.bookName, sizeIndexBookName);
    assignString(newAssign.author, toAssign.author, sizeIndexAuthor);
    assignString(newAssign.keyword, toAssign.keyword, sizeIndexKeyword);
    newAssign.price_100 = toAssign.price_100;
    newAssign.stock = toAssign.stock;
}









