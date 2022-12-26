//
// Created by irfnfnkemed on 2022/12/24.
//

#include "book.h"

book::book(login *loginPoint_) :
        ISBNStore(nodeNameISBN, infoNameISBN, sizeIndexISBN, numInfoISBN),
        BookNameStore(nodeNameBookName, infoNameBookName, sizeIndexBookName, numInfoBookName),
        AuthorStore(nodeNameAuthor, infoNameAuthor, sizeIndexAuthor, numInfoAuthor),
        KeywordStore(nodeNameKeyword, infoNameKeyword, sizeIndexKeyword, numInfoKeyword) {
    loginPoint = loginPoint_;
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
    int stoISBN, posISBN, stoBookName, posBookName, stoAuthor, posAuthor, stoKeyword, posKeyword;
    if (ISBNStore.find(ISBN, stoISBN, posISBN)) {
        bookISBN buyISBN = ISBNStore.get(posISBN);//找到目标信息
        if (buyISBN.stock < quantity) { error("Invalid\n"); }//本书不够，操作失败
        std::cout << std::fixed << std::setprecision(2)
                  << double(buyISBN.price_100) * quantity / 100 << std::endl;//输出总金额
        buyISBN.stock -= quantity;
        ISBNStore.modify(posISBN, buyISBN, assignISBN);
        //在另外几种存储方式中找到目标信息,并处理
        BookNameStore.find(buyISBN.bookName, ISBN, stoBookName, posBookName);
        AuthorStore.find(buyISBN.author, ISBN, stoAuthor, posAuthor);
        bookBookName buyBookName = BookNameStore.get(posBookName);
        bookAuthor buyAuthor = AuthorStore.get(posAuthor);
        buyBookName.stock -= quantity;
        buyAuthor.stock -= quantity;
        BookNameStore.modify(posBookName, buyBookName, assignBookName);
        AuthorStore.modify(posAuthor, buyAuthor, assignAuthor);
        //切片，并处理
        bookKeyword buyKeyword;
        std::vector<char *> tmp = parser(buyISBN.keyword);
        for (int i = 0; i < tmp.size(); ++i) {
            KeywordStore.find(tmp[i], ISBN, stoKeyword, posKeyword);
            if (i == 0) { buyKeyword = KeywordStore.get(posKeyword); }
            else { assignString(buyKeyword.index, tmp[i], sizeIndexKeyword); }
            buyKeyword.stock -= quantity;
            KeywordStore.modify(posKeyword, buyKeyword, assignKeyword);
        }
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
        ISBNStore.insert(toInsert.index, toInsert, assignISBN);
    }
}

