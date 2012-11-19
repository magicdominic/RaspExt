#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <list>
#include <algorithm>

template <class T>
class PriorityQueue
{
private:
    std::vector<T> m_container;
public:
    typedef typename std::vector<T>::iterator iterator;

    void push(T& x)
    {
        // pushes a new element into the heap
        m_container.push_back(x);
        std::push_heap(m_container.begin(), m_container.end());
    }

    const T& top() const
    {
        return m_container.front();
    }

    void pop()
    {
        std::pop_heap(m_container.begin(), m_container.end());
        m_container.pop_back();
    }

    bool empty() const
    {
        return m_container.empty();
    }

    bool modify(T& x)
    {
        for(iterator it = m_container.begin(); it != m_container.end(); it++)
        {
            if(*it == x)
            {
                // we overwrite the old value with the new one
                // obviously this only makes sense, if we overload the == operator
                *it = x;
                std::make_heap(m_container.begin(), m_container.end());
                return true;
            }
        }

        return false;
    }

    iterator begin()
    {
        return m_container.begin();
    }

    iterator end()
    {
        return m_container.end();
    }

    void remove(T& x)
    {
        // maybe this can be done in a better way
        for(iterator it = m_container.begin(); it != m_container.end(); it++)
        {
            if(*it == x)
            {
                // as there is no remove in std::vector, we need to iterate over the container
                // as soon as we have found the element, we can stop. There should not be any duplicates
                m_container.erase(it);
                break;
            }
        }

        std::make_heap(m_container.begin(), m_container.end());
    }

    void erase(iterator it)
    {
        m_container.erase(it);
        // maybe this can be done in a better way
        std::make_heap(m_container.begin(), m_container.end());
    }
};

#endif // PRIORITYQUEUE_H
