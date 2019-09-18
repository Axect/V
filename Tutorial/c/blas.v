#flag -lopenblas
#include <cblas.h>

fn C.cblas_daxpy(n int, a f64, x []f64, inc_x int, y mut []f64, inc_y int)

fn main() {
    n := 3
    a := 1.0
    x := [1., 2., 3.]
    y := [1., 2., 3.]
    C.cblas_daxpy(n, a, x, 1, y, 1)
    for t in y {
        println(t)
    }
}