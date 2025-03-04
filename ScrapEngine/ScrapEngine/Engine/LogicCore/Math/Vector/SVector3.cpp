#include <Engine/LogicCore/Math/Vector/SVector3.h>
#include <glm/detail/func_geometric.inl>
#include <glm/ext/quaternion_geometric.inl>
#include <algorithm>

ScrapEngine::Core::SVector3::SVector3(const glm::vec3 input)
{
	vec3_ = glm::vec3(input);
}

ScrapEngine::Core::SVector3::SVector3()
{
	vec3_ = glm::vec3();
}

ScrapEngine::Core::SVector3::SVector3(const float x, const float y, const float z)
{
	vec3_ = glm::vec3(x, y, z);
}

ScrapEngine::Core::SVector3 ScrapEngine::Core::SVector3::operator+(const SVector3& other) const
{
	return SVector3(vec3_ + other.vec3_);
}

ScrapEngine::Core::SVector3 ScrapEngine::Core::SVector3::operator-(const SVector3& other) const
{
	return SVector3(vec3_ - other.vec3_);
}

ScrapEngine::Core::SVector3 ScrapEngine::Core::SVector3::operator*(const SVector3& other) const
{
	return SVector3(vec3_ * other.vec3_);
}

ScrapEngine::Core::SVector3 ScrapEngine::Core::SVector3::operator*(const float value) const
{
	return SVector3(vec3_ * value);
}

ScrapEngine::Core::SVector3::operator glm::vec3() const
{
	return vec3_;
}

float ScrapEngine::Core::SVector3::get_x() const
{
	return vec3_.x;
}

float ScrapEngine::Core::SVector3::get_y() const
{
	return vec3_.y;
}

float ScrapEngine::Core::SVector3::get_z() const
{
	return vec3_.z;
}

void ScrapEngine::Core::SVector3::set_x(const float x)
{
	vec3_.x = x;
}

void ScrapEngine::Core::SVector3::set_y(const float y)
{
	vec3_.y = y;
}

void ScrapEngine::Core::SVector3::set_z(const float z)
{
	vec3_.z = z;
}

float ScrapEngine::Core::SVector3::get_max_value() const
{
	return std::max(std::max(vec3_.x, vec3_.y), vec3_.z);
}

ScrapEngine::Core::SVector3 ScrapEngine::Core::SVector3::get_vector() const
{
	return {*this};
}

void ScrapEngine::Core::SVector3::set_vector(const SVector3& other)
{
	vec3_ = glm::vec3(other.vec3_.x, other.vec3_.y, other.vec3_.z);
}

ScrapEngine::Core::SVector3 ScrapEngine::Core::SVector3::normalize() const
{
	return SVector3(glm::normalize(vec3_));
}

float ScrapEngine::Core::SVector3::length() const
{
	return glm::length(vec3_);
}

float ScrapEngine::Core::SVector3::distance(const SVector3& other) const
{
	return glm::distance(vec3_, other.vec3_);
}

ScrapEngine::Core::SVector3 ScrapEngine::Core::SVector3::cross_product(const SVector3& a, const SVector3& b)
{
	//call glm::cross
	return SVector3(cross(a.vec3_, b.vec3_));
}

float ScrapEngine::Core::SVector3::dot(const SVector3& a, const SVector3& b)
{
	return glm::dot(a.vec3_, b.vec3_);
}

glm::vec3 ScrapEngine::Core::SVector3::get_glm_vector() const
{
	return vec3_;
}
