#include <iostream>
#include "tmatrix.h"

int main()
{
    const int size = 5;

    // Инициализация матриц A и C
    TDynamicMatrix<int> A(size), C(size);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            A[i][j] = i * size + j + 1; // Заполнение матрицы A
            C[i][j] = 1;                // Заполнение матрицы C
        }
    }

    // Вывод матрицы A
    std::cout << "Matrix A:\n" << A << std::endl;

    // Использование обычного массива
    int arr[size] = { 1, 1, 1, 1, 1 };
    TDynamicVector<int> a(arr, size);

    // Операции с векторами и матрицами
    TDynamicVector<int> b = A * a;
    TDynamicMatrix<int> B = A * A;

    // Вывод матрицы B
    std::cout << "Matrix B (A * A):\n" << B << std::endl;

    // Операции над матрицами C и B
    C = B;
    C *= 2;
    B *= 3;

    // Вывод изменений в матрицах
    std::cout << "Matrix C (B * 2):\n" << C << '\n';
    std::cout << "Matrix B (B * 3):\n" << B << '\n';

    return 0;
}
