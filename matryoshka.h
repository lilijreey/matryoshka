#ifndef MATRYOSHKA_H_  /*ͷ�ļ�Ԥ���� */
#define MATRYOSHKA_H_

/*
 *
 * =====================================================================================
 *
 *       Filename:  matryoshka.h
 *
 *    Description:  �������͵������Լ����Ƕ����ͺ���������
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
 *  Description:  �������Ͷ��壬��ɴӶ�ȡ������������������
 *                ��װ�йر����ͺ�����
 *
 * =====================================================================================
 */
class cMatrMap
{
public:
    cMatrMap (int r = 0, int c = 0, int m = 0) 
       : rowSize(r), colSize(c), maxGold(m) {}  /* constructor      */

    void InitcMatrMap() ;                        /* ��ʼ��cMatrMap */
    void FindMaxGold() ;                        /* Ѱ�����·��  */
    void ShowMaxPath() ;                       /* ��ʾ���·��  */

private:
    /* =============================  ������  ================================== */
    struct sNode //���ÿһ��ʮ��·�ڽڵ�Ľṹ
    {
        int row ;                       /* �ڵ����ڵ��У���0Ϊ��ʼ */
        int col ;                       /* �ڵ����ڵ��У���0Ϊ��ʼ */
        int value ;                     /* �ڵ�����޵����� */
        int index ;                     /* line�ж�Ӧ·�ڵ��±� */
        vector<vector<sNode>::iterator> link ;  /* ����뱾·�������Ĳ�������������
                                                   ��·�ڴ��·��ָ��sNode�ĵ����������� */
        sNode(int r, int c, int v, int i)
            : row(r), col(c), value(v), index(i) {}     /* constructor */
        bool operator<(const sNode& k) const 
        {   return this->value < k.value ;}             /*Ϊsort�㷨�ṩ��С�ں�����  */
    };

    // =====================================================================================
    //       struct:  sListNode
    //  Description:  �����һ���ڵ㿪ʼ�����·���������ýڵ�
    //                �ڵݹ�����ڵ�ʱ�ɴ���Ż�����ʱ��
    // =====================================================================================
    struct sListNode //�����һ���ڵ㿪ʼ�����·���������ýڵ�
    {
        int index ;                      /* �ڵ���line�е��±� */
        int gold ;                       /* �Ըýڵ㿪ʼ�����·�������������� */
        vector<int> path ;               /* ���·����·��˳����line�е��±� */

        sListNode(int i, int g, vector<int>::const_iterator begin,
                  vector<int>::const_iterator end) 
		  : index(i), gold(g), path(begin, end) {}      /* constructor */
    };

    /* ================================ ���ݳ�Ա  ============================== */
    int rowSize ;                        /* �������·�� */
    int colSize ;                        /* �ϱ����·�� */
    vector<sNode> line ;                 /* ����cross.txt��·�ھ���
                                            ����ַ��С���Դ洢 */
    int maxGold ;                        /* ���·�������������� */
    vector<int> max ;                /* ���·����·��˳����line�е��±� */

    /* ============================== ˽�к�����Ա ============================= */
    int GetLineIndex(int r, int c) const        /* ��ά�����±�ת���ɶ�Ӧ��һά�±� */
    {   return r * colSize + c ;}
    void GetNeighbor(vector<sNode>& line) ;     /* �õ����Լ������ص��ھ� */

    // ===  FUNCTION FindCore ==============================================================
    //  Description:  �������·���ĺ����㷨��pNodeָ��������������㣩
    //                �������·�ڵ���Ϣline��
    //                �Լ�Ҫ���ʵ�·����line�еĵ�����pNode
    //                ��ǰ��ѯ·��road�����Ը���·�ڿ�ʼ�����·���б� list
    // =====================================================================================
    void FindCore(const vector<sNode>& line, 
                  vector<sNode>::const_iterator pNode,
                  vector<int>& road, vector<sListNode>& list) ;

    // ===  FUNCTION FindCoreZero ==========================================================
    //  Description:  �������·���ĺ����㷨��pNodeָ�����������Ϊ�㣩
    //                value ����һ��·�����޵�����
    // =====================================================================================
    void FindCoreZero(const vector<sNode>& line, 
                  vector<sNode>::const_iterator pNode, int value ,
                  vector<int>& road, vector<sListNode>& list) ;

    // Description: ��line�в��Ҹ����ڵ� 
    vector<sListNode>::iterator GT(vector<sListNode>& list, int index) ;

}; /* -----  end of class cMatrMap  ----- */


#endif /* ͷ�ļ��� */


