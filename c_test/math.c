#include <math.h>
#include <stdio.h>

int main() 
{
    float x, y;
    for (y = 1; y >= -1; y -= 0.05f, putchar('\n'))
        for (x = -1; x <= 1; x += 0.025f)
            putchar(x * x + y * y > 1 ? 'M' : "@@%#*+=;:. "[(int)(((x + y + sqrt(1 - (x * x + y * y))) * 0.5773502692f + 1)* 5.0f + 0.5f)]);

         //puts("\xf0\x9f\x8e\x84");		//需要支持utf-8和emoji表情

    return 0;
}

//作者：Milo Yip
//链接：https://www.zhihu.com/question/38587526/answer/158984012
//来源：知乎
//著作权归作者所有，转载请联系作者获得授权。