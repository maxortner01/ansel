#pragma once

namespace ECS
{
	class Component
	{

	public:
		enum TYPES {
			RENDERABLE,
			BASELESS,
			SCRIPT,
			CONTROLLER
		} typedef type;

		virtual void* getData() { return nullptr; }
		virtual type getType() = 0;
	};

	using ComponentInstance = Component * ;
}