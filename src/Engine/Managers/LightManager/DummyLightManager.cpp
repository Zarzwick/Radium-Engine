#include "DummyLightManager.hpp"

#include <Engine/RadiumEngine.hpp>
#include <Engine/Renderer/RenderObject/RenderObject.hpp>
#include <Engine/Renderer/OpenGL/OpenGL.hpp>

namespace Ra {
    namespace Engine {

        DummyLightManager::DummyLightManager()
        {
            m_data.reset( new DummyLightStorage() );

            // Add a dummy light FIXME (Hugo).
            DirectionalLight *l = new DirectionalLight();
            l->setDirection(Core::Vector3(0.3f, -1.0f, 0.0f));
            m_data->push(l);
        }

        const Light* DummyLightManager::getLight(uint li) const
        {
            return (*m_data)[li];
        }

        //
        // Pre/Post render operations.
        //

        void DummyLightManager::preprocess(const Ra::Engine::RenderData &rd)
        {
            renderData = rd;
        }

        void DummyLightManager::prerender(unsigned int li)
        {
            Light *light = (*m_data.get())[li];
            params = RenderParameters();
            light->getRenderParameters(params);
        }
        
        void DummyLightManager::render(RenderObject *ro, unsigned int li, RenderTechnique::PassName passname)
        {
            ro->render( params, renderData, passname );
        }

        void DummyLightManager::postrender(unsigned int li)
        {
        }

        void DummyLightManager::postprocess()
        {
            // Eventually, this would be a good idea to disable GL_BLEND, and
            // all what was enabled in preprocess().
        }



        DummyLightStorage::DummyLightStorage()
        {
        }

        void DummyLightStorage::upload() const
        {
        }

        void DummyLightStorage::push(Light* li)
        {
            // FIXME (Hugo) This code shouldn't exist.
            // It was done for deadline purpose :)
            // It will be removed.
            DirectionalLight *dl = static_cast<DirectionalLight*>(li);
            // TODO: evil here, but work as sample
            // URG:  would be change
            m_lights.push_back(std::shared_ptr<DirectionalLight>(dl));
        }

        size_t DummyLightStorage::size() const
        {
            return m_lights.size();
        }

        void DummyLightStorage::clear()
        {
            m_lights.clear();
        }

        Light* DummyLightStorage::operator[](unsigned int n)
        {
            return m_lights[n].get();
        }
    }
}
