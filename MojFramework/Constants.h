#pragma once

class Constants
{
public:
	static constexpr const char* prefix = "start_position:";
	static constexpr size_t prefix_len = sizeof("start_position:") - 1;
	static constexpr const char* player_prefix = "player:";
	static constexpr const char* enemy_prefix = "enemy:";
};