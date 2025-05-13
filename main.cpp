#include "includes/mine/auxiliary.h"
#include "includes/mine/Model.h"
#include <iostream>
#include <thread>
#include <future>
#include <fstream>

enum class LightType
{
    POINT,
    DIRECTIONAL,
    POINT_DIRECTIONAL,
    PBR
};

struct Light
{
    Model source;

    glm::vec3 diffuse;

    glm::vec3 specular;

    glm::vec3 ambient;

    glm::vec3 color;

    glm::vec3 direction;

    LightType type;

    float constant;
    float linear;
    float quadratic;
    float cutoff;

    Light(const Camera &camera, LightType type) : source("models/sun/sun.gltf", camera), type(type)
    {
        direction = source.position = glm::vec3(0.f);
        ambient = specular = color = diffuse = glm::vec3(1.f);
        constant = 1.f;
        linear = 0.2f;
        quadratic = 0.032f;
    }

    void apply_to_shader(MShader &shader, int index = 0)
    {
        if (type == LightType::POINT)
        {
            shader.setInt(("light[" + std::to_string(index) + "].type").c_str(), 0);
            shader.setVec3(("light[" + std::to_string(index) + "].position").c_str(), source.position);
            shader.setVec3(("light[" + std::to_string(index) + "].diffuse").c_str(), diffuse);
            shader.setVec3(("light[" + std::to_string(index) + "].specular").c_str(), specular);
            shader.setVec3(("light[" + std::to_string(index) + "].ambient").c_str(), ambient);
            shader.setVec3(("light[" + std::to_string(index) + "].color").c_str(), color);
            shader.setFloat(("light[" + std::to_string(index) + "].linear").c_str(), linear);
            shader.setFloat(("light[" + std::to_string(index) + "].constant").c_str(), constant);
            shader.setFloat(("light[" + std::to_string(index) + "].quadratic").c_str(), quadratic);
        }
        else if (type == LightType::DIRECTIONAL)
        {
            shader.setInt(("light[" + std::to_string(index) + "].type").c_str(), 1);
            shader.setVec3(("light[" + std::to_string(index) + "].direction").c_str(), direction);
            shader.setVec3(("light[" + std::to_string(index) + "].ambient").c_str(), ambient);
        }
        else if (type == LightType::POINT_DIRECTIONAL)
        {
            shader.setInt(("light[" + std::to_string(index) + "].type").c_str(), 2);
            shader.setVec3(("light[" + std::to_string(index) + "].direction").c_str(), direction);
            shader.setVec3(("light[" + std::to_string(index) + "].ambient").c_str(), ambient);
            shader.setFloat(("light[" + std::to_string(index) + "].cutoff").c_str(), cutoff);
        }
        else if (type == LightType::PBR)
        {
            shader.setInt(("light[" + std::to_string(index) + "].type").c_str(), 3);
            shader.setVec3(("light[" + std::to_string(index) + "].position").c_str(), source.position);
            shader.setVec3(("light[" + std::to_string(index) + "].color").c_str(), color);
        }
    }
};

unsigned int depthMapFBO, depthMap;
constexpr unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

