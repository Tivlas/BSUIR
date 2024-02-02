#include <vector>

class Algorithm {
   public:
    using Matrix = std::vector<std::vector<double>>;

   private:
    Matrix A_inverse;
    std::vector<double> x;
    size_t I;
    size_t size;

    std::vector<double> get_vector_l() {
        std::vector<double> l(size);
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                l[i] += A_inverse[i][j] * x[j];
            }
        }
        return std::move(l);
    }

    Matrix get_matrix_Q(const std::vector<double>& l_xor) {
        Matrix Q(size, std::vector<double>(size, 0));
        for (int i = 0; i < size; ++i) {
            Q[i][i] = 1;
            Q[i][I] = l_xor[i];
        }
        return std::move(Q);
    }

   public:
    Algorithm(const Matrix& A, const Matrix& A_inverse, size_t I,
              const std::vector<double>& x)
        : A_inverse(A_inverse), x(x), I(I), size(A.size()) {}

    ~Algorithm() {}

    Matrix get_inverse() {
        auto l = std::move(get_vector_l());
        if (l[I] == 0) return {};
        auto l_tilda = l;
        l_tilda[I] = -1;
        std::vector<double> l_xor(size);
        for (size_t i = 0; i < size; i++) {
            l_xor[i] = (-1 / l[I]) * l_tilda[i];
        }
        auto Q = std::move(get_matrix_Q(l_xor));

        Matrix Ax_inverse(size, std::vector<double>(size, 0));
        for (size_t j = 0; j < size; j++) {
            for (size_t k = 0; k < size; k++) {
                Ax_inverse[j][k] +=
                    Q[j][I] * A_inverse[I][k] + (j != I ?  Q[j][j] * A_inverse[j][k] : 0);
            }
        }
        return std::move(Ax_inverse);
    }
};
