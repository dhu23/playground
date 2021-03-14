#ifndef INCLUDED_DHM_MATRIX_H
#define INCLUDED_DHM_MATRIX_H

#include <vector>
#include <tr1/memory>
#include <iostream>
#include <stdexcept>

namespace dhm{

template<typename T>
class Matrix
{
    public:
        // Matrix(const T& scalar);
        Matrix(const unsigned int dimension, 
                const std::vector<std::tr1::shared_ptr<T> >& data);
        Matrix(const unsigned int row, const unsigned int column, 
                const std::vector<std::tr1::shared_ptr<T> >& data);
        Matrix(const unsigned int dimension, const std::vector<T>& data);
        Matrix(const unsigned int row, const unsigned int column, 
                const std::vector<T>& data);
        Matrix(const Matrix<T>& m);
        Matrix<T>& operator=(const Matrix<T>& m);
        ~Matrix();

        // const T operator T() const;

        T& getElement(const unsigned int row, const unsigned int column);
        const T& getElement(
                const unsigned int row, const unsigned int column) const;
        const Matrix<T> getRow(const unsigned int row) const;
        const Matrix<T> getColumn(const unsigned int column) const;
        const Matrix<T> getTranspose() const;
            
        const unsigned int rowNum() const;
        const unsigned int columnNum() const;

        bool isRowVector() const;
        bool isColumnVector() const;
        bool isScalar() const;

        std::ostream& print(std::ostream& os) const;

        // mathematical operations
        Matrix<T>& operator+=(const Matrix<T>& m);
        Matrix<T>& operator*=(const T& s);
        Matrix<T>& operator*=(const Matrix<T>& m);

    private:
        void assignElements(const std::vector<T>& data);
        void assignElements(const std::vector<std::tr1::shared_ptr<T> >& data);

        const int getElementIndex(
                const unsigned int row, const unsigned int column) const;
        const int getElementIndex(
                const unsigned int row, const unsigned int column, 
                const unsigned int rowNum, const unsigned int columnNum) const;

        static const T multiplyRowAndColumnVectors(
                const Matrix<T>& rowVector, const Matrix<T>& columnVector);

        std::vector<std::tr1::shared_ptr<T> > m_data;

