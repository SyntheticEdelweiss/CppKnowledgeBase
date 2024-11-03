#pragma once

// Allows to do this:
// int* mat<int>(int size1);
// double** mat<double>(int size1, int size2);
// UserType*** mat<UserType>(int size1, int size2, int size3);

template<typename T, typename size_type>
auto mat(size_type size)
{
    return new T[size];
}

template<typename T, typename size_type, typename... size_types>
auto mat(size_type size, size_types... sizes)
{
    using inner_type = decltype(mat<T>(sizes...));
    inner_type* m = new inner_type[size];
    for (int j = 0; j < size; j++)
        m[j] = mat<T>(sizes...);
    return m;
}

