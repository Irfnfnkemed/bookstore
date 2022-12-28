//
// Created by irfnfnkemed on 2022/12/18.
//

#ifndef BOOKSTROREOFWGJ_BLOCKCHAIN_H
#define BOOKSTROREOFWGJ_BLOCKCHAIN_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>


const size_t sizeMax = 4096 * 50;//每个节点对应信息的最大内存，为4096*50字节

template<class node, class info, class everyInfo>
class blockchain {

private:
    const size_t sizeIndex;//索引的字节数
    const size_t numInfo;//每个节点存储的最大信息条数(需为偶数，方便操作)
    const std::string fileNodeName;//节点文件名
    const std::string fileInfoName;//信息文件名
    std::fstream fileNode;//节点文件
    std::fstream fileInfo;//信息文件
    int sum;//表示总节点数（包括文件中并块造成的空节点），除了删完和析构外，sum只增不减

    //除文件只有一个节点的情况外，每个节点存储的信息条数应在[numInfo/2 , numInfo)范围内

    //每个节点存储内容（按顺序）：
    //            节点内索引值下限，节点内索引值上限，
    //            下一个节点的位置（第几个）（0-based），上一个节点的位置（第几个）（0-based），
    //            对应信息在信息文件中的位置（第几个）（0-based）,块内存储的信息条数（0-based），
    //     注：第一个节点的前一个节点位置、最后一个节点的后一个节点位置均用-1来表示

    //每个信息块存储内容（按顺序）：
    //            依次：每条信息的索引(index)，
    //                 该条信息的其他内容（先是用于查找插入的具体信息值(value)）（排列顺序由对应的写入顺序决定）

public:
//------------------------------------------------------------------
    //构造函数

    blockchain(std::string fileNodeName_, std::string fileInfoName_,
               size_t sizeIndex_, size_t numInfo_);

//------------------------------------------------------------------
    //析构函数
    //功能：由于删除时会在文件中留下无效块，析构时需删掉这些无效块，将文件重新储存
    ~blockchain();

//------------------------------------------------------------------
    //find函数：

    //功能：利用索引值 或 索引值和具体的值 查找目标是否存在（查找）
    //     同时，还兼有寻找某条要插入信息的插入位置的作用（定位）
    //若找到符合要求的，返回true; 反之，返回false
    //不论如何，都找到要插入这条信息的位置，使节点文件指针指向该节点开头
    //sto表示要信息若要插入，插入信息块已储存信息条数（0-based）
    //pos表示信息若要插入，应插在第pos（0-based）条前面

    //功能：利用索引值查找目标是否存在，并寻找某条要插入信息的插入位置
    //参数：字符串类的索引值，引用整数类的已储存信息条数和要插入位置
    bool find(const char *index, int &sto, int &pos);

    //功能：利用索引值和具体的值查找目标是否存在，并寻找某条要插入信息的插入位置的作用
    //参数：字符串类的索引值，引用整数类的已储存信息条数和要插入位置
    bool find(const char *index, const char *value, int &sto, int &pos);

    //功能：利用索引值和具体的值查找目标是否存在
    //参数：字符串类的索引值
    bool find(const char *index);

    //功能：利用索引值和具体的值查找目标是否存在
    //参数：字符串类的索引值
    bool find(const char *index, const char *value);

//------------------------------------------------------------------

    //print函数：

    //功能：按具体信息值从小到大输出所有索引值为index的信息
    void print(const char *index, void(*printFunc)(const everyInfo &));

    //功能：输出所有储存的信息
    void print(void(*printFunc)(const everyInfo &));

//------------------------------------------------------------------

    //get函数：

    //功能：按信息所在位置pos返回当前所在信息
    //需要现利用find函数，确认所get的信息存在以及定位
    //参数：信息所在位置pos
    everyInfo get(int pos);

//------------------------------------------------------------------

    //modify函数：

    //功能：修改当前位置pos处的信息
    //需要现利用find函数，确认所modify的信息存在以及定位
    //用于不改变索引值和排列顺序的修改，即不会改变顺序、节点上下限
    //参数：信息所在位置pos，修改后的新的信息newInfo，指向修改用的函数的指针modifyFunc
    void modify(int pos, const everyInfo &newInfo,
                void (*modifyFunc)(everyInfo &, const everyInfo &));

//------------------------------------------------------------------
    //insert函数:

    //功能：根据索引值 或 索引值和具体信息值 插入信息
    //插入后，将节点文件指针、信息文件指针指向开头
    //索引值可以重复，但同一索引下，信息具体值不可重复
    //若插入成功，返回true;反之（已有重复信息），返回false

