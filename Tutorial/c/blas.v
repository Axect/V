#flag -L/opt/OpenBLAS/lib
#flag -lopenblas
#include <cblas.h>

fn C.cblas_daxpy(n int, a f64, x &f64, inc_x int, mut y &f64, inc_y int)

fn main() {
    n := 3
    a := 1.0
    x := [f64(1), f64(2), f64(3)]
    y := [f64(1), f64(2), f64(3)]
    C.cblas_daxpy(n, a, &x[0], 1, &y[0], 1)
	println(y)
}
