#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <algorithm>
#include <format>
#include <sstream>
#include <syncstream>

using Matrix = std::vector<std::vector<std::int32_t>>;

Matrix generate_random_matrix(std::random_device::result_type seed, std::size_t n, std::size_t m) {
    std::mt19937 gen{seed};
    std::uniform_int_distribution<std::int32_t> dist{1, 10};
    Matrix matrix;
    matrix.reserve(n);
    for (std::size_t i{0}; i < n; ++i) {
        matrix.emplace_back(m);
        std::generate(matrix[i].begin(), matrix[i].end(), [&] { return dist(gen); });
    }
    return matrix;
}


void print_matrix(const Matrix &matrix) {
    for (auto &row: matrix) {
        for (auto v: row) {
            std::cout << v << ' ';
        }
        std::cout << '\n';
    }
}


int main(int argc, char **argv) {
    std::string usage{std::format("Usage: {} n m k thread_num verbose\n", argv[0])};
    if (argc != 6) {
        std::cout << usage;
        return EXIT_FAILURE;
    }
    std::stringstream args;
    args << argv[1] << ' ' << argv[2] << ' ' << argv[3] << ' ' << argv[4] << ' ' << argv[5];
    std::size_t n, m, k, thread_num;
    bool verbose;
    if (!(args >> n >> m >> k >> thread_num >> verbose)) {
        std::cout << usage;
        return EXIT_FAILURE;
    }
    if (thread_num == 0) {
        thread_num = n * k;
    }

    std::random_device rd{};
    Matrix a{generate_random_matrix(rd(), n, m)};
    Matrix b{generate_random_matrix(rd(), m, k)};
    Matrix c{n, std::vector<std::int32_t>(k)};

    {
        std::vector<std::jthread> threads{};
        threads.reserve(thread_num);
        for (std::size_t t{0}; t < thread_num; ++t) {
            threads.emplace_back([&, t] {
                for (std::size_t i = t; i < n * k; i += thread_num) {
                    std::size_t row{i / k}, col{i % k};
                    for (int j = 0; j < m; ++j) {
                        c[row][col] += a[row][j] * b[j][col];
                    }

                    if (verbose) {
                        std::osyncstream{std::cout} << std::format("i={}, j={}: {}\n", row, col, c[row][col]);
                    }
                }
            });
        }
    }

    std::cout << "A:\n";
    print_matrix(a);
    std::cout << "B:\n";
    print_matrix(b);
    std::cout << "A*B:\n";
    print_matrix(c);

    return EXIT_SUCCESS;
}
