#pragma once
#include <memory>

namespace mutils {

	template<typename Ret, typename... Args>
	class InterpretedFunction;

	template<typename Ret, typename... Args>
	class DynamicFunction;

	struct LoaderException : public std::exception {
		char const * const err;
		LoaderException(char const * const err):err(err){}
		//whatever you need to do to throw and stuff, the compiler will tell me. 
	};
	
	class DynamicModule {
	protected:
		struct c_internals;
		c_internals* i;

		void* load_symbol(char const * const name) const ;
		
		template<typename Ret, typename... Args>
		Ret (*) (Args...) load_internal (const std::string&) const;
	public:

		DynamicModule(DynamicModule&& o):i(o.i){o.i = nullptr;}
		DynamicModule(const std::string &filename);
		DynamicModule(const DynamicModule&) = delete;
		virtual ~DynamicModule();
		
		template<typename Ret, typename... Args>
		DynamicFunction<Ret, Args...> load (const std::string&) const;
		
		template<typename, typename...>
		friend class DynamicFunction;
	};

	template<typename Compiler>
	class InterpretedModule : public DynamicModule {
	public:
		const Compiler& compiler;
		const std::string code;		

		InterpretedModule(Compiler& _compiler, const std::string code)
			:DynamicModule(std::move(*_compiler.compile(code))),
			 compiler(_compiler),
			 code(code){}
		
		template<typename, typename...>
		friend class InterpretedFunction;

		template<typename, typename...>
		friend class DynamicFunction;
	};
}
