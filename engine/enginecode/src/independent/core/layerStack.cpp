/* \file layerStack.cpp */

#include "engine_pch.h"
#include "core/layerStack.h"
#include "include/independent/systems/log.h"

namespace Engine
{
	LayerStack::~LayerStack()
	{
		for (uint32_t i = m_stack.size(); i > 0; i--) pop();
	}

	void LayerStack::push(Layer* layer)
	{
		layer->onAttach();
		std::shared_ptr<Layer> tmp(layer);
		m_stack.push_back(tmp);
	}

	void LayerStack::pop()
	{
		m_stack.back()->onDetach();
		m_stack.pop_back();
	}

	void LayerStack::update(float timestep)
	{
		for (auto& layer : m_stack) if (layer->isActive())layer->onUpdate(timestep);
		int newscores[4] = { 0,0,0,0 };
		int newtime = 0;
		for (auto& layer : m_stack)
		{
			if (layer->getName() == "Scene Layer") {
				for (int i = 0; i < 4; i++) {
					newscores[i] = layer->scores[i];
				}
				newtime = layer->selecttime;
			}
		}

		for (auto& layer : m_stack)
		{
			if (layer->getName() == "InGame Layer") {
				for (int i = 0; i < 4; i++) {
					layer->scores[i] = newscores[i];
				}
				layer->selecttime = newtime;
			}
		}
	}

	void LayerStack::render()
	{
		for (auto& layer : m_stack) if (layer->isDisplayed()) layer->onRender();
	}

	void LayerStack::onKeyPressed(KeyPressedEvent& e)
	{
		for (auto& layer : m_stack) if (layer->isFocused() && !e.handled()) layer->onKeyPressed(e);
	}

	void LayerStack::onKeyReleased(KeyReleasedEvent& e)
	{
		e.handle(false);
	}

	void LayerStack::onMousePressed(MouseButtonPressedEvent& e)
	{
		e.handle(false);
		for (auto& layer : m_stack) if (layer->isFocused() && !e.handled()) layer->onMousePressed(e);
	}

	void LayerStack::onMouseReleased(MouseButtonReleasedEvent& e)
	{
		e.handle(false);
	}

	void LayerStack::onMouseWheel(MouseScrolledEvent& e)
	{
		e.handle(false);
	}

	void LayerStack::onMouseMoved(MouseMovedEvent& e)
	{
		e.handle(false);
	}

	void LayerStack::setActive(const char* layerName, bool activeState)
	{
		for (auto& layer : m_stack)
		{
			if (layer->getName() == layerName) layer->setActive(activeState);
		}
	}

	void LayerStack::setAllActive(bool activeState)
	{
		for (auto& layer : m_stack)
		{
			layer->setActive(activeState);
		}
	}

	void LayerStack::setDisplayed(const char* layerName, bool displayState)
	{
		for (auto& layer : m_stack)
		{
			if (layer->getName() == layerName) layer->setActive(displayState);
		}
	}

	void LayerStack::setAllDisplayed(bool displayState)
	{
		for (auto& layer : m_stack)
		{
			layer->setActive(displayState);
		}
	}

	void LayerStack::setFocused(const char* layerName, bool focusedState)
	{
		for (auto& layer : m_stack)
		{
			if (layer->getName() == layerName) layer->setActive(focusedState);
		}
	}

	void LayerStack::setAllFocused(bool focusedState)
	{
		for (auto& layer : m_stack)
		{
			layer->setActive(focusedState);
		}
	}

	void LayerStack::SetVariables() {
		std::shared_ptr<Layer> newscene = nullptr;
		std::shared_ptr<Layer> newgame = nullptr;
		std::shared_ptr<Layer> newmenu = nullptr;
		for (auto& layer : m_stack)
		{
			if (layer->getName() == "Scene Layer") newscene = layer;
			if (layer->getName() == "InGame Layer") newgame = layer;
			if (layer->getName() == "Menu Layer") newmenu = layer;
		}

		for (auto& layer : m_stack)
		{
			layer->scene = newscene;
			layer->game = newgame;
			layer->menu = newmenu;
		}
	}
}