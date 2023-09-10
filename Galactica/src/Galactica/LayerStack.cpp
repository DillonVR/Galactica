#include "glpch.h"
#include "LayerStack.h"
namespace Galactica
{
	LayerStack::LayerStack()
	{
		m_LayerInsert = m_layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for(Layer* layer : m_layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_layers.emplace(m_LayerInsert, layer);
	}

	void LayerStack::PushOverLayer(Layer* overlayer)
	{
		m_layers.emplace_back(overlayer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverLayer(Layer* overlayer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlayer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
		}
	}
}