    //功能：根据索引值插入信息
    //参数：字符串类的索引值，具体信息值，要插入的信息，指向插入信息函数的函数指针
    bool insert(const char *index, const everyInfo &toInsert,
                void (*insertFunc)(everyInfo &, const everyInfo &));

    //功能：根据索引值和具体信息值插入信息
    //参数：字符串类的索引值，具体信息值，要插入的信息，指向插入信息函数的函数指针
    bool insert(const char *index, const char *value, const everyInfo &toInsert,
                void (*insertFunc)(everyInfo &, const everyInfo &));

//------------------------------------------------------------------

    //erase函数：

    //功能：利用索引值（适用于索引值不会重复、具体信息值未知的情况）或
    //     利用索引值和具体信息值（适用于索引值会重复、具体信息值已知的情况）删除信息
    //执行后，将节点文件指针、信息文件指针指向开头
    //若删除成功，返回true; 反之，删除失败（没有对应目标），返回false

    //功能：利用索引值（适用于索引值不会重复、具体信息值未知的情况）删除信息
    //参数：字符串类的索引值
    bool erase(const char *index);

    //功能：利用索引值和具体信息值（适用于索引值会重复、具体信息值已知的情况）删除信息
    //参数：字符串类的索引值，具体信息值
    bool erase(const char *index, const char *value);

//------------------------------------------------------------------

    //empty函数
    //功能：用于判断文件是否为空
    //若空，返回true; 反之，返回false
    bool empty();

//------------------------------------------------------------------

    //insertFind函数
    //功能：用于在已找到的位置的信息中（调用前需定位好）插入信息，在insert函数中使用
    //参数：目标信息块储存信息条数（0-based），目标信息块位置（0-based），
    //     要插入的信息，指向插入信息函数的函数指针
    void insertFind(int sto, int pos, const everyInfo &toInsert,
                    void (*insertFunc)(everyInfo &, const everyInfo &));

//------------------------------------------------------------------

    //eraseDelete函数
    //功能：用于删除已经找到位置的信息（调用前需定位好），在erase函数中使用
    //参数：整数类的要删去信息的块的储存信息条数sto，整数类的要删去信息的位置pos
    void eraseDelete(int sto, int pos);

//------------------------------------------------------------------

    //assignIndex函数
    //功能：索引的复制
    inline void assignIndex(char *lhs, const char *rhs) {
        for (int i = 0; i < sizeIndex; ++i) {
            lhs[i] = rhs[i];
        }
    }
};

//template<class node, class info, class everyInfo>
//blockchain<node, info, everyInfo>::blockchain():
//        fileNodeName(""), fileInfoName(""), numInfo(0), sizeIndex(0) {};


template<class node, class info, class everyInfo>
blockchain<node, info, everyInfo>::blockchain
        (std::string fileNodeName_, std::string fileInfoName_,
         size_t sizeIndex_, size_t numInfo_) :
        fileNodeName(fileNodeName_), fileInfoName(fileInfoName_),
        numInfo(numInfo_), sizeIndex(sizeIndex_) {
    //打开文件，若无则创建
    fileNode.open(fileNodeName, std::ios::in);
    fileInfo.open(fileInfoName, std::ios::in);
    if (fileNode.is_open()) {
        fileNode.close();
        fileNode.open(fileNodeName, std::ios::in | std::ios::out);
    } else {
        fileNode.open(fileNodeName, std::ios::out);
        fileNode.close();
        fileNode.open(fileNodeName, std::ios::in | std::ios::out);
    }
    if (fileInfo.is_open()) {
        fileInfo.close();
        fileInfo.open(fileInfoName, std::ios::in | std::ios::out);
    } else {
        fileInfo.open(fileInfoName, std::ios::out);
        fileInfo.close();
        fileInfo.open(fileInfoName, std::ios::in | std::ios::out);
    }
    //初始化
    fileNode.seekg(0, std::ios::end);
    sum = fileNode.tellg() / sizeof(node);
    fileNode.seekg(0, std::ios::beg);
}

