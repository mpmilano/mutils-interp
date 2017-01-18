#pragma once
#include "mutils.hpp"
#include "DynamicModule.hpp"
#include "Compiler.hpp"
#include <iostream>
#include <cassert>

namespace mutils{

	template<typename Ret, typename... Args>
	class InterpretedFunction {
	private:
		const DynamicModule parent;
		Ret (const * const loaded_fun) (Args...);
		InterpretedFunction(DynamicModule parent, const std::string &name)
			:parent(parent),
			 :loaded_fun(parent.load_internal<Ret,Args...>(name)){}
	public:

		Ret operator()(Args... args) const {
			return loaded_fun(std::forward<Args>(args)...);
		}

		template<typename Compiler, typename... Headers>
		static std::enable_if_t<is_compiler<Compiler>::value,InterpretedFunction>
		build(const std::string ret,
			  const std::string args,
			  const std::string body,
			  const Headers&... headers){
			static_assert(forall(std::is_same<std::string,Headers>::value...),
						  "Error: headers should be a list of strings");
			std::stringstream temp_module;
			auto map_fun = [&](const std::string& header){temp_module << header; return true};
			bool worked = forall(map_fun(headers)...);
			assert(worked);
			temp_module << std::endl;
			temp_module << "extern \"C\" " << ret << "single_fun(" << args << "); ";
			temp_module << ret << "single_fun(" << args << ")" << "{" << body << "}" << std::endl;
			return Compiler::compile(temp_module.str()).load("single_fun");
		}

		friend class DynamicModule;
	};
}

#include "DynamicModule_impl.hpp"
