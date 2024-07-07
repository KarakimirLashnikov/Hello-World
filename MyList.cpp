#include <iostream>
#include <stdexcept>
using namespace std;

template<typename T>
struct node
{
    node<T>* m_prev;
    T m_data;
    node<T>* m_next;

    node<T>(node<T>* prev, const T& data, node<T>* next);
};

template<typename T>
node<T>::node(node<T>* prev, const T& data, node<T>* next)
{
    m_prev = prev;
    m_data = data;
    m_next = next;
}

template<typename T>
class MyList
{
private:
    node<T>* m_head;
    node<T>* m_tail;

public:
    MyList();
    MyList(MyList const& that);
    ~MyList();

    bool empty() const;
    void push_front(const T& data);
    void pop_front();
    T& front();
    const T& front() const;
    void push_back(const T& data);
    void pop_back();
    T& back();
    const T& back() const;
    void clear();
    size_t size() const;

    class iterator;
    iterator begin();
    iterator end();
    iterator insert(const iterator &loc, const T &data);

    template <typename U>
    friend ostream &operator<<(ostream &os, const MyList<U> &lst);
};

template<typename T>
void MyList<T>::push_front(const T& data)
{
    node<T>* temp = new node<T>(nullptr, data, m_head);
    
    if (m_head != nullptr)
    {
        m_head->m_prev = temp;
    }
    else
    {
        m_tail = temp;
    }
    
    m_head = temp;
    temp = nullptr;
}

template<typename T>
void MyList<T>::pop_front()
{
    if (m_head == nullptr)
        return;
    
    node<T>* temp = m_head;
    m_head = m_head->m_next;

    if (m_head != nullptr)
        m_head->m_prev = nullptr;
    else
        m_tail = nullptr;
    
    delete temp;
    temp = nullptr;
}

template<typename T>
void MyList<T>::push_back(const T& data)
{
    node<T>* temp = new node<T>(m_tail, data, nullptr);

    if (m_tail != nullptr)
    {
        m_tail->m_next = temp;
    }
    else
    {
        m_head = temp;
    }
    
    m_tail = temp;
    temp = nullptr;
}

template<typename T>
void MyList<T>::pop_back()
{
    if (m_tail == nullptr)
        return;
    
    node<T>* temp = m_tail;
    m_tail = m_tail->m_prev;

    if (m_tail != nullptr)
        m_tail->m_next = nullptr;
    else
        m_head = nullptr;
    
    delete temp;
    temp = nullptr;
}

template<typename T>
T& MyList<T>::front()
{
    if (this->empty())
        throw underflow_error("null node");

    return m_head->m_data;
}

template<typename T>
const T& MyList<T>::front() const
{
    return const_cast<MyList*>(this)->front();
}

template<typename T>
T& MyList<T>::back()
{
    if (this->empty())
        throw overflow_error("null node");

    return m_tail->m_data;
}

template<typename T>
const T& MyList<T>::back() const
{
    return const_cast<MyList*>(this)->back();
}

template<typename T>
void MyList<T>::clear()
{
    while(!empty())
    {
        pop_front();
    }
}

template<typename T>
bool MyList<T>::empty() const
{
    return m_head == nullptr && m_tail == nullptr;
}

template<typename T>
size_t MyList<T>::size() const
{
    size_t num = 0;
    node<T>* temp = m_head;
    while (temp != nullptr)
    {
        ++num;
        temp = temp->m_next;
    }

    return num;
}

template<typename T>
MyList<T>::MyList()
: m_head(nullptr), m_tail(nullptr) 
{
}

template<typename T>
MyList<T>::~MyList()
{
    clear();
}

template<typename T>
MyList<T>::MyList(MyList const& that)
: m_head(nullptr), m_tail(nullptr) 
{
    for (node<T>* pnode = that.m_head; pnode; pnode = pnode->m_next)
    {
        push_back(pnode->m_data);
    }
}

template<typename U>
ostream &operator<<(ostream& os, const MyList<U>& lst)
{
    node<U>* temp = lst.m_head;
    while(temp != nullptr)
    {
        os << temp->m_data;
        temp = temp->m_next;
    }

    return os;
}

template<class T>
class MyList<T>::iterator
{
    friend MyList<T>;

private:
    node<T>* m_start;
    node<T>* m_cur;
    node<T>* m_end;

public:
    iterator(node<T>* start, node<T>* cur, node<T>* end)
            : m_start(start), m_cur(cur), m_end(end) {}
    T& operator*() const{
        if (m_cur == nullptr)
            throw underflow_error("null node");
        return m_cur->m_data;
    }

    iterator& operator++(){
        if (m_cur == nullptr)
            m_cur = m_start;
        else
            m_cur = m_cur->m_next;
        
        return *this;
    }
    iterator& operator--(){
        if (m_cur == nullptr)
            m_cur = m_end;
        else
            m_cur = m_cur->m_prev;

        return *this;
    }
    bool operator==(const iterator& it) const{
        return m_start == it.m_start &&
            m_cur == it.m_cur &&
            m_end == it.m_end;
    }
    bool operator!=(const iterator& it) const{
        return !(*this == it);
    }
};

template<class T>
typename MyList<T>::iterator MyList<T>::begin()
{
    return MyList<T>::iterator(m_head, m_head, m_tail);
}

template<class T>
typename MyList<T>::iterator MyList<T>::end()
{
    return MyList<T>::iterator(m_head, nullptr, m_tail);
}

template<class T>
typename MyList<T>::iterator MyList<T>::insert(const iterator& loc, const T& data)
{
    if (loc == this->end())
    {   
        this->push_back(data);
        return iterator(m_head, m_tail, m_tail);
    }
    else if (loc == this->begin())
        this->push_front(data);
    else
    {
        node<T>* prev = loc.m_cur->m_prev;
        prev->m_next = new node<T>(prev, data, loc.m_cur);
        loc.m_cur->m_prev = prev->m_next;
    }

    return iterator(m_head, loc.m_cur->m_prev, m_tail);
}

int main()
{
    MyList<int> myLst;

    for (int i = 0; i < 10; ++i)
        myLst.push_back(i);
    
    for (int i = 10; i > 0; --i)
        myLst.push_front(i);
    
    cout << myLst << endl;
    int i = 0;
    MyList<int>::iterator it = myLst.begin();
    it = myLst.insert(it, 100);
    while (it != myLst.end())
    {
        cout << *it << " " << endl;
        ++i;
        ++it;
        if (i == 2)
        {
            it = myLst.insert(it, 100);
        }
    }
    it = myLst.insert(it, 100);
    cout << *it;
    cout << endl;

    MyList<int> mst2(myLst);
    cout << mst2.front() << " " << mst2.back() << endl;
    mst2.clear();

    cout << mst2.size() << endl;
    
    return 0;
}