template<class node, class info, class everyInfo>
blockchain<node, info, everyInfo>::~blockchain() {
    if (sum != 0) {
        std::fstream tmpNode, tmpInfo;
        node nodeNow;
        info infoNow;
        int i = 0;//表节点总数（0-based）
        //清空
        tmpNode.open("tmpNode", std::ios::out);
        tmpInfo.open("tmpInfo", std::ios::out);
        tmpNode.close();
        tmpInfo.close();
        tmpNode.open("tmpNode", std::ios::in | std::ios::out);
        tmpInfo.open("tmpInfo", std::ios::in | std::ios::out);
        //初始化
        fileNode.seekg(0, std::ios::beg);
        int next;
        char init[sizeMax];
        while (1) {//移动信息
            fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
            fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);
            fileInfo.read(reinterpret_cast<char *>(&infoNow), sizeof(info));
            nodeNow.prev = i - 1;
            nodeNow.number = i;
            next = nodeNow.next;
            if (next == -1) {
                nodeNow.next = -1;
                tmpNode.write(reinterpret_cast<char *>(&nodeNow), sizeof(node));
                tmpInfo.write(reinterpret_cast<char *>(&infoNow), sizeof(info));
                break;
            }//已读完
            nodeNow.next = i + 1;
            tmpNode.write(reinterpret_cast<char *>(&nodeNow), sizeof(node));
            tmpInfo.write(reinterpret_cast<char *>(&init), sizeMax);
            tmpInfo.seekg(-sizeMax, std::ios::cur);
            tmpInfo.write(reinterpret_cast<char *>(&infoNow), sizeof(info));
            tmpInfo.seekg(0, std::ios::end);
            ++i;
            fileNode.seekg(next * sizeof(node), std::ios::beg);//跳至下一节点
        }
        //清空原文件
        fileNode.close();
        fileInfo.close();
        fileNode.open(fileNodeName, std::ios::out);
        fileInfo.open(fileInfoName, std::ios::out);
        //将新的内容拷贝
        char Node[(i + 1) * sizeof(node)];
        char Info[(i + 1) * sizeMax];
        tmpNode.seekg(0, std::ios::beg);
        tmpInfo.seekg(0, std::ios::beg);
        tmpNode.read(Node, (i + 1) * sizeof(node));
        tmpInfo.read(Info, (i + 1) * sizeMax);
        fileNode.seekg(0, std::ios::beg);
        fileInfo.seekg(0, std::ios::beg);
        fileNode.write(Node, (i + 1) * sizeof(node));
        fileInfo.write(Info, (i + 1) * sizeMax);
        fileNode.close();
        fileInfo.close();
        //清空
        tmpNode.close();
        tmpInfo.close();
        tmpNode.open("tmpNode", std::ios::out);
        tmpInfo.open("tmpInfo", std::ios::out);
        tmpNode.close();
        tmpInfo.close();
    }
}

template<class node, class info, class everyInfo>
bool blockchain<node, info, everyInfo>::find(const char *index, int &sto, int &pos) {
    node nodeNow;
    info infoNow;
    //判断文件是否为空
    if (sum == 0) {
        sto = -1;
        pos = 0;//空
        return false;
    }
    //初始化，将信息文件、节点文件指针指针指向开头
    fileNode.seekg(0, std::ios::beg);
    //寻找目标所在的节点
    while (1) {
        fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
        if (strcmp(nodeNow.high, index) >= 0) {
            //目标索引落在范围内，进入信息文件对应位置，进一步寻找
            fileNode.seekg(-sizeof(node), std::ios::cur);//将节点文件指针置于该节点开头
            fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);//信息文件指针跳至指定位置
            sto = nodeNow.stored;
            break;
        } else {
            if (nodeNow.next == -1) {
                //表明已经读到结尾，目标索引值未落在所有节点的范围中，未找到
                fileNode.seekg(-sizeof(node), std::ios::cur);//将节点文件指针置于该节点开头
                sto = nodeNow.stored;
                pos = nodeNow.stored + 1;//若插入，需插在最后面
                return false;
            }
            fileNode.seekg(nodeNow.next * sizeof(node), std::ios::beg);//跳至下一个节点
        }
    }
    //寻找目标对应的信息
    fileInfo.read(reinterpret_cast<char *>(&infoNow), sizeof(info));
    for (int i = 0; i <= nodeNow.stored; ++i) {//由于索引不大于节点上限，故必然可以找到指定位置
        if (strcmp(infoNow.infoMem[i].index, index) == 0) {
            pos = i;//若插入，需插在这条前面
            return true;//找到，返回true
        } else if (strcmp(infoNow.infoMem[i].index, index) > 0) {
            pos = i;//若插入，需插在这条前面
            return false;//未找到
        }//若索引小于当前信息索引 ，则找下一条信息
    }
}

