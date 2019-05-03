// Main Application
//
// Author: Matthew Knight
// File Name: main.c
// Date: 2018-10-03
//
// This file holds the main application

class Point {
	int x;
	int y;

public:
	Point(int a, int b) : x(a), y(b) {}

	int& getX() {
		return x;
	}
};

int main() {
	volatile int i = 5;
	Point point(1, 2);
	i = point.getX();
}
