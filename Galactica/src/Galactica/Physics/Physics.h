#pragma once
#include "Galactica/Renderer/Mesh.h"
#include "Galactica/Renderer/Model.h"

namespace Galactica
{
	class SoftBody;
	class PointMass;

	struct Spring
	{
		PointMass* a;
		PointMass* b;
		float restLength;
	};

	class GALACTICA_API PointMass
	{
	public:
		PointMass(Vertex* vert, SoftBody* body, float restitution, float mass, float stiffness, float damping);
		~PointMass();

		Vertex* vert; // Pointer to vert being integrated
		SoftBody* body;

		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::vec3 forces;

		float restitution;
		float mass;
		float stiffness;
		float damping;

		void Integrate(float dt);

		bool operator==(const PointMass& other) const {
			return *this == other;
		}

		bool operator!=(const PointMass& other) const {
			return !(*this == other);
		}
	};

	class GALACTICA_API SoftBody : public Model
	{
	public:
		SoftBody(std::string path, float restitution, float mass, float stiffness, float damping);
		~SoftBody();

		float restitution;
		float mass;
		float stiffness;
		float damping;

		int springCount = 0;

		// Vertices we draw, initially set to model's verts
		std::vector<Vertex> dynamicVertices;
		std::vector<Spring> springs;
		std::vector<PointMass> pointMasses;

		void AddForce(glm::vec3(force));
		void Update(float dt);
		void Reset();
		void AddSpring(PointMass* a, PointMass* b);
		int GetSpringCount() { return springs.size(); }
	};;
}
