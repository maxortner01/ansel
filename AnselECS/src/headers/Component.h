#pragma once

namespace ECS
{
	class Component
	{
	protected:
		int __derivative = -1;

	public:
		enum TYPES {
			RENDERABLE,
			BASELESS,
			SCRIPT,
			CONTROLLER
		};

		virtual int getType() = 0;
		virtual void* getData() { return nullptr; }
		
		int getDerivative() {
			return __derivative;
		}

		template<typename T>
		T cast() {
			return (T)this;
		}
	};

	using ComponentInstance = Component * ;
}