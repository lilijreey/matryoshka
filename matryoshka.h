#ifndef MATRYOSHKA_H_  /*头文件预处理 */
#define MATRYOSHKA_H_

/*
 *
 * =====================================================================================
 *
 *       Filename:  matryoshka.h
 *
 *    Description:  套娃类型的声明以及相关嵌套类和函数的声明
 *
 *        Version:  1.0
 *        Created:  2010-5-23 22:59:26
 *       Revision:  ZhaoLi
 *       Compiler:  MS_VC2005
 *
 *
 * =====================================================================================
 */


#include	<vector>
#include	<iostream>

using       std::vector ;
using       std::ostream ;


/*
 * =====================================================================================
 *        Class:  cMatrMap
 *  Description:  套娃类型定义，完成从读取到计算输出等相关任务
 *                封装有关变量和函数。
 *
 * =====================================================================================
 */
class cMatrMap
{
public:
    cMatrMap (int r = 0, int c = 0, int m = 0) 
       : rowSize(r), colSize(c), maxGold(m) {}  /* constructor      */

    void InitcMatrMap() ;                        /* 初始化cMatrMap */
    void FindMaxGold() ;                        /* 寻找最大路径  */
    void ShowMaxPath() ;                       /* 显示最大路径  */

private:
    /* =============================  襄套类  ================================== */
    struct sNode //存放每一个十字路口节点的结构
    {
        int row ;                       /* 节点所在的行，以0为起始 */
        int col ;                       /* 节点所在的列，以0为起始 */
        int value ;                     /* 节点的套娃的重量 */
        int index ;                     /* line中对应路口的下标 */
        vector<vector<sNode>::iterator> link ;  /* 存放与本路口相连的并且套娃重量比
                                                   本路口大的路口指向sNode的迭代器的向量 */
        sNode(int r, int c, int v, int i)
            : row(r), col(c), value(v), index(i) {}     /* constructor */
        bool operator<(const sNode& k) const 
        {   return this->value < k.value ;}             /*为sort算法提供的小于号重载  */
    };

    // =====================================================================================
    //       struct:  sListNode
    //  Description:  存放以一个节点开始的最大路径，包括该节点
    //                在递归遍历节点时可大大优化查找时间
    // =====================================================================================
    struct sListNode //存放以一个节点开始的最大路径，包括该节点
    {
        int index ;                      /* 节点在line中的下标 */
        int gold ;                       /* 以该节点开始的最大路径的套娃总重量 */
        vector<int> path ;               /* 最大路径各路口顺序在line中的下标 */

        sListNode(int i, int g, vector<int>::const_iterator begin,
                  vector<int>::const_iterator end) 
		  : index(i), gold(g), path(begin, end) {}      /* constructor */
    };

    /* ================================ 数据成员  ============================== */
    int rowSize ;                        /* 东西向道路数 */
    int colSize ;                        /* 南北向道路数 */
    vector<sNode> line ;                 /* 将从cross.txt中路口矩阵
                                            按地址大小线性存储 */
    int maxGold ;                        /* 最大路径的套娃总重量 */
    vector<int> max ;                /* 最大路径各路口顺序在line中的下标 */

    /* ============================== 私有函数成员 ============================= */
    int GetLineIndex(int r, int c) const        /* 二维矩阵下标转换成对应的一维下标 */
    {   return r * colSize + c ;}
    void GetNeighbor(vector<sNode>& line) ;     /* 得到比自己套娃重的邻居 */

    // ===  FUNCTION FindCore ==============================================================
    //  Description:  查找最大路径的核心算法（pNode指向的套娃重量非零）
    //                传入各个路口的信息line，
    //                以及要访问的路口在line中的迭代器pNode
    //                当前查询路径road，和以各个路口开始的最大路径列表 list
    // =====================================================================================
    void FindCore(const vector<sNode>& line, 
                  vector<sNode>::const_iterator pNode,
                  vector<int>& road, vector<sListNode>& list) ;

    // ===  FUNCTION FindCoreZero ==========================================================
    //  Description:  查找最大路径的核心算法（pNode指向的套娃重量为零）
    //                value 是上一个路口套娃的重量
    // =====================================================================================
    void FindCoreZero(const vector<sNode>& line, 
                  vector<sNode>::const_iterator pNode, int value ,
                  vector<int>& road, vector<sListNode>& list) ;

    // Description: 在line中查找给定节点 
    vector<sListNode>::iterator GT(vector<sListNode>& list, int index) ;

}; /* -----  end of class cMatrMap  ----- */


#endif /* 头文件完 */


