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

		virtual type getType() = 0;
		virtual void* getData() { return nullptr; }
	};

	using ComponentInstance = Component * ;
}