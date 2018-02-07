#include <Engine/Managers/LightManager/LightManager.hpp>


namespace Ra {
    namespace Engine {

        LightManager::LightManager()
        : m_data( nullptr )
        {
        }

        LightManager::~LightManager()
        {
        }


        //
        // Renderer pre/post calls
        //

        int LightManager::count() const
        {
            return m_data->size();
        }


        //
        // System
        //

        void LightManager::generateTasks( Core::TaskQueue* taskQueue, const Engine::FrameInfo& frameInfo )
        {
        }

        void LightManager::handleAssetLoading( Entity* entity, const Asset::FileData* data )
        {
            // ...
        }
    }
}
