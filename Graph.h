#ifndef GRAPH_H
#define GRAPH_H

#include "object.h"
#include "SharedPointer.h"
#include "Array.h"
#include "DynamicArray.h"
#include "LinkQueue.h"
#include "LinkStack.h"
#include "Sort.h"

namespace HBCLib
{

template <typename E>
struct Edge: public Object
{
    int b;
    int e;
    E data;

    Edge(int i = -1, int j = -1)
    {
        b = i;
        e = j;
    }

    Edge(int i, int j, const E& value)
    {
        b = i;
        e = j;
        data = value;
    }

    bool operator==(const Edge<E>& obj)
    {
        return (b == obj.b) && (e == obj.e);
    }

    bool operator!=(const Edge<E>& obj)
    {
        return !(*this == obj);
    }

    bool operator<(const Edge<E>& obj)
    {
        return (data < obj.data);
    }

    bool operator>(const Edge<E>& obj)
    {
        return (data > obj.data);
    }
};

template <typename V, typename E>
class Graph: public Object
{
protected:
    template <typename T>
    DynamicArray<T>* toArray(LinkQueue<T>& queue)
    {
        DynamicArray<T>* ret = new DynamicArray<T>(queue.length());

        if (ret)
        {
            for (int i = 0; i < ret->length(); ++i,queue.dequeue())
            {
                ret->set(i,queue.front());
            }
        }
        else
        {
            EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to create a new dynamic array...");
        }

        return ret;
    }

    SharedPointer< Array< Edge<E> > > getAllEdges()
    {
        DynamicArray< Edge<E> >* ret = NULL;

        if (asUndirected())
        {
            LinkQueue< Edge<E> > queue;
            for (int i = 0; i < vCount(); ++i)
            {
                for (int j = 0; j < vCount(); ++j)
                {
                    if (isAdjacent(i,j))
                    {
                        queue.enqueue(Edge<E>(i,j,getEdge(i,j)));
                    }
                }
            }

            ret = toArray(queue);
        }
        else
        {
            EXCEPTION_THROW(InvalidOperationException,"this function is for undirected graph only...");
        }

        return ret;
    }

    int find(Array<int>& array, int n)
    {
        while (array[n] != -1)
        {
            n = array[n];
        }

        return n;
    }

public:
    virtual V getVertex(int i) = 0;
    virtual bool getVertex(int i, V& value) = 0;
    virtual bool setVertex(int i, const V& value) = 0;
    virtual SharedPointer< Array<int> > getAdjacent(int i) = 0;
    virtual bool isAdjacent(int i, int j) = 0;
    virtual E getEdge(int i, int j) = 0;
    virtual bool getEdge(int i, int j, E& value) = 0;
    virtual bool setEdge(int i, int j, const E& value) = 0;
    virtual bool removeEdge(int i, int j) = 0;
    virtual int vCount() = 0;
    virtual int eCount() = 0;
    virtual int ID(int i) = 0;
    virtual int OD(int i) = 0;
    virtual int TD(int i)
    {
        return ID(i) + OD(i);
    }

    bool asUndirected()
    {
        bool ret = true;

        for (int i = 0; i < vCount(); ++i)
        {
            for (int j = 0; j < vCount(); ++j)
            {
                if (isAdjacent(i,j))
                {
                    ret = ret && (isAdjacent(j,i)) && (getEdge(i,j) == getEdge(j,i));
                }
            }
        }

        return ret;
    }

    SharedPointer< Array<int> > BFS(int i)
    {
        DynamicArray<int>* ret = NULL;

        if ((0 <= i) && (i < vCount()))
        {
            LinkQueue<int> q;
            LinkQueue<int> r;
            DynamicArray<bool> visited(vCount());

            for (int j = 0; j < visited.length(); ++j)
            {
                visited[j] = false;
            }

            q.enqueue(i);

            while (q.length() > 0)
            {
                int v = q.front();

                q.dequeue();

                if (!visited[v])
                {
                    SharedPointer< Array<int> > aj = getAdjacent(v);

                    for (int j = 0; j < aj->length(); ++j)
                    {
                        q.enqueue((*aj)[j]);
                    }

                    r.enqueue(v);

                    visited[v] = true;
                }
            }

            ret = toArray(r);
        }

        return ret;
    }

    SharedPointer< Array<int> > DFS(int i)
    {
        DynamicArray<int>* ret = NULL;

        if ((0 <= i) && (i < vCount()))
        {
            LinkStack<int> s;
            LinkQueue<int> r;
            DynamicArray<bool> visited(vCount());

            for (int j = 0; j < visited.length(); ++j)
            {
                visited[j] = false;
            }

            s.push(i);

            while (s.size() > 0)
            {
                int v = s.top();

                s.pop();

                if (!visited[v])
                {
                    SharedPointer< Array<int> > aj = getAdjacent(v);

                    for (int j = aj->length()-1; j >= 0; j--)
                    {
                        s.push((*aj)[j]);
                    }

                    r.enqueue(v);

                    visited[v] = true;
                }
            }

            ret = toArray(r);
        }

        return ret;
    }

