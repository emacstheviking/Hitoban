#ifndef VERSION_H
#define VERSION_H

namespace htb{
	
	//Date Version Types
	static const char VER_DATE[] = "13";
	static const char VER_MONTH[] = "05";
	static const char VER_YEAR[] = "2017";
	static const char VER_UBUNTU_VERSION_STYLE[] =  "17.05";
	
	//Software Status
	static const char VER_STATUS[] =  "Alpha";
	static const char VER_STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long VER_MAJOR  = 1;
	static const long VER_MINOR  = 0;
	static const long VER_BUILD  = 7;
	static const long VER_REVISION  = 39;
	
	//Miscellaneous Version Types
	static const long VER_BUILDS_COUNT  = 103;
	#define VER_RC_FILEVERSION 1,0,7,39
	#define VER_RC_FILEVERSION_STRING "1, 0, 7, 39\0"
	static const char VER_FULLVERSION_STRING [] = "1.0.7.39";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long VER_BUILD_HISTORY  = 7;
	

}
#endif //VERSION_H
