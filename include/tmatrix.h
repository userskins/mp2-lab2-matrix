#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;

// Константы для ограничения размеров векторов и матриц
const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Класс динамического вектора
template<typename T>
class TDynamicVector
{
protected:
    size_t sz; // Размер вектора
    T* pMem;   // Указатель на массив элементов вектора

public:
    // Конструктор по умолчанию с возможностью задания размера
    TDynamicVector(size_t size = 1) : sz(size)
    {
        if (sz == 0 || sz > MAX_VECTOR_SIZE)
            throw out_of_range("Vector size must be between 1 and 100000000");
        pMem = new T[sz](); // Инициализация элементов нулями
    }

    // Конструктор на основе массива
    TDynamicVector(T* arr, size_t s) : sz(s)
    {
        assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
        pMem = new T[sz];
        std::copy(arr, arr + sz, pMem); // Копирование элементов из массива
    }

    // Конструктор копирования
    TDynamicVector(const TDynamicVector& v) : sz(v.sz)
    {
        pMem = new T[sz];
        std::copy(v.pMem, v.pMem + sz, pMem); // Копирование данных
    }

    // Конструктор перемещения
    TDynamicVector(TDynamicVector&& v) noexcept : sz(v.sz), pMem(v.pMem)
    {
        v.sz = 0;       // Обнуление размера у перемещенного объекта
        v.pMem = nullptr; // Обнуление указателя
    }

    // Деструктор
    ~TDynamicVector() { delete[] pMem; }

    // Оператор присваивания (копирования)
    TDynamicVector& operator=(const TDynamicVector& v)
    {
        if (this != &v)
        {
            if (sz != v.sz) // Если размеры не совпадают, выделяем новую память
            {
                delete[] pMem;
                sz = v.sz;
                pMem = new T[sz];
            }
            std::copy(v.pMem, v.pMem + sz, pMem); // Копируем элементы
        }
        return *this;
    }

    // Оператор присваивания (перемещения)
    TDynamicVector& operator=(TDynamicVector&& v) noexcept
    {
        swap(sz, v.sz);     // Обмениваемся данными
        swap(pMem, v.pMem); // Обмениваемся указателями на память
        return *this;
    }

    // Возвращает размер вектора
    size_t size() const noexcept { return sz; }

    // Доступ к элементу по индексу с проверкой на корректность
    T& operator[](size_t ind) { return pMem[ind]; }
    const T& operator[](size_t ind) const { return pMem[ind]; }

    T& at(size_t ind)
    {
        if (ind >= sz)
            throw out_of_range("Index is out of range");
        return pMem[ind];
    }

    const T& at(size_t ind) const
    {
        if (ind >= sz)
            throw out_of_range("Index is out of range");
        return pMem[ind];
    }

    // Оператор сравнения
    bool operator==(const TDynamicVector& v) const noexcept
    {
        return sz == v.sz && std::equal(pMem, pMem + sz, v.pMem); // Сравнение элементов
    }

    bool operator!=(const TDynamicVector& v) const noexcept { return !(*this == v); }

    // Шаблоны для арифметических операторов
    template<typename Op>
    TDynamicVector applyOp(T val, Op op) const
    {
        TDynamicVector result(sz);
        for (size_t i = 0; i < sz; ++i)
            result[i] = op(pMem[i], val); // Применение операции к каждому элементу
        return result;
    }

    template<typename Op>
    TDynamicVector& applyOpEq(T val, Op op)
    {
        for (size_t i = 0; i < sz; ++i)
            pMem[i] = op(pMem[i], val); // Применение операции к каждому элементу с присваиванием
        return *this;
    }

    // Арифметические операторы
    TDynamicVector operator+(T val) { return applyOp(val, std::plus<T>()); }
    TDynamicVector operator-(T val) { return applyOp(val, std::minus<T>()); }
    TDynamicVector operator*(T val) { return applyOp(val, std::multiplies<T>()); }

