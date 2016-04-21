#pragma once

class Game
{
private:
	bool quit;
public:
	Game();
	void setStatus(bool quit);
	bool getStatus();
};