template<class node, class info, class everyInfo>
bool blockchain<node, info, everyInfo>::find
        (const char *index, const char *value, int &sto, int &pos) {
    node nodeNow;
    info infoNow;
    if (find(index, sto, pos)) {
        fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
        fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);//信息文件指针跳至指定位置
        fileInfo.read(reinterpret_cast<char *>(&infoNow), sizeof(info));//寻找目标对应的信息
        for (int i = pos; i <= sto; ++i) {
            if (strcmp(infoNow.infoMem[i].index, index) == 0) {
                if (strcmp(infoNow.infoMem[i].value, value) == 0) {
                    fileNode.seekg(-sizeof(node), std::ios::cur);//将节点文件指针置于该节点开头
                    pos = i;
                    return true;
                } else if (strcmp(infoNow.infoMem[i].value, value) > 0) {
                    fileNode.seekg(-sizeof(node), std::ios::cur);//将节点文件指针置于该节点开头
                    pos = i;
                    return false;
                }//若值小于当前信息值 ，则找下一条信息
            } else {
                fileNode.seekg(-sizeof(node), std::ios::cur);//将节点文件指针置于该节点开头
                pos = i;
                return false;
            }
        }
        //找后续
        while (1) {
            if (nodeNow.next == -1) {
                fileNode.seekg(-sizeof(node), std::ios::cur);//将节点文件指针置于该节点开头
                sto = nodeNow.stored;
                pos = nodeNow.stored + 1;//若插入，需插在最后面
                return false;
            }
            fileNode.seekg(nodeNow.next * sizeof(node), std::ios::beg);//跳至下一个节点
            fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
            fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);//信息文件指针跳至指定位置
            fileInfo.read(reinterpret_cast<char *>(&infoNow), sizeof(info));//寻找目标对应的信息
            for (int i = 0; i <= nodeNow.stored; ++i) {
                if (strcmp(infoNow.infoMem[i].index, index) == 0) {
                    if (strcmp(infoNow.infoMem[i].value, value) == 0) {
                        fileNode.seekg(-sizeof(node), std::ios::cur);//将节点文件指针置于该节点开头
                        sto = nodeNow.stored;
                        pos = i;
                        return true;
                    } else if (strcmp(infoNow.infoMem[i].value, value) > 0) {
                        fileNode.seekg(-sizeof(node), std::ios::cur);//将节点文件指针置于该节点开头
                        sto = nodeNow.stored;
                        pos = i;
                        return false;
                    }//若值小于当前信息值 ，则找下一条信息
                } else {
                    fileNode.seekg(-sizeof(node), std::ios::cur);//将节点文件指针置于该节点开头
                    sto = nodeNow.stored;
                    pos = i;
                    return false;
                }
            }
        }
    } else {
        return false;//未找到同index的信息，此时的若要插入，与无value限值的情况相同
    }
}

template<class node, class info, class everyInfo>
bool blockchain<node, info, everyInfo>::find(const char *index) {
    int tmp1, tmp2;
    return find(index, tmp1, tmp2);
}

template<class node, class info, class everyInfo>
bool blockchain<node, info, everyInfo>::find(const char *index, const char *value) {
    int tmp1, tmp2;
    return find(index, value, tmp1, tmp2);
}

template<class node, class info, class everyInfo>
void blockchain<node, info, everyInfo>::print
        (const char *index, void(*printFunc)(const everyInfo &)) {
    int sto, pos;//要插入信息块已有的信息条数，以及要插入位置
    node nodeNow;
    info infoNow;
    if (find(index, sto, pos)) {
        fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
        fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);
        fileInfo.read(reinterpret_cast<char *>(&infoNow), sizeof(info));
        for (int i = pos; i <= sto; ++i) {
            if (strcmp(infoNow.infoMem[i].index, index) == 0) {
                printFunc(infoNow.infoMem[i]);
            } else {
                //结束，初始化
                fileNode.seekg(0, std::ios::beg);
                fileInfo.seekg(0, std::ios::beg);
                return;
            }
        }
        //找后续
        while (1) {
            if (nodeNow.next == -1) {
                //结束，初始化
                fileNode.seekg(0, std::ios::beg);
                fileInfo.seekg(0, std::ios::beg);
                return;
            }
            fileNode.seekg(nodeNow.next * sizeof(node), std::ios::beg);//跳至下一个节点
            fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
            fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);//信息文件指针跳至指定位置
            fileInfo.read(reinterpret_cast<char *>(&infoNow), sizeof(info));//寻找目标对应的信息
            for (int i = 0; i <= nodeNow.stored; ++i) {
                if (strcmp(infoNow.infoMem[i].index, index) == 0) {
                    printFunc(infoNow.infoMem[i]);
                } else {
                    //结束，初始化
                    fileNode.seekg(0, std::ios::beg);
                    fileInfo.seekg(0, std::ios::beg);
                    return;
                }
            }
        }
    } else { std::cout << '\n'; }
}

template<class node, class info, class everyInfo>
void blockchain<node, info, everyInfo>::print(void (*printFunc)(const everyInfo &)) {
    fileNode.seekg(0, std::ios::end);
    if (fileNode.tellg() == 0) {//空，输出空行
        fileNode.seekg(0, std::ios::beg);
        std::cout << '\n';
        return;
    } else {
        node nodeNow;
        info infoNow;
        fileNode.seekg(0, std::ios::beg);
        while (1) {
            fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
            fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);
            fileInfo.read(reinterpret_cast<char *>(&infoNow), sizeof(info));
            for (int i = 0; i <= nodeNow.stored; ++i) { printFunc(infoNow.infoMem[i]); }
            if (nodeNow.next == -1) {//结束，初始化
                fileNode.seekg(0, std::ios::beg);
                fileInfo.seekg(0, std::ios::beg);
                return;
            } else { fileNode.seekg(nodeNow.next * sizeof(node), std::ios::beg); }//跳至下一个节点
        }
    }
}