void book::modify(bool token[5], const char *modifyString[4], long modifyPrice_100) {
    ////////////////////////////////////////////////////////////////
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 3) { error("Invalid\n"); }//权限不足，操作失败
    if (loginPoint->front()->selectISBN[0] == '\0') { error("Invalid\n"); }//未选择，操作失败
    bookISBN modISBN;
    bookBookName modBookName;
    bookAuthor modAuthor;
    bookKeyword modKeyword;
    int stoISBN, posISBN, stoBookName, posBookName, stoAuthor, posAuthor, stoKeyword, posKeyword;
    std::vector<char *> tmp;//切片用
    if (token[0] && strncmp(modifyString[0], loginPoint->front()->selectISBN, sizeIndexISBN) == 0) {
        error("Invalid\n"); //将 ISBN 改为原有的 ISBN，操作失败
    }
    ISBNStore.find(loginPoint->front()->selectISBN, stoISBN, posISBN);
    modISBN = ISBNStore.get(posISBN);//找到目标信息
    ISBNStore.eraseDelete(stoISBN, posISBN);//删去信息
    if (modISBN.bookName[0] != '\0') {
        //原图书有具体信息，需先删去其他存储模式中的信息
        BookNameStore.find(modISBN.bookName, modISBN.index, stoBookName, posBookName);
        BookNameStore.eraseDelete(stoBookName, posBookName);//删去信息
        AuthorStore.find(modISBN.author, modISBN.index, stoAuthor, posAuthor);
        AuthorStore.eraseDelete(stoAuthor, posAuthor);//删去信息
        tmp = parser(modISBN.keyword);
        for (int i = 0; i < tmp.size(); ++i) {
            KeywordStore.find(tmp[i], modISBN.index, stoKeyword, posKeyword);
            KeywordStore.eraseDelete(stoKeyword, posKeyword);
        }
    }
    //修改信息
    try {
        assignStringISBN(modISBN.index, modifyString[0]);
        if (token[1]) {
            assignString(modISBN.bookName, modifyString[1], sizeIndexBookName);
        }
        if (token[2]) {
            assignString(modISBN.author, modifyString[2], sizeIndexAuthor);
        }
        if (token[3]) {
            tmp = parser(modifyString[3]);
            assignString(modISBN.keyword, modifyString[3], sizeIndexKeyword);
        }
        if (token[4]) { modISBN.price_100 = modifyPrice_100; }
    } catch (...) { throw; }//有非法修改，操作失败
    //重新插入信息
    ISBNStore.insert(modISBN.index, modISBN, assignISBN);
    assingISBNtoBookName(modBookName, modISBN);
    BookNameStore.insert(modBookName.index, modBookName.value, modBookName, assignBookName);
    assingISBNtoAuthor(modAuthor, modISBN);
    AuthorStore.insert(modAuthor.index, modAuthor.value, modAuthor, assignAuthor);
    for (int i = 0; i < tmp.size(); ++i) {
        if (i == 0) { assingISBNtoKeyword(tmp[0], modKeyword, modISBN); }
        else { assignString(modKeyword.index, tmp[i], sizeIndexKeyword); }
        KeywordStore.insert(modKeyword.index, modKeyword.value, modKeyword, assignKeyword);
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
    if (impISBN.bookName[0] != '\0') {
        //原书籍有信息，只需修改即可
        impISBN.stock += quantity;
        ISBNStore.modify(posISBN, impISBN, assignISBN);
        //在另外几种存储方式中找到目标信息,并处理
        BookNameStore.find(impISBN.bookName, impISBN.index, stoBookName, posBookName);
        AuthorStore.find(impISBN.author, impISBN.index, stoAuthor, posAuthor);
        impBookName = BookNameStore.get(posBookName);
        impAuthor = AuthorStore.get(posAuthor);
        impBookName.stock += quantity;
        impAuthor.stock += quantity;
        BookNameStore.modify(posBookName, impBookName, assignBookName);
        AuthorStore.modify(posAuthor, impAuthor, assignAuthor);
        //切片，并处理
        tmp = parser(impISBN.keyword);
        for (int i = 0; i < tmp.size(); ++i) {
            KeywordStore.find(tmp[i], impISBN.index, stoKeyword, posKeyword);
            if (i == 0) { impKeyword = KeywordStore.get(posKeyword); }
            else { assignString(impKeyword.index, tmp[i], sizeIndexKeyword); }
            impKeyword.stock += quantity;
            KeywordStore.modify(posKeyword, impKeyword, assignKeyword);
        }
    } else {//原书籍为默认值
        assignStringISBN(impISBN.index, loginPoint->front()->selectISBN);
        impISBN.stock += quantity;
        assingISBNtoBookName(impBookName, impISBN);
        assingISBNtoAuthor(impAuthor, impISBN);
        ISBNStore.modify(posISBN, impISBN, assignISBN);
        //插入
        BookNameStore.insert(impBookName.index, impBookName.value, impBookName, assignBookName);
        AuthorStore.insert(impAuthor.index, impAuthor.value, impAuthor, assignAuthor);
        for (int i = 0; i < tmp.size(); ++i) {
            if (i == 0) { assingISBNtoKeyword(tmp[0], impKeyword, impISBN); }
            else { assignString(impKeyword.index, tmp[i], sizeIndexKeyword); }
            KeywordStore.insert(impKeyword.index, impKeyword.value, impKeyword, assignKeyword);
        }
    }
}

std::vector<char *> book::parser(const char *keyword) {
    ///////////////////////////////////////////////////////
    std::vector<char *> keywords;
    int j = 0, k = 0;//j表示当前切片字符串的下标，k表示当前切片字符串的序数
    char newKeyword[sizeIndexKeyword];
    keywords.push_back(newKeyword);//增加新的切片
    for (int i = 0; i < sizeIndexKeyword; ++i) {
        if (keyword[i] == '\0' && j != 0) {//防止形如 '...|' 的情况
            keywords[k][j] = '\0';//结束一段切片
            return keywords;
        } else if (keyword[i] == '|' && j != 0) {//防止形如 '...||...' '|...' 的情况
            keywords[k][j] = '\0';//结束一段切片
            for (int t = 0; t < k; ++t) {//若有重复字段，报错
                if (strcmp(keywords[k], keywords[t]) == 0) { error("Invalid\n"); }
            }
            j = 0;
            ++k;
            char newKeyword[sizeIndexKeyword];
            keywords.push_back(newKeyword);//增加新的切片
        } else if (int(keyword[i]) >= 33 && int(keyword[i]) <= 126 &&
                   keyword[i] != '\"' && keyword[i] != '|' && keyword[i] != '\0') {
            keywords[k][j] = keyword[i];
            ++j;
        } else if (keyword[i] == '\0' && k == 0 && j == 0) {// '' 的情况
            keywords[0][0] = '\0';
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









