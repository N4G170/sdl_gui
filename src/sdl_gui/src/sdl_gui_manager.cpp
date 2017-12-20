#include "sdl_gui_manager.hpp"
#include <utility>
#include <iostream>

namespace sdl_gui
{
//<f> Constructors & operator=
GuiManager::GuiManager(SDL_Renderer* renderer_ptr, ResourceManager* resource_manager_ptr): m_elements{}, m_renderer_ptr{renderer_ptr}, m_resource_manager_ptr{resource_manager_ptr},
    m_main_camera_ptr{new Camera({0,0},{2000,2000})}
{

}

GuiManager::~GuiManager() noexcept
{

}

GuiManager::GuiManager(const GuiManager& other) : m_elements{}, m_renderer_ptr{other.m_renderer_ptr}, m_resource_manager_ptr{other.m_resource_manager_ptr},
    m_main_camera_ptr{new Camera(*other.m_main_camera_ptr.get())}
{
//duplicate m_elements{}
}

GuiManager::GuiManager(GuiManager&& other) noexcept : m_elements{std::move(other.m_elements)}, m_renderer_ptr{std::move(other.m_renderer_ptr)},
    m_resource_manager_ptr{std::move(other.m_resource_manager_ptr)}, m_main_camera_ptr{std::move(other.m_main_camera_ptr)}
{

}

GuiManager& GuiManager::operator=(const GuiManager& other)
{
    if(this != &other)//not same ref
    {
        GuiManager tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

GuiManager& GuiManager::operator=(GuiManager&& other) noexcept
{
    if(this != &other)
    {
        m_renderer_ptr = std::move(other.m_renderer_ptr);
        m_resource_manager_ptr = std::move(other.m_resource_manager_ptr);
        m_main_camera_ptr = std::move(other.m_main_camera_ptr);
    }
    return *this;
}
//</f>

//<f> Element Virtual Calls
void GuiManager::Input(const SDL_Event& event)
{
    for(auto& element : m_elements)
    {
        if(element->IsActive() && !element->DisabledSelfOrParent())
            element->Input(event);
    }
}

void GuiManager::FixedLogic(float fixed_delta_time)
{
    for(auto& element : m_elements)
    {
        if(!element->DisabledSelfOrParent())
            element->FixedLogic(fixed_delta_time);
    }
}

void GuiManager::Logic(float delta_time)
{
    for(auto& element : m_elements)
    {
        if(!element->DisabledSelfOrParent())
            element->Logic(delta_time);
    }
}

void GuiManager::Render(float delta_time)
{
    //order based on renderindex
    if(!std::is_sorted(std::begin(m_elements), std::end(m_elements), [](auto& lhs, auto& rhs){ return lhs->GlobalRenderIndex() > rhs->GlobalRenderIndex(); }))
        std::sort( std::begin(m_elements), std::end(m_elements), [](auto& lhs, auto& rhs){ return lhs->GlobalRenderIndex() > rhs->GlobalRenderIndex(); } );

    for(auto& element : m_elements)
    {
        if(element->CanRender() && !element->DisabledSelfOrParent())
            element->Render(delta_time);
    }
}
//</f>

//<f> Element Management
/**
 * \brief Specialization of \see T* ReleaseElement(UID uid)
 */
template<>
GuiElement* GuiManager::ReleaseElement<GuiElement>(UID uid)
{
    auto find_result{ std::find_if( std::begin(m_elements), std::end(m_elements), [uid](auto& element)->bool{ return uid == element.get()->ElementUID(); } ) };
    if(find_result != std::end(m_elements))//found element
    {
        auto tmp_ptr{find_result->release()};//release pointer from unique_ptr
        m_elements.erase(find_result);//remove from vector table

        return tmp_ptr;
    }
    return nullptr;
}

void GuiManager::DeleteElement(UID uid)
{
    auto find_result{ std::find_if( std::begin(m_elements), std::end(m_elements), [uid](auto& element)->bool{ return uid == element.get()->ElementUID(); } ) };
    if(find_result != std::end(m_elements))//found element
    {
        find_result->reset();//reset pointer from unique_ptr
        m_elements.erase(find_result);//remove from vector table
    }
}

void GuiManager::ClearElementsInput()
{
    for(auto& element : m_elements)
        element->ClearInput();
}
//</f>
}//namespace