template<class node, class info, class everyInfo>
everyInfo blockchain<node, info, everyInfo>::get(int pos) {
    node nodeNow;
    info infoNow;
    fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
    fileNode.seekg(-sizeof(node), std::ios::cur);
    fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);//信息文件指针跳至指定位置
    fileInfo.read(reinterpret_cast<char *>(&infoNow), sizeof(info));//寻找目标对应的信息
    return infoNow.infoMem[pos];
}

template<class node, class info, class everyInfo>
void blockchain<node, info, everyInfo>::modify(
        int pos, const everyInfo &newInfo,
        void (*modifyFunc)(everyInfo &, const everyInfo &)) {
    node nodeNow;
    info infoNow;
    fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
    fileNode.seekg(-sizeof(node), std::ios::cur);
    fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);//信息文件指针跳至指定位置
    fileInfo.read(reinterpret_cast<char *>(&infoNow), sizeof(info));//寻找目标对应的信息
    fileInfo.seekg(-sizeof(info), std::ios::cur);
    modifyFunc(infoNow.infoMem[pos], newInfo);//修改信息(不改变索引值和排列顺序)
    fileInfo.write(reinterpret_cast<char *>(&infoNow), sizeof(info));
}

template<class node, class info, class everyInfo>
bool blockchain<node, info, everyInfo>::insert
        (const char *index, const char *value, const everyInfo &toInsert,
         void (*insertFunc)(everyInfo &, const everyInfo &)) {
    int sto, pos;//要插入信息块已有的信息条数，以及要插入位置
    if (find(index, value, sto, pos)) { return false; }//已有完全一样的信息，忽略
    else {
        insertFind(sto, pos, toInsert, insertFunc);
        return true;
    }
}

template<class node, class info, class everyInfo>
bool blockchain<node, info, everyInfo>::insert
        (const char *index, const everyInfo &toInsert,
         void (*insertFunc)(everyInfo &, const everyInfo &)) {
    int sto, pos;//要插入信息块已有的信息条数，以及要插入位置
    if (find(index, sto, pos)) { return false; }//已有完全一样的信息，忽略
    else {
        insertFind(sto, pos, toInsert, insertFunc);
        return true;
    }
}

template<class node, class info, class everyInfo>
bool blockchain<node, info, everyInfo>::erase(const char *index) {
    int sto, pos;
    if (find(index, sto, pos)) {
        eraseDelete(sto, pos);
        return true;
    } else { return false; }
}

template<class node, class info, class everyInfo>
bool blockchain<node, info, everyInfo>::erase(const char *index, const char *value) {
    int sto, pos;
    if (find(index, value, sto, pos)) {
        eraseDelete(sto, pos);
        return true;
    } else { return false; }
}

template<class node, class info, class everyInfo>
bool blockchain<node, info, everyInfo>::empty() {
    fileNode.seekg(0, std::ios::end);
    if (fileNode.tellg() == 0) {
        fileNode.seekg(0, std::ios::beg);
        return true;
    } else {
        fileNode.seekg(0, std::ios::beg);
        return false;
    }
}

