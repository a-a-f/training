#include <iostream>
#include <vector>
#include <string>

int compute(const std::vector<int>& heights) {
    int volume = 0;

    size_t left = 0;
    size_t right = heights.size() - 1;

    int cur_left_max = -1;
    int cur_right_max = -1;

    while (left < right) {
        while (left < right && heights[left] <= heights[right]) {
            if (heights[left] > cur_left_max) {
                cur_left_max = heights[left];
            } else {
                volume += cur_left_max - heights[left];
            }
            ++left;
        }
        while (left < right && heights[right] <= heights[left]) {
            if (heights[right] > cur_right_max) {
                cur_right_max = heights[right];
            } else {
                volume += cur_right_max - heights[right];
            }
            --right;
        }
    }

    return volume;
}

void test() {
    std::cout << compute({1,0,2,1,0,1,3,2,1,2,1}) << "\n";
    std::cout << compute({10,0,10}) << "\n";
    std::cout << compute({0,10,0}) << "\n";
}

int main() {
    test();
    return 0;
}
