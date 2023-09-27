#include <string>
#include <vector>

extern "C" {
int test_cpp(void);
}

int test_cpp(void)
{
    std::string str = "Heloo Valera!";
    printf("%s\n", str.c_str());
    
	std::vector<int>    vi;
    vi.push_back(1);
    vi.push_back(10);
    vi.push_back(123);
    
	for (int i = 0; i < (int)vi.size(); i++) {
        int value = vi.at(i);
        printf("value=%d\n", value);
    }
	return 0;
}

