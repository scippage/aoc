#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>


std::vector<std::vector<char>> matrix;

bool validate_part(int i, int start_j, int end_j) {
    // indices inclusive
    int i_low = std::max(i-1, 0);
    int i_high = std::min(i+1, (int)matrix[0].size()-1);
    int j_low = std::max(start_j-1, 0);
    int j_high = std::min(end_j+1, (int)matrix[0].size()-1);

    for (int ii=i_low; ii <= i_high; ii++) {
        for (int jj=j_low; jj <= j_high; jj++) {
            if (matrix[ii][jj] != '.' && !isdigit(matrix[ii][jj])) {
                return true;
            }
        }
    }
    return false;
}

int scan_matrix() {
    int result = 0;
    int num = 0;
    // sentinel that marks if we're currently assessing a number
    int num_start_j = -1;
    char elm;
    int M = matrix.size();
    int N = matrix[0].size();
    for (int i=0; i < M; i++) {
        for (int j=0; j < N; j++) {
            elm = matrix[i][j];
            if (isdigit(elm)) {
                num = num*10 + (elm - '0');
                if (num_start_j == -1) {
                    num_start_j = j;
                }
            }
            if ((num_start_j != -1) && (!isdigit(elm) || j == N-1)) {
                if (validate_part(i, num_start_j, j-1)) {
                    result += num;
                }
                num = 0;
                num_start_j = -1;
            }
        }
    }
    return result;
}

int main() {
    std::ifstream file("./input.txt");
    std::string str;
    while (std::getline(file, str)) {
        std::vector<char> data(str.begin(), str.end());
        matrix.push_back(data);
    }

    std::cout << "Cols: " << matrix[0].size() << std::endl;
    std::cout << "Rows: " << matrix.size() << std::endl;

    if (file.is_open()) {
        file.close();
    }

    int ans = scan_matrix();

    printf("Ans: %d\n", ans);
    exit(EXIT_SUCCESS);
}