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
	int strenght = 1;
};

class Graph : public olcConsoleGameEngine {

	
	
private:
	vector<vec2d> points;
	vector<stick> sticks;


	float default_velocity_clamp = 1;
	float gravity = 0.5;
	float floor_height = 5; //height where floor friction is aplied
	float floor_friction = 0.85;
	float bounce = 0.9; //how bounce will slow down
	float friction = 0.99; //all time friction
	float hardness = 1; // how many times to controll if line is wrong lenght or point coliding

	void render_points() {
		for (int i = 0; i < points.size(); i++) {
			Draw(points[i].x, points[i].y, PIXEL_SOLID, FG_WHITE);
			Draw(points[i].oldx, points[i].oldy, PIXEL_SOLID, FG_RED);
		}
	}


	void update_sticks() { //controling the stick lenght
		for (int i = 0; i < sticks.size(); i++) {
			stick s = sticks[i];
			for (int i2 = 0; i2 < s.strenght; i2++) {
				float current_lenght = get_distance(points[s.p1], points[s.p2]);
				float diff = s.lenght - current_lenght;
				float fraction = diff / current_lenght / 2;
				points[s.p1].x += (points[s.p1].x - points[s.p2].x) * fraction;
				points[s.p1].y += (points[s.p1].y - points[s.p2].y) * fraction;

				points[s.p2].x -= (points[s.p1].x - points[s.p2].x) * fraction;
				points[s.p2].y -= (points[s.p1].y - points[s.p2].y) * fraction;
				if (s.strenght >= 2) {
					control_point(s.p1);
					control_point(s.p2);
				}
				
			}
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

	float clamp_velocity(float v, float clamp) {
		if (v < clamp) {
			return 0;
		}
		return v;
	}

	void control_point(int p) {
		vec2d point = points[p];
		float velox = clamp_velocity((point.x - point.oldx) * friction, default_velocity_clamp);

		float veloy = clamp_velocity((point.y - point.oldy) * friction, default_velocity_clamp);




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


		points[p] = point;
	}

	void control_points() { //calculating collision
		for (int i = 0; i < points.size(); i++) {
			vec2d point = points[i];
			float velox = clamp_velocity((point.x - point.oldx) * friction,default_velocity_clamp);

			float veloy = clamp_velocity((point.y - point.oldy) * friction, default_velocity_clamp);
			
		


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

	

	void update_points() { //updating the point position acording to velocity and gravity
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
			point.y += gravity;
			
		
	

			points[i] = point;
					}
	}

	
public:
	Graph()
	{}
	virtual bool OnUserCreate() {

		float obj_x = 50;
		float obj_y = 50;
		// objects constucted here
		/*
		points.push_back({ obj_x,obj_y,obj_x-20,obj_y -30 });
		points.push_back({ obj_x + 40,obj_y,obj_x + 40 - 20,obj_y });
		points.push_back({ obj_x,obj_y + 40,obj_x - 20,obj_y + 40 });
		points.push_back({ obj_x + 40,obj_y + 40,obj_x + 40 - 20,obj_y + 40 });
		points.push_back({ obj_x + 20,obj_y - 20,obj_x + 20,obj_y - 20 });

		sticks.push_back({ 0,1,40,10});
		sticks.push_back({ 0,2,40 ,10 });
		sticks.push_back({ 1,3,40 ,10 });
		sticks.push_back({ 2,3,40 ,10 });
		sticks.push_back({ 0,3,56.56854249,10 });
		sticks.push_back({ 1,2,56.56854249 ,10 });


		sticks.push_back({ 0,4,30 ,10 });
		sticks.push_back({ 1,4,30 ,10 });
		*/
		points.push_back({ obj_x,obj_y,obj_x - 20,obj_y -20 });
		points.push_back({ obj_x + 40,obj_y,obj_x + 40 - 20,obj_y });
		points.push_back({ obj_x,obj_y + 40,obj_x - 20,obj_y + 40 });
		points.push_back({ obj_x + 40,obj_y + 40,obj_x + 40 - 20,obj_y + 40 });
		
		sticks.push_back({ 0,1,40,40 });
		sticks.push_back({ 0,2,40 ,40 });
		sticks.push_back({ 1,3,40 ,40 });
		sticks.push_back({ 2,3,40 ,40 });
		sticks.push_back({ 1,2,40 ,40 });


	

		
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