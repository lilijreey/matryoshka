/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  主要建立cMatrMap对象，调用成员方法完成
 *                  查找最大路径。
 *
 *        Version:  1.0
 *        Created:  2010-5-23 14:36:38
 *       Compiler:  MS_VC2005
 *
 *         Author:  Zhao.Li (lili), lilijreey@126.com
 *
 * =====================================================================================
 */


#include	<iostream>
#include	<ctime>

#include    "matryoshka.h"

using std::cout ;
using std::cin ;
using std::endl ;


#include	<stdlib.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  调用成员方法完成查找，并提供效率测试
 * =====================================================================================
 */
int
main() 
{
   // clock_t start, finish ;                   /* 测试用 */
   // start = clock() ;

    cMatrMap matr ;                             /* 构造cMatrMap对象 */
    matr.InitcMatrMap() ;                       /* 初始化matr */
    matr.FindMaxGold() ;                        /* 查找最大路径 */
    matr.ShowMaxPath() ;                      /* 输出最大路径 */

   // finish = clock() ;
   //cout << endl << "Used time: " << (double)(finish - start )/ CLOCKS_PER_SEC << endl ;
    
    system("pause") ;
    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