        unsigned int m_rowNum;
        unsigned int m_columnNum;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m);

template<typename T>
const Matrix<T> operator+(const Matrix<T>& m1, const Matrix<T>& m2);

template<typename T>
const Matrix<T> operator*(const Matrix<T>& m, const T& s);

template<typename T>
const Matrix<T> operator*(const T& s, const Matrix<T>& m);

template<typename T>
const Matrix<T> operator*(const Matrix<T>& m1, const Matrix<T>& m2);


// implementation as follows

/*
template<typename T>
Matrix<T>::Matrix(const T& scalar):
    m_rowNum(1), m_columnNum(1), 
    m_data(std::vector<std::tr1::shared_ptr<T> >(
                1, std::tr1::shared_ptr<T>(new T(scalar))))
{
}
*/

template<typename T>
Matrix<T>::Matrix(
        const unsigned int dimension, 
        const std::vector<std::tr1::shared_ptr<T> >& data):
    m_rowNum(dimension), m_columnNum(dimension), m_data(data)
{
}

template<typename T>
Matrix<T>::Matrix(
        const unsigned int row, const unsigned int column, 
        const std::vector<std::tr1::shared_ptr<T> >& data):
    m_rowNum(row), m_columnNum(column), m_data(data)
{
}

template<typename T>
Matrix<T>::Matrix(const unsigned int dimension, const std::vector<T>& data):
    m_rowNum(dimension), m_columnNum(dimension), m_data()
{
    if(dimension*dimension != data.size())
    {
        throw std::runtime_error("data size NOT matching");
    }
    this->assignElements(data);
}

template<typename T>
Matrix<T>::Matrix(
        const unsigned int row, const unsigned int column,
        const std::vector<T>& data):
    m_rowNum(row), m_columnNum(column), m_data()
{
    if(row*column != data.size())
    {
        throw std::runtime_error("data size NOT matching");
    }
    this->assignElements(data);
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& m):
    m_rowNum(m.m_rowNum), m_columnNum(m.m_columnNum), m_data()
{
    // making deep copy
    this->assignElements(m.m_data);
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m)
{
    m_rowNum = m.rowNum();
    m_columnNum = m.columnNum();
    this->assignElements(m.m_data);   
}

template<typename T> 
Matrix<T>::~Matrix()
{
}

/*
const T Matrix<T>::operator T() const
{
    if(1 != m_rowNum || 1 != m_columnNum)
    {
        throw std::runtime_error("dimension NOT 1 by 1");
    }
    return this->getElement(1, 1);
}
*/

template<typename T>
T& Matrix<T>::getElement(const unsigned int row, const unsigned int column)
{
    return *m_data.at(this->getElementIndex(row, column));
}

template<typename T>
const T& Matrix<T>::getElement(
        const unsigned int row, const unsigned int column) const
{
    return *m_data.at(this->getElementIndex(row, column));
}

template<typename T>
const Matrix<T> Matrix<T>::getRow(const unsigned int row) const
{
    if(row > m_rowNum)
    {
        throw std::runtime_error("OUT of matrix dimension");
    }
    std::vector<std::tr1::shared_ptr<T> > rowData;
    for(unsigned int j = 1; j <= m_columnNum; ++j)
    {
        rowData.push_back(m_data.at(this->getElementIndex(row, j)));
    }
    return Matrix<T>(1, m_columnNum, rowData);
}

template<typename T>
const Matrix<T> Matrix<T>::getColumn(const unsigned int column) const
{
    if(column > m_columnNum)
    {
        throw std::runtime_error("OUT of matrix dimension");
    }
    std::vector<std::tr1::shared_ptr<T> > columnData;
    for(unsigned int i = 1; i <= m_rowNum; ++i)
    {
        columnData.push_back(m_data.at(this->getElementIndex(i, column)));
    }
    return Matrix<T>(m_rowNum, 1, columnData);
}

template<typename T>
const Matrix<T> Matrix<T>::getTranspose() const
{
    std::vector<std::tr1::shared_ptr<T> > data(m_rowNum*m_columnNum);
    // transpose matrix uses m_columnNum as row
    for(unsigned int i = 1; i <= m_columnNum; ++i)
    {
        // transpose matrix uses m_rowNum as column
        for(unsigned int j = 1; j <= m_rowNum; ++j)
        {
            data.at(this->getElementIndex(i, j, m_columnNum, m_rowNum)) =
                m_data.at(this->getElementIndex(j, i));
        }
    }
    return Matrix<T>(m_columnNum, m_rowNum, data);
}

template<typename T>
inline const unsigned int Matrix<T>::rowNum() const
{
    return m_rowNum;
}

template<typename T>
inline const unsigned int Matrix<T>::columnNum() const
{
    return m_columnNum;
}

template<typename T>
inline bool Matrix<T>::isRowVector() const
{
    return 1 == m_rowNum && 1 < m_columnNum;
}

template<typename T>
inline bool Matrix<T>::isColumnVector() const
{
    return 1 == m_columnNum && 1 < m_rowNum;
}

template<typename T>
inline bool Matrix<T>::isScalar() const
{
    return 1 == m_columnNum && 1 == m_rowNum;
} 

template<typename T>
std::ostream& Matrix<T>::print(std::ostream& os) const
{
    os << '[' << std::endl;
    for(unsigned int i = 1; i <= m_rowNum; ++i)
    {
        os << '\t';
        for(unsigned int j = 1; j <= m_columnNum; ++j)
        {
            os << this->getElement(i, j) << '\t';
        }
        os << std::endl;
    }
    os << ']' << std::endl;
    return os;
}

template<typename T>
void Matrix<T>::assignElements(const std::vector<T>& data)
{
    m_data.clear();

    for(typename std::vector<T>::const_iterator itr = data.begin();
            data.end() != itr; ++itr)
    {
        typename std::tr1::shared_ptr<T> pElement(new T(*itr));
        m_data.push_back(pElement);
    }
}

template<typename T>
void Matrix<T>::assignElements(
        const std::vector<std::tr1::shared_ptr<T> >& data)
{
    m_data.clear();

    for(typename std::vector<std::tr1::shared_ptr<T> >::const_iterator
            itr = data.begin();
            data.end() != itr; ++itr)
    {
        typename std::tr1::shared_ptr<T> pElement(new T(*(*itr)));
        m_data.push_back(pElement);
    }
}

template<typename T>
const int Matrix<T>::getElementIndex(
        const unsigned int row, const unsigned int column) const
{
    return this->getElementIndex(row, column, m_rowNum, m_columnNum);
}

template<typename T>
const int Matrix<T>::getElementIndex(
        const unsigned int row, const unsigned int column, 
        const unsigned int rowNum, const unsigned int columnNum) const
{
    if(row > rowNum || column > columnNum)
    {
        throw std::runtime_error("OUT of matrix dimension");
    }
    return columnNum*(row-1)+column-1;
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& m)
{
    if(m.rowNum() != m_rowNum || m.columnNum() != m_columnNum)
    {
        throw std::runtime_error("dimension NOT matching");
    }
    for(unsigned int i = 1; i <= m_rowNum; ++i)
    {
        for(unsigned int j = 1; j <= m_columnNum; ++j)
        {
            this->getElement(i, j) += m.getElement(i, j);
        }
    }
    return *this;
} 

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const T& s)
{
    for(unsigned int i = 1; i <= m_rowNum; ++i)
    {
        for(unsigned int j = 1; j <= m_columnNum; ++j)
        {
            this->getElement(i, j) *= s;
        }
    }
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& m)
{
    if(m.rowNum() != m_columnNum)
    {
        throw std::runtime_error("dimension NOT matching");
    }
    
    unsigned int column = m.columnNum();
    std::vector<T> data;
    for(unsigned int i = 1; i <= m_rowNum; ++i)
    {
        for(unsigned int j = 1; j <= column; ++j)
        {
            const T element = 
                multiplyRowAndColumnVectors(this->getRow(i), m.getColumn(j));
            data.push_back(element);
        }
    }
    
    // rowNum stays unchanged
    m_columnNum = column;
    this->assignElements(data);

    return *this;
}

