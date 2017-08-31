#ifndef VERSION_H
#define VERSION_H

namespace htb{
	
	//Date Version Types
	static const char VER_DATE[] = "30";
	static const char VER_MONTH[] = "08";
	static const char VER_YEAR[] = "2017";
	static const char VER_UBUNTU_VERSION_STYLE[] =  "17.08";
	
	//Software Status
	static const char VER_STATUS[] =  "Alpha";
	static const char VER_STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long VER_MAJOR  = 1;
	static const long VER_MINOR  = 7;
	static const long VER_BUILD  = 86;
	static const long VER_REVISION  = 530;
	
	//Miscellaneous Version Types
	static const long VER_BUILDS_COUNT  = 613;
	#define VER_RC_FILEVERSION 1,7,86,530
	#define VER_RC_FILEVERSION_STRING "1, 7, 86, 530\0"
	static const char VER_FULLVERSION_STRING [] = "1.7.86.530";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long VER_BUILD_HISTORY  = 36;
	

}
#endif //VERSION_H
