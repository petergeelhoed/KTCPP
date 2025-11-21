#include <algorithm>
#include <iostream>
#include <vector>

struct Point
{
    int x, y;
};

void gather(std::vector<Point>::iterator begin,
            std::vector<Point>::iterator end,
            std::vector<Point>::iterator pivot)
{
    std::stable_partition(begin, pivot, [&pivot](const Point& point) {
        return pivot->x != point.x;
    });
    std::stable_partition(pivot, end, [&pivot](const Point& point) {
        return pivot->x == point.x;
    });
}

void printVec(const std::vector<Point>& points)
{
    for (const auto& point : points)
    {
        std::cout << point.x << "," << point.y << "\n";
    }
}

int main()
{
    std::vector<Point> points = {
        {1, 2}, {4, 3}, {3, 9}, {4, 2}, {1, 4}, {4, 8}, {4, 6}};

    printVec(points);
    std::cout << "\n";
    gather(points.begin(), points.end(), points.begin() + 3);
    printVec(points);
}
