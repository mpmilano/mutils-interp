#pragma once
#include "mutils.hpp"
#include "DynamicModule.hpp"
#include "Compiler.hpp"
#include <iostream>
#include <cassert>

namespace mutils{

	template<typename Ret, typename... Args>
	class DynamicFunction {
	private:
		const std::shared_ptr<DynamicModule> parent;
		Ret (const * const loaded_fun) (Args...);
	public:

		DynamicFunction(std::shared_ptr<DynamicModule> parent, const std::string &name)
			:parent(parent),
			 :loaded_fun(parent.load_internal<Ret,Args...>(name)){}
		
		Ret operator()(Args... args) const {
			return loaded_fun(std::forward<Args>(args)...);
		}
		
		virtual ~DynamicFunction(){}

		friend class DynamicModule;
	};

	template<typename Ret, typename... Args>
	class InterpretedFunction : public DynamicFunction<Ret,Args...>{
	public:
		static constexpr char function_name[] = "single_fun";
	private:
		template<typename Compiler, typename... Headers>
		std::shared_ptr<DynamicModule> build_module(Compiler& compiler,
			  const std::string ret,
			  const std::string args,
			  const std::string body,
			  const Headers&... headers){
			std::stringstream temp_module;
			auto map_fun = [&](const std::string& header){temp_module << header; return true};
			bool worked = forall(map_fun(headers)...);
			assert(worked);
			temp_module << std::endl;
			temp_module << "extern \"C\" " << ret << "single_fun(" << args << "); ";
			temp_module << ret << "single_fun(" << args << ")" << "{" << body << "}" << std::endl;
			return InterpretedModule<Compiler>{};
			return std::shared_ptr<DynamicModule>(compiler.compile(temp_module.str()).release());
		}
	public:
		template<typename Compiler, typename... Headers>
		InterpretedFunction(Compiler& compiler,
			  const std::string ret,
			  const std::string args,
			  const std::string body,
			  const Headers&... headers)
			:DynamicFunction(build_module(compiler,ret,args,body,headers...)),)
			{
			static_assert(is_compiler<Compiler>::value,
				      "Error: first paramater does not satisfy compiler constraints");
			static_assert(forall(std::is_same<std::string,Headers>::value...),
						  "Error: headers should be a list of strings");
		}

		friend class DynamicModule;
		friend class InterpretedModule;
	}; 

	template<typename Ret, typename... Args>
	char InterpretedFunction::function_name[];
}

#include "DynamicModule_impl.hpp"
