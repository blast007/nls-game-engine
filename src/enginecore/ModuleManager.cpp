/**
* \file
* \author Adam Martin
* \date 2011-10-23
* \brief ModuleManager definitions.
*/

#include "ModuleManager.h"

// Standard Includes
#include <EngineConfig.h>

// Library Includes

// Local Includes
#include "../sharedbase/ModuleInterface.h"
#include "../sharedbase/EventLogger.h"
#include "../sharedbase/ModuleScriptInterface.h"
#include "ScriptEngine.h"

// Static class member initialization

// Class methods in the order they are defined within the class header

/**
* \param name The filename of the dynamic library to load.
* \return LOADED if the modules is loaded otherwise LOAD_ERROR/START_ERROR on error..
*/
MODULE_STATUS::TYPE ModuleManager::Load(const std::string &name) {
	if (name == "") {
		LOG(LOG_PRIORITY::CONFIG, "Can't load a library with no name.");
		return MODULE_STATUS::LOAD_ERROR;
	}
	char buf[256];
	
	std::string location =
		NLS_ENGINE_LIBRARY_PATH + "/" +
#ifdef _MSC_VER
	#ifdef _DEBUG
		"Debug/" +
	#else
		"Release/" +
	#endif
#endif
		name +
#ifdef _WIN32
		".dll";
#else
		".so";
#endif
	;
	
	if (this->libraries.find(name) == this->libraries.end()) {
		LOG(LOG_PRIORITY::FLOW, "Attempting to load library '" + name + "' from '" + location + "'.");
		
		// Load the DLL ONLY if the library has not already been loaded in the past.
#ifdef _WIN32
		HMODULE libdll = LoadLibrary(location.c_str());
#else
		void * libdll = dlopen(location.c_str(), RTLD_LAZY);
#endif
		if (libdll != NULL) {
			this->libraries[name] = libdll;
			
			LOG(LOG_PRIORITY::FLOW, "Loaded library '" + name + "' successfully.");
		}
		else {
#ifdef _WIN32
			DWORD errcode = GetLastError();
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errcode, 0, buf, 256, NULL); // *TODO: Remove the trailing newline as it goofs our output.
#else
			buf = "Error loading library: " + name;
#endif
			LOG(LOG_PRIORITY::RESTART, buf);
			
			LOG(LOG_PRIORITY::RESTART, "Library loading failed due to error.");
			return MODULE_STATUS::LOAD_ERROR;
		}
	}
	else {
		LOG(LOG_PRIORITY::CONFIG, "Library '" + name + "' already loaded, not reloading.");
	}

	{
		ModuleInstanceFactory fact;
#ifdef _WIN32
		fact = (ModuleInstanceFactory)GetProcAddress(this->libraries[name], "ModuleFactory");
#else
		fact = (ModuleInstanceFactory)dlsym(this->libraries[name], "ModuleFactory");
#endif

		if (fact != nullptr) {
			LOG(LOG_PRIORITY::FLOW, "Module factory acquired successfully.");
			
			ModuleInterface* module = fact(EventLogger::GetEventLogger());
			this->modules[name] = module;
			
			// *TODO: Remove this section for script registration and place it in a game player registration method.
			try // Try to cast to a ModuleScriptInterface. If it fails we just have a basic module.
			{
				ModuleScriptInterface* scriptable_module = dynamic_cast<ModuleScriptInterface*>(module);
				if (scriptable_module != nullptr) {
					scriptable_module->Register(this->engine->GetasIScriptEngine());
				}
			}
			catch (std::bad_cast)
			{
				LOG(LOG_PRIORITY::FLOW, "Module not of type ModuleScriptInterface.");
			}
			
		}
		else {
#ifdef _WIN32
			DWORD errcode = GetLastError();
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errcode, 0, buf, 256, NULL);
#else
			buf = "Error loading module factory";
#endif
			LOG(LOG_PRIORITY::RESTART, buf);
			
			LOG(LOG_PRIORITY::RESTART, "Module factory loading aborted due to error.");
			
			Unload(name);
			
			return MODULE_STATUS::START_ERROR;
		}
	}

	return MODULE_STATUS::LOADED;
}

