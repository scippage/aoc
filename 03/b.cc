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

int scan_surroundings(int i_gear, int j_gear, int M, int N) {
    std::vector<int> parts;
    std::string part = "";

    // right
    for (int j=j_gear+1; j < N; j++) {
        if (isdigit(matrix[i_gear][j])) {
            part += matrix[i_gear][j];
        } else {
            break;
        }
    }
    if (part != "") parts.push_back(std::stoi(part));
    part = "";

    // left
    for (int j=j_gear-1; j >= 0; j--) {
        if (isdigit(matrix[i_gear][j])) {
            part += matrix[i_gear][j];
        } else {
            break;
        }
    }
    std::reverse(part.begin(), part.end());
    if (part != "") parts.push_back(std::stoi(part));
    part = "";

    // up
    if (i_gear-1 >= 0) {
        for (int j=0; j < N; j++) {
            if (isdigit(matrix[i_gear-1][j])) {
                part += matrix[i_gear-1][j];
            } 
            if ((part != "") && (!isdigit(matrix[i_gear-1][j]) || j == N-1)) {
                // overlap?
                if (j_gear-1 <= j-1 && j_gear+1 >= j-part.size()) {
                    parts.push_back(std::stoi(part));
                }
                part = "";
            }
        }
    }
    part = "";

    // down
    if (i_gear+1 < M) {
        for (int j=0; j < N; j++) {
            if (isdigit(matrix[i_gear+1][j])) {
                part += matrix[i_gear+1][j];
            }
            if ((part != "") && (!isdigit(matrix[i_gear+1][j]) || j == N-1)) {
                // overlap?
                if (j_gear-1 <= j-1 && j_gear+1 >= j-part.size()) {
                    parts.push_back(std::stoi(part));
                }
                part = "";
            }
        }
    }
    part = "";

    return (parts.size() == 2) ? parts[0]*parts[1] : 0;
}

int scan_matrix() {
    int result = 0;
    char elm;
    int M = matrix.size();
    int N = matrix[0].size();
    for (int i=0; i < M; i++) {
        for (int j=0; j < N; j++) {
            elm = matrix[i][j];
            if (elm == '*') {
                int gear_ratio = scan_surroundings(i, j, M, N);
                result += gear_ratio;
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