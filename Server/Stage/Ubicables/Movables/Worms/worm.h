#ifndef WORM_H
#define WORM_H

#include <vector>
#include <utility>
#include "usable.h"
#include "movable.h"
#include "Box2D/Box2D.h"
#include "stage.h"
#include <mutex>
#include <string>
#include "sensor.h"

#define WORM_TYPE "Worm"
#define JUMP_COOLDOWN 5

enum class MoveDirection : char {
	NONE = 0,
	RIGHT,
	LEFT,
	JUMP_FORW,
	JUMP_BACK
};

class Worm : public Movable {
private:
	Game& game;
	Stage& stage;
	static int id_worms;
    const int id_obj;

	b2Body* body;
	//b2Fixture* fixture;
	Sensor sensor_for_jump;

	const float x;
	const float y;
	const float angle_rad;
	const float restitution;
	const int total_health; //Useful for percentage calculations of hp
	const float mov_speed;
	const std::pair<float, float> forw_jump_speed;
	const std::pair<float, float> back_jump_speed;
	const float max_height_dmg;
	const float min_height_for_dmg;
	const float fall_dmg_per_meter;
	const float longitude;
	const float height;
	int actual_health;
	int jump_cooldown;
	bool dead;
	bool should_slide;
	bool should_jump;
	bool sliding;
	bool last_on_ground;
	MoveDirection last_direction;
	float angle_for_mov;

	std::mutex direction_m;

	MoveDirection facing_direction;
	MoveDirection move_direction;
	b2Vec2 actual_velocity;
	b2Vec2 last_position;

	bool is_on_ground();
public:
	Worm(Game& game
		, Stage& stage
		, const float x
		, const float y
		, const float angle_rad
		, const float longitude
		, const float height
		, const float restitution
		, const int health
		, const float mov_speed
		, const std::pair<float, float> forw_jump_speed
		, const std::pair<float, float> back_jump_speed
		, const float max_height_dmg
		, const float min_height_for_dmg
		, const float fall_dmg_per_meter);

	virtual std::string get_type() override;
	virtual int get_id() override;
	virtual bool is_explosive() override;
	virtual void create_myself(b2World& world) override;
	virtual void delete_myself(b2World& world) override;

	virtual void start_contacting(b2Contact* contact) override;
	virtual void stop_contacting(b2Contact* contact) override;

	virtual bool should_collide_with(Ubicable* ubicable) override;
	
	virtual bool should_collide_with(Girder* girder) override;
	virtual bool should_collide_with(Worm* worm) override;
	virtual bool should_collide_with(Throwable* throwable) override;
	virtual bool should_collide_with(Sensor* sensor) override;

	virtual void pre_solve_contact(b2Contact* contact, const b2Manifold* oldManifold) override;

	virtual void move_step(float32 time_step) override;
	virtual b2Body* get_body() override;
	virtual bool im_dead() override;
	virtual void force_death() override;
	virtual bool is_affected_by_wind() override;

	void set_slide(bool slide);
	bool is_sliding();
	void set_angle(float angle);
	void set_velocity(b2Vec2 velocity);
	void set_position(const b2Vec2& pos);

	const MoveDirection& get_facing_direction();

	float get_longitude();
	float get_height();

	int get_health();
	void add_health(int health);

	void receive_dmg(int damage);
	void receive_explosion(const b2Vec2& impulse);

	void start_moving(MoveDirection mdirect);

	void use(std::unique_ptr<Usable>& usable, const b2Vec2& dest, const std::vector<int> params);	
};

#endif
