/*
 * =====================================================================================
 *
 *       Filename:  matryoshka.cpp
 *
 *    Description:  套娃结构的相关函数的定义和实现
 *                  完成从数据读入到路径输出的工作
 *
 *        Version:  1.0
 *        Created:  2010-5-23 22:52:11
 *       Compiler:  MS_VC2005
 *
 *
 * =====================================================================================
 */


#include	<iostream>
#include	<fstream>
#include	<vector>
#include    <algorithm>
#include    <cstdlib>
#include    <ctime>
#include    <cmath>

#include    "matryoshka.h"

using std::vector ;
using std::cout ;
using std::cin ;
using std::cerr ;
using std::endl ;
using std::ifstream ;
using std::ios_base ;
using std::ostream ;
using std::sort ;
using std::find ;
using std::back_inserter ;
using std::advance ;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GetNeighbor
 *  Description:  得到比自己套娃重的相邻路口
 *                对于重量为0的路口他的邻居都比他大（包括0）
 * =====================================================================================
 */
void
cMatrMap::GetNeighbor ( vector<sNode>& line )
{
  const int offset[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}} ;
  vector<sNode>::iterator iter = line.begin() ;

  for(; iter != line.end(); ++iter)
  {
    int r = iter->row ;                     /* 得到当前路口的行 */
    int c = iter->col ;                     /* 得到当前路口的列 */

    for(int i = 0; i < 4; ++i)
    {
      int orr= r + offset[i][0] ;         /* 得到相邻路口的行 */
      int oc = c + offset[i][1] ;         /* 得到相邻路口的列 */

      if(orr >= 0 && orr < rowSize &&
         oc >= 0 && oc < colSize)         /* 相邻路口必须有效 */
      {
        int oindex = GetLineIndex(orr, oc) ; /* 得到相邻路口再line中的下标 */
        if((iter->value < line[oindex].value)
           || (line[oindex].value == 0))     /* 套娃重量必须大于现路径的重量必须 
                                                或者为0 */
        {
          iter->link.push_back(line.begin() + oindex) ; /* 得到邻域，
                                                           将相邻节点的迭代器放入 */
        }
      }
    }
  }

}		/* -----  end of method cMatrMap::GetNeighbor  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  InitcMatrMap
 *  Description:  初始化cMatr，从文件读入数据存入相关数据
 *                有输入文件有效性检查，数据的数量要等与
 *                并且取得各个路口的有效邻域
 * =====================================================================================
 */
void
cMatrMap::InitcMatrMap ()
{
  ifstream infile("cross.txt") ;
  if(!infile.is_open())
  {
    cout << "error: unable to open cross.text" << endl ;
    exit(0) ;
  }

  infile >> rowSize;                          /* 读入数据  */
  infile >> colSize ;

  line.reserve(rowSize * colSize) ;            /* 预先分配做够大小，减少再次分配 */

  int count = 0 ;
  int v ;
  for(int r = 0; r < rowSize; ++r)
  {
    for(int c = 0; c < colSize; ++c)
    {
      if(infile >> v)
      {	cout.width(4) ;
        cout << v ;
        line.push_back(sNode(r, c, v, GetLineIndex(r,c))) ; 
        count++ ;                       /* 取得路口矩阵，并输出 */
      }
      else
        break ;
    }
    cout << endl ;
  }

  //检测文件的有效性
  if(rowSize == 0 || colSize == 0 || count != rowSize * colSize )
  {
    cout << "无效文件,请重新输入!" << endl ;
    exit(1) ;
  }

  infile.close() ;

  GetNeighbor(line) ;                         /* 得到相邻路口 */

}		/* -----  end of method cMatrMap::InitcMatrMap  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  cMatrMap
 *      Method:  cMatrMap :: FindCoreZero
 * Description:  当前路口套娃重量为0时，调用此函数
 *               找到通过此路口的最大路径
 *--------------------------------------------------------------------------------------
 */
void
cMatrMap::FindCoreZero (const vector<sNode>& line, 
                        vector<sNode>::const_iterator pNode,
                        int value, vector<int>& road,
                        vector<sListNode>& list)
{
  /* 当路口套娃为0时，必须保证零的一个非零邻域
   * 大于传入的value值，2不能重复已走过路径 */
  typedef vector<vector<sNode>::iterator>::const_iterator 
      pLink ;                         /* pLink是一个以vector<sNode>的迭代器
                                         为类型的向量的长迭代器      */
  road.push_back(pNode->index) ;      /* 将当前路口套娃重量存入当前路径 */
  road[0] += pNode->value ;           /* 路径套娃总量累加，
                                         road[0]记录当前路径的套娃总重量
                                         路径从road[1]开始记录 */

  pLink citer = (pNode->link).begin() ;       /* 赋值pNodelink域的首节点，准备递归遍历*/
  for(; citer != (pNode->link).end(); ++citer)
  {
    /* 当下个路口为0时，并且不是没有走过的 */
    if(0 == (*citer)->value &&
       road.end() == find(road.begin()+1, road.end(), (*citer)->index))
    {
      FindCoreZero(line, *citer, value, road, list) ; /* 访问下个路口 */
    }
    else
    {
      if(value < (*citer)->value)         /* 当下个路口的重量大于value进入下个路口 */
        FindCore(line, *citer, road, list) ;
    }
  }

  if(maxGold < road[0])               /* 比较当前路径比已走过的最大路径大，更新最大路径 */
  {
    max.clear() ;                           /* 清空原来的最大路径 */
    copy(road.begin()+1, road.end(), back_inserter(max)) ; /* 更新 */
    maxGold = road[0] ;
  }

  road[0] -= pNode->value ;       /* 当前节点已经递归完成 即将跳出，从road中删除pNode */
  road.pop_back() ;
}		/* -----  end of method cMatrMap::FindCoreZero  ----- */



/*
 *--------------------------------------------------------------------------------------
 *       Class:  cMatrMap
 *      Method:  cMatrMap :: GT
 * Description:  查找以某个路口为起点的最大路径是否在line表中
 *               若有责直接将当前road与查出的最大路径链接，与max比较
 *               放回查找路口再line中的迭代器，若没做到责返回line.end()
 *               不用递归的查找，使得程序效率大大提高
 *--------------------------------------------------------------------------------------
 */

vector<cMatrMap::sListNode>::iterator
cMatrMap::GT ( vector<sListNode>& list, int index )
{
  vector<sListNode>::iterator it = list.begin() ;
  for(; it != list.end(); ++it)
  {
    if(index == it->index)
      break ;
  }
  return it ;
}		/* -----  end of method cMatrMap::GT  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  cMatrMap
 *      Method:  cMatrMap :: FindCore
 * Description:  对于非零路口的路径查询
 *--------------------------------------------------------------------------------------
 */
void
cMatrMap::FindCore(const vector<sNode>& line,
                   vector<sNode>::const_iterator pNode,
                   vector<int>& road, vector<sListNode>& list)
{
  typedef vector<vector<sNode>::iterator>::const_iterator 
      pLink ;                         /* pLink是一个以vector<sNode>的迭代器
                                         为类型的向量的长迭代器      */
  pLink citer = (pNode->link).begin() ;       /* 传入pNode的link域的首节点 */

  int before = road[0] ;                      /* 记录road(不包括当pNode)套娃总量 */
  /* 取得pNode在list中的迭代器 */
  vector<sListNode>::iterator it = GT(list, pNode->index) ;
  if(it != list.end())                        /* list中有pNode */
  {
    road[0] += it->gold ;

    if(maxGold < road[0])               /* 比较当前路径比已走过的最大路径大，更新最大路径 */
    {
      max.clear() ;                           /* 清空原来的最大路径 */
      copy(road.begin()+1, road.end(), back_inserter(max)) ; /* 更新 */
      copy(it->path.begin(), it->path.end(), back_inserter(max)) ; 
      maxGold = road[0] ;
    }
    road[0] -= it->gold ;       /* 当前节点已经递归完成 即将跳出，从road中删除pNode */
  }
  else
  {
    road.push_back(pNode->index) ;
    road[0] += pNode->value ;               /* 将pNode插入road，增加road的重量 */

    for(; citer != (pNode->link).end(); ++citer)
    {
      if(0 == (*citer)->value)            /* pNode的相邻路口为0 */
      {                                   /* 不是已走过的路口 */
        if(road.end() == find(road.begin()+1, road.end(), (*citer)->index))
        {
          FindCoreZero(line, *citer, pNode->value, road, list) ; 
        }
      }
      else
      {
        FindCore(line, *citer, road, list) ;
      }
    }

    if(maxGold < road[0])               /* 比较当前路径比已走过的最大路径大，更新最大路径 */
    {
      max.clear() ;                           /* 清空原来的最大路径 */
      copy(road.begin()+1, road.end(), back_inserter(max)) ; /* 更新 */
      maxGold = road[0] ;
    }

    /* 将以pNode为起始路口的最大路径保存到line */
    vector<int>::iterator itt = 
        find(max.begin(), max.end(), pNode->index) ; //在最大路径中招pNode节点
    /* 构造pNode最大路径信息的sListNode对象 */
    sListNode aa(GetLineIndex(pNode->row, pNode->col),
                 maxGold - before, itt, max.end()) ;
    list.push_back(aa) ;                    /* 写入list */

    road[0] -= pNode->value ;       /* 当前节点已经递归完成 即将跳出，从road中删除pNode */
    road.pop_back() ;
  }
}		/* -----  end of method cMatrMap::FindCore  ----- */



/*
 *--------------------------------------------------------------------------------------
 *       Class:  cMatrMap
 *      Method:  cMatrMap :: FindMaxGold
 * Description:  cMatMap的接口函数，找出最大路径，并输出。
 *               由他从00点进入，调用FindCroe, FindCroeZero
 *--------------------------------------------------------------------------------------
 */
void
cMatrMap::FindMaxGold()
{
  /* 建立road用来记录路径road[0]为路径套娃的总重量 初始化road[0]=0 */
  vector<int> road(1, 0) ;
  vector<sListNode> list ;          /* 记录以各路口为初始路口的最大路径 */
  list.reserve(rowSize * colSize) ;           /* 初始化空间大小 */

  FindCore(line, line.begin(),road, list) ;   /* 从00开始递归查找 */


}		/* -----  end of method CMatrMap::FindMaxGold  ----- */

void
cMatrMap::ShowMaxPath()                        /* 输出最大路径 */
{
  cout << endl << "Max Road" << endl ;
  vector<sNode>::const_iterator begin = line.begin();

  int i = 0 ;
  for(vector<int>::const_iterator cit = max.begin();
      cit != max.end(); cit++)
  {
    cout << "[" ; 
    cout << (*cit+begin)->row << "," ;
    cout << (*cit+begin)->col << "]" ; 
    cout << (*cit + begin)->value << " ";
    if(++i %5 == 0)
      cout << endl ;
  }
  cout << endl ;
}
