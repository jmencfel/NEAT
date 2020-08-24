#include "ecm.h"

//Entity
Entity::Entity()
{
	_forDeletion = false;
	_points = 0;
}

void Entity::update(double dt) {
	if (!is_forDeletion()) {
		for (auto &_c : _components) {
			if (!_c->is_forDeletion())
				_c->update(dt);
		}
	}
	//std::cout << std::to_string(_points) << std::endl;
}

void Entity::render()
{
	if (!_forDeletion) {
		for (auto &_c : _components) {
			if (!_c->is_forDeletion())
				_c->render();
		}
	}
}

sf::Vector2f& Entity::getPosition()
{
	return _position;
}

void Entity::setPosition(const sf::Vector2f &pos) {
	_position = pos;
}

bool Entity::is_forDeletion() const
{
	return _forDeletion;
}

float Entity::getRotation() const
{
	return _rotation;
}

void Entity::setRotation(float _rot)
{
	_rotation = _rot;
}

bool Entity::isAlive() const
{
	return _alive;
}

void Entity::setAlive(bool _value)
{
	_alive = true;
}

void Entity::setForDelete()
{
	_forDeletion = true;
}

bool Entity::isVisible() const
{
	return _visible;
}

void Entity::setVisible(bool _value)
{
	_visible = _value;
}

void Entity::setPoints(int p)
{
	_points += p;
}

int Entity::getPoints()
{
	return _points;
}

//Component
Component::Component(Entity *p)
{
	_parent = p;
	_forDeletion = false;
}

bool Component::is_forDeletion()
{
	return _forDeletion;
}

void Component::setForDeletion() {
	_forDeletion = true;
}

Component::~Component()
{
}

//Entity Manager
void EntityManager::render() {
	for (auto &_e : list) {
		_e->render();
	}
}

void EntityManager::update(double dt) {
	for (auto &e : list) {
		e->update(dt);
	}
}