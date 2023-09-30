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
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		//m_LayerInsert = m_layers.emplace(m_LayerInsert, layer);

		m_layers.emplace(m_layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverLayer(Layer* overlayer)
	{
		m_layers.emplace_back(overlayer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.begin() + m_LayerInsertIndex, layer);
		if (it != m_layers.end())
		{
			layer->OnDetach();
			m_layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverLayer(Layer* overlayer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlayer);
		if (it != m_layers.end())
		{
			overlayer->OnDetach();
			m_layers.erase(it);
		}
	}
}