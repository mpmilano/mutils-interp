#include "DynamicModule_impl.hpp"
#include <dlfcn.h>

/*
void *dlopen(const char
void *dlsym(void *handle, char *symbol);
const char *dlerror();
int dlclose(void *handle);
 */


namespace mutils{
	struct DynamicModule::c_internals{
		const std::string filename;
		void *mod_handle;
		c_internals(const std::string& filename)
			:filename(filename),mod_handle(filename.c_str(),RTLD_LAZY)
			{
				if (!mod_handle) throw LoaderException{dlerror()};
			}
		~c_internals(){
			assert(mod_handle);
			dlclose(mod_handle);
		}
	};

	DynamicModule(const std::string &filename)
		:i(new c_internals(filename))
	{}

	void* DynamicModule::load_symbol(char const * const name) const {
		auto ret = dlsym(state->i->mod_handle,name);
		if (!ret) throw LoaderException{dlerror()};
		return ret;
	}
	
	DynamicModule::~DynamicModule(){
		delete i;
	}
}