//insertFindAlready的实现：
//        插入（insertFindAlready）
//       |          |          |
//     块内够     块内不够      空文件
//       |          |           |
//   块内处理  先块内插入，再裂块  新建块状链表
//
template<class node, class info, class everyInfo>
void blockchain<node, info, everyInfo>::insertFind
        (int sto, int pos, const everyInfo &toInsert,
         void (*insertFunc)(everyInfo &, const everyInfo &)) {
    node nodeNow;
    info infoNow;
    char init[sizeMax];
    if (sto == -1) {//文件为空
        sum = 1;
        //新建节点和信息块
        nodeNow.prev = nodeNow.next = -1;
        nodeNow.number = 0;
        nodeNow.stored = 0;
        insertFunc(infoNow.infoMem[0], toInsert);
        assignIndex(nodeNow.low, infoNow.infoMem[0].index);
        assignIndex(nodeNow.high, infoNow.infoMem[0].index);
        fileNode.write(reinterpret_cast<char *>(&nodeNow), sizeof(node));
        fileInfo.write(init, sizeMax);
        fileInfo.seekg(-sizeMax, std::ios::cur);
        fileInfo.write(reinterpret_cast<char *>(&infoNow), sizeof(info));
        //结束，初始化
        fileNode.seekg(0, std::ios::beg);
        fileInfo.seekg(0, std::ios::beg);
        return;
    } else {//先读取现在的信息
        fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
        fileNode.seekg(-sizeof(node), std::ios::cur);
        fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);
        fileInfo.read(reinterpret_cast<char *>(&infoNow), sizeof(info));
        fileInfo.seekg(-sizeof(info), std::ios::cur);
        if (sto + 2 < numInfo) {//块内空间足够，块内操作
            ++nodeNow.stored;//更新节点
            for (int i = sto; i >= pos; --i) {
                infoNow.infoMem[i + 1] = infoNow.infoMem[i];//移动信息
            }
            //插入信息
            insertFunc(infoNow.infoMem[pos], toInsert);
            //更新节点范围
            if (pos == 0) { assignIndex(nodeNow.low, infoNow.infoMem[0].index); }
            if (pos == sto + 1) { assignIndex(nodeNow.high, infoNow.infoMem[pos].index); }
            //写回节点和信息
            fileNode.write(reinterpret_cast<char *>(&nodeNow), sizeof(node));
            fileInfo.write(reinterpret_cast<char *>(&infoNow), sizeof(info));
        } else if (sto + 2 == numInfo) {//块内空间不够，需要裂块
            node nodeNew;
            info infoNew;
            ++sum;
            //信息插入和移动
            if (pos >= numInfo / 2) {//此时，要插入的信息在新的块中
                //移动要插入信息前的信息
                for (int i = 0; i < pos - numInfo / 2; ++i) {
                    infoNew.infoMem[i] = infoNow.infoMem[i + numInfo / 2];
                }
                //插入信息
                insertFunc(infoNew.infoMem[pos - numInfo / 2], toInsert);
                //移动要插入信息后的信息
                for (int i = pos - numInfo / 2 + 1; i < numInfo / 2; ++i) {
                    infoNew.infoMem[i] = infoNow.infoMem[i + numInfo / 2 - 1];
                }
            } else {//此时，要插入的信息在原来的块中
                //移动至新块的信息
                for (int i = 0; i < numInfo / 2; ++i) {
                    infoNew.infoMem[i] = infoNow.infoMem[i + numInfo / 2 - 1];
                }
                //移动要插入信息后的信息
                for (int i = numInfo / 2 - 2; i >= pos; --i) {
                    infoNow.infoMem[i + 1] = infoNow.infoMem[i];
                }
                //插入信息
                insertFunc(infoNow.infoMem[pos], toInsert);
            }
            //更新新节点
            nodeNew.prev = fileNode.tellg() / sizeof(node);
            nodeNew.next = nodeNow.next;
            nodeNew.number = sum - 1;
            nodeNew.stored = numInfo / 2 - 1;
            assignIndex(nodeNew.low, infoNew.infoMem[0].index);
            assignIndex(nodeNew.high, infoNew.infoMem[numInfo / 2 - 1].index);
            //更新原节点
            nodeNow.stored = numInfo / 2 - 1;
            assignIndex(nodeNow.low, infoNow.infoMem[0].index);
            assignIndex(nodeNow.high, infoNow.infoMem[numInfo / 2 - 1].index);
            nodeNow.next = sum - 1;
            fileNode.write(reinterpret_cast<char *>(&nodeNow), sizeof(node));
            //写入新节点
            fileNode.seekg(0, std::ios::end);
            fileNode.write(reinterpret_cast<char *>(&nodeNew), sizeof(node));
            //更新原信息块
            fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);
            fileInfo.write(reinterpret_cast<char *>(&infoNow), sizeof(info));
            //更新后一节点(若存在)
            if (nodeNew.next != -1) {
                fileNode.seekg(nodeNew.next * sizeof(node), std::ios::beg);
                //为避免开新变量，用nodeNow储存下一节点信息
                fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
                fileNode.seekg(-sizeof(node), std::ios::cur);
                nodeNow.prev = sum - 1;
                fileNode.write(reinterpret_cast<char *>(&nodeNow), sizeof(node));
            }
            //写入新信息块
            char init[sizeMax];
            fileInfo.seekg(0, std::ios::end);
            fileInfo.write(init, sizeMax);
            fileInfo.seekg(-sizeMax, std::ios::cur);
            fileInfo.write(reinterpret_cast<char *>(&infoNew), sizeof(info));
        }
        //结束，初始化
        fileNode.seekg(0, std::ios::beg);
        fileInfo.seekg(0, std::ios::beg);
        return;
    }
}

