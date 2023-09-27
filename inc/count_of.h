#ifndef	COUNT_OF_
#define	COUNT_OF_

//#define COUNT_OF(a)	static_cast<int>(sizeof(a)) / static_cast<int>(sizeof(a[0]))
#define COUNT_OF(a)	static_cast<int>(sizeof(a)) / static_cast<int>(sizeof(a[0]))

#endif	// COUNT_OF_
