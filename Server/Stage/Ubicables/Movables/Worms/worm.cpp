#include "worm.h"
#include <vector>
#include <utility>
#include "movable.h"
#include "stage.h"
#include "Box2D/Box2D.h"
#include <string>
#include "sensor.h"
#include "game.h"

int Worm::id_worms = 0;

Worm::Worm(Game& game
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
	, const float fall_dmg_per_meter)
	: Movable(x, y)
	, game(game)
	, stage(stage)
	, id_obj(id_worms++)
	, sensor_for_jump(*this)
	, x(x)
	, y(y)
	, angle_rad(angle_rad)
	, restitution(restitution)
	, total_health(health)
	, mov_speed(mov_speed)
	, forw_jump_speed(forw_jump_speed)
	, back_jump_speed(back_jump_speed)
	, max_height_dmg(max_height_dmg)
	, min_height_for_dmg(min_height_for_dmg)
	, fall_dmg_per_meter(fall_dmg_per_meter)
	, longitude(longitude)
	, height(height) {

	this->actual_health = health;
	this->facing_direction = MoveDirection::RIGHT;
	this->move_direction = MoveDirection::NONE;
	this->actual_velocity.Set(0, 0);
	this->dead = true;
	this->jump_cooldown = 0;
	this->should_slide = false;
	this->should_jump = false;
	this->angle_for_mov = 0.0;

	this->last_on_ground = false;
	this->last_direction = MoveDirection::NONE;

	this->last_position.Set(x, y);
}

void Worm::receive_dmg(int damage) {
	if (this->actual_health - damage <= 0) {
		this->actual_health = 0;
		this->dead = true;
	} else {
		this->actual_health -= damage;
	}

	this->game.notify_health(this);
}

float Worm::get_longitude() {
	return this->longitude;
}

float Worm::get_height() {
	return this->height;
}

int Worm::get_health() {
	return this->actual_health;
}

void Worm::add_health(int health) {
	this->actual_health += health;
}

void Worm::start_moving(MoveDirection mdirect) {
	if (this->dead)
		return;

	std::lock_guard<std::mutex> lock(this->direction_m);

	this->move_direction = mdirect;
	//Don't move while jump cooldown or
	//i need to jump
	//if (!this->jump_cooldown && !this->should_jump && !this->should_slide && this->is_on_ground()) {
		switch (this->move_direction) {
			case MoveDirection::RIGHT: {
				this->actual_velocity.Set(mov_speed*cos(angle_for_mov), mov_speed*sin(angle_for_mov));
				this->facing_direction = MoveDirection::RIGHT;
				break;
			}
			case MoveDirection::LEFT: {
				this->actual_velocity.Set(-mov_speed*cos(angle_for_mov), -mov_speed*sin(angle_for_mov));
				this->facing_direction = MoveDirection::LEFT;
				break;
			}	
			case MoveDirection::JUMP_FORW: {
				//if (this->is_on_ground() && !this->should_slide) {
					/*this->actual_velocity.Set(0, 0);
					this->body->SetLinearVelocity(this->actual_velocity);*/
					//b2Vec2 impulse_speed;

					if(this->facing_direction == MoveDirection::LEFT) {
						this->actual_velocity.Set(-forw_jump_speed.first, forw_jump_speed.second);
						//impulse_speed.Set(-forw_jump_speed.first, forw_jump_speed.second);
					}
					else if (this->facing_direction == MoveDirection::RIGHT) {
						this->actual_velocity.Set(forw_jump_speed.first, forw_jump_speed.second);
						//impulse_speed.Set(forw_jump_speed.first, forw_jump_speed.second);
					}

					this->should_jump = true;
					/*this->body->ApplyLinearImpulse(impulse_speed, this->body->GetWorldCenter());
					this->jump_cooldown = JUMP_COOLDOWN;
					this->game.notify_worm_status(this->get_id(),this->is_on_ground(),this->move_direction);*/
				//}
				break;
			}
			case MoveDirection::JUMP_BACK: {
				//if (this->is_on_ground() && !this->should_slide) {		
					/*this->actual_velocity.Set(0, 0);	
					this->body->SetLinearVelocity(this->actual_velocity);*/
					//b2Vec2 impulse_speed;

					if(this->facing_direction == MoveDirection::LEFT) {
						this->actual_velocity.Set(back_jump_speed.first, back_jump_speed.second);
						//impulse_speed.Set(back_jump_speed.first, back_jump_speed.second);
					}
					else if (this->facing_direction == MoveDirection::RIGHT) {
						this->actual_velocity.Set(-back_jump_speed.first, back_jump_speed.second);
						//impulse_speed.Set(-back_jump_speed.first, back_jump_speed.second);
					}

					this->should_jump = true;
					/*this->body->ApplyLinearImpulse(impulse_speed, this->body->GetWorldCenter());
					this->jump_cooldown = JUMP_COOLDOWN;	
					this->game.notify_worm_status(this->get_id(), this->is_on_ground(), this->move_direction);*/	
				//}	
				break;
			}										
			case MoveDirection::NONE: {
				this->actual_velocity.Set(0, 0);
				this->should_jump = false;
				break;
			}
		}
	//}
}

const MoveDirection& Worm::get_facing_direction() {
	std::lock_guard<std::mutex> lock(this->direction_m);

	return this->facing_direction;
}