    // Операторы с присваиванием
    TDynamicVector& operator+=(T val) { return applyOpEq(val, std::plus<T>()); }
    TDynamicVector& operator-=(T val) { return applyOpEq(val, std::minus<T>()); }
    TDynamicVector& operator*=(T val) { return applyOpEq(val, std::multiplies<T>()); }

    // Оператор скалярного произведения двух векторов
    T operator*(const TDynamicVector& v) const
    {
        if (sz != v.sz)
            throw out_of_range("Different sizes");
        return std::inner_product(pMem, pMem + sz, v.pMem, T(0)); // Скалярное произведение
    }

    // Оператор вывода
    friend ostream& operator<<(ostream& os, const TDynamicVector& v)
    {
        for (size_t i = 0; i < v.sz; ++i)
            os << v.pMem[i] << ' '; // Вывод элементов вектора через пробел
        return os;
    }

    // Оператор ввода
    friend istream& operator>>(istream& is, TDynamicVector& v)
    {
        for (size_t i = 0; i < v.sz; ++i)
            is >> v.pMem[i]; // Чтение элементов вектора
        return is;
    }
};

// Класс динамической матрицы
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
    using TDynamicVector<TDynamicVector<T>>::pMem;
    using TDynamicVector<TDynamicVector<T>>::sz;
public:
    // Конструктор матрицы с размером
    TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s)
    {
        if (s == 0 || s > MAX_MATRIX_SIZE)
            throw std::out_of_range("Matrix size must be between 1 and 10000");
        for (size_t i = 0; i < sz; ++i)
            pMem[i] = TDynamicVector<T>(sz); // Инициализация строк матрицы векторами
    }

    // Оператор умножения матрицы на вектор
    TDynamicVector<T> operator*(const TDynamicVector<T>& vec)
    {
        if (this->size() != vec.size())
            throw std::out_of_range("Matrix and vector sizes do not match");

        TDynamicVector<T> result(this->size());
        for (size_t i = 0; i < this->size(); ++i)
        {
            result[i] = 0;
            for (size_t j = 0; j < this->size(); ++j)
            {
                result[i] += (*this)[i][j] * vec[j]; // Умножение строки матрицы на вектор
            }
        }
        return result;
    }

    using TDynamicVector<TDynamicVector<T>>::operator[];
    using TDynamicVector<TDynamicVector<T>>::size;
    using TDynamicVector<TDynamicVector<T>>::at;

    // Оператор умножения матрицы на скаляр
    TDynamicMatrix operator*(const T& val) const
    {
        TDynamicMatrix result(sz);
        for (size_t i = 0; i < sz; ++i)
            result[i] = pMem[i] * val; // Умножение каждой строки на скаляр
        return result;
    }

    // Оператор умножения матрицы на скаляр с присваиванием
    TDynamicMatrix& operator*=(const T& val)
    {
        for (size_t i = 0; i < sz; ++i)
            pMem[i] *= val; // Умножение каждой строки на скаляр
        return *this;
    }

    // Оператор умножения матриц
    TDynamicMatrix operator*(const TDynamicMatrix& m) const
    {
        if (sz != m.sz)
            throw out_of_range("Different sizes");

        TDynamicMatrix result(sz);
        for (size_t i = 0; i < sz; ++i)
        {
            for (size_t j = 0; j < sz; ++j)
            {
                for (size_t k = 0; k < sz; ++k)
                    result[i][j] += pMem[i][k] * m[k][j]; // Умножение матриц
            }
        }
        return result;
    }

    // Оператор вывода для матрицы
    friend ostream& operator<<(ostream& os, const TDynamicMatrix& m)
    {
        for (size_t i = 0; i < m.sz; ++i)
            os << m[i] << endl; // Вывод строк матрицы
        return os;
    }

    // Оператор ввода для матрицы
    friend istream& operator>>(istream& is, TDynamicMatrix& m)
    {
        for (size_t i = 0; i < m.sz; ++i)
            is >> m[i]; // Чтение строк матрицы
        return is;
    }
};

#endif
