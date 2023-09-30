#pragma once

#include "Galactica/Core.h"
#include "Layer.h"

namespace Galactica
{

	class GALACTICA_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overlayer);
		void PopLayer(Layer* layer);
		void PopOverLayer(Layer* overlayer);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }

	private:
		std::vector<Layer*> m_layers;

		std::vector<Layer*>::iterator m_LayerInsert;

		unsigned int m_LayerInsertIndex = 0;
	};

}