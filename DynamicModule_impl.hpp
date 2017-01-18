#include "DynamicModule.hpp"
#include "InterpretedFunction.hpp"

namespace mutils{
	template<typename Ret, typename... Args>
	InterpretedFunction<Ret, Args...> DynamicModule::load (const std::string& name) const{
		return InterpretedFunction<Ret, Args...>{*this,name};
	}

	template<typename Ret, typename... Args>
	Ret (*) (Args...) DynamicModule::load_internal (const std::string& name) const{
		return load_symbol(name.c_str());
	}
	
	DynamicModule::~DynamicModule();
}
