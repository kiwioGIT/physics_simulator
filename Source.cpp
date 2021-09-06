#include <iostream>
#include "olcConsoleGameEngine.h"
using namespace std;

struct vec2d
{
	float x;
	float y;
	float oldx;
	float oldy;
};

struct stick
{
	int p1;
	int p2;

	float lenght;
};

class Graph : public olcConsoleGameEngine {

	
	
private:
	vector<vec2d> points;
	vector<stick> sticks;

	float floor_height = 5;
	float floor_friction = 0.9;
	float bounce = 0.9;
	float friction = 0.99;
	float hardness = 3;

	void render_points() {
		for (int i = 0; i < points.size(); i++) {
			Draw(points[i].x, points[i].y, PIXEL_SOLID, FG_WHITE);
			Draw(points[i].oldx, points[i].oldy, PIXEL_SOLID, FG_RED);
		}
	}


	void update_sticks() {
		for (int i = 0; i < sticks.size(); i++) {
			stick s = sticks[i];
			float current_lenght = get_distance(points[s.p1], points[s.p2]);
			float diff = s.lenght - current_lenght;
			float fraction = diff / current_lenght / 2;
			points[s.p1].x += (points[s.p1].x - points[s.p2].x) * fraction;
			points[s.p1].y += (points[s.p1].y - points[s.p2].y) * fraction;

			points[s.p2].x -= (points[s.p1].x - points[s.p2].x) * fraction;
			points[s.p2].y -= (points[s.p1].y - points[s.p2].y) * fraction;
		}
	}

	

	float get_distance(vec2d p1, vec2d p2) {
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	}

	void render_sticks() {
		for (int i = 0; i < sticks.size(); i++) {
			stick s = sticks[i];
			draw_line_vec2d(points[s.p1],points[s.p2]);
		}
	}

	void draw_line_vec2d(vec2d p1, vec2d p2) {
		DrawLine(p1.x, p1.y, p2.x, p2.y,PIXEL_SOLID,FG_WHITE);
	}



	void control_points() {
		for (int i = 0; i < points.size(); i++) {
			vec2d point = points[i];
			float velox = (point.x - point.oldx) * friction;

			float veloy = (point.y - point.oldy) * friction;
			
		


			if (point.x >= ScreenWidth()) {
				point.x = ScreenWidth();
				point.oldx = point.x + velox * bounce;
			}

			if (point.x < 0) {
				point.x = 0;
				point.oldx = point.x + velox * bounce;
			}

			if (point.y >= ScreenHeight()) {
				point.y = ScreenHeight();
				point.oldy = point.y + veloy * bounce;
			}

			if (point.y < 0) {
				point.y = 0;
				point.oldy = point.y + veloy * bounce;
			}


			points[i] = point;
		}

	}


	void update_points() {
		for (int i = 0; i < points.size(); i++) {
			vec2d point = points[i];
			float add_frict = 1;
			if (point.y >= ScreenHeight()-floor_height) {
				add_frict = floor_friction;
			}

			float velox = (point.x - point.oldx) * friction * add_frict;
			float veloy = (point.y - point.oldy)*friction;
			point.oldx = point.x;
			point.oldy = point.y;
			point.x += velox;
			point.y += veloy;
			point.y += 0.5;
			
			/*
			if (point.x >= ScreenWidth()) {
				point.x = ScreenWidth();
				point.oldx = point.x + velox * bounce;
			}

			if (point.x < 0) {
				point.x = 0;
				point.oldx = point.x + velox * bounce;
			}

			if (point.y >= ScreenHeight()) {
				point.y = ScreenHeight();
				point.oldy = point.y + veloy * bounce;
			}

			if (point.y < 0) {
				point.y = 0;
				point.oldy = point.y + veloy * bounce;
			}*/
	

			points[i] = point;
					}
	}

	
public:
	Graph()
	{}
	virtual bool OnUserCreate() {
		points.push_back({50,0,40,0});
		points.push_back({ 80,0,70,0 });
		points.push_back({ 50,30,40,20 });
		points.push_back({ 80,30,70,20 });

		sticks.push_back({ 0, 1, 30 });
		sticks.push_back({ 0, 2, 30 });
		sticks.push_back({ 0, 3, 30 });
		sticks.push_back({ 1, 3, 30 });
		sticks.push_back({ 2, 3, 30 });
		return true;

	}

	virtual bool OnUserUpdate(float fElapsedTime) {
		
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ');

		update_points();

		for (int i = 0; i < hardness; i++) {
			update_sticks();
			control_points();
		}
		render_points();
		render_sticks();
		


		return true;
	}




	
};

int main() {
	Graph demo;
	demo.ConstructConsole(500, 400, 2, 2);
	demo.Start();

	return 0;
}