void Worm::set_velocity(b2Vec2 velocity) {
	//No need of mutex, calculated in time step	
	this->actual_velocity = velocity;
}

void Worm::set_slide(bool slide) {
	this->should_slide = slide;
}

bool Worm::is_sliding() {
	return this->should_slide;
}

void Worm::receive_explosion(const b2Vec2& impulse) {
	//Mutex to prevent moving if it was going to set velocity
	std::lock_guard<std::mutex> lock(this->direction_m);

	this->body->ApplyLinearImpulse(impulse, this->body->GetWorldCenter());
	this->jump_cooldown = JUMP_COOLDOWN;		
}

void Worm::move_step(float32 time_step) {
	std::lock_guard<std::mutex> lock(this->direction_m);

	b2Vec2 actual_position = this->body->GetPosition();

	if (this->last_direction != this->move_direction || this->last_on_ground != this->is_on_ground()) {
		this->game.notify_worm_status(this->get_id(),this->is_on_ground(),this->move_direction);
	}

	if (this->is_on_ground()) {
		if (this->jump_cooldown == 0 && !this->should_slide && !this->should_jump) {
			this->body->SetLinearVelocity(this->actual_velocity);
		}

		if (this->jump_cooldown == 0 && !this->should_slide && this->should_jump) {
			this->body->SetLinearVelocity(b2Vec2(0, 0));
			this->body->ApplyLinearImpulse(this->actual_velocity, this->body->GetWorldCenter());
			//this->game.notify_worm_status(this->get_id(),this->is_on_ground(),this->move_direction);
			//this->should_jump = false;
			this->jump_cooldown = JUMP_COOLDOWN;
		}

		float fall_height = last_position.y - actual_position.y;

		if (fall_height >= this->min_height_for_dmg) {
			if (fall_height >= max_height_dmg)
				this->receive_dmg(max_height_dmg);
			else
				this->receive_dmg(fall_height);
		}	

		this->last_position = actual_position;	
	} else {
		//Get the highest position for fall dmg
		if (actual_position.y > last_position.y)
			this->last_position = actual_position;

		//this->last_should_jump = this->should_jump;
	}

	if(this->jump_cooldown > 0) {
		this->jump_cooldown--;
	}

	this->last_direction = this->move_direction;
	this->last_on_ground = this->is_on_ground();
}

void Worm::set_position(const b2Vec2& pos) {
	this->last_position = pos;
	this->stage.set_position(this, pos);
}

void Worm::set_angle(float angle) {
	this->angle_for_mov = angle;
}

void Worm::pre_solve_contact(b2Contact* contact, const b2Manifold* oldManifold) {
}

bool Worm::is_on_ground() {
	return this->sensor_for_jump.get_number_colisions() > 0;
}

void Worm::use(std::unique_ptr<Usable>& usable, const b2Vec2& dest, const std::vector<int> params) {
	if (this->dead)
		return;

	//b2Vec2 pos = this->body->GetPosition();

	usable->use(this, dest, std::move(params));
}

std::string Worm::get_type() {
	return WORM_TYPE;
}

int Worm::get_id() {
	return this->id_obj;
}

void Worm::create_myself(b2World& world) {
	this->dead = false;

	b2BodyDef body_def;
	//b2PolygonShape body_shape;
	b2CircleShape body_shape;
	b2FixtureDef fixture_def;

	body_def.type = b2_dynamicBody;
	body_def.position.Set(x, y);
	body_def.angle = angle_rad;
	body_def.fixedRotation = true;

	//body_shape.SetAsBox(longitude, height);
	body_shape.m_radius = longitude;
	body_shape.m_p.Set(0, 0);

	fixture_def.shape = &(body_shape);
	fixture_def.density = 1.0;
	fixture_def.restitution = restitution;

	this->body = world.CreateBody(&body_def);
	this->body->SetUserData(this);

	b2Fixture* fixture = this->body->CreateFixture(&fixture_def);		
	fixture->SetUserData(this);

	//Set a sensor at the floor of the body
	//Barely showing
	//Add more sensor to move from borders (BUG)
	this->sensor_for_jump.add_at_position(body
										, b2Vec2(0, -height)
										, longitude*0.6
										, height*0.1);

	this->last_on_ground = this->is_on_ground();
	this->last_direction = MoveDirection::NONE;	
}

void Worm::delete_myself(b2World& world) {
	this->actual_health = 0;
	this->game.notify_health(this);	
	world.DestroyBody(this->body);
}

void Worm::start_contacting(b2Contact* contact) {

}

void Worm::stop_contacting(b2Contact* contact) {

}

b2Body* Worm::get_body() {
	return this->body;
}

bool Worm::im_dead() {
	return this->dead;
}

void Worm::force_death() {
	this->dead = true;
}

bool Worm::is_affected_by_wind() {
	return false;
}

bool Worm::should_collide_with(Ubicable* ubicable) {
	return ubicable->should_collide_with(this);
}
	
bool Worm::should_collide_with(Girder* girder) {
	return true;
}

bool Worm::should_collide_with(Worm* worm) {
	//Dont collide with other worms
	return false;
}

bool Worm::should_collide_with(Throwable* throwable) {
	return true;
}

bool Worm::should_collide_with(Sensor* sensor) {
	return true;
}

bool Worm::is_explosive() {
	return false;
}