/**
* \param name The filename of the module/library to check.
* \return LOADED if the modules is loaded otherwise EXISTS if a library exists by that name else NOT_FOUND.
*/
MODULE_STATUS::TYPE ModuleManager::GetStatus( const std::string &name ) {
	// Check if already loaded.
	if (this->libraries.find(name) != this->libraries.end()) {
		return MODULE_STATUS::LOADED;
	}

	std::string libFileName = name;

	// Attempt to load the library
#ifdef _WIN32
	libFileName += ".dll";
	HMODULE libdll = LoadLibrary(libFileName.c_str());
#else
	libFileName += ".so";
	void * libdll = dlopen(libFileName.c_str(), RTLD_LAZY);
#endif

	// If it loaded, unload it and return success.
	if (libdll != NULL) {
#ifdef _WIN32
		FreeLibrary(libdll);
#else
		dlclose(libdll);
#endif
		return MODULE_STATUS::EXISTS;
	}

	// Failed to load so return failure.
	return MODULE_STATUS::NOT_FOUND;
}

/**
* \param name The filename of the dynamic library to unload.
*/
void ModuleManager::Unload( std::string name) {
	if (name == "") {
		LOG(LOG_PRIORITY::CONFIG, "Can't unload a library with no name.");
		return;
	}
	if (this->libraries.find(name) != this->libraries.end()) { // Lib WAS found
		if (this->modules.find(name) != this->modules.end()) { // Mod WAS found
			delete this->modules[name];
			this->modules.erase(name);
		}

#ifdef _WIN32
		if (FreeLibrary(this->libraries[name]) != 0) {
#else
		if (dlclose(this->libraries[name])) {
#endif
			this->libraries.erase(name);
		}
		else {
			LOG(LOG_PRIORITY::RESTART, "Unable to unload the library '" + name + "'!");
		}
	}
}

/**
* \param dt The amount of time that has passed since the last call to update.
*/
void ModuleManager::Update( double dt /*= 0.0f*/ ) {
	for (auto it = this->modules.begin(); it != this->modules.end(); ++it) {
		if (it->second != nullptr) {
			it->second->Update(dt);
		}
	}
}

void ModuleManager::Shutdown() {
	for (auto itr = this->modules.begin(); itr != this->modules.end(); ++itr) {
		LOG(LOG_PRIORITY::INFO, "Deleting module '" + itr->first + "'!");
		delete itr->second;
	}

	for (auto itr = this->libraries.begin(); itr != this->libraries.end(); ++itr) {
		LOG(LOG_PRIORITY::INFO, "Unloading library '" + itr->first + "'!");
#ifdef _WIN32
		if (FreeLibrary(this->libraries[itr->first]) != 0) {
#else
		if (dlclose(this->libraries[name])) {
#endif
		}
	}
}

void ModuleManager::ConfigRegister() {
	asIScriptEngine* engine = this->engine->GetasIScriptEngine();
	int ret = 0;
	ret = engine->RegisterObjectType("modldr", 0, asOBJ_REF | asOBJ_NOHANDLE);
	ret = engine->RegisterGlobalProperty("modldr ModuleLoader", this);
	ret = engine->RegisterEnum("MODULE_STATUS"); assert(ret >= 0);
	ret = engine->RegisterEnumValue("MODULE_STATUS", "NOT_FOUND", MODULE_STATUS::NOT_FOUND); assert(ret >= 0);
	ret = engine->RegisterEnumValue("MODULE_STATUS", "EXISTS", MODULE_STATUS::EXISTS); assert(ret >= 0);
	ret = engine->RegisterEnumValue("MODULE_STATUS", "LOAD_ERROR", MODULE_STATUS::LOAD_ERROR); assert(ret >= 0);
	ret = engine->RegisterEnumValue("MODULE_STATUS", "START_ERROR", MODULE_STATUS::START_ERROR); assert(ret >= 0);
	ret = engine->RegisterEnumValue("MODULE_STATUS", "LOADED", MODULE_STATUS::LOADED); assert(ret >= 0);
	ret = engine->RegisterObjectMethod("modldr", "MODULE_STATUS LoadModule(const string &in)", asMETHOD(ModuleManager, Load), asCALL_THISCALL); assert(ret >= 0);
	ret = engine->RegisterObjectMethod("modldr", "MODULE_STATUS GetModuleStatus(const string &in)", asMETHOD(ModuleManager, GetStatus), asCALL_THISCALL); assert(ret >= 0);
}
