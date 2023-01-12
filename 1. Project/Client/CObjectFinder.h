#pragma once
class CObjectFinder
{
public:
	static void FindAndSetObject();
	
private:
	static bool InRect(Vec2 _dot, Vec2 _RectPos, Vec2 _RectScale );
};

