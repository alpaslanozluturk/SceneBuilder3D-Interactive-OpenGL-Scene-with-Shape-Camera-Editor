#ifndef scene_hpp
#define scene_hpp
#include "Model.hpp"
namespace graf
{
    class ShapeCreator;
    class Camera;
    class Scene
    {
    public:
        Scene();
        // Destructor'ı ekleyelim
        ~Scene();
        
        void addModel(Model* model);
        void addModel(const string& fileName);
        void removeModel(Model* model);
        void render();
        void renderGui();
        Camera* getActiveCamera();
        void createGameField(int width = 10, int length = 10, float cubeScale = 1.0f);

    private:
        Camera*     m_activeCamera;
        ModelList   m_modelList;
    };
}
#endif