/*
 * =====================================================================================
 *
 *       Filename:  matryoshka.cpp
 *
 *    Description:  ���޽ṹ����غ����Ķ����ʵ��
 *                  ��ɴ����ݶ��뵽·������Ĺ���
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
 *  Description:  �õ����Լ������ص�����·��
 *                ��������Ϊ0��·�������ھӶ������󣨰���0��
 * =====================================================================================
 */
void
cMatrMap::GetNeighbor ( vector<sNode>& line )
{
  const int offset[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}} ;
  vector<sNode>::iterator iter = line.begin() ;

  for(; iter != line.end(); ++iter)
  {
    int r = iter->row ;                     /* �õ���ǰ·�ڵ��� */
    int c = iter->col ;                     /* �õ���ǰ·�ڵ��� */

    for(int i = 0; i < 4; ++i)
    {
      int orr= r + offset[i][0] ;         /* �õ�����·�ڵ��� */
      int oc = c + offset[i][1] ;         /* �õ�����·�ڵ��� */

      if(orr >= 0 && orr < rowSize &&
         oc >= 0 && oc < colSize)         /* ����·�ڱ�����Ч */
      {
        int oindex = GetLineIndex(orr, oc) ; /* �õ�����·����line�е��±� */
        if((iter->value < line[oindex].value)
           || (line[oindex].value == 0))     /* �����������������·������������ 
                                                ����Ϊ0 */
        {
          iter->link.push_back(line.begin() + oindex) ; /* �õ�����
                                                           �����ڽڵ�ĵ��������� */
        }
      }
    }
  }

}		/* -----  end of method cMatrMap::GetNeighbor  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  InitcMatrMap
 *  Description:  ��ʼ��cMatr�����ļ��������ݴ����������
 *                �������ļ���Ч�Լ�飬���ݵ�����Ҫ����
 *                ����ȡ�ø���·�ڵ���Ч����
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

  infile >> rowSize;                          /* ��������  */
  infile >> colSize ;

  line.reserve(rowSize * colSize) ;            /* Ԥ�ȷ���������С�������ٴη��� */

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
        count++ ;                       /* ȡ��·�ھ��󣬲���� */
      }
      else
        break ;
    }
    cout << endl ;
  }

  //����ļ�����Ч��
  if(rowSize == 0 || colSize == 0 || count != rowSize * colSize )
  {
    cout << "��Ч�ļ�,����������!" << endl ;
    exit(1) ;
  }

  infile.close() ;

  GetNeighbor(line) ;                         /* �õ�����·�� */

}		/* -----  end of method cMatrMap::InitcMatrMap  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  cMatrMap
 *      Method:  cMatrMap :: FindCoreZero
 * Description:  ��ǰ·����������Ϊ0ʱ�����ô˺���
 *               �ҵ�ͨ����·�ڵ����·��
 *--------------------------------------------------------------------------------------
 */
void
cMatrMap::FindCoreZero (const vector<sNode>& line, 
                        vector<sNode>::const_iterator pNode,
                        int value, vector<int>& road,
                        vector<sListNode>& list)
{
  /* ��·������Ϊ0ʱ�����뱣֤���һ����������
   * ���ڴ����valueֵ��2�����ظ����߹�·�� */
  typedef vector<vector<sNode>::iterator>::const_iterator 
      pLink ;                         /* pLink��һ����vector<sNode>�ĵ�����
                                         Ϊ���͵������ĳ�������      */
  road.push_back(pNode->index) ;      /* ����ǰ·�������������뵱ǰ·�� */
  road[0] += pNode->value ;           /* ·�����������ۼӣ�
                                         road[0]��¼��ǰ·��������������
                                         ·����road[1]��ʼ��¼ */

  pLink citer = (pNode->link).begin() ;       /* ��ֵpNodelink����׽ڵ㣬׼���ݹ����*/
  for(; citer != (pNode->link).end(); ++citer)
  {
    /* ���¸�·��Ϊ0ʱ�����Ҳ���û���߹��� */
    if(0 == (*citer)->value &&
       road.end() == find(road.begin()+1, road.end(), (*citer)->index))
    {
      FindCoreZero(line, *citer, value, road, list) ; /* �����¸�·�� */
    }
    else
    {
      if(value < (*citer)->value)         /* ���¸�·�ڵ���������value�����¸�·�� */
        FindCore(line, *citer, road, list) ;
    }
  }

  if(maxGold < road[0])               /* �Ƚϵ�ǰ·�������߹������·���󣬸������·�� */
  {
    max.clear() ;                           /* ���ԭ�������·�� */
    copy(road.begin()+1, road.end(), back_inserter(max)) ; /* ���� */
    maxGold = road[0] ;
  }

  road[0] -= pNode->value ;       /* ��ǰ�ڵ��Ѿ��ݹ���� ������������road��ɾ��pNode */
  road.pop_back() ;
}		/* -----  end of method cMatrMap::FindCoreZero  ----- */



