/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  ��Ҫ����cMatrMap���󣬵��ó�Ա�������
 *                  �������·����
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
 *  Description:  ���ó�Ա������ɲ��ң����ṩЧ�ʲ���
 * =====================================================================================
 */
int
main() 
{
   // clock_t start, finish ;                   /* ������ */
   // start = clock() ;

    cMatrMap matr ;                             /* ����cMatrMap���� */
    matr.InitcMatrMap() ;                       /* ��ʼ��matr */
    matr.FindMaxGold() ;                        /* �������·�� */
    matr.ShowMaxPath() ;                      /* ������·�� */

   // finish = clock() ;
   //cout << endl << "Used time: " << (double)(finish - start )/ CLOCKS_PER_SEC << endl ;
    
    system("pause") ;
    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

