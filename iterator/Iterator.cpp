#include <iostream>
#include "Iterator.h"

ConcreateAggregate::ConcreateAggregate(int nSize) : m_nSize(nSize), m_pData(NULL)
{
    m_pData = new DATA[m_nSize];
    
    for (int i = 0; i < nSize; ++i)
    {
        m_pData[i] = i;
    }
}

ConcreateAggregate::~ConcreateAggregate()
{
    delete [] m_pData;
    m_pData = NULL;
}

Iterater* ConcreateAggregate::CreateIterater(Aggregate *pAggregate)
{
    return new ConcreateIterater(this);
}

int ConcreateAggregate::GetSize()
{
    return m_nSize;
}
