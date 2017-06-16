#ifndef VERSION_H
#define VERSION_H

namespace htb{
	
	//Date Version Types
	static const char VER_DATE[] = "16";
	static const char VER_MONTH[] = "06";
	static const char VER_YEAR[] = "2017";
	static const char VER_UBUNTU_VERSION_STYLE[] =  "17.06";
	
	//Software Status
	static const char VER_STATUS[] =  "Alpha";
	static const char VER_STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long VER_MAJOR  = 1;
	static const long VER_MINOR  = 0;
	static const long VER_BUILD  = 27;
	static const long VER_REVISION  = 161;
	
	//Miscellaneous Version Types
	static const long VER_BUILDS_COUNT  = 283;
	#define VER_RC_FILEVERSION 1,0,27,161
	#define VER_RC_FILEVERSION_STRING "1, 0, 27, 161\0"
	static const char VER_FULLVERSION_STRING [] = "1.0.27.161";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long VER_BUILD_HISTORY  = 27;
	

}
#endif //VERSION_H
