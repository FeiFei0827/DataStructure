//
// Created by 飞飞 on 2023/10/16.
//
// Created by 飞飞 on 2023/10/12.
//
#include "Vector.h"
#include "fibonacci.h"
#include "iostream"
template<typename T>
const T &Vector<T>::operator[](Rank r) const {
    return _elem[r];
}

template<typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {//以数组区间 [lo,hi)为蓝本
    _elem = new T[_capacity = 2 * (hi - lo)]; _size = 0;// 用new来申请内存
    while (lo < hi)
    {
        _elem[_size++] = A[lo++];
    }
}

template<typename T>
void Vector<T>::expand()
{
    if (_size < _capacity)
        return;
    if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
    T* oldElem = _elem;
    _elem = new T[_capacity <<= 1];
    for (int i = 0; i < _size; ++i)
        _elem[i] = oldElem[i];
    delete[] oldElem;
}

template<typename T>
void Vector<T>::shrink()
{

    if (_capacity < DEFAULT_CAPACITY << 1) return; //不致收缩到DEFAULT_CAPACITY以下
    if (_size << 2 > _capacity) return; //以25%为界
    T* oldElem = _elem; _elem = new T[_capacity >>= 1]; //容量减半
    for (Rank i = 0; i < _size; i++) _elem[i] = oldElem[i]; //复制原向量内容
    delete[] oldElem; //释放原空间

}
template<typename T>
void Vector<T>::permute()
{
    for (int i = this->size(); i > 0; i--)
        swap(this->operator[](i - 1), this->operator[](rand() % i));
}
template<typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const
{
    while ((lo < hi--) && (e != _elem[hi]));
    return hi;
}
template<typename T>
T& Vector<T>::operator[](Rank r)
{
    return _elem[r];
}
//template<typename T>
//ostream& operator<<(ostream& out, Vector<T>& b)//友元函数是非成员函数 这都能忘！！！！！
//{
//
//}

template<typename T>
T Vector<T>::remove(Rank r)
{
    T e = _elem[r];
    remove(r, r + 1);
    return e;
}

template<typename T>
Rank Vector<T>::remove(Rank lo, Rank hi)
{
    if (lo == hi) return 0;
    while (hi < _size)
    {
        _elem[lo++] = _elem[hi++];
    }
    _size = lo;
    shrink();
    return hi - lo;
}

template<typename T>
Rank Vector<T>::insert(Rank r, T const& e)
{
    expand();
    for (int i = _size; i > r; --i)
    {
        _elem[i] = _elem[i - 1];
    }
    _elem[r] = e;;
    ++_size;
    return r;

    return Rank();
}

template<typename T>
void Vector<T>::unsort(Rank lo, Rank hi)
{
    T* V = _elem + lo;
    for (Rank i = hi - lo; i > 0; i--)
        swap(V[i - 1], V[rand() % i]);
}

template<typename T>
int Vector<T>::deuplicate()
{
    if (empty())
        return 0;
    int i = 0;
    Rank size = _size;
    while (i < _size)
    {
        if (_elem[i] = _elem[++i])
            remove(i);
    }
    return size - _size;

}
template<typename T>
void Vector<T>::traverse(void (*visit)(T &)) {
    for(int i=0;i<_size;++i)
    {
        visit (_elem[i]);
    }
}//借用函数指针机制实现遍历
template <typename T> template <typename VST>
void Vector<T>::traverse(VST &visit) {
    for(int i=0;i<_size;++i)
    {
        visit (_elem[i]);
    }
}//借用函数对象机制实现遍历
template<typename T>
void gaga(T& a)
{
    std::cout<<a<<"  ";
}
template<typename T>
void printElement(T &elem) {
    std::cout << elem << " ";
}//gpt 提供的函数 原来traverse后的函数要用<>
// 定义一个类模板
template<typename T>
struct PrintElement {
    void operator()(T &elem) {
        std::cout << elem << " ";
    }
};
//同样是gpt实现
//int main() {
//int a[]{ 2,2 };
//Vector<int> b(a,2);
// 创建一个PrintElement对象并使用它来打印向量中的每个元素
// PrintElement<int> printElement;
//b.traverse(printElement);
//b.deuplicate();
// cout<<std::endl;
//}
template <typename T> int Vector<T>::disordered() const {
    int n=0;
    for(int i =1;i<_size;i++)
        if(_elem[i-1]>_elem[i])
            n++;
    return n;
}
template <typename T>
Rank Vector<T>::uniquify() {
    Rank i =0,j=0;
    while(++j<_size)
        if(_elem[i]!=_elem[j])
            _elem[++i]=_elem[j];
    _size =++i;
    shrink();
    return j-i;
}
template <typename T>
Rank Vector<T>::search(const T &e, Rank lo, Rank hi) const {
    return (rand()%2)? binSearch(_elem, e ,lo,hi):fibSearch(_elem,e,lo,hi); //这里只是为了测试到两种算法
}
template <typename T>
Rank Vector<T>::search(const T &e) const  {
    return (0 >= _size) ? -1 : search(e, 0, _size);
}

template<typename T> static Rank binSearch(T* A, T const& e, Rank lo, Rank hi){
    while(lo<hi) {
        Rank mi = (lo + hi) >> 1;
        if (e < A[mi])
            hi=mi;
        else if(A[mi]>e)
            lo =mi+1;//需要+1
        else
            return mi;
    }
    return -1;
}
template<typename T> static Rank fibSearch (T* A, T const& e, Rank lo, Rank hi)
{
    fibonacci fib ( hi - lo ); //用O(log_phi(n = hi - lo)时间创建Fib数列
    while ( lo < hi ) { //殏步迭代可能要做两次比较刞断，有三个分支
        while ( hi - lo < fib.get() ) fib.prev(); //通过向前顺序查找（分摊O(1)）——至夗迭代几次？
        Rank mi = lo + fib.get() - 1; //确定形如Fib(k) - 1癿轴点
        if ( e < A[mi] ) hi = mi; //深入前半殌[lo, mi)继续查找
        else if ( A[mi] < e ) lo = mi + 1; //深入后半殌(mi, hi)继续查找
        else return mi; //在mi处命中
    } //成功查找可以提前终止
}
