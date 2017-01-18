#pragma once
#include <memory>

namespace mutils {

	template<typename Ret, typename... Args>
	class InterpretedFunction;

	struct LoaderException : public std::exception {
		char const * const err;
		LoaderException(char const * const err):err(err){}
		//whatever you need to do to throw and stuff, the compiler will tell me. 
	};
	
	class DynamicModule {
		struct Internals{
			Internals(const Internals&) = delete;
			struct c_internals;
			c_internals const * const i;
			Internals(c_internals const * const);
			~Internals();
		};

		void* load_symbol(char const * const name) const ;
		
		template<typename Ret, typename... Args>
		Ret (*) (Args...) load_internal (const std::string&) const;
	public:

		DynamicModule(const std::string filename);
		
		template<typename Ret, typename... Args>
		InterpretedFunction<Ret, Args...> load (const std::string&) const;
		
		template<typename, typename...>
		friend class InterpretedFunction;
	};
}
