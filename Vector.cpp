#include <iostream>
#include <vector>
#include<typeinfo>
using std::vector;
using std::ostream;
using std::cout;
using Rank = unsigned int; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）

template <typename T> class Vector { //向量模板类
protected:
    Rank _size; Rank _capacity;  T* _elem; //规模、容量、数据区
    void copyFrom(T const* A, Rank lo, Rank hi); //复制数组区间A[lo, hi)
    void expand(); //空间不足时扩容
    void shrink(); //装填因子过小时压缩
    bool bubble(Rank lo, Rank hi); //扫描交换
    void bubbleSort(Rank lo, Rank hi); //起泡排序算法
    Rank maxItem(Rank lo, Rank hi); //选取最大元素
    void selectionSort(Rank lo, Rank hi); //选择排序算法
    void merge(Rank lo, Rank mi, Rank hi); //归并算法
    void mergeSort(Rank lo, Rank hi); //归并排序算法
    void heapSort(Rank lo, Rank hi); //堆排序（稍后结合完全堆讲解）
    Rank partition(Rank lo, Rank hi); //轴点构造算法
    void quickSort(Rank lo, Rank hi); //快速排序算法
    void shellSort(Rank lo, Rank hi); //希尔排序算法
public:
    // 构造函数
    //没有初始化列表那怎么能行
    //Vector(std::initializer_list<T> vars) {    如果内存分配算法自己来写那么初始化列表也得改变 需要改变初始化列表的内存分配方式 来适应自己的析构 这对看不懂源码的我来说简直是不可能的
    //_elem = vars.begin();
    //};


    Vector(Rank c = DEFAULT_CAPACITY, Rank s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
    {
        _elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
    } //s<=c
    Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //数组整体复制
    Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //区间
    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //向量整体复制
    Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //区间
    // 析构函数
    ~Vector() { delete[] _elem; } //释放内部空间 就感觉这里有哪里不对
    Rank size() const { return _size; } //规模
    bool empty() const { return !_size; } //判空
    void permute();//实际上已经没用了我留着玩
    Rank find(T const& e) const { return find(e, 0, _size); } //无序向量整体查找
    Rank find(T const& e, Rank lo, Rank hi) const; //无序向量区间查找
    Rank search(T const& e) const //有序向量整体查找
    {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    Rank search(T const& e, Rank lo, Rank hi) const; //有序向量区间查找
    // 可写访问接口
    T& operator[] (Rank r); //重载下标操作符，可以类似于数组形式引用各元素
    const T& operator[] (Rank r) const; //仅限于做右值的重载版本
    Vector<T>& operator= (Vector<T> const&); //重载赋值操作符，以便直接克隆向量
    T remove(Rank r); //删除秩为r的元素
    Rank remove(Rank lo, Rank hi); //删除秩在区间[lo, hi)之内的元素
    Rank insert(Rank r, T const& e); //插入元素
    Rank insert(T const& e) { return insert(_size, e); } //默认作为末元素插入
    void sort(Rank lo, Rank hi); //对[lo, hi)排序
    void sort() { sort(0, _size); } //整体排序
    void unsort(Rank lo, Rank hi); //对[lo, hi)置乱
    void unsort() { unsort(0, _size); } //整体置乱 可以这样写 避免出现一个输入 一个默认值的情况
    int deuplicate();
    Rank dedup(); //无序去重
    Rank uniquify(); //有序去重
    // 遍历
    void traverse(void (*) (T&)); //遍历（使用函数指针，只读或局部性修改）
    template <typename VST> void traverse(VST&); //遍历（使用函数对象，可全局性修改）
    //friend ostream& operator<<(ostream& out, T& b); //Vector             忘记了vector根本不能直接cout

};
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
    cout<<a<<"  ";
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
int main() {
    int a[]{ 2,2 };
    // 向myVector添加元素...
    Vector<int> b(a,2);
    // 创建一个PrintElement对象并使用它来打印向量中的每个元素
    PrintElement<int> printElement;
    b.traverse(printElement);
    b.deuplicate();
    cout<<std::endl;
    b.traverse(printElement);

}
