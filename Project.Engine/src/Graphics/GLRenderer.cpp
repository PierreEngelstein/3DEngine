#include <Graphics/GLRenderer.hpp>
#include "GL/glew.h"
#include <iostream>

namespace Graphics
{
    GLRenderer::GLRenderer() : m_width(0), m_height(0)
    {

    }
    GLRenderer::~GLRenderer()
    {

    }

    bool GLRenderer::Init(unsigned int width, unsigned int height)
    {
        m_width = width;
        m_height = height;

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if(glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to init glew !\n";
            return false;
        }
        if(!GLEW_VERSION_3_0)
        {
            std::cerr << "No correct glew version!\n";
            return false;
        }

        return true;
    }

    void GLRenderer::AddDebugMesh()
    {

    }

    void GLRenderer::OnFrameStart()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.17578125f, 0.17578125f, 0.17578125f, 0.0f);
        glViewport(0, 0, m_width, m_height);
    }

    void GLRenderer::Render()
    {

    }

    void GLRenderer::DrawDebug()
    {

    }

    void GLRenderer::OnFrameEnd()
    {

    }

    void GLRenderer::SetViewport(unsigned int width, unsigned int height)
    {
        m_width = width;
        m_height = height;
    }

    void GLRenderer::ShutDown()
    {

    }
}