/*
 *--------------------------------------------------------------------------------------
 *       Class:  cMatrMap
 *      Method:  cMatrMap :: GT
 * Description:  ������ĳ��·��Ϊ�������·���Ƿ���line����
 *               ������ֱ�ӽ���ǰroad���������·�����ӣ���max�Ƚ�
 *               �Żز���·����line�еĵ���������û�����𷵻�line.end()
 *               ���õݹ�Ĳ��ң�ʹ�ó���Ч�ʴ�����
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
 * Description:  ���ڷ���·�ڵ�·����ѯ
 *--------------------------------------------------------------------------------------
 */
void
cMatrMap::FindCore(const vector<sNode>& line,
                   vector<sNode>::const_iterator pNode,
                   vector<int>& road, vector<sListNode>& list)
{
  typedef vector<vector<sNode>::iterator>::const_iterator 
      pLink ;                         /* pLink��һ����vector<sNode>�ĵ�����
                                         Ϊ���͵������ĳ�������      */
  pLink citer = (pNode->link).begin() ;       /* ����pNode��link����׽ڵ� */

  int before = road[0] ;                      /* ��¼road(��������pNode)�������� */
  /* ȡ��pNode��list�еĵ����� */
  vector<sListNode>::iterator it = GT(list, pNode->index) ;
  if(it != list.end())                        /* list����pNode */
  {
    road[0] += it->gold ;

    if(maxGold < road[0])               /* �Ƚϵ�ǰ·�������߹������·���󣬸������·�� */
    {
      max.clear() ;                           /* ���ԭ�������·�� */
      copy(road.begin()+1, road.end(), back_inserter(max)) ; /* ���� */
      copy(it->path.begin(), it->path.end(), back_inserter(max)) ; 
      maxGold = road[0] ;
    }
    road[0] -= it->gold ;       /* ��ǰ�ڵ��Ѿ��ݹ���� ������������road��ɾ��pNode */
  }
  else
  {
    road.push_back(pNode->index) ;
    road[0] += pNode->value ;               /* ��pNode����road������road������ */

    for(; citer != (pNode->link).end(); ++citer)
    {
      if(0 == (*citer)->value)            /* pNode������·��Ϊ0 */
      {                                   /* �������߹���·�� */
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

    if(maxGold < road[0])               /* �Ƚϵ�ǰ·�������߹������·���󣬸������·�� */
    {
      max.clear() ;                           /* ���ԭ�������·�� */
      copy(road.begin()+1, road.end(), back_inserter(max)) ; /* ���� */
      maxGold = road[0] ;
    }

    /* ����pNodeΪ��ʼ·�ڵ����·�����浽line */
    vector<int>::iterator itt = 
        find(max.begin(), max.end(), pNode->index) ; //�����·������pNode�ڵ�
    /* ����pNode���·����Ϣ��sListNode���� */
    sListNode aa(GetLineIndex(pNode->row, pNode->col),
                 maxGold - before, itt, max.end()) ;
    list.push_back(aa) ;                    /* д��list */

    road[0] -= pNode->value ;       /* ��ǰ�ڵ��Ѿ��ݹ���� ������������road��ɾ��pNode */
    road.pop_back() ;
  }
}		/* -----  end of method cMatrMap::FindCore  ----- */



/*
 *--------------------------------------------------------------------------------------
 *       Class:  cMatrMap
 *      Method:  cMatrMap :: FindMaxGold
 * Description:  cMatMap�Ľӿں������ҳ����·�����������
 *               ������00����룬����FindCroe, FindCroeZero
 *--------------------------------------------------------------------------------------
 */
void
cMatrMap::FindMaxGold()
{
  /* ����road������¼·��road[0]Ϊ·�����޵������� ��ʼ��road[0]=0 */
  vector<int> road(1, 0) ;
  vector<sListNode> list ;          /* ��¼�Ը�·��Ϊ��ʼ·�ڵ����·�� */
  list.reserve(rowSize * colSize) ;           /* ��ʼ���ռ��С */

  FindCore(line, line.begin(),road, list) ;   /* ��00��ʼ�ݹ���� */


}		/* -----  end of method CMatrMap::FindMaxGold  ----- */

void
cMatrMap::ShowMaxPath()                        /* ������·�� */
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