//eraseDelete的实现：
//           删除（eraseDelete）
//            |            |
//         块内够        块内不够
//        |            |       |
//  块内处理      前面存在         前面不存在
//              |     |          |      |
//          前面够  前面不够    后面存在  后面不存在
//            |       |       |    |        |
//           借块  合并到前面  后面够 后面不够  块内处理
//                            |    |
//                           借块 合并到原来
//
//优先向前并，因此文件开头总是非空的
template<class node, class info, class everyInfo>
void blockchain<node, info, everyInfo>::eraseDelete(int sto, int pos) {
    node nodeNow;
    info infoNow;
    if (sto == 0) {//表示只有一条信息，全部清空即可
        sum = 0;
        fileNode.close();
        fileNode.open(fileNodeName, std::ios::out);
        fileNode.close();
        fileNode.open(fileNodeName, std::ios::in | std::ios::out);
        fileInfo.close();
        fileInfo.open(fileInfoName, std::ios::out);
        fileInfo.close();
        fileInfo.open(fileInfoName, std::ios::in | std::ios::out);
        return;
    }
    fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
    fileNode.seekg(-sizeof(node), std::ios::cur);
    fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);
    fileInfo.read(reinterpret_cast<char *>(&infoNow), sizeof(info));
    fileInfo.seekg(-sizeof(info), std::ios::cur);
    if (sto >= numInfo / 2 || (nodeNow.prev == -1 && nodeNow.next == -1)) {
        //直接在信息块内进行操作
        --nodeNow.stored;
        for (int i = pos; i < sto; ++i) {
            infoNow.infoMem[i] = infoNow.infoMem[i + 1];
        }
        //更新节点上下限
        if (pos == 0) { assignIndex(nodeNow.low, infoNow.infoMem[0].index); }
        if (pos == sto) { assignIndex(nodeNow.high, infoNow.infoMem[pos - 1].index); }
        //写回信息
        fileNode.write(reinterpret_cast<char *>(&nodeNow), sizeof(node));
        fileInfo.write(reinterpret_cast<char *>(&infoNow), sizeof(info));
        //结束，初始化
        fileNode.seekg(0, std::ios::beg);
        fileInfo.seekg(0, std::ios::beg);
        return;
    } else {//需要借块或并块
        if (nodeNow.prev != -1) {//向前借信息或并块
            node nodePrev;
            info infoPrev;
            fileNode.seekg(nodeNow.prev * sizeof(node), std::ios::beg);
            fileNode.read(reinterpret_cast<char *>(&nodePrev), sizeof(node));
            fileNode.seekg(-sizeof(node), std::ios::cur);
            fileInfo.seekg(nodePrev.number * sizeMax, std::ios::beg);
            fileInfo.read(reinterpret_cast<char *>(&infoPrev), sizeof(info));
            fileInfo.seekg(-sizeof(info), std::ios::cur);
            if (nodePrev.stored >= numInfo / 2) {//向前借信息
                //移动信息
                for (int i = pos - 1; i >= 0; --i) {
                    infoNow.infoMem[i + 1] = infoNow.infoMem[i];
                }
                //在最前面插入借的信息
                infoNow.infoMem[0] = infoPrev.infoMem[nodePrev.stored];
                //更新节点范围
                assignIndex(nodeNow.low, infoNow.infoMem[0].index);
                if (pos == sto) { assignIndex(nodeNow.high, infoNow.infoMem[sto].index); }
                assignIndex(nodePrev.high, infoPrev.infoMem[--nodePrev.stored].index);
                //写回信息块和节点
                fileNode.write(reinterpret_cast<char *>(&nodePrev), sizeof(node));
                fileNode.seekg(nodePrev.next * sizeof(node), std::ios::beg);
                fileNode.write(reinterpret_cast<char *>(&nodeNow), sizeof(node));
                fileInfo.write(reinterpret_cast<char *>(&infoPrev), sizeof(info));
                fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);
                fileInfo.write(reinterpret_cast<char *>(&infoNow), sizeof(info));
                //结束，初始化
                fileNode.seekg(0, std::ios::beg);
                fileInfo.seekg(0, std::ios::beg);
                return;
            } else {//将现在的块向前合并
                //移动信息
                for (int i = 0; i < pos; ++i) {
                    infoPrev.infoMem[i + nodePrev.stored + 1] = infoNow.infoMem[i];
                }
                for (int i = pos + 1; i <= nodeNow.stored; ++i) {
                    infoPrev.infoMem[i + nodePrev.stored] = infoNow.infoMem[i];
                }
                nodePrev.stored += nodeNow.stored;
                //更新节点上限
                assignIndex(nodePrev.high, infoPrev.infoMem[nodePrev.stored].index);
                nodePrev.next = nodeNow.next;//更新节点间指向关系
                //写回前一节点，前一信息块
                fileNode.write(reinterpret_cast<char *>(&nodePrev), sizeof(node));
                fileInfo.write(reinterpret_cast<char *>(&infoPrev), sizeof(info));
                //若后一节点存在，更新
                if (nodeNow.next != -1) {
                    fileNode.seekg(nodeNow.next * sizeof(node), std::ios::beg);
                    //此处为避免再开新变量，用nodePrev存储下一节点信息
                    fileNode.read(reinterpret_cast<char *>(&nodePrev), sizeof(node));
                    fileNode.seekg(-sizeof(node), std::ios::cur);
                    nodePrev.prev = nodeNow.prev;
                    fileNode.write(reinterpret_cast<char *>(&nodePrev), sizeof(node));
                }
                //结束，初始化
                fileNode.seekg(0, std::ios::beg);
                fileInfo.seekg(0, std::ios::beg);
                return;
            }
        } else {//向后借信息，或后信息块并到本信息块中
            node nodeNext;
            info infoNext;
            fileNode.seekg(nodeNow.next * sizeof(node), std::ios::beg);
            fileNode.read(reinterpret_cast<char *>(&nodeNext), sizeof(node));
            fileNode.seekg(-sizeof(node), std::ios::cur);
            fileInfo.seekg(nodeNext.number * sizeMax, std::ios::beg);
            fileInfo.read(reinterpret_cast<char *>(&infoNext), sizeof(info));
            fileInfo.seekg(-sizeof(info), std::ios::cur);
            if (nodeNext.stored >= numInfo / 2) {//向后借信息
                //移动信息
                for (int i = pos; i < nodeNow.stored; ++i) {
                    infoNow.infoMem[i] = infoNow.infoMem[i + 1];
                }
                //在最后面插入借的信息
                infoNow.infoMem[nodeNow.stored] = infoNext.infoMem[0];
                //移动后面的信息
                for (int i = 0; i < nodeNext.stored; ++i) {
                    infoNext.infoMem[i] = infoNext.infoMem[i + 1];
                }
                //更新节点范围
                assignIndex(nodeNow.high, infoNow.infoMem[nodeNow.stored].index);
                if (pos == 0) { assignIndex(nodeNow.low, infoNow.infoMem[0].index); }
                --nodeNext.stored;
                assignIndex(nodeNext.low, infoNext.infoMem[0].index);
                //写回信息块和节点
                fileNode.write(reinterpret_cast<char *>(&nodeNext), sizeof(node));
                fileNode.seekg(nodeNext.prev * sizeof(node), std::ios::beg);
                fileNode.write(reinterpret_cast<char *>(&nodeNow), sizeof(node));
                fileInfo.write(reinterpret_cast<char *>(&infoNext), sizeof(info));
                fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);
                fileInfo.write(reinterpret_cast<char *>(&infoNow), sizeof(info));
                //结束，初始化
                fileNode.seekg(0, std::ios::beg);
                fileInfo.seekg(0, std::ios::beg);
                return;
            } else {//将后信息块并到本信息块中
                //移动信息
                for (int i = pos; i < nodeNow.stored; ++i) {
                    infoNow.infoMem[i] = infoNow.infoMem[i + 1];
                }
                for (int i = 0; i <= nodeNext.stored; ++i) {
                    infoNow.infoMem[i + nodeNow.stored] = infoNext.infoMem[i];
                }
                nodeNow.stored += nodeNext.stored;
                //更新节点范围
                if (pos == 0) { assignIndex(nodeNow.low, infoNow.infoMem[0].index); }
                assignIndex(nodeNow.high, infoNow.infoMem[nodeNow.stored].index);
                nodeNow.next = nodeNext.next;//更新节点间指向关系
                //写回现节点和现信息块
                fileNode.seekg(nodeNext.prev * sizeof(node), std::ios::beg);
                fileNode.write(reinterpret_cast<char *>(&nodeNow), sizeof(node));
                fileInfo.seekg(nodeNow.number * sizeMax, std::ios::beg);
                fileInfo.write(reinterpret_cast<char *>(&infoNow), sizeof(info));
                //若后一节点的后一节点存在，更新
                if (nodeNext.next != -1) {
                    fileNode.seekg(nodeNext.next * sizeof(node), std::ios::beg);
                    //此处为避免再开新变量，用nodeNow存储下一节点信息
                    fileNode.read(reinterpret_cast<char *>(&nodeNow), sizeof(node));
                    fileNode.seekg(-sizeof(node), std::ios::cur);
                    nodeNow.prev = nodeNext.prev;
                    fileNode.write(reinterpret_cast<char *>(&nodeNow), sizeof(node));
                }
                //结束，初始化
                fileNode.seekg(0, std::ios::beg);
                fileInfo.seekg(0, std::ios::beg);
                return;
            }
        }
    }
}


#endif //BOOKSTROREOFWGJ_BLOCKCHAIN_H
