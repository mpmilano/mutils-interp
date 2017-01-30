#pragma once
#include "has_mem_func.hpp"
#include <type_traits>
#include <memory>

namespace mutils {

	namespace compiler_concept_utilitis{
		HAS_MEM_FUNC(compile, has_compile);
		HAS_MEM_FUNC(mangle,has_mangle);
	}
	
	//this should be a concept, but I don't want to rely on those right now.

	template<typename T>
	using is_compiler = std::integral_constant<
		bool,
		compiler_concept_utilitis::has_compile<T,std::unique_ptr<DynamicModule> (T::*)(const std::string&)>::value &&
		compiler_concept_utilitis::has_mangle<T,std::string (T::*)(const std::string&)>::value >;

}
