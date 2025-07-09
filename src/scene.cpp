#include "scene.hpp"
#include "camera.hpp"
#include "shadermanager.hpp"
#include "texturemanager.hpp"
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>
#include "shapecreator.hpp"
#include "transform.hpp"
#include <iostream>

namespace graf
{
    Scene::Scene()
    {
        try {
            m_activeCamera = new Camera();
            if (!m_activeCamera) {
                throw std::runtime_error("Kamera oluşturulamadı!");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Scene oluşturma hatası: " << e.what() << std::endl;
            throw;
        }
    }

    void Scene::addModel(const string& fileName)
    {
        try {
            Model* model = Model::loadModelFromFile(fileName);
            if (!model) {
                std::cerr << "Model yüklenemedi: " << fileName << std::endl;
                return;
            }
            addModel(model);
        }
        catch (const std::exception& e) {
            std::cerr << "Model yükleme hatası: " << e.what() << std::endl;
        }
    }

    void Scene::addModel(Model* model)
    {
        if (!model) {
            std::cerr << "Null model eklenemez!" << std::endl;
            return;
        }

        if (std::find(m_modelList.begin(), m_modelList.end(), model) == m_modelList.end())
        {
            try {
                string textureName = model->getTextureName();
                string shaderName = model->getShaderProgramName();
                
                if (!textureName.empty()) {
                    TextureManager::addTextureFromFile(textureName);
                }
                
                if (!shaderName.empty()) {
                    ShaderManager::addShaderFromFile(shaderName);
                }
                
                m_modelList.push_back(model);
            }
            catch (const std::exception& e) {
                std::cerr << "Model ekleme hatası: " << e.what() << std::endl;
            }
        }
    }

    Camera* Scene::getActiveCamera()
    {
        return m_activeCamera;
    }

    void Scene::removeModel(Model* model)
    {
        if (!model) {
            std::cerr << "Null model silinemez!" << std::endl;
            return;
        }

        auto it = std::find(m_modelList.begin(), m_modelList.end(), model);
        if (it != m_modelList.end())
        {
            m_modelList.erase(it);
        }
    }

    void Scene::render()
    {
        if (!m_activeCamera) {
            std::cerr << "Aktif kamera bulunamadı!" << std::endl;
            return;
        }

        try {
            for (auto model : m_modelList)
            {
                if (model) {
                    model->draw(m_activeCamera->getProjMatrix() * m_activeCamera->getViewMatrix());
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Render hatası: " << e.what() << std::endl;
        }
    }

    void Scene::createGameField(int width, int length, float cubeScale)
{
    if (width <= 0 || length <= 0) {
        std::cerr << "Geçersiz oyun alanı boyutları!" << std::endl;
        return;
    }

    float startX = -(width * cubeScale) / 2.0f;
    float startZ = -(length * cubeScale) / 2.0f;
    float baseHeight = -2.0f;
    float wallHeight = 3.0f; // Duvar yüksekliği

    // Zemini oluştur
    for (int x = 0; x < width; x++) {
        for (int z = 0; z < length; z++) {
            try {
                Model* cube = Model::loadModelFromFile("LightTextureCube.json");
                if (!cube) {
                    std::cerr << "Küp modeli yüklenemedi!" << std::endl;
                    continue;
                }

                float posX = startX + (x * cubeScale);
                float posZ = startZ + (z * cubeScale);
                cube->getTransform()->setPosition(glm::vec3(posX, baseHeight, posZ));
                addModel(cube);
            }
            catch (const std::exception& e) {
                std::cerr << "Küp oluşturma hatası: " << e.what() << std::endl;
            }
        }
    }

    // Duvarları oluştur
    try {
        // Sol duvar
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < wallHeight; y++) {
                Model* cube = Model::loadModelFromFile("LightTextureCube1.json");
                if (!cube) continue;
                float posX = startX + (x * cubeScale);
                float posY = baseHeight + (y * cubeScale) + cubeScale;
                float posZ = startZ - cubeScale;
                cube->getTransform()->setPosition(glm::vec3(posX, posY, posZ));
                addModel(cube);
            }
        }

        // Sağ duvar
     for (int x = 0; x < width; x++) {
        for (int y = 0; y < wallHeight; y++) {
            Model* cube = Model::loadModelFromFile("LightTextureCube1.json");
            if (!cube) continue;
            
            float posX = startX + (x * cubeScale);
            float posY = baseHeight + (y * cubeScale) + cubeScale;
            float posZ = startZ + (length * cubeScale);
            
            // Küpü 180 derece Y ekseni etrafında döndür
            cube->getTransform()->setPosition(glm::vec3(posX, posY, posZ));
            cube->getTransform()->setEuler(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
            
            addModel(cube);
        }
    }

        // Ön duvar
        for (int z = -1; z <= length; z++) {
            for (int y = 0; y < wallHeight; y++) {
                Model* cube = Model::loadModelFromFile("LightTextureCube1.json");
                if (!cube) continue;
                float posX = startX - cubeScale;
                float posY = baseHeight + (y * cubeScale) + cubeScale;
                float posZ = startZ + (z * cubeScale);
                cube->getTransform()->setPosition(glm::vec3(posX, posY, posZ));
                addModel(cube);
            }
        }

        // Arka duvar
        for (int z = -1; z <= length; z++) {
            for (int y = 0; y < wallHeight; y++) {
                Model* cube = Model::loadModelFromFile("LightTextureCube1.json");
                if (!cube) continue;
                float posX = startX + (width * cubeScale);
                float posY = baseHeight + (y * cubeScale) + cubeScale;
                float posZ = startZ + (z * cubeScale);
                cube->getTransform()->setPosition(glm::vec3(posX, posY, posZ));
                addModel(cube);
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Duvar oluşturma hatası: " << e.what() << std::endl;
    }
}

    Scene::~Scene()
    {
        try {
            // Model listesini temizle
            for (auto model : m_modelList)
            {
                delete model;
            }
            m_modelList.clear();

            // Kamerayı temizle
            if (m_activeCamera) {
                delete m_activeCamera;
                m_activeCamera = nullptr;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Scene yıkıcı hatası: " << e.what() << std::endl;
        }
    }
}