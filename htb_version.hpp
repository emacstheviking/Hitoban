#ifndef VERSION_H
#define VERSION_H

namespace htb{
	
	//Date Version Types
	static const char VER_DATE[] = "14";
	static const char VER_MONTH[] = "05";
	static const char VER_YEAR[] = "2017";
	static const char VER_UBUNTU_VERSION_STYLE[] =  "17.05";
	
	//Software Status
	static const char VER_STATUS[] =  "Alpha";
	static const char VER_STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long VER_MAJOR  = 1;
	static const long VER_MINOR  = 0;
	static const long VER_BUILD  = 9;
	static const long VER_REVISION  = 50;
	
	//Miscellaneous Version Types
	static const long VER_BUILDS_COUNT  = 125;
	#define VER_RC_FILEVERSION 1,0,9,50
	#define VER_RC_FILEVERSION_STRING "1, 0, 9, 50\0"
	static const char VER_FULLVERSION_STRING [] = "1.0.9.50";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long VER_BUILD_HISTORY  = 9;
	

}
#endif //VERSION_H
