#pragma once

#ifndef HUD_H
#define HUD_H

class HUD
{
private:
	bool m_hudShow;

public:
	bool Init();
	void Render();
};

#endif