    SharedPointer< Array< Edge<E> > > prim(const E& LIMIT,const bool MINIMUN = true)
    {
        LinkQueue< Edge<E> > ret;

        if (asUndirected())
        {
            DynamicArray<bool> mark(vCount());
            DynamicArray<E> cost(vCount());
            DynamicArray<int> Advex(vCount());
            SharedPointer< Array<int> > aj = NULL;
            int v = 0;
            bool end = false;

            for (int i = 0; i < vCount(); ++i)
            {
                mark[i] = false;
                cost[i] = LIMIT;
                Advex[i] = -1;
            }

            mark[v] = true;

            aj = getAdjacent(v);

            for (int i = 0; i < aj->length(); ++i)
            {
                cost[(*aj)[i]] = getEdge(v,(*aj)[i]);
                Advex[(*aj)[i]] = v;
            }

            while (!end)
            {
                E m = LIMIT;
                int k = -1;

                for (int j = 0; j < vCount(); ++j)
                {
                    if ((!mark[j]) && (MINIMUN ?(cost[j] < m) : (cost[j] > m)))
                    {
                        m = cost[j];
                        k = j;
                    }
                }

                end = (k == -1);

                if (!end)
                {
                    ret.enqueue(Edge<E> (Advex[k],k,getEdge(Advex[k], k)));

                    mark[k] = true;

                    aj = getAdjacent(k);

                    for (int i = 0; i < aj->length(); ++i)
                    {
                        if ( !mark[(*aj)[i]] && (MINIMUN ? getEdge(k,(*aj)[i]) < cost[(*aj)[i]] : getEdge(k,(*aj)[i]) > cost[(*aj)[i]]))
                        {
                            cost[(*aj)[i]] = getEdge(k,(*aj)[i]);
                            Advex[(*aj)[i]] = k;
                        }
                    }
                }
            }

        }
        else
        {
            EXCEPTION_THROW(InvalidOperationException,"prim operation is only for directed graph...");
        }

        if (ret.length() != vCount() -1)
        {
            EXCEPTION_THROW(InvalidOperationException,"no enough edge for prim operation...");
        }

        return toArray(ret);
    }

    SharedPointer< Array< Edge<E> > > kruskal(bool MINIMUM = true)
    {
        DynamicArray<int> p(eCount());
        SharedPointer< Array< Edge<E> > > edges = getAllEdges();
        LinkQueue< Edge<E> > ret;

        Sort::Shell< Edge<E> >(*edges,MINIMUM);

        for (int i = 0; i < p.length(); ++i)
        {
            p[i] = -1;
        }

        for (int i = 0; (i < edges->length()) && (ret.length() < vCount()-1); ++i)
        {
            int b = find(p,(*edges)[i].b);
            int e = find(p,(*edges)[i].e);

            if (b != e)
            {
                ret.enqueue((*edges)[i]);
                p[e] = b;
            }
        }

        if (ret.length() != vCount() -1)
        {
            EXCEPTION_THROW(InvalidOperationException,"no enough edge for kruskal operation...");
        }

        return toArray(ret);
    }

    SharedPointer< Array<int> > dijkstra(int i, int j, const E& LIMIT, bool MINIMUM = true)
    {
        LinkQueue<int> ret;

        if ((0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()))
        {
            DynamicArray<E> dist(vCount());
            DynamicArray<int> path(vCount());
            DynamicArray<bool> mark(vCount());

            for (int k = 0; k < vCount(); ++k)
            {
                path[k] = -1;
                mark[k] = false;

                dist[k] = (isAdjacent(i,k)) ? (path[k] = i, getEdge(i,k)) : LIMIT;
            }

            mark[i] = true;

            for (int k = 0; k < vCount(); ++k)
            {
                E m = LIMIT;
                int u = -1;

                for (int n = 0; n < vCount(); ++n)
                {
                    if ((!mark[n]) && (dist[n] < m))
                    {
                        m = dist[n];

                        u = n;
                    }
                }

                if (u == -1)
                {
                    break;
                }

                mark[u] = true;

                for (int n = 0; n < vCount(); ++n)
                {
                    if ((!mark[n]) && (isAdjacent(u,n)) && (getEdge(u,n) + dist[u] < dist[n]))
                    {
                        dist[n] = getEdge(u,n) + dist[u];
                        path[n] = u;
                    }
                }
            }
            LinkStack<int> stack;

            stack.push(j);

            while (path[j] != -1)
            {
                stack.push(path[j]);
                j = path[j];
            }

            while (stack.size() > 0)
            {
                ret.enqueue(stack.top());

                stack.pop();
            }
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"parameter <i,j> is out of index...");
        }

        if (ret.length() < 2)
        {
            EXCEPTION_THROW(InvalidOperationException,"cannot find the shortest path from i to j...");
        }

        return toArray(ret);
    }

    SharedPointer< Array<int> > floyd(int x, int y, const E& LIMIT)
    {
        LinkQueue<int> ret;

        if ((0 <= x) && (x < vCount()) && (0 <= y) && (y < vCount()))
        {
            DynamicArray< DynamicArray<E> > dist(vCount());
            DynamicArray< DynamicArray<int> > path(vCount());

            for (int i = 0; i < vCount(); ++i)
            {
                dist[i].resize(vCount());
                path[i].resize(vCount());
            }

            for (int i = 0; i < vCount(); ++i)
            {
                for (int j = 0; j < vCount(); ++j)
                {
                    path[i][j] = -1;
                    dist[i][j] = isAdjacent(i,j) ? (path[i][j] = j, getEdge(i,j)) : LIMIT;
                }
            }

            for (int k = 0; k < vCount(); ++k)
            {
                for (int i = 0; i < vCount(); ++i)
                {
                    for (int j = 0; j < vCount(); ++j)
                    {
                        if (dist[i][k] + dist[k][j] < dist[i][j])
                        {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            path[i][j] = path[i][k];
                        }
                    }
                }
            }

            while ((x != -1) && (x != y))
            {
                ret.enqueue(x);
                x = path[x][y];
            }

            if (x != -1)
            {
                ret.enqueue(x);
            }
        }
        else
        {
            EXCEPTION_THROW(IndexOutOfBoundsException,"parameter <x,y> is out of index...");
        }

        if (ret.length() < 2)
        {
            EXCEPTION_THROW(InvalidOperationException,"cannot find the shortest path from i to j...");
        }

        return toArray(ret);
    }
};

}

#endif // GRAPH_H
