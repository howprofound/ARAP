#pragma once

class Game
{
private:
	bool started;
	bool quit;
public:
	Game();
	void setStatus(bool);
	bool getStatus();
	void setIsStarted(bool);
	bool isStarted();
};