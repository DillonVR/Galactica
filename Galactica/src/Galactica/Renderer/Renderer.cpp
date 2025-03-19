#include "glpch.h"

#include "glad/glad.h"
#include "Renderer.h"
#include <backends/imgui_impl_opengl3_loader.h>
#include <Galactica/Vendor/glm/glm/gtc/type_ptr.hpp>


namespace Galactica::Renderer
{
        
        Shader* shader;
        Model* model;
        Model* light;
        Model* plane;

    void Renderer::Setup(const char* vertexPath, const char* fragmentPath)
    {
        // Set up gummy shader
    	shader = new Shader(vertexPath, fragmentPath);

        // Enable z-depth buffer
        glEnable(GL_DEPTH_TEST);

        

        // Use default shader
        shader->use();

        // Enable openGL color/alpha blending.
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer::Draw(Model* light, std::vector<Model*> scene, CameraControl* cam, bool debug)
    {
        // Check we have a light and models
        assert(light != nullptr && "ERROR: No light provided!");
        assert(scene.size() > 0 && "ERROR: Scene is empty!");

        if (debug == true)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
			

        // Projection matrix

        glm::mat4 projection = cam->GetProjection();

        // View matrix
        glm::mat4 view = cam->GetViewMatrix();

        // Uniforms
        int transformLoc = glGetUniformLocation(shader->ID, "transform");
        int modelViewLoc = glGetUniformLocation(shader->ID, "view");
        int modelViewPosLoc = glGetUniformLocation(shader->ID, "viewPos");
        int modelProjectionLoc = glGetUniformLocation(shader->ID, "projection");
        int boolLoc = glGetUniformLocation(shader->ID, "calculateLighting");
        int colorLoc = glGetUniformLocation(shader->ID, "color");

        // Update light translate, send to uniform
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(light->getTransform()));

        // Update light color, enable rendering calculations 
        glUniform3f(colorLoc, light->color.x, light->color.y, light->color.z);
        glUniform1i(boolLoc, 0);

        int lightPosLoc = glGetUniformLocation(shader->ID, "lightPos");
        glUniform3f(lightPosLoc, light->pos.x, light->pos.y, light->pos.z);// Set light position.
        light->draw(*shader);

        // Render all other objects in the scene
        for (Model* model : scene) 
        {
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model->getTransform()));
            glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(modelProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

            // Send model data
            glUniform3f(colorLoc, model->color.x, model->color.y, model->color.z);
            glUniform3f(modelViewPosLoc, cam->GetPosition().x, cam->GetPosition().y, cam->GetPosition().z);
            glUniform1i(boolLoc, 1);

            // Render model
            model->draw(*shader);
        }
    }
}
