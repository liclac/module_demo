#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <vector>
#include <map>

/**
 * Base class for a module. We need some kind of common interface to be able to
 * actually interact with modules - self-registering modules is cool and all,
 * but what's the point if we can't make them run?
 */
class module
{
public:
	// Keep the registry in an inner class, defined below
	class registry;
	
	/// Just define this to make the destructor virtual
	virtual ~module() {};
	
	/// Runs the module. The same prototype as main(), with a vector.
	virtual int run(std::vector<std::string> params) = 0;
};

/**
 * Singleton class holding information about all registered modules.
 * 
 * Note that this has to be a singleton, rather than just a static map, as
 * otherwise it's possible for a registration to occur before the map has been
 * initialized, which will cause a rather confusing pre-main crash on start.
 */
class module::registry
{
public:
	/// An entry for a registered module.
	struct entry {
		module*(*createfn)();	///< Factory function
		std::string desc;		///< Human-readable description
		
		// ...add any other fields you want here...
	};
	
	/**
	 * A type whose constructor registers a class.
	 * Will fail spectacularly on classes that aren't modules!
	 */
	template<typename T>
	struct register_t {
		/**
		 * Uses Parameter Packing and Perfect Forwarding to allow you to toss
		 * damn near anything into your entry structs and just pass in the
		 * corresponding values, without having to change register_t at all.
		 * 
		 * If you'd rather not do this, this does the exact same thing:
		 * 
		 *     register_t(std::string name, std::string desc)
		 *     {
		 *         module::registry::get().all.insert(
		 *             std::make_pair(name, entry { &register_t<T>::create, desc })
		 *         );
		 *     };
		 * 
		 * @see http://en.cppreference.com/w/cpp/language/parameter_pack
		 * @see http://en.cppreference.com/w/cpp/utility/forward
		 * 
		 * @param name The name/identifier of the module
		 * @param args Additional fields to fill into module::registry::entry
		 */
		template<typename... Args>
		register_t(std::string name, Args&&... args)
		{
			module::registry::get().all.insert(
				std::make_pair(name, entry { &register_t<T>::create, std::forward<Args>(args)... })
			);
		};
		
		/// Simple function that just creates a new instance of T
		static module* create() { return new T; }
	};
	
	/// Returns the shared registry instance.
	static registry& get()
	{
		/*
		 * Starting with C++11, this is guaranteed to be thread safe; no mutex
		 * nonsense required. While a static variable is being initialized,
		 * which happens the first time it goes into scope (= on the first
		 * call to the containing function), everything else will wait for it.
		 * 
		 * WARNING: For this promise to hold, the program must be compiled with
		 * C++11 support, or the old, non-threadsafe behavior is used!
		 */
		static registry _instance;
		return _instance;
	};
	
	/// A map of all registered modules, by name
	std::map<std::string,entry> all;
	
private:
	registry() {};
	registry(const registry&) = delete;
};



/**
 * Expands to the variable name for the registrator variable; for a class named
 * my_module, you'd get one called _module_my_module_reg, which is hopefully
 * weird enough that it doesn't conflict with anything.
 */
#define _REGISTER_MODULE_VNAME(T) _module_##T##_reg
 
/**
 * A macro for easily registering a module. Usage:
 * 
 *     class my_module : public module
 *     {
 *         // ...
 *     };
 *     
 *     REGISTER_MODULE(my_module, "mymod", "My self-registering module");
 * 
 * It's varadic, so you can pass in anything you like, though you'll obviously
 * get a compiler error if you pass in anything weird that doesn't fit.
 */
#define REGISTER_MODULE(T, name, ...) static module::registry::register_t<T> _REGISTER_MODULE_VNAME(T)(name, __VA_ARGS__);

#endif
