    
#pragma once
class UI_Manager
{
	vector<Area> areas;
	
public:
	UI_Manager();
	~UI_Manager();
	
	void display(string text);
	void set_areas();
	
	int get_area_of_point(Point p);
	
	
	/*
	void display_question(string question);
	void display_answer(int pos,string text);
	void display_player_score();
	*/
};
