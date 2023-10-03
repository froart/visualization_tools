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
  class Columns : public vector<T> 
  {
    public:
  };

  template<typename T>
  class Rows : public vector<T> 
  {
    public:
  };

  template<typename T>
  class Data2D 
  {
    protected:
      unsigned int rowNum;
      unsigned int colNum;
      Rows<Columns<T>> data;
    public:
      /* Constructors */
      Data2D() 
      {
        this->rowNum = 0;
        this->colNum = 0;
      }; 
      Data2D( const unsigned int rowNum, const unsigned int colNum )
      { // parametrized constructor #1
        this->resize( rowNum, colNum );
        for( auto& row: this->data ) // assign 0 to each element of the matrix
          for( auto& element: row )
            element = static_cast<T>( 0 ); // statically cast to type T
        this->rowNum = rowNum;
        this->colNum = colNum;
      };
      Data2D(const Data2D<T>& other) 
      {  // copy constructor
        this->resize( other.rowNum, other.colNum );
        this->data = other.data;
      };
      Data2D<T>& operator=( const Data2D<T>& other ) 
      {  // copy assign constructor
        this->resize( other.rowNum, other.colNum );
        this->data = other.data;
        return *this;
      };
      /* useful member functions */
      void resize( const unsigned int rowNum, const unsigned int colNum ) 
      {
        this->data.resize( rowNum ); // resize the number of rows of the matrix
        for( auto& row: this->data ) // resize each row
          row.resize( colNum );
        this->rowNum = rowNum;
        this->colNum = colNum;
      };
      virtual void print() {};
  }; // class Data2D
  
  template<typename T>
  class Matrix : public Data2D<T>
  {
    public:

      Matrix( const unsigned int rowNum, const unsigned int colNum ) : Data2D<T>( rowNum, colNum ) {};
      Matrix( const initializer_list<initializer_list<T>> lst ) 
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
      // operator overloads
      Matrix<T>& operator=( const initializer_list<initializer_list<T>> lst )
      {
        Data2D<T> result = lst;
        *this = result; // copy
        return *this;
      }
      Matrix<T> operator*( const Matrix<T>& other ) const
      {
        if( this->colNum != other.rowNum ) // check matrices compatibility
          throw invalid_argument( "Matrices can't be multiplied due to their size incompatibility!" );
        Matrix<T> result( this->rowNum, other.colNum );
        for( int j = 0; j < this->rowNum; ++j )
          for( int i = 0; i < other.colNum; ++i )
            for( int k = 0; k < this->colNum; ++k )
              result.data[j][i] += this->data[j][k] * other.data[k][i];
        return result;
      }; 
      Matrix<T> operator+( const Matrix<T> other ) const 
      {
        if( this->rowNum != other.rowNum || this->colNum != other.colNum ) // check matrices compatibility
          throw invalid_argument("Matrices can't be summed up due to their size incompatibility!");
        Matrix<T> result( this->rowNum, this->colNum );
        for( int j = 0; j < this->rowNum; ++j )
          for( int i = 0; i < this->colNum; ++i )
            result.data[j][i] = this->data[j][i] + other.data[j][i];
        return result;
      };
      Matrix<T> operator-( const Matrix<T> other ) const 
      {
        if( this->rowNum != other.rowNum || this->colNum != other.colNum ) // check matrices compatibility
          throw invalid_argument("Matrices can't be subtracted up due to their size incompatibility!");
        Matrix<T> result( this->rowNum, this->colNum );
        for( int j = 0; j < this->rowNum; ++j )
          for( int i = 0; i < this->colNum; ++i )
            result.data[j][i] = this->data[j][i] - other.data[j][i];
        return result;
      };
      void print() const 
      {
        for ( int j = 0; j < this->rowNum; ++j )
        {
          for ( int i = 0; i < this->colNum; ++i ) 
            cout << this->data[j][i] << " ";
          cout << endl;
        }
        cout << "--------------" << endl;
      };
    /* public */
  }; // class Matrix
} // namespace vt
