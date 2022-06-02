#ifndef SORT_H
#define SORT_H

#include "object.h"
#include "exception.h"
#include "Array.h"

namespace HBCLib
{

class Sort: public Object
{
protected:
    Sort();
    Sort(const Sort& s);
    Sort& operator= (const Sort& s);

    template <typename T>
    static void Swap(T& a, T& b)
    {
        T c(a);
        a = b;
        b = c;
    }

    template <typename T>
    static void Merge(T src[], T helper[], int begin, int mid, int end, bool minTOmax = true)
    {
        int i = begin;
        int j = mid+1;
        int k = begin;

        while ((i <= mid) && (j <= end))
        {
            if (minTOmax ? (src[i] < src[j]) : (src[i] > src[j]))
            {
                helper[k++] = src[i++];
            }
            else
            {
                helper[k++] = src[j++];
            }
        }

        while (i <= mid)
        {
            helper[k++] = src[i++];
        }

        while(j <= end)
        {
            helper[k++] = src[j++];
        }

        for (i = begin; i <= end; ++i)
        {
            src[i] = helper[i];
        }
    }

    template <typename T>
    static void Merge(T src[], T helper[], int begin, int end, bool minTOmax = true)
    {
        if (begin < end)
        {
            int mid = (begin+end) / 2;
            Merge(src, helper, begin, mid, minTOmax);
            Merge(src, helper, mid+1, end, minTOmax);
            Merge(src, helper, begin, mid, end, minTOmax);
        }
    }

    template <typename T>
    static int Partition(T src[], int begin, int end, bool minTOmax = true)
    {
        T pv = src[begin];

        while (begin < end)
        {
            while ((begin < end) && (minTOmax? src[end] > pv :(src[end] < pv)))
            {
                end--;
            }

            Swap(src[end], src[begin]);

            while ((begin < end) && (minTOmax? src[begin] <= pv :(src[begin] >= pv)))
            {
                begin++;
            }

            Swap(src[end], src[begin]);
        }

        src[begin] = pv;

        return begin;
    }

    template <typename T>
    static void Quick(T src[], int begin, int end, bool minTOmax = true)
    {
        if (begin < end)
        {
            int qviot = Partition(src, begin, end, minTOmax);

            Quick(src, begin, qviot-1, minTOmax);
            Quick(src, qviot+1, end, minTOmax);
        }
    }

public:
    template <typename T>
    static void Select(T a[], int len, bool minTOmax = true)
    {
        for (int i = 0; i < len; ++i)
        {
            int min = i;
            for (int j = i+1; j < len; ++j)
            {
                if (minTOmax ? (a[min] > a[j]): (a[min] < a[j]))
                {
                    min = j;
                }
            }

            if (min != i)
            {
                Swap(a[min],a[i]);
            }
        }
    }

    template <typename T>
    static void Insert(T a[], int len, bool minTOmax = true)
    {
        for (int i=1; i < len; ++i)
        {
            int k = i;
            T e = a[i];

            for (int j = i-1; (j>=0) && (minTOmax ? (a[j] > e): (a[j] < e)); j--)
            {
                a[j+1] = a[j];
                k = j;
            }

            if (k != i)
            {
                a[k] = e;
            }
        }
    }

    template <typename T>
    static void Bubble(T a[], int len, bool minTOmax = true)
    {
        bool exchange = true;

        for (int i = 0; i < len-1 && exchange; ++i)
        {
            exchange = false;
            for (int j = len-1; j > i; j--)
            {
                if (minTOmax ? (a[j] < a[j-1]): (a[j] > a[j-1]))
                {
                    Swap(a[j],a[j-1]);
                    exchange = true;
                }
            }
        }
    }

    template <typename T>
    static void Shell(T a[], int len, bool minTOmax = true)
    {
        int d = len;

        do
        {
            d = d/3 + 1;

            for (int i = d; i < len; i += d)
            {
                int k = i;
                T e = a[i];

                for (int j = i-d; (j>=0) && (minTOmax ? (a[j] > e): (a[j] < e)); j-=d)
                {
                    a[j+d] = a[j];
                    k = j;
                }

                if (k != i)
                {
                    a[k] = e;
                }
            }
        }
        while (d > 1);
    }

    /*template <typename T>
    static void Shell_bubble(T a[], int len, bool minTOmax = true)
    {
        int d = len;

        do
        {
            d = d/3 + 1;
            bool exchange = true;

            int x = len%d;
            int y = len - x;

            for (int i = y; i > 0 && exchange; i-=d)
            {
                exchange = false;
                for (int j = 0; j < i; j+=d)
                {
                    if (j+d < len && (minTOmax ? (a[j] > a[j+d]): (a[j] < a[j+d])))
                    {
                        Swap(a[j],a[j+d]);
                        exchange = true;
                    }
                }
            }
        }
        while (d > 1);
    }*/

    template <typename T>
    static void Merge(T a[], int len, bool minTOmax = true)
    {
        T* helper = new T[len];

        if (helper)
        {
            Merge(a, helper, 0, len-1, minTOmax);
        }
        else
        {
            EXCEPTION_THROW(NoEnoughMemoryException,"no enough memory to create helper...");
        }

        delete[] helper;
    }

    template <typename T>
    static void Quick(T a[], int len, bool minTOmax = true)
    {
        Quick(a, 0, len-1, minTOmax);
    }

    template <typename T>
    static void Select(Array<T>& a, bool minTOmax = true)
    {
        Select(a.array(),a.length(),minTOmax);
    }

    template <typename T>
    static void Insert(Array<T>& a, bool minTOmax = true)
    {
        Insert(a.array(),a.length(),minTOmax);
    }

    template <typename T>
    static void Bubble(Array<T>& a, bool minTOmax = true)
    {
        Bubble(a.array(),a.length(),minTOmax);
    }

    template <typename T>
    static void Shell(Array<T>& a, bool minTOmax = true)
    {
        Shell(a.array(),a.length(),minTOmax);
    }

    template <typename T>
    static void Quick(Array<T>& a, bool minTOmax = true)
    {
        Quick(a.array(),a.length(),minTOmax);
    }

    template <typename T>
    static void Merge(Array<T>& a, bool minTOmax = true)
    {
        Merge(a.array(),a.length(),minTOmax);
    }
};
}

#endif // SORT_H
