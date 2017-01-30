#pragma once

#include "Compiler.hpp"
#include <map>
#include <string>

namespace mutils {
	class Clang {

		std::map<std::string,std::string> mangled_names;
		
		void store_mangled_names() {
			const auto getMangledName = [&](FunctionDecl* decl) {
				auto mangleContext = context.createMangleContext();
				
				if (!mangleContext->shouldMangleDeclName(decl)) {
					return decl->getNameInfo().getName().getAsString();
				}
				
				std::string mangledName;
				llvm::raw_string_ostream ostream(mangledName);
				
				mangleContext->mangleName(decl, ostream);
				
				ostream.flush();
				
				delete mangleContext;
				
				return mangledName;
			};
		}
	};
}
