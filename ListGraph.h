#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include "Graph.h"
#include "SharedPointer.h"
#include "DynamicArray.h"
#include "exception.h"
#include "LinkList.h"

namespace HBCLib
{

template <typename V, typename E>
class ListGraph: public Graph<V,E>
{
protected:
    struct Vertex: public Object
    {
        V* data;
        LinkList<Edge<E> > edge;

        Vertex()
        {
            data = NULL;
        }
    };

    LinkList<Vertex*> m_list;
public:
    ListGraph(unsigned int n = 0)
    {
        for (unsigned int i = 0; i < n; ++i)
        {
            addVertex();
        }
    }

    int addVertex()
    {
        int ret = -1;

        Vertex* vertex = new Vertex();

        if (vertex)
        {
            m_list.insert(vertex);

            ret = m_list.length()-1;
        }
        else
        {
            EXCEPTION_THROW(NoEnoughMemoryException,"no memory to create a new vertex...");
        }

        return ret;
    }

    int addVertex(const V& value)
    {
        int ret = addVertex();

        if (ret >= 0)
        {
            setVertex(ret,value);
        }

        return ret;
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
            Vertex* vertex = m_list.get(i);
            V* v = vertex->data;

            if (v)
            {
                value = *(v);
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
            Vertex* vertex = m_list.get(i);
            V* data = vertex->data;

            if (data == NULL)
            {
                data = new V();
            }

            if (data != NULL)
            {
                *data = value;

                vertex->data = data;
            }
            else
            {
                EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to create a new vertex...");
            }
        }

        return ret;
    }

    void removeVertex()
    {
        int len = m_list.length();

        if (0 < len)
        {
            Vertex* vertex = m_list.get(len-1);

            if (m_list.remove(len-1))
            {
                int i = 0;
                for (m_list.move(0), i = 0; !m_list.end(); m_list.next(),++i)
                {
                    Vertex* v = m_list.current();
                    LinkList<Edge<E> >& le = v->edge;
                    int pos = le.find(Edge<E>(i,len-1));

                    if (0 <= pos)
                    {
                        le.remove(pos);
                    }
                }
            }

            delete vertex->data;
            delete vertex;
        }
        else
        {
            EXCEPTION_THROW(InvalidOperationException,"no vertex at current graph...");
        }
    }

    SharedPointer< Array<int> > getAdjacent(int i)
    {
        DynamicArray<int>* ret = NULL;

        if (((0 <= i) && (i < vCount())))
        {
            Vertex* vertex = m_list.get(i);

            LinkList< Edge<E> >& le = vertex->edge;

            ret = new DynamicArray<int>(le.length());

            if (ret)
            {
                int k = 0;
                for (le.move(0), k = 0; !le.end(); le.next())
                {
                    ret->set(k++,le.current().e);
                }
            }
            else
            {
                EXCEPTION_THROW(NoEnoughMemoryException,"no memory to create a new dynamic array...");
            }
        }

        return ret;
    }

    bool isAdjacent(int i, int j)
    {
        return (0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()) && (m_list.get(i)->edge.find(Edge<E>(i,j)) >= 0);
    }

    E getEdge(int i, int j)
    {
        E ret;

        if (!getEdge(i,j,ret))
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"parameter <i,j> is out of index...");
        }

        return ret;
    }

    bool getEdge(int i, int j, E& value)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()));

        if (ret)
        {
            Vertex* vertex = m_list.get(i);
            LinkList< Edge<E> >& le = vertex->edge;

            int pos = le.find(Edge<E>(i,j));

            if (0 <= pos)
            {
                value = (le.get(pos).data);
            }
            else
            {
                EXCEPTION_THROW(InvalidOperationException,"no value assigned to this edge...");
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
            Vertex* vertex = m_list.get(i);
            LinkList< Edge<E> >& le = vertex->edge;

            int pos = le.find(Edge<E>(i,j));

            if (0 <= pos)
            {
                ret = le.set(pos,Edge<E>(i,j,value));
            }
            else
            {
                ret = le.insert(Edge<E>(i,j,value));
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
            Vertex* vertex = m_list.get(i);
            LinkList< Edge<E> >& le = vertex->edge;
            int len = le.length();

            if (0 < len)
            {
                int pos = le.find(Edge<E>(i,j));

                if (0 <= pos)
                {
                    ret = le.remove(pos);
                }
                else
                {
                    EXCEPTION_THROW(InvalidOperationException,"no value assigned to this edge...");
                }
            }
        }

        return ret;
    }

    int vCount()
    {
        return m_list.length();
    }

    int eCount()
    {
        int edge = 0;


        for (m_list.move(0); !m_list.end(); m_list.next())
        {
            Vertex* vertex = m_list.current();
            LinkList<Edge<E> >& le = vertex->edge;

            edge += le.length();
        }

        return edge;
    }

    int OD(int i)
    {
        bool ret = ((0 <= i) && (i < vCount()));

        int od = 0;

        if (ret)
        {
            Vertex* vertex = m_list.get(i);
            LinkList<Edge<E> >& le = vertex->edge;
            od = le.length();
        }

        return od;
    }

    int ID(int i)
    {
        bool ret = ((0 <= i) && (i < vCount()));

        int id = 0;

        if (ret)
        {
            for (m_list.move(0); !m_list.end(); m_list.next())
            {
                Vertex* vertex = m_list.current();
                LinkList<Edge<E> >& le = vertex->edge;

                for (le.move(0); !le.end();le.next())
                {
                    if (le.current().e == i)
                    {
                        id++;
                        break;
                    }
                }
            }
        }

        return id;
    }

    ~ListGraph()
    {
        while(m_list.length()>0)
        {
            Vertex* toDel = m_list.get(0);

            m_list.remove(0);
            delete toDel->data;
            delete toDel;
        }
    }
};

}

#endif // LISTGRAPH_H
