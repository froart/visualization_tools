#include <vt_matrix.hpp>
#include <vt_framebuffer.hpp>

using namespace std;

int main ()
{
  vt::Matrix<int> mat1 = { { 1, 2, 3 },
                           { 3, 4, 5 },
                           { 7, 7, 2 } };
  vt::Matrix<int> mat2 = { { 1, 5, 3 },
                           { 2, 4, 5 },
                           { 7, 2, 2 } };
  vt::Matrix<int> mat3  = mat1 * mat2;

  mat1.print();
  mat2.print();
  mat3.print();
  
  return 0;
}
