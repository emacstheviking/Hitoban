#ifndef VERSION_H
#define VERSION_H

namespace htb{
	
	//Date Version Types
	static const char VER_DATE[] = "20";
	static const char VER_MONTH[] = "06";
	static const char VER_YEAR[] = "2017";
	static const char VER_UBUNTU_VERSION_STYLE[] =  "17.06";
	
	//Software Status
	static const char VER_STATUS[] =  "Alpha";
	static const char VER_STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long VER_MAJOR  = 1;
	static const long VER_MINOR  = 0;
	static const long VER_BUILD  = 38;
	static const long VER_REVISION  = 229;
	
	//Miscellaneous Version Types
	static const long VER_BUILDS_COUNT  = 355;
	#define VER_RC_FILEVERSION 1,0,38,229
	#define VER_RC_FILEVERSION_STRING "1, 0, 38, 229\0"
	static const char VER_FULLVERSION_STRING [] = "1.0.38.229";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long VER_BUILD_HISTORY  = 38;
	

}
#endif //VERSION_H
