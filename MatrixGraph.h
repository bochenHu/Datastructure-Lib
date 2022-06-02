#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H

#include "Graph.h"
#include "exception.h"
#include "DynamicArray.h"
#include "SharedPointer.h"

namespace HBCLib
{

template <int N, typename V, typename E>
class MatrixGraph: public Graph<V,E>
{
protected:
    V* m_vertexes[N];
    E* m_edges[N][N];
    int m_ecount;
public:
    MatrixGraph()
    {
        for (int i = 0; i < vCount(); ++i)
        {
            m_vertexes[i] = NULL;

            for (int j = 0; j < vCount(); ++j)
            {
                m_edges[i][j] = NULL;
            }
        }

        m_ecount = 0;
    }

    V getVertex(int i)
    {
        V ret;

        if (!getVertex(i,ret))
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"parameter i is out of index...");
        }

        return ret;
    }

    bool getVertex(int i, V& value)
    {
        bool ret = ((0 <= i) && (i < vCount()));

        if (ret)
        {
            if (m_vertexes[i])
            {
                value = (* m_vertexes[i]);
            }
            else
            {
                EXCEPTION_THROW(InvalidParameterException,"No assigned value to this vertex...");
            }
        }

        return ret;
    }

    bool setVertex(int i, const V &value)
    {
        bool ret = ((0 <= i) && (i < vCount()));

        if (ret)
        {
            V* data = m_vertexes[i];
            if (data == NULL)
            {
                data = new V();
            }

            if (data)
            {
                *data = value;

                m_vertexes[i] = data;
            }
            else
            {
                EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to create a new vertex...");
            }
        }

        return ret;
    }

    SharedPointer< Array<int> > getAdjacent(int i)
    {
        DynamicArray<int>* ret = NULL;

        if (((0 <= i) && (i < vCount())))
        {
            int n = 0;

            for (int j = 0; j < vCount(); ++j)
            {
                if (m_edges[i][j] != NULL)
                {
                    n++;
                }
            }

            ret = new DynamicArray<int>(n);

            for (int j = 0, k = 0; j < vCount(); j++)
            {
                if (m_edges[i][j])
                {
                    ret->set(k++,j);
                }
            }
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"parameter i is out of index...");
        }

        return ret;
    }

    bool isAdjacent(int i, int j)
    {
        return (0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()) && (m_edges[i][j] != NULL);
    }

    E getEdge(int i, int j)
    {
        E ret;

        if (!getEdge(i,j,ret))
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"index parameter <i,j> is out of index...");
        }

        return ret;
    }

    bool getEdge(int i, int j, E& value)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()));

        if (ret)
        {
            if (m_edges[i][j])
            {
                value = (* m_edges[i][j]);
            }
            else
            {
                EXCEPTION_THROW(InvalidParameterException,"No assigned value to this vertex...");
            }
        }

        return ret;
    }

    bool setEdge(int i, int j, const E& value)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()));

        if (ret)
        {
            E* data = m_edges[i][j];

            if (data)
            {
                *data = value;
            }
            else
            {
                data = new E();

                if (data)
                {
                    *data = value;

                    m_edges[i][j] = data;

                    m_ecount++;
                }
                else
                {
                    EXCEPTION_THROW(NoEnoughMemoryException,"no memory to create a new Edge...");
                }
            }
        }

        return ret;
    }

    bool removeEdge(int i, int j)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()));

        if (ret)
        {
            E* data = m_edges[i][j];

            m_edges[i][j] = NULL;

            if (data)
            {
                m_ecount--;

                delete data;
            }
            else
            {
                EXCEPTION_THROW(InvalidParameterException,"Edge does not exist...");
            }
        }

        return ret;
    }

    int vCount()
    {
        return N;
    }

    int eCount()
    {
        return m_ecount;
    }

    int OD(int i)
    {
        int ret = 0;

        if ((0 <= i) && (i < vCount()))
        {
            for (int j = 0; j < vCount(); ++j)
            {
                if (m_edges[i][j])
                {
                    ret++;
                }
            }
        }

        return ret;
    }

    int ID(int i)
    {
        int ret = 0;

        if ((0 <= i) && (i < vCount()))
        {
            for (int j = 0; j < vCount(); ++j)
            {
                if (m_edges[j][i])
                {
                    ret++;
                }
            }
        }

        return ret;
    }

    ~MatrixGraph()
    {
        for (int i = 0; i < vCount(); ++i)
        {
            delete m_vertexes[i];

            for (int j = 0; j < vCount(); ++j)
            {
                delete m_edges[i][j];
            }
        }
    }
};

}

#endif // MATRIXGRAPH_H