void setupShadowMap()
{
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main()
{
    GLFWwindow *window = initGLFWGLAD();

    glClearColor(0.f, 0.f, 0.f, 1.f);

    ImGuiIO &io = initImGui(window);

    Camera camera(window, glm::vec3(0.f, 1.f, 1.f), 3.f);

    MShader shader;
    shader.autoCompileAndLink("shaders/common.vert", "shaders/ar.frag");

    MShader sunShader;
    sunShader.autoCompileAndLink("shaders/common.vert", "shaders/lightSource.frag");

    MShader sponzaShader;
    sponzaShader.autoCompileAndLink("shaders/common.vert", "shaders/sponzaScene.frag");

    MShader sphereShader;
    sphereShader.autoCompileAndLink("shaders/pbrSphere.vert", "shaders/pbrSphere.frag");

    MShader rectShader;
    rectShader.autoCompileAndLink("shaders/pbrRect.vert", "shaders/pbrRect.frag");

    MShader shadowMapShader;
    shadowMapShader.autoCompileAndLink("shaders/shadowMap.vert", "shaders/shadowMap.frag");

    setupShadowMap();

    Model scene("models/sponza/Sponza.gltf", camera);

    scene.scale = glm::vec3(0.005f);

    scene.position = glm::vec3(0.f, 0.f, -4.1f);

    scene.angles = glm::vec3(0.f, 83.72f, 0.f);

    Model sphere("models/highPolySphere/sphere.gltf", camera);

    sphere.position = glm::vec3(0.f, 0.635f, 0.f);

    sphere.scale = glm::vec3(0.1f);

    glm::vec3 sphereAlbedo = glm::vec3(0.5f, 0.5f, 0.5f);
    float sphereRoughness = 0.5f;
    float sphereMetallic = 0.5f;
    float sphereAO = 1.f;

    constexpr int lightCount = 1;

    std::vector<Light> lights;
    bool currentLight[lightCount];

    for (int i = 0; i < lightCount; ++i)
        currentLight[i] = false;

    for (int i = 0; i < lightCount; ++i)
    {
        lights.emplace_back(camera, LightType::POINT);
        lights[i].source.scale = glm::vec3(0.02f);
    }

    lights[0].source.position = glm::vec3(-0.116f, 1.977f, 0.f);

    std::vector<Light> pbrLights;

    for (int i = 0; i < lightCount; i++)
        pbrLights.emplace_back(camera, LightType::PBR);

    for (Light &light : pbrLights)
        light.type = LightType::PBR;

    for (int i = 0; i < lightCount; i++)
    {
        pbrLights[i].source.position = lights[i].source.position;
        pbrLights[i].color = lights[i].color;
    }

    shader.setInt("lightCount", lightCount);
    rectShader.setInt("lightCount", lightCount);



    sponzaShader.setInt("lightCount", lightCount);
    sponzaShader.setInt("light[0].type", 0);

    sphereShader.setInt("lightCount", lightCount);

    glm::vec3 sunColor = glm::vec3(1.f, 1.f, 0.f);

    bool freeScale = false;

    float perspFov = 45.f;
    float perspNear = 0.1f;
    float perspFar = 10.f;

    float orthoSize = 10.f;

    while (!glfwWindowShouldClose(window))
    {
        glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, perspNear, perspFar);

        glm::mat4 lightView = glm::lookAt(
            lights[0].source.position,
            glm::vec3(0.f),
            glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        shadowMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        shadowMapShader.setMat4("model", scene.getModel());
        scene.render(shadowMapShader, false);

        shadowMapShader.setMat4("model", sphere.getModel());
        sphere.render(shadowMapShader, false);

        for (auto& light : lights) {
            shadowMapShader.setMat4("model", light.source.getModel());
            light.source.render(shadowMapShader, false);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (windowResized)
            camera.updateProjection(WINDOW_WIDTH, WINDOW_HEIGHT);

        sunShader.setVec3("color", sunColor);

        int lightIndex = 0;

        for (auto &light : lights)
        {
            light.apply_to_shader(sponzaShader, lightIndex);
            ++lightIndex;
        }

        lightIndex = 0;
        for (auto &light : pbrLights)
        {
            light.apply_to_shader(sphereShader, lightIndex);
            light.apply_to_shader(rectShader, lightIndex);
            ++lightIndex;
        }

        for (int i = 0; i < lightCount; i++)
        {
            pbrLights[i].source.position = lights[i].source.position;
            pbrLights[i].color = lights[i].color;
        }

        sphereShader.setVec3("material.albedo", sphereAlbedo);
        sphereShader.setFloat("material.roughness", sphereRoughness);
        sphereShader.setFloat("material.metallic", sphereMetallic);
        sphereShader.setFloat("material.ao", sphereAO);

        rectShader.setVec3("material.albedo", sphereAlbedo);
        rectShader.setFloat("material.roughness", sphereRoughness);
        rectShader.setFloat("material.metallic", sphereMetallic);
        rectShader.setFloat("material.ao", sphereAO);

        glActiveTexture(GL_TEXTURE0 + 10);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        sphereShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        sphereShader.setInt("shadowMap", 10);

        sponzaShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        sponzaShader.setInt("shadowMap", 10);

        scene.render(sponzaShader);

        sphere.render(sphereShader);

        for (auto &light : lights)
            light.source.render(sunShader);



        newFrame();

        ImGui::Begin("Perspective");

        ImGui::SliderFloat("FOV", &perspFov, 1.f, 179.f);

        ImGui::SliderFloat("Near", &perspNear, 0.1f, 10.f);

        ImGui::SliderFloat("Far", &perspFar, 0.f, 100.f);

        ImGui::End();

        ImGui::Begin("Sphere");

        ImGui::SliderFloat3("Position", glm::value_ptr(sphere.position), -10.f, 10.f);

        ImGui::SliderFloat3("Scale", glm::value_ptr(sphere.scale), 0.f, 5.f);

        ImGui::SliderFloat3("Albedo", glm::value_ptr(sphereAlbedo), 0.f, 1.f);

        ImGui::SliderFloat("Roughness", &sphereRoughness, 0.f, 1.f);

        ImGui::SliderFloat("Metallic", &sphereMetallic, 0.f, 1.f);

        ImGui::SliderFloat("AO", &sphereAO, 0.f, 1.f);

        ImGui::End();

        ImGui::Begin("City");

        ImGui::SliderFloat3("Position", glm::value_ptr(scene.position), -5.f, 5.f);

        ImGui::SliderFloat3("Scale", glm::value_ptr(scene.scale), 0.001f, 0.015f);

        ImGui::SliderFloat3("Rotation", glm::value_ptr(scene.rotation), 0.f, 1.f);

        ImGui::SliderFloat3("Angle", glm::value_ptr(scene.angles), 0.f, 360.f);

        ImGui::End();

        ImGui::Begin("Lights");

        for (int i = 0; i < lightCount; ++i)
        {
            ImGui::Checkbox(("Light " + std::to_string(i)).c_str(), &currentLight[i]);

            if (currentLight[i])
            {
                ImGui::SliderFloat3("Position", glm::value_ptr(lights[i].source.position), -5.f, 10.f);

                ImGui::SliderFloat3("Scale", glm::value_ptr(lights[i].source.scale), 0.f, 5.f);

                ImGui::SliderFloat3("Color", glm::value_ptr(lights[i].color), 0.f, 1.f);

                ImGui::SliderFloat3("Diffuse", glm::value_ptr(lights[i].diffuse), 0.f, 1.f);

                ImGui::SliderFloat3("Specular", glm::value_ptr(lights[i].specular), 0.f, 1.f);

                ImGui::SliderFloat3("Ambient", glm::value_ptr(lights[i].ambient), 0.f, 1.f);

                ImGui::SliderFloat("Constant", &lights[i].constant, 0.f, 2.f);

                ImGui::SliderFloat("Linear", &lights[i].linear, 0.f, 2.f);

                ImGui::SliderFloat("Quadratic", &lights[i].quadratic, 0.f, 2.f);
            }
        }

        ImGui::End();

        ImGui::Begin("Lights values");

        if (ImGui::Button("Save"))
        {
            std::ofstream file("values/lights.txt");
            for (int i = 0; i < lights.size(); ++i)
            {
                file << lights[i].source.position.x << " " << lights[i].source.position.y << " " << lights[i].source.position.z << "\n";
                file << lights[i].color.x << " " << lights[i].color.y << " " << lights[i].color.z << "\n";
                file << lights[i].diffuse.x << " " << lights[i].diffuse.y << " " << lights[i].diffuse.z << "\n";
                file << lights[i].specular.x << " " << lights[i].specular.y << " " << lights[i].specular.z << "\n";
                file << lights[i].ambient.x << " " << lights[i].ambient.y << " " << lights[i].ambient.z << "\n";
                file << lights[i].constant << "\n";
                file << lights[i].linear << "\n";
                file << lights[i].quadratic << "\n";
            }
            file.close();
        }

        if (ImGui::Button("Load"))
        {
            std::ifstream file("values/lights.txt");
            if (file.is_open())
            {
                for (int i = 0; i < lights.size(); ++i)
                {
                    file >> lights[i].source.position.x >> lights[i].source.position.y >> lights[i].source.position.z;
                    file >> lights[i].color.x >> lights[i].color.y >> lights[i].color.z;
                    file >> lights[i].diffuse.x >> lights[i].diffuse.y >> lights[i].diffuse.z;
                    file >> lights[i].specular.x >> lights[i].specular.y >> lights[i].specular.z;
                    file >> lights[i].ambient.x >> lights[i].ambient.y >> lights[i].ambient.z;
                    file >> lights[i].constant;
                    file >> lights[i].linear;
                    file >> lights[i].quadratic;
                }
                file.close();
            }
        }

        ImGui::End();

        ImGui::Begin("Misc");

        if (ImGui::Button("Cap FPS"))
            glfwSwapInterval(1);

        if (ImGui::Button("Uncap FPS"))
            glfwSwapInterval(0);

        ImGui::End();

        renderFrame();

        glfwSwapBuffers(window);

        proccesEvents(window);

        camera.update(window);
    }

    terminateImGui();
    terminateGLFW(window);
}