template<typename T>
const T Matrix<T>::multiplyRowAndColumnVectors(
        const Matrix<T>& rowVector, const Matrix<T>& columnVector)
{
    if(!rowVector.isRowVector() || !columnVector.isColumnVector())
    {
        throw std::runtime_error("m1 NOT row vec or m2 NOT column vec");
    }
    const unsigned int dimension = rowVector.columnNum();
    if(dimension != columnVector.rowNum())
    {
        throw std::runtime_error("dimension NOT matching");
    }
    T result = 0;
    for(unsigned int i = 1; i <= dimension; ++i)
    {
        result += rowVector.getElement(1, i)*columnVector.getElement(i, 1);
    }
    return result;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
{
    return m.print(os);
}

template<typename T>
const Matrix<T> operator+(const Matrix<T>& m1, const Matrix<T>& m2)
{
    Matrix<T> temp(m1);
    temp += m2;
    return temp;
}

template<typename T>
const Matrix<T> operator*(const Matrix<T>& m, const T& s)
{
    Matrix<T> temp(m);
    temp *= s;
    return temp;
}

template<typename T>
const Matrix<T> operator*(const T& s, const Matrix<T>& m)
{
    return m*s;
}

template<typename T>
const Matrix<T> operator*(const Matrix<T>& m1, const Matrix<T>& m2)
{
    Matrix<T> temp(m1);
    temp *= m2;
    return temp;
}

} // end of namespace dhm
#endif
