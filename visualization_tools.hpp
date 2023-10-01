#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <memory>
#include <algorithm>

namespace vt
{
  using namespace std;

  template<typename T>
  class Matrix 
  {
    private:
      unsigned int rowNum;
      unsigned int colNum;
      vector<vector<T>> data;
    public:
      /* Constructors */
      Matrix() 
      {
        this->rowNum = 0;
        this->colNum = 0;
      }; 
      Matrix( int rowNum, int colNum ) 
      { // parametrized constructor #1
        this->resize( rowNum, colNum );
        for( auto& row: this->data ) // assign 0 to each element of the matrix
          for( auto& element: row )
            element = static_cast<T>( 0 ); // statically cast to type T
        this->rowNum = rowNum;
        this->colNum = colNum;
      };
      Matrix( initializer_list<initializer_list<T>> lst ) 
      {  // parametrized constructor #2
        /* find the longest nested initializer_list */
        int size = 0;
        for( const auto& e: lst )
          if( e.size() > size ) size = e.size();
        // resize matrix accoring to initializer_list
        this->resize( lst.size(), size );
        // map initializer_list values to out matrix
        auto lst_row_it = lst.begin();
        for( int j = 0; j < this->rowNum; ++j ) 
        {
          for( int i = 0; i < this->colNum; ++i ) 
            this->data[j][i] = ( i > lst_row_it->size() ) ? 0 : *( lst_row_it->begin() + i ); // fill with zeroes for the smaller nested lists
          ++lst_row_it;
        }
      };
      Matrix<T>& operator=( initializer_list<initializer_list<T>> lst )
      {
        Matrix<T> result = lst;
        *this = result; // copy
        return *this;
      }
      Matrix(const Matrix<T>& other) 
      {  // copy constructor
        this->resize( other.rowNum, other.colNum );
        this->data = other.data;
      };
      Matrix<T>& operator=( const Matrix<T>& other ) 
      {  // copy assign constructor
        this->resize( other.rowNum, other.colNum );
        this->data = other.data;
        return *this;
      };
      /* usefull member functions */
      void resize( int rowNum, int colNum ) 
      {
        this->data.resize( rowNum ); // resize the number of rows of the matrix
        for( auto& row: this->data ) // resize each row
          row.resize(colNum);
        this->rowNum = rowNum;
        this->colNum = colNum;
      };
      void print() // FIXME
      {
        for( int i = 0; i < this->colNum; ++i )
          cout << "--";
        cout << endl;
        for( const auto& row: this->data )
          for( const auto& element: row )
            cout << element << ( ( &element == &row.back() ) ? '\n' : ' ' );
        for( int i = 0; i < this->colNum; ++i )
          cout << "--";
        cout << endl;
      };
      // operator overloads
      Matrix<T> operator*( const Matrix<T>& other )
      {
        if( this->colNum != other.rowNum ) // check matrices compatibility
          throw invalid_argument( "Matrices can't be multiplied due to their size incompatibility!" );
        Matrix<T> result = Matrix<T>( this->rowNum, other.colNum );
        for( int j = 0; j < this->rowNum; ++j )
          for( int i = 0; i < other.colNum; ++i )
            for( int k = 0; k < this->colNum; ++k )
              result.data[j][i] += this->data[j][k] * other.data[k][i];
        return result;
      }; 
      Matrix<T> operator+( const Matrix<T> other ) {
        if( this->rowNum != other.rowNum || this->colNum != other.colNum ) // check matrices compatibility
          throw invalid_argument("Matrices can't be summed up due to their size incompatibility!");
        Matrix<T> result( this->rowNum, this->colNum );
        for( int j = 0; j < this->rowNum; ++j )
          for( int i = 0; i < this->colNum; ++i )
            result.data[j][i] = this->data[j][i] + other.data[j][i];
        return result;
      };
      Matrix<T> operator-( const Matrix<T> other ) {
        if( this->rowNum != other.rowNum || this->colNum != other.colNum ) // check matrices compatibility
          throw invalid_argument("Matrices can't be subtracted up due to their size incompatibility!");
        Matrix<T> result( this->rowNum, this->colNum );
        for( int j = 0; j < this->rowNum; ++j )
          for( int i = 0; i < this->colNum; ++i )
            result.data[j][i] = this->data[j][i] - other.data[j][i];
        return result;
      };
    // public
  }; // class Matrix
} // namespace vt
