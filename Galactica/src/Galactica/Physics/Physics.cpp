#include "glpch.h"

#include "Physics.h"

namespace Galactica
{
	//Soft Body
	SoftBody::SoftBody(std::string path, float restitution, float mass, float stiffness, float damping)
	: Model(path)
	{
		assert(meshes.size() > 0 && "ERROR: More than one mesh provided!");
		springCount = 0;
		
		this->restitution = restitution;
		this->mass = mass;
		this->stiffness = stiffness;
		this->damping = damping;

		dynamicVertices = std::vector<Vertex>(meshes[0].vertices);

		// Create mass points
		for (Vertex& v : dynamicVertices) 
		{
			PointMass p(&v, this, restitution, mass, stiffness, damping);
			pointMasses.push_back(p);
		}

		// Create springs between mass points
		for (PointMass& p1 : pointMasses)
		{
			for (PointMass& p2 : pointMasses)
			{
				float d = glm::distance(p1.vert->position, p2.vert->position);
				if (d > 0.1)
				{
					AddSpring(&p1, &p2);
				}
			}
		}
	}

	SoftBody::~SoftBody()
	{

	}

	void SoftBody::AddForce(glm::vec3(force))
	{
		for (PointMass& p : pointMasses)
		{
			p.forces += force;
		}
	}

	void SoftBody::AddSpring(PointMass* a, PointMass* b)
	{
		Spring s;
		s.a = a;
		s.b = b;

		s.restLength = glm::distance(a->vert->position, b->vert->position);
		springs.push_back(s);
		springCount++;
	}

	// ToDo: Change to use my own math.
	void SoftBody::Update(float dt)
	{
		// Calculate spring forces (Hooke's law)
		for (Spring s : springs)
		{
			glm::vec3 aPos = s.a->vert->position;
			glm::vec3 bPos = s.b->vert->position;
			glm::vec3 dir = glm::normalize(bPos - aPos);

			float currentLength = glm::distance(aPos, bPos);
			float dX = currentLength - s.restLength;

			// Hooke's law
			s.a->forces += dir * dX * stiffness;
			s.b->forces -= dir * dX * stiffness;

			// Damping
			float relativeVelocity = glm::dot(dir, s.b->velocity - s.a->velocity);
			s.a->forces += dir * relativeVelocity * damping * s.a->mass;
			s.b->forces -= dir * relativeVelocity * damping * s.b->mass;
		}

		// Integrate all point masses with their forces
		for (PointMass& p : pointMasses)
		{
			p.Integrate(dt);
		}

		// Update vertices for rendering
		meshes[0].UpdateVertices(dynamicVertices);
	}

	void SoftBody::Reset()
	{
		
		dynamicVertices.clear();
		dynamicVertices.reserve(meshes[0].vertices.size());

		for (const Vertex& vertex : meshes[0].vertices) {
			dynamicVertices.push_back(vertex);
		}

		for (PointMass& p : pointMasses) {
			p.velocity = glm::vec3(0.0);
			p.acceleration = glm::vec3(0.0);
			p.forces = glm::vec3(0.0);
		}
	}

	//Point Mass
	PointMass::PointMass(Vertex* vert, SoftBody* body, float restitution, float mass, float stiffness, float damping) 
	{
		this->vert = vert;
		this->body = body;

		this->restitution = restitution;
		this->mass = mass;
		this->stiffness = stiffness;
		this->damping = damping;

		velocity = glm::vec3(0.0);
		acceleration = glm::vec3(0.0);
		forces = glm::vec3(0.0);
	}

	PointMass::~PointMass()
	{}

	//ToDo: Change to use my own math.
	void PointMass::Integrate(float dt)
	{
		glm::mat4 transform = body->getTransform();
		glm::mat4 inverse = glm::inverse(transform);

		glm::vec3 world = transform * glm::vec4(vert->position, 1);

		if (world.y <= 0.1)
		{
			velocity = glm::vec4(velocity.x, -velocity.y * restitution, velocity.z, 1);
			vert->position.y = (inverse * glm::vec4(0, 0.1, 0, 1)).y;
		}
		if (world.x <= -10)
		{
			velocity = glm::vec4(-velocity.x * restitution, velocity.y, velocity.z, 1);
			vert->position.x = (inverse * glm::vec4(-10, 0, 0, 1)).x;
		}
		if (world.x >= 10)
		{
			velocity = glm::vec4(velocity.x * restitution, velocity.y, velocity.z , 1);
			vert->position.x = (inverse * glm::vec4(10, 0, 0, 1)).x;
		}
		if (world.z <= -10)
		{
			velocity = glm::vec4(-velocity.x, velocity.y, -velocity.z * restitution, 1);
			vert->position.z = (inverse * glm::vec4(0, 0, -10, 1)).z;
		}

		// ToDo: Integrate euler method
		acceleration = forces / mass;
		velocity += acceleration * dt;
		vert->position += velocity * dt;
		forces = glm::vec3(0.0);
	}
}