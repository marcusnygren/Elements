#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <functional>
#include <typeinfo>
#include <thread>
#include <mutex>
#include <vector>
#include <typeinfo>
#include <iomanip>

#define NO_ARGUMENTS__ "Not defined."
#define NO_DESCRIPTION__ "Not defined."
#define COMMAND_LENGTH__ 10
#define CONSOLE_LINE__ std::cout << "--------------------------" << std::endl;


// --- Instructions ---
// ---------------------------------------------------------------------		
//
// To use a command, type:
//
// "ITEM_NAME" "ARG1" "ARG2" "ARG3" "ARGn"
//
// ---------------------------------------------------------------------		
//
// quickAdd("ITEM_NAME", POINTER_TO_OBJECT, REFERENCE_TO_FUNCTION
//  				, "ARGUMENTS" /*OPTIONAL*/, "DESCRIPTION" /*OPTIONAL*/);
//
// There are quickAdd templates for void functions with these arguments:
// void
// (void) const
// int
// int int 
// int int int
// string
// char
// bool
// char*
// const char*
// float
// float float
// float float float
// double
// double double
// double double double
// unsigned int
// unsigned int, unsigned int
// unsigned int, unsigned int, unsigned int
//
// ---------------------------------------------------------------------
//
// Use addItem for more complicated functions:
//
// addItem("ITEM_NAME", [VARIABLES](std::string args) { WHAT_TO_DO }
//				 , "ARGUMENTS" /*OPTIONAL*/, "DESCRIPTION" /*OPTIONAL*/);
//
// ---------------------------------------------------------------------



struct CommandItem
{
	CommandItem(std::string name, std::function<void(std::string)> function
							, std::string arguments, std::string description)
	: name_(name)
	, function_(function)
	, arguments_(arguments)
	, description_(description)
	{
	}

	std::function<void(std::string)> function_;
	std::string name_;
	std::string arguments_;
	std::string description_;
};



class Console
{

public:

	static Console* getInstance()
	{
		if(theOnlyInstance == NULL)
		{
			std::unique_lock<std::mutex> lock(_instanceMutex);
			if(theOnlyInstance == NULL)
			{
				theOnlyInstance = new Console();
			}	
		}
		return theOnlyInstance;
	}
	

	void release()
	{
		delete theOnlyInstance;
		theOnlyInstance = NULL;
	}


	void addItem(const std::string& name, std::function<void(std::string)> function
							, std::string arguments = NO_ARGUMENTS__, std::string description = NO_DESCRIPTION__)
	{
		bool isAlreadyAdded = false;
	
		{
			std::unique_lock<std::recursive_mutex> lock(_commandItemsMutex);

			for_each(_commandItems.begin(), _commandItems.end(), 
				[&isAlreadyAdded, &name, &arguments, &description](CommandItem& item)
				{
					if(item.name_ == name) 
					{
						isAlreadyAdded = true;
						//std::cout << "Command " << name << " is already in the list..." << std::endl;
						return;
					}
				}
			);

			if(!isAlreadyAdded)
			{
				_commandItems.push_back(CommandItem(name, function, arguments, description));
				//std::cout << "Added command: " << name << std::endl;
			}
		}
	}


	void executeCommand(std::string command)
	{
		 //std::cout << "Execute: " << command << std::endl;
		bool isExecuted = false;
		std::istringstream is(command);
		std::string name, args;

		is >> name >> std::ws;
		getline(is, args);
		
		{
			std::unique_lock<std::recursive_mutex> lock(_commandItemsMutex);

			for_each(_commandItems.begin(), _commandItems.end(), 
				[&name, &args, &isExecuted, this](CommandItem& item)
				{
					if(item.name_ == name) 
					{
						//std::cout << "Executing: " << name << "(" << args << ")" << std::endl;
						try 
						{
							item.function_(args);
						}
						catch(...)
						{
							std::cout << "Failed to execute: " << name << "(" << args << ")" << std::endl;
						}

						isExecuted = true;
						return;
					}
				}
			);
		}

		if (!isExecuted) 
			std::cout << "No matching command: " << name << std::endl;
			
	}


	void displayCommands(void) const
	{
		CONSOLE_LINE__
		std::cout << "Available commands:" << std::endl;
		CONSOLE_LINE__

		{
		std::unique_lock<std::recursive_mutex> lock(_commandItemsMutex);
			for_each(_commandItems.begin(), _commandItems.end(),
			[](CommandItem item)
			{
				std::cout << std::setw(COMMAND_LENGTH__) << std::left << item.name_ 
									<< "(" << item.arguments_ << ")" << std::endl;
			});
		}

		CONSOLE_LINE__
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(void)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { (t->*f)(); }, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(void) const
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { (t->*f)(); }, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(std::string)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { (t->*f)(args); }, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(const char*)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			const char* cstr = args.c_str();
			(t->*f)(cstr); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(char*)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			const char* cstr = args.c_str();
			(t->*f)(cstr); 
		}, arguments, description);
	}
	

	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(int)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			int tal;
			is >> tal;
			(t->*f)(tal); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(int, int)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			int tal[2];
			is >> tal[0] >> tal[1];
			(t->*f)(tal[0], tal[1]); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(int, int, int)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			int tal[3];
			is >> tal[0] >> tal[1] >> tal[2];
			(t->*f)(tal[0], tal[1], tal[2]); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(unsigned int)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			unsigned int tal;
			is >> tal;
			(t->*f)(tal); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(unsigned int, unsigned int)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			unsigned int tal[2];
			is >> tal[0] >> tal[1];
			(t->*f)(tal[0], tal[1]); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(unsigned int, unsigned int, unsigned int)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			unsigned int tal[3];
			is >> tal[0] >> tal[1] >> tal[2];
			(t->*f)(tal[0], tal[1], tal[2]); 
		}, arguments, description);
	}
		

	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(float)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			float tal;
			is >> tal;
			(t->*f)(tal); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(float, float)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			float tal[2];
			is >> tal[0] >> tal[1];
			(t->*f)(tal[0], tal[1]); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(float, float, float)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			float tal[3];
			is >> tal[0] >> tal[1] >> tal[2];
			(t->*f)(tal[0], tal[1], tal[2]); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(double)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			double tal;
			is >> tal;
			(t->*f)(tal); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(double, double)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			double tal[2];
			is >> tal[0] >> tal[1];
			(t->*f)(tal[0], tal[1]); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(double, double, double)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			double tal[3];
			is >> tal[0] >> tal[1] >> tal[2];
			(t->*f)(tal[0], tal[1], tal[2]); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(char)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::istringstream is(args);
			char c;
			is >> c;
			(t->*f)(c); 
		}, arguments, description);
	}


	template<typename T>
	inline void quickAdd(const std::string& name, T* t, void (T::*f)(bool)
											, std::string arguments = NO_ARGUMENTS__
											, std::string description = NO_DESCRIPTION__)
	{
		if(arguments == NO_ARGUMENTS__)
			arguments = getArguments(f);

		addItem(name, [t, f](std::string args) { 
			std::transform(args.begin(), args.end(), args.begin(), tolower);
			if (args == "true")
			{
				(t->*f)(true); 
				return;
			} 
			else if (args == "false")
			{
				(t->*f)(false); 
				return;
			} 
			std::istringstream is(args);
			bool b;
			is >> b;
			(t->*f)(b); 
		}, arguments, description);
	}


	template<typename F>
	inline std::string getArguments(F f)
	{
		std::string info =  typeid(f).name();
		std::string arguments =  info.substr(info.rfind('(')+1, info.rfind(')') - info.rfind('(')-1);  	

		if(arguments.empty()) 
			return "void";

		return arguments;
	}





	private:

		static Console* theOnlyInstance;
		
		static std::mutex _instanceMutex;
		static std::recursive_mutex _commandItemsMutex;

		std::list<CommandItem> _commandItems;

		Console()
		{
			std::cout << "Added CONSOLE" << std::endl;


			addItem("Jocke", [](std::string args)
			{
				std::cout << "Hej Jocke!" << std::endl;
			}, "void", "Jocke hälsar!");
	

			addItem("threads", [](std::string args)
			{
				std::cout << "Number of threads: " << std::thread::hardware_concurrency() << std::endl;
			}, "void");

			addItem("id", [](std::string args)
			{
				std::cout << "Console thread id: " << std::this_thread::get_id() << std::endl;
			}, "void", "Prints the thread id for Console.");

			addItem("info", [&](std::string args)
			{
				std::istringstream is(args);
				std::string name;
				is >> name;
		
				bool foundCommand = false;

				{
					std::unique_lock<std::recursive_mutex> lock(_commandItemsMutex);

					for_each(_commandItems.begin(), _commandItems.end(), [&](CommandItem item)
					{
						if(item.name_ == name) 
						{
							foundCommand = true;
							std::cout << "Command: \t" << item.name_ << "(" << item.arguments_ << ")" << std::endl;
							std::cout << "Description: \t" << item.description_ << std::endl;
							return;
						}
					});

				}

				if(!foundCommand)
					std::cout << "No matching command: " << name << std::endl;

			}, "string", "Prints info about the given command.");

			quickAdd("ls", this, &Console::displayCommands, "void", "Prints all available commands.");
		}
		

		~Console() {}

	

};


#endif // CONSOLE_H