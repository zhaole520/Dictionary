#include "Dictionary.h"

/*
说明：根据中文查询英文
easyX不支持从键盘输入中文，需要使用WindowsAPI，在easyX贴吧仅有的关于中文输入的帖子只有对应c++语言的实现
我改进了贴吧里面的代码，用了贴吧里面的void GetIMEString(HWND hWnd, string& str);函数实现了中文查找英文的功能
由于在纯c语言风格的代码中插入了c++的代码，在每一个c++代码块之前我都用//c++ style标注，以示区分
*/

int main()
{
	setlocale(LC_ALL, "");
	initgraph(1000, 600);
	// 获取窗口句柄
	HWND hwnd = GetHWnd();
	//把名称设置在窗口
	SetWindowText(hwnd, _T("CET4英汉词典"));
	//批量制图开始
	BeginBatchDraw();
	loadimage(&imgMenu, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\bk.jpg"));
	init();
	readWord();//调用函数从文件中读取单词存入结构体数组
	writeWord();//调用函数把结构体数组的数据以二进制的形式存放到文件中
	MOUSEMSG m;
	loop:putimage(0, 0, &imgMenu);

	// 设置 XOR 绘图模式
	setwritemode(R2_XORPEN);
	//设置画线风格
	setlinestyle(PS_JOIN_BEVEL, 5);
	setlinecolor(RGB(255, 1, 255));
	//根据音乐音效的开关决定是否画线
	if (musicIsOpen == false)
		line(19, 198, 77, 260);
	FlushBatchDraw();

	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了载入文本文件
				if (m.x > 393 && m.x < 600 && m.y > 27 && m.y < 113)
				{
					click();
					loadWordTxt();
				}
				//如果点击到了音效开关
				else if (m.x > 2 && m.x < 97 && m.y > 185 && m.y < 273)
				{
					click();
					if (musicIsOpen == false)
					{
						musicIsOpen = true;
						// 设置 XOR 绘图模式
						setwritemode(R2_XORPEN);
						//设置画线风格
						setlinestyle(PS_JOIN_BEVEL, 5);
						setlinecolor(RGB(255, 1, 255));
						//根据音乐音效的开关决定是否画线
						line(19, 198, 77, 260);
						FlushBatchDraw();
					}
					else
					{
						musicIsOpen = false;
						// 设置 XOR 绘图模式
						setwritemode(R2_XORPEN);
						//设置画线风格
						setlinestyle(PS_JOIN_BEVEL, 5);
						setlinecolor(RGB(255, 1, 255));
						//根据音乐音效的开关决定是否画线
						line(19, 198, 77, 260);
						FlushBatchDraw();
					}
				}
				//如果点击到了载入历史数据
				else if (m.x > 393 && m.x < 603 && m.y > 161 && m.y < 256)
				{
					click();
					loadHistoryData();
				}
				//如果点击到了此次数据存档
				else if (m.x > 392 && m.x < 604 && m.y > 304 && m.y < 384)
				{
					click();
					saveThisData();
				}
				//如果点击到了单词浏览
				else if (m.x > 389 && m.x < 608 && m.y > 427 && m.y < 514)
				{
					click();
					BrowseWords();
					if (isReturnBrowseWords == 1)
					{
						isReturnBrowseWords = 0;
						goto loop;
					}
				}
				//如果点击到了单词背诵
				else if (m.x > 670 && m.x < 875 && m.y > 26 && m.y < 107)
				{
					click();
					choseReciteWords();
					if (isReturnReciteWords == 1)
					{
						isReturnReciteWords = 0;
						goto loop;
					}
				}
				//如果点击到了单词测验
				else if (m.x > 672 && m.x < 869 && m.y > 161 && m.y < 251)
				{
					click();
					testWordsInit();
					if (isReturnTestWords == 1)
					{
						isReturnTestWords = 0;
						goto loop;
					}
				}
				//如果点击到了单词查询
				else if (m.x > 669 && m.x < 876 && m.y > 304 && m.y < 385)
				{
					click();
					searchInit();
					if (isReturnSearchWords == 1)
					{
						isReturnSearchWords = 0;
						goto loop;
					}
				}
				//如果点击到了单词本
				else if (m.x > 662 && m.x < 879 && m.y > 428 && m.y < 512)
				{
					click();
					wordsBook();
					if (isReturnWordsBook == 1)
					{
						isReturnWordsBook = 0;
						goto loop;
					}
				}
				//如果点击到了EXIT
				else if (m.x > 897 && m.x < 995 && m.y > 8 && m.y < 46)
				{
					click();
					MessageBox(GetHWnd(), "     期待您的下次使用", "CET4英汉词典", MB_OK);
					exit(0);
				}
			}
		}
	}
	EndBatchDraw();
	return 0;
}

void readWord()
{
	FILE* wordFile;
	if ((wordFile = fopen("C:\\Users\\ztlzl\\Desktop\\cet4.txt", "rt"))
		== NULL) {
		printf("Can not open the file!");
		exit(0);
	}
	char s[120];
	int i = 0, j = 0;
	int wordCnt = 0;
	while ((fgets(s, 100, wordFile)) != NULL)
		//一行一行的读取文件分析并且完成结构体数组的赋值
	{
		i = 0;
		j = 0;
		if ((isalpha(s[0]) != 0) && (isalpha(s[1]) != 0))//该行为单词行
		{
			while (s[i] != 32)//直到读到一行的空格，也就是把单词读完
			{
				(word[wordCnt]).English[j++] = s[i++];
			}
			(word[wordCnt]).English[j] = '\0';//保证字符串以\0结尾
			j = 0;
			while (s[i] != '\n')
				//直到读到一行的末尾，也就是把中文释义读完
			{
				(word[wordCnt]).Chinese[j++] = s[i++];
			}
			(word[wordCnt]).Chinese[j] = '\0';//保证字符串以\0结尾
			wordCnt++;
		}
	}
	fclose(wordFile);
}

void writeWord()
{
	FILE *fp;
	if ((fp = fopen("C:\\Users\\ztlzl\\Desktop\\binaryCet4.txt", "wb+"))
		== NULL) {//读写二进制文本
		printf("Can not open the file!");
		exit(0);
	}
	fwrite(word, sizeof(Word), 3665, fp);
	fclose(fp);
}

void init()
{
	//给每个单词结构体的索引赋初值
	for (int i = 0; i < 3665; i++)
	{
		word[i].index = i;
		word[i].isInWordsBook = false;
	}

	for (int i = 0; i < 3665; i++)
	{
		notSortWords[i].index = i;
	}
}

void BrowseWords()
{
	loadimage(&browseWords, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\browseWords.jpg"));
	MOUSEMSG m;
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了往左翻页
				if (m.x > 0 && m.x < 95 && m.y > 256 && m.y < 362)
				{
					click();
					if (browseIndex == 0)
						browseIndex = 0;
					else
						browseIndex--;
				}
				//如果点击到了往右翻页
				else if (m.x > 896 && m.x < 995 && m.y > 247 && m.y < 352)
				{
					click();
					if (browseIndex == 3665 - 1)
						browseIndex = 3665 - 1;
					else
						browseIndex++;
				}
				//如果点击到了加入单词本
				else if (m.x > 763 && m.x < 871 && m.y > 5 && m.y < 100)
				{
					click();
					if (word[browseIndex].isInWordsBook == true)
					{
						word[browseIndex].isInWordsBook = false;
						wordsInWordsBook--;
					}
					else
					{
						word[browseIndex].isInWordsBook = true;
						wordsInWordsBook++;
					}
				}
				//如果点击到了返回
				else if (m.x > 891 && m.x < 995 && m.y > 4 && m.y < 101)
				{
					click();
					isReturnBrowseWords = 1;
					break;
				}
				//如果点击到了查询按钮
				else if (m.x > 235 && m.x < 276 && m.y > 22 && m.y < 66)
				{
					click();
					browseIndex = browseSearchNum;
				}
				//如果点击到了删除按钮
				else if (m.x > 303 && m.x < 351 && m.y > 23 && m.y < 62)
				{
					click();
					browseSearchNum /= 10;
				}
			}
		}
		//如果有键盘消息
		if (_kbhit())
		{
			char ch = _getch();
			if (ch >= '0' && ch <= '9')
			{
				browseSearchNum = browseSearchNum * 10 + (ch - '0');
				if (browseSearchNum >= 3665)
					browseSearchNum = 3664;
			}
			else if (ch == 13)
				//识别到回车键
			{
				browseIndex = browseSearchNum;
			}
			else if (ch == 8)
				//识别到退格键
			{
				browseSearchNum /= 10;
			}
		}
		drawBrowseWords();
		FlushBatchDraw();
	}
}

void drawBrowseWords()
{
	CHAR chinese[40];
	CHAR english[15];
	CHAR isInWordsBOOK[21];
	CHAR searchNum[10];
	putimage(0, 0, &browseWords);

	if (word[browseIndex].isInWordsBook == true)
		wsprintf(isInWordsBOOK, "%s", "该单词已经加入单词本");
	else
		wsprintf(isInWordsBOOK, "%s", "该单词未加入单词本");

	wsprintf(chinese, "%s", word[browseIndex].Chinese);
	wsprintf(english, "%s", word[browseIndex].English);

	if (browseSearchNum == 0)
		wsprintf(searchNum, "%s", " ");
	else
		wsprintf(searchNum, "%d", browseSearchNum);

	settextstyle(20, 0, _T("宋体"));
	settextcolor(RED);
	outtextxy(420, 30, isInWordsBOOK);

	settextstyle(80, 0, _T("宋体"));
	settextcolor(BLACK);
	outtextxy(100, 200, chinese);
	outtextxy(330, 350, english);

	settextstyle(20, 0, _T("宋体"));
	outtextxy(38, 33, searchNum);
}

void wordsBook()
{
	loadimage(&WordsBook, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\wordsBook.jpg"));
	MOUSEMSG m;
	int tmp;
	bool isNext;
	setbkmode(TRANSPARENT);	//设置字体背景色为透明

	if (wordsInWordsBook != 0)
	{
		tmp = worldsBookIndex;
		//用tmp去试探单词本当中第一个单词
		while (word[tmp].isInWordsBook == false && tmp < 3665)
		{
			tmp++;
		}
		if (tmp < 3665)
		{
			worldsBookIndex = tmp;
		}
	}

	while (wordsInWordsBook != 0)//单词本不为空
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了往左翻页
				if (m.x > 0 && m.x < 95 && m.y > 256 && m.y < 362)
				{
					click();
					tmp = worldsBookIndex - 1;
					//用tmp去试探单词本左边到底还有没有单词，没有的话保持位置不变
					while (word[tmp].isInWordsBook == false && tmp >= 0)
					{
						tmp--;
					}
					if (tmp >= 0)
					{
						worldsBookIndex = tmp;
					}
				}
				//如果点击到了往右翻页
				else if (m.x > 896 && m.x < 995 && m.y > 247 && m.y < 352)
				{
					click();
					tmp = worldsBookIndex + 1;
					//用tmp去试探单词本右边到底还有没有单词，没有的话保持位置不变
					while (word[tmp].isInWordsBook == false && tmp < 3665)
					{
						tmp++;
					}
					if (tmp < 3665)
					{
						worldsBookIndex = tmp;
					}
				}
				//如果点击到了移出单词本
				else if (m.x > 763 && m.x < 871 && m.y > 5 && m.y < 100)
				{
					click();
					wordsInWordsBook--;
					word[worldsBookIndex].isInWordsBook = false;
					//将当前的单词移出单词本之后，显示下一个单词
					isNext = false;//表示该单词后面在单词本里面还有没有下一个
					tmp = worldsBookIndex + 1;
					//用tmp去试探单词本右边到底还有没有单词，没有的话保持位置不变
					while (word[tmp].isInWordsBook == false && tmp < 3665)
					{
						tmp++;
					}
					if (tmp < 3665)
					{
						worldsBookIndex = tmp;
						isNext = true;
					}

					if (isNext == false)
						//表明该单词后面在单词本里面没有下一个，那我们寻找单词本中的上一个单词
					{
						tmp = worldsBookIndex - 1;
						//用tmp去试探单词本左边到底还有没有单词，没有的话保持位置不变
						while (word[tmp].isInWordsBook == false && tmp >= 0)
						{
							tmp--;
						}
						if (tmp >= 0)
						{
							isNext = true;
							worldsBookIndex = tmp;
						}
					}
				}
				//如果点击到了返回
				else if (m.x > 891 && m.x < 995 && m.y > 4 && m.y < 101)
				{
					click();
					isReturnWordsBook = 1;
					break;
				}
			}
		}
		drawWordsBook();
		FlushBatchDraw();
	}

	if (wordsInWordsBook == 0)
		//如果是单词本为空
	{
		loadimage(&EmptyWordsBook, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\emptyWordsBook.jpg"));
		while (1)
		{
			if (MouseHit())
			{
				m = GetMouseMsg();
				//如果鼠标点击
				if (m.mkLButton == true)
				{
					//如果鼠标点击到了返回
					if (m.x > 891 && m.x < 995 && m.y > 4 && m.y < 101)
					{
						click();
						isReturnWordsBook = 1;
						break;
					}
				}
			}
			drawEmptyWordsBook();
			FlushBatchDraw();
		}
	}
}

void drawWordsBook()
{
	CHAR chinese[40];
	CHAR english[15];
	putimage(0, 0, &WordsBook);

	wsprintf(chinese, "%s", word[worldsBookIndex].Chinese);
	wsprintf(english, "%s", word[worldsBookIndex].English);

	settextstyle(80, 0, _T("宋体"));
	settextcolor(BLACK);
	outtextxy(100, 200, chinese);
	outtextxy(330, 350, english);
}

void drawEmptyWordsBook()
{
	putimage(0, 0, &EmptyWordsBook);
}

void choseReciteWords()
{
	isReturnReciteWords = 0;
	loadimage(&reciteInit, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\reciteInit.jpg"));
	MOUSEMSG m;
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了返回
				if (m.x > 850 && m.x < 994 && m.y > 5 && m.y < 145)
				{
					click();
					//如果在背诵初始界面点击到了返回
					isReturnReciteWords = 1;
						break;
				}
				//如果点击到了顺序背诵
				else if (m.x > 278 && m.x < 711 && m.y > 126 && m.y < 219)
				{
					click();
					reciteWords();
					//如果在顺序背单词中点击到了返回，则返回背诵初始界面
					if (isReturnReciteWords == 1)
						isReturnReciteWords = 0;
				}
				//如果点击到了强化背诵
				else if (m.x > 279 && m.x < 709 && m.y > 254 && m.y < 343)
				{
					click();
					reciteStrengthenWords();
					//如果在强化背单词中点击到了返回，则返回背诵初始界面
					if (isReturnReciteWords == 1)
						isReturnReciteWords = 0;
				}
			}
		}
		putimage(0, 0, &reciteInit);
		FlushBatchDraw();
	}
}

void reciteStrengthenWords()
{
	loadimage(&ReciteStrengthenChinese, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\reciteStrengthenChinese.jpg"));
	loadimage(&ReciteStrengthenEnglish, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\reciteStrengthenEnglish.jpg"));
	loadimage(&EndReciteStrengthenWord, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\endReciteStrengthenWord.jpg"));
	sortMistakeNumOfWords();//对前面单词的情况进行排序
	
	if (sortedWords[0].errorTimes == 0)
	{
		endReciteStrengthenWord();
		//如果点击到了返回
		if (isReturnReciteWords == 1)
			return;
	}

	//判断强化记忆单词的在排序数组当中的最大值,最大值为maxIndex，且不包含maxIndex
	maxIndex = reciteStrengthenWordIndex;
	while (sortedWords[maxIndex].errorTimes > 0)
	{
		maxIndex++;
	}

	while (1)
	{
		if (modeOfReciteWords == 0)
			//背中文
		{
			reciteStrengthenChinese();
			//如果点击到了返回
			if (isReturnReciteWords == 1)
				break;
		}
		else
			//背英文
		{
			reciteStrengthenEnglish();
			//如果点击到了返回
			if (isReturnReciteWords == 1)
				break;
		}
	}
}
 
void endReciteStrengthenWord()
{
	MOUSEMSG m;
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了返回
				if (m.x > 813 && m.x < 995 && m.y > 5 && m.y < 184)
				{
					click();
					isReturnReciteWords = 1;
					userAnswer = -1;
					isNextOrLast = true;
					break;
				}
			}
		}
		putimage(0, 0, &EndReciteStrengthenWord);
		FlushBatchDraw();
	}
}

void reciteStrengthenChinese()
{
	putimage(0, 0, &ReciteStrengthenChinese);
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	MOUSEMSG m;
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击背中文
			if (m.mkLButton == true)
			{
				//如果点击到了背中文
				if (m.x > 5 && m.x < 178 && m.y > 5 && m.y < 130)
				{
					click();
					modeOfReciteWords = 0;
				}
				//如果点击到了背英文
				else if (m.x > 7 && m.x < 167 && m.y > 145 && m.y < 270)
				{
					click();
					modeOfReciteWords = 1;
					break;
				}
				//如果点击到了返回
				else if (m.x > 877 && m.x < 994 && m.y > 1 && m.y < 112)
				{
					click();
					isReturnReciteWords = 1;
					userAnswer = -1;
					isNextOrLast = true;
					break;
				}
				//如果点击到了last
				else if (m.x > 12 && m.x < 129 && m.y > 532 && m.y < 588)
				{
					click();
					reciteStrengthenWordIndex--;
					if (reciteStrengthenWordIndex < 0)
						reciteStrengthenWordIndex = 0;
					else
					{
						isNextOrLast = true;
						userAnswer = -1;
					}
				}
				//如果点击到了next
				else if (m.x > 847 && m.x < 949 && m.y > 539 && m.y < 595)
				{
					click();
					reciteStrengthenWordIndex++;
					if (reciteStrengthenWordIndex >= maxIndex)
						reciteStrengthenWordIndex = maxIndex - 1;
					else
					{
						isNextOrLast = true;
						userAnswer = -1;
					}
				}
				//如果点击到了选项A
				else if (m.x > 322 && m.x < 400 && m.y > 185 && m.y < 256)
				{
					click();
					userAnswer = 0;
				}
				//如果点击到了选项B
				else if (m.x > 323 && m.x < 390 && m.y > 274 && m.y < 348)
				{
					click();
					userAnswer = 1;
				}
				//如果点击到了选项C
				else if (m.x > 326 && m.x < 399 && m.y > 375 && m.y < 436)
				{
					click();
					userAnswer = 2;
				}
				//如果点击到了选项D
				else if (m.x > 321 && m.x < 403 && m.y > 466 && m.y < 529)
				{
					click();
					userAnswer = 3;
				}
			}
		}

		//如果有键盘消息
		if (_kbhit())
		{
			char ch = _getch();
			if (ch == 'a' || ch == 'A')
			{
				userAnswer = 0;
			}
			else if (ch == 'b' || ch == 'B')
			{
				userAnswer = 1;
			}
			else if (ch == 'c' || ch == 'C')
			{
				userAnswer = 2;
			}
			else if (ch == 'd' || ch == 'D')
			{
				userAnswer = 3;
			}
		}

		drawReciteStrengthenChinese();
		FlushBatchDraw();
	}
}

void drawReciteStrengthenChinese()
{
	srand((unsigned)time(NULL));
	CHAR WORD[15];
	putimage(0, 0, &ReciteStrengthenChinese);

	if (isNextOrLast == true)
	{
		answer = rand() % 4;//当前的答案放在哪个选项里面，随机生成
		int num;
		int optionsNum[4];

		//给四个选项随机赋上不是非当前背诵选项的值,并且这四个值不相等
		do
		{
			do
			{
				num = rand() % 3665;
			} while (num == sortedWords[reciteStrengthenWordIndex].index);
			wsprintf(A, "%s", word[num].Chinese);
			optionsNum[0] = num;
			do
			{
				num = rand() % 3665;
			} while (num == sortedWords[reciteStrengthenWordIndex].index);
			wsprintf(B, "%s", word[num].Chinese);
			optionsNum[1] = num;
			do
			{
				num = rand() % 3665;
			} while (num == sortedWords[reciteStrengthenWordIndex].index);
			wsprintf(C, "%s", word[num].Chinese);
			optionsNum[2] = num;
			do
			{
				num = rand() % 3665;
			} while (num == sortedWords[reciteStrengthenWordIndex].index);
			wsprintf(D, "%s", word[num].Chinese);
			optionsNum[3] = num;
		} while (fourNumIsNotEqual(optionsNum) == false);

		//正确答案在哪个选项上随机生成后开始赋值
		switch (answer)
		{
		case 0:wsprintf(A, "%s", word[sortedWords[reciteStrengthenWordIndex].index].Chinese);
			break;
		case 1:wsprintf(B, "%s", word[sortedWords[reciteStrengthenWordIndex].index].Chinese);
			break;
		case 2:wsprintf(C, "%s", word[sortedWords[reciteStrengthenWordIndex].index].Chinese);
			break;
		case 3:wsprintf(D, "%s", word[sortedWords[reciteStrengthenWordIndex].index].Chinese);
			break;
		}

		isNextOrLast = false;
	}

	wsprintf(WORD, "%s", word[sortedWords[reciteStrengthenWordIndex].index].English);

	settextstyle(30, 0, _T("宋体"));
	settextcolor(BLACK);
	outtextxy(431, 196, A);
	outtextxy(420, 290, B);
	outtextxy(418, 387, C);
	outtextxy(420, 478, D);
	settextstyle(60, 0, _T("宋体"));
	settextcolor(RED);
	outtextxy(374, 85, WORD);

	CHAR tips[30];
	if (userAnswer == -1)
		//用户还未进行作答
	{
		wsprintf(tips, "%s", "请点击屏幕或用键盘作答");
	}
	else if (userAnswer == answer)
	{
		wsprintf(tips, "%s", "作答正确");
		notSortWords[sortedWords[reciteStrengthenWordIndex].index].errorTimes--;
		if (notSortWords[sortedWords[reciteStrengthenWordIndex].index].errorTimes < 0)
		{
			notSortWords[sortedWords[reciteStrengthenWordIndex].index].errorTimes = 0;
		}
	}
	else
	{
		wsprintf(tips, "%s", "作答错误");
	}

	settextstyle(30, 0, _T("宋体"));
	settextcolor(GREEN);
	outtextxy(320, 25, tips);
}

void reciteStrengthenEnglish()
{
	isSubmit = false;
	putimage(0, 0, &ReciteStrengthenEnglish);
	FlushBatchDraw();
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	MOUSEMSG m;
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了返回
				if (m.x > 871 && m.x < 995 && m.y > 1 && m.y < 128)
				{
					click();
					isReturnReciteWords = 1;
					numOfLetterInReciteWords = 0;
					userAnswer = -1;
					break;
				}
				//如果点击到了LAST
				else if (m.x > 18 && m.x < 150 && m.y > 533 && m.y < 590)
				{
					click();
					reciteStrengthenWordIndex--;
					if (reciteStrengthenWordIndex < 0)
						reciteStrengthenWordIndex = 0;
					else
					{
						userWord[0] = '\0';
						isSubmit = false;
						numOfLetterInReciteWords = 0;
						userAnswer = -1;
					}
				}
				//如果点击到了NEXT
				else if (m.x > 822 && m.x < 988 && m.y > 535 && m.y < 588)
				{
					click();
					reciteStrengthenWordIndex++;
					if (reciteStrengthenWordIndex >= maxIndex)
						reciteStrengthenWordIndex = maxIndex - 1;
					else
					{
						userWord[0] = '\0';
						isSubmit = false;
						numOfLetterInReciteWords = 0;
						userAnswer = -1;
					}
				}
				//如果点击到了背中文
				else if (m.x > 5 && m.x < 201 && m.y > 5 && m.y < 138)
				{
					click();
					modeOfReciteWords = 0;
					break;
				}
				//如果点击到了背英文
				else if (m.x > 4 && m.x < 191 && m.y > 153 && m.y < 302)
				{
					click();
					modeOfReciteWords = 1;
				}
				//如果点击到了回车
				else if ((m.x > 861 && m.x < 937 && m.y > 289 && m.y < 427) ||
					(m.x > 802 && m.x < 855 && m.y > 348 && m.y < 425))
				{
					click();
					isSubmit = true;
					userWord[numOfLetterInReciteWords] = '\0';
					if ((strcmp(word[sortedWords[reciteStrengthenWordIndex].index].English, userWord)) == 0)
					{
						userAnswer = 1;//代表作答正确

					}
					else
					{
						userAnswer = 2;//代表作答错误
					}
				}
				//如果点击到了删除
				else if (m.x > 815 && m.x < 922 && m.y > 439 && m.y < 501)
				{
					click();
					if (numOfLetterInReciteWords == 0)
						userWord[0] = '\0';
					else if (numOfLetterInReciteWords > 0)
					{
						userWord[numOfLetterInReciteWords - 1] = '\0';
						numOfLetterInReciteWords--;
					}
				}
			}
		}

		//如果有键盘消息
		if (_kbhit())
		{
			char ch = _getch();
			if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
				//如果是字母
			{
				if (ch >= 'a' && ch <= 'z')
					//如果是小写
				{
					userWord[numOfLetterInReciteWords] = ch;
				}
				else
					//如果是大写
				{
					userWord[numOfLetterInReciteWords] = ch + 32;
				}
				numOfLetterInReciteWords++;
			}
			else if (ch == 8)
				//删除键（退格）
			{
				if (numOfLetterInReciteWords == 0)
					userWord[0] = '\0';
				else if (numOfLetterInReciteWords > 0)
				{
					userWord[numOfLetterInReciteWords - 1] = '\0';
					numOfLetterInReciteWords--;
				}
			}
			else if (ch == 13)
				//回车键
			{
				isSubmit = true;
				userWord[numOfLetterInReciteWords] = '\0';
				if ((strcmp(word[sortedWords[reciteStrengthenWordIndex].index].English, userWord)) == 0)
				{
					userAnswer = 1;//代表作答正确
				}
				else
				{
					userAnswer = 2;//代表作答错误
				}
			}
		}

		drawReciteStrengthenEnglish();
		FlushBatchDraw();
	}
}

void drawReciteStrengthenEnglish()
{
	CHAR CHINESE[40];
	CHAR tips[30];

	if (isSubmit == false)
	{
		wsprintf(tips, "%s", "请输入单词并按回车");
	}
	else
	{
		if (userAnswer == 1)
		{
			wsprintf(tips, "%s", "作答正确");
			notSortWords[sortedWords[reciteStrengthenWordIndex].index].errorTimes--;
			if (notSortWords[sortedWords[reciteStrengthenWordIndex].index].errorTimes < 0)
			{
				notSortWords[sortedWords[reciteStrengthenWordIndex].index].errorTimes = 0;
			}
		}
		else
		{
			wsprintf(tips, "%s", "作答错误");
		}
	}

	wsprintf(CHINESE, "%s", word[sortedWords[reciteStrengthenWordIndex].index].Chinese);

	putimage(0, 0, &ReciteStrengthenEnglish);
	settextstyle(30, 0, _T("宋体"));
	settextcolor(GREEN);
	outtextxy(320, 25, tips);
	settextstyle(60, 0, _T("宋体"));
	settextcolor(RED);
	outtextxy(375, 182, CHINESE);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("宋体"));
	userWord[numOfLetterInReciteWords] = '\0';
	outtextxy(360, 370, userWord);
}

void reciteWords()
{
	loadimage(&ReciteChinese, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\reciteChinese.jpg"));
	loadimage(&ReciteEnglish, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\reciteEnglish.jpg"));
	while (1)
	{
		if (modeOfReciteWords == 0)
			//背中文
		{
			reciteChinese();
			//如果点击到了返回
			if (isReturnReciteWords == 1)
				break;
		}
		else
			//背英文
		{
			reciteEnglish();
			//如果点击到了返回
			if (isReturnReciteWords == 1)
				break;
		}
	}
}

void reciteChinese()
{
	putimage(0, 0, &ReciteChinese);
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	MOUSEMSG m;
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击背中文
			if (m.mkLButton == true)
			{
				//如果点击到了背中文
				if (m.x > 5 && m.x < 178 && m.y > 5 && m.y < 130)
				{
					click();
					modeOfReciteWords = 0;
				}
				//如果点击到了背英文
				else if (m.x > 7 && m.x < 167 && m.y > 145 && m.y < 270)
				{
					click();
					modeOfReciteWords = 1;
					break;
				}
				//如果点击到了返回
				else if (m.x > 877 && m.x < 994 && m.y > 1 && m.y < 112)
				{
					click();
					isReturnReciteWords = 1;
					userAnswer = -1;
					isNextOrLast = true;
					break;
				}
				//如果点击到了last
				else if (m.x > 12 && m.x < 129 && m.y > 532 && m.y < 588)
				{
					click();
					reciteWordIndex--;
					if (reciteWordIndex < 0)
						reciteWordIndex = 0;
					else
					{
						isNextOrLast = true;
						userAnswer = -1;
					}
				}
				//如果点击到了next
				else if (m.x > 847 && m.x < 949 && m.y > 539 && m.y < 595)
				{
					click();
					reciteWordIndex++;
					if (reciteWordIndex >= 3665)
						reciteWordIndex = 3664;
					else
					{
						isNextOrLast = true;
						userAnswer = -1;
					}
				}
				//如果点击到了加入单词本
				else if (m.x > 761 && m.x < 875 && m.y > 3 && m.y < 114)
				{
					click();
					if (word[reciteWordIndex].isInWordsBook == false)
					{
						word[reciteWordIndex].isInWordsBook = true;
						wordsInWordsBook++;
					}
				}
				//如果点击到了选项A
				else if (m.x > 322 && m.x < 400 && m.y > 185 && m.y < 256)
				{
					click();
					userAnswer = 0;
				}
				//如果点击到了选项B
				else if (m.x > 323 && m.x < 390 && m.y > 274 && m.y < 348)
				{
					click();
					userAnswer = 1;
				}
				//如果点击到了选项C
				else if (m.x > 326 && m.x < 399 && m.y > 375 && m.y < 436)
				{
					click();
					userAnswer = 2;
				}
				//如果点击到了选项D
				else if (m.x > 321 && m.x < 403 && m.y > 466 && m.y < 529)
				{
					click();
					userAnswer = 3;
				}
			}
		}

		//如果有键盘消息
		if (_kbhit())
		{
			char ch = _getch();
			if (ch == 'a' || ch == 'A')
			{
				userAnswer = 0;
			}
			else if (ch == 'b' || ch == 'B')
			{
				userAnswer = 1;
			}
			else if (ch == 'c' || ch == 'C')
			{
				userAnswer = 2;
			}
			else if (ch == 'd' || ch == 'D')
			{
				userAnswer = 3;
			}
		}

		drawReciteChinese();
		FlushBatchDraw();
	}
}

void drawReciteChinese()
{
	srand((unsigned)time(NULL));
	CHAR WORD[15];
	putimage(0, 0, &ReciteChinese);

	if (isNextOrLast == true)
	{
		answer = rand() % 4;//当前的答案放在哪个选项里面，随机生成
		int num;
		int optionsNum[4];

		//给四个选项随机赋上不是非当前背诵选项的值,并且这四个值不相等
		do
		{
			do
			{
				num = rand() % 3665;
			} while (num == reciteWordIndex);
			wsprintf(A, "%s", word[num].Chinese);
			optionsNum[0] = num;
			do
			{
				num = rand() % 3665;
			} while (num == reciteWordIndex);
			wsprintf(B, "%s", word[num].Chinese);
			optionsNum[1] = num;
			do
			{
				num = rand() % 3665;
			} while (num == reciteWordIndex);
			wsprintf(C, "%s", word[num].Chinese);
			optionsNum[2] = num;
			do
			{
				num = rand() % 3665;
			} while (num == reciteWordIndex);
			wsprintf(D, "%s", word[num].Chinese);
			optionsNum[3] = num;
		} while (fourNumIsNotEqual(optionsNum) == false);

		//正确答案在哪个选项上随机生成后开始赋值
		switch (answer)
		{
		case 0:wsprintf(A, "%s", word[reciteWordIndex].Chinese);
			break;
		case 1:wsprintf(B, "%s", word[reciteWordIndex].Chinese);
			break;
		case 2:wsprintf(C, "%s", word[reciteWordIndex].Chinese);
			break;
		case 3:wsprintf(D, "%s", word[reciteWordIndex].Chinese);
			break;
		}

		isNextOrLast = false;
	}

	wsprintf(WORD, "%s", word[reciteWordIndex].English);

	settextstyle(30, 0, _T("宋体"));
	settextcolor(BLACK);
	outtextxy(431, 196, A);
	outtextxy(420, 290, B);
	outtextxy(418, 387, C);
	outtextxy(420, 478, D);
	settextstyle(60, 0, _T("宋体"));
	settextcolor(RED);
	outtextxy(374, 85, WORD);

	CHAR tips[30];
	if (userAnswer == -1)
		//用户还未进行作答
	{
		wsprintf(tips, "%s", "请点击屏幕或用键盘作答");
	}
	else if (userAnswer == answer)
	{
		wsprintf(tips, "%s", "作答正确");
	}
	else
	{
		wsprintf(tips, "%s", "作答错误");
	}

	settextstyle(30, 0, _T("宋体"));
	settextcolor(GREEN);
	outtextxy(320, 25, tips);
}

void reciteEnglish()
{
	putimage(0, 0, &ReciteEnglish);
	FlushBatchDraw();
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	MOUSEMSG m;
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了加入单词本
				if (m.x > 726 && m.x < 866 && m.y > 2 && m.y < 131)
				{
					click();
					if (word[reciteWordIndex].isInWordsBook == false)
					{
						word[reciteWordIndex].isInWordsBook = true;
						wordsInWordsBook++;
					}
				}
				//如果点击到了返回
				else if (m.x > 871 && m.x < 995 && m.y > 1 && m.y < 128)
				{
					click();
					isReturnReciteWords = 1;
					numOfLetterInReciteWords = 0;
					userAnswer = -1;
					break;
				}
				//如果点击到了LAST
				else if (m.x > 18 && m.x < 150 && m.y > 533 && m.y < 590)
				{
					click();
					reciteWordIndex--;
					if (reciteWordIndex < 0)
						reciteWordIndex = 0;
					else
					{
						userWord[0] = '\0';
						isSubmit = false;
						numOfLetterInReciteWords = 0;
						userAnswer = -1;
					}
				}
				//如果点击到了NEXT
				else if (m.x > 822 && m.x < 988 && m.y > 535 && m.y < 588)
				{
					click();
					reciteWordIndex++;
					if (reciteWordIndex >= 3665)
						reciteWordIndex = 3664;
					else
					{
						userWord[0] = '\0';
						isSubmit = false;
						numOfLetterInReciteWords = 0;
						userAnswer = -1;
					}
				}
				//如果点击到了背中文
				else if (m.x > 5 && m.x < 201 && m.y > 5 && m.y < 138)
				{
					click();
					modeOfReciteWords = 0;
					break;
				}
				//如果点击到了背英文
				else if (m.x > 4 && m.x < 191 && m.y > 153 && m.y < 302)
				{
					click();
					modeOfReciteWords = 1;
				}
				//如果点击到了回车
				else if ((m.x > 861 && m.x < 937 && m.y > 289 && m.y < 427) ||
					(m.x > 802 && m.x < 855 && m.y > 348 && m.y < 425))
				{
					click();
					isSubmit = true;
					userWord[numOfLetterInReciteWords] = '\0';
					if ((strcmp(word[reciteWordIndex].English, userWord)) == 0)
					{
						userAnswer = 1;//代表作答正确
					}
					else
					{
						userAnswer = 2;//代表作答错误
					}
				}
				//如果点击到了删除
				else if (m.x > 815 && m.x < 922 && m.y > 439 && m.y < 501)
				{
					click();
					if (numOfLetterInReciteWords == 0)
						userWord[0] = '\0';
					else if (numOfLetterInReciteWords > 0)
					{
						userWord[numOfLetterInReciteWords - 1] = '\0';
						numOfLetterInReciteWords--;
					}
				}
			}
		}

		//如果有键盘消息
		if (_kbhit())
		{
			char ch = _getch();
			if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
				//如果是字母
			{
				if (ch >= 'a' && ch <= 'z')
					//如果是小写
				{
					userWord[numOfLetterInReciteWords] = ch;
				}
				else
					//如果是大写
				{
					userWord[numOfLetterInReciteWords] = ch + 32;
				}
				numOfLetterInReciteWords++;
			}
			else if (ch == 8)
				//删除键（退格）
			{
				if (numOfLetterInReciteWords == 0)
					userWord[0] = '\0';
				else if (numOfLetterInReciteWords > 0)
				{
					userWord[numOfLetterInReciteWords - 1] = '\0';
					numOfLetterInReciteWords--;
				}
			}
			else if (ch == 13)
				//回车键
			{
				isSubmit = true;
				userWord[numOfLetterInReciteWords] = '\0';
				if ((strcmp(word[reciteWordIndex].English, userWord)) == 0)
				{
					userAnswer = 1;//代表作答正确
				}
				else
				{
					userAnswer = 2;//代表作答错误
				}
			}
		}

		drawReciteEnglish();
		FlushBatchDraw();
	}
}

void drawReciteEnglish()
{
	CHAR CHINESE[40];
	CHAR tips[30];

	if (isSubmit == false)
	{
		wsprintf(tips, "%s", "请输入单词并按回车");
	}
	else
	{
		if (userAnswer == 1)
		{
			wsprintf(tips, "%s", "作答正确");
		}
		else
		{
			wsprintf(tips, "%s", "作答错误");
		}
	}

	wsprintf(CHINESE, "%s", word[reciteWordIndex].Chinese);

	putimage(0, 0, &ReciteEnglish);
	settextstyle(30, 0, _T("宋体"));
	settextcolor(GREEN);
	outtextxy(320, 25, tips);
	settextstyle(60, 0, _T("宋体"));
	settextcolor(RED);
	outtextxy(375, 182, CHINESE);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("宋体"));
	userWord[numOfLetterInReciteWords] = '\0';
	outtextxy(360, 370, userWord);
}

void testWordsInit()
{
	loadimage(&testWords, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\testInit.jpg"));
	MOUSEMSG m;
	while (1)
	{
		putimage(0, 0, &testWords);
		FlushBatchDraw();
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了中文测验
				if (m.x > 280 && m.x < 583 && m.y > 118 && m.y < 205)
				{
					click();
					beginTime = time(NULL);
					testWordsChilese();
				}
				//如果点击到了英文测验
				else if (m.x > 271 && m.x < 576 && m.y > 252 && m.y < 329)
				{
					click();
					beginTime = time(NULL);
					testWordsEnglish();
				}
				//如果点击到了返回
				else if (m.x > 905 && m.x < 995 && m.y > 5 && m.y < 92)
				{
					click();
					isReturnTestWords = 1;
					break;
				}
			}
		}
	}
}

bool tenNumIsNotEqual(int *a)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = i + 1; j < 10; j++)
		{
			if (a[i] == a[j])
				return false;
		}
	}
	return true;
}

bool fourNumIsNotEqual(int *a)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			if (a[i] == a[j])
				return false;
		}
	}
	return true;
}

void testWordsChilese()
{
	srand((unsigned)time(NULL));
	//初始化数据
	isSubmitInTestChinese = 0;
	testWordChinese = 0;//当前单词测验显示第几个单词的下标
	isTextedInTestChinese = 0;//当前单词测验已经测验了几个单词
	loadimage(&testWordsChileseIMG, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\testWordsChilese.jpg"));
	int wordsNum[10];//储存着题目的索引
	int optionsNum[4];//储存着四个选项的索引

	//生成不同的10个题目
	do
	{
		for (int i = 0; i < 10; i++)
		{
			wordsNum[i] = rand() % 3665;
		}
	} while (tenNumIsNotEqual(wordsNum) == false);//当生成的题目有重复的时候，就继续生成

	//生成十个不同题目的选项
	for (int i = 0; i < 10; i++)
	{
		chineseExercise[i].index = wordsNum[i];
		//当前的答案放在哪个选项里面，随机生成. 1 2 3 4表示A B C D
		chineseExercise[i].trueAnswer = (rand() % 4) + 1;
		chineseExercise[i].userAnswer = 0;

		//用do-while确保四个选项是不相等的
		do
		{
			//给四个选项随机赋上不是非当前背诵选项的值
			do
			{
				chineseExercise[i].a = rand() % 3665;
			} while (chineseExercise[i].a == chineseExercise[i].index);
			do
			{
				chineseExercise[i].b = rand() % 3665;
			} while (chineseExercise[i].b == chineseExercise[i].index);
			do
			{
				chineseExercise[i].c = rand() % 3665;
			} while (chineseExercise[i].c == chineseExercise[i].index);
			do
			{
				chineseExercise[i].d = rand() % 3665;
			} while (chineseExercise[i].d == chineseExercise[i].index);

			optionsNum[0] = chineseExercise[i].a;
			optionsNum[1] = chineseExercise[i].b;
			optionsNum[2] = chineseExercise[i].c;
			optionsNum[3] = chineseExercise[i].d;
		} while (fourNumIsNotEqual(optionsNum) == false);

		//正确答案在哪个选项上随机生成后开始赋值
		switch (chineseExercise[i].trueAnswer)
		{
		case 1:chineseExercise[i].a = chineseExercise[i].index;
			break;
		case 2:chineseExercise[i].b = chineseExercise[i].index;
			break;
		case 3:chineseExercise[i].c = chineseExercise[i].index;
			break;
		case 4:chineseExercise[i].d = chineseExercise[i].index;
			break;
		}
	}
	MOUSEMSG m;
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了选项A
				if (m.x > 325 && m.x < 403 && m.y > 213 && m.y < 265)
				{
					click();
					if (chineseExercise[testWordChinese].userAnswer == 0)
						isTextedInTestChinese++;
					chineseExercise[testWordChinese].userAnswer = 1;
				}
				//如果点击到了选项B
				else if (m.x > 321 && m.x < 393 && m.y > 296 && m.y < 348)
				{
					click();
					if (chineseExercise[testWordChinese].userAnswer == 0)
						isTextedInTestChinese++;
					chineseExercise[testWordChinese].userAnswer = 2;
				}
				//如果点击到了选项C
				else if (m.x > 324 && m.x < 392 && m.y > 389 && m.y < 449)
				{
					click();
					if (chineseExercise[testWordChinese].userAnswer == 0)
						isTextedInTestChinese++;
					chineseExercise[testWordChinese].userAnswer = 3;
				}
				//如果点击到了选项D
				else if (m.x > 323 && m.x < 392 && m.y > 475 && m.y < 535)
				{
					click();
					if (chineseExercise[testWordChinese].userAnswer == 0)
						isTextedInTestChinese++;
					chineseExercise[testWordChinese].userAnswer = 4;
				}
				//如果点击到了选项LAST
				else if (m.x > 9 && m.x < 140 && m.y > 539 && m.y < 592)
				{
					click();
					testWordChinese--;
					if (testWordChinese < 0)
						testWordChinese = 0;
				}
				//如果点击到了选项NEXT
				else if (m.x > 855 && m.x < 990 && m.y > 546 && m.y < 592)
				{
					click();
					testWordChinese++;
					if (testWordChinese >= 9)
						testWordChinese = 9;
				}
				//如果点击到了选项提交
				else if (m.x > 680 && m.x < 863 && m.y > 1 && m.y < 50)
				{
					click();
					trueNumInTestChinese = 0;
					isSubmitInTestChinese = 1;
					for (int i = 0; i < 10; i++)
					{
						if (chineseExercise[i].userAnswer == chineseExercise[i].trueAnswer)
						{
							trueNumInTestChinese++;
							//如果答对了减少错误次数
							notSortWords[wordsNum[i]].errorTimes--;
							if (notSortWords[wordsNum[i]].errorTimes < 0)
								notSortWords[wordsNum[i]].errorTimes = 0;
						}
						//如果错误则记录到错误次数
						else
						{
							notSortWords[wordsNum[i]].errorTimes++;
						}
					}
				}
				//如果点击到了选项返回
				else if (m.x > 873 && m.x < 993 && m.y > 5 && m.y < 123)
				{
					click();
					break;
				}
			}
		}

		//如果有键盘消息
		if (_kbhit())
		{
			char ch = _getch();
			if (ch == 'a' || ch == 'A')
			{
				if (chineseExercise[testWordChinese].userAnswer == 0)
					isTextedInTestChinese++;
				chineseExercise[testWordChinese].userAnswer = 1;
			}
			else if (ch == 'b' || ch == 'B')
			{
				if (chineseExercise[testWordChinese].userAnswer == 0)
					isTextedInTestChinese++;
				chineseExercise[testWordChinese].userAnswer = 2;
			}
			else if (ch == 'c' || ch == 'C')
			{
				if (chineseExercise[testWordChinese].userAnswer == 0)
					isTextedInTestChinese++;
				chineseExercise[testWordChinese].userAnswer = 3;
			}
			else if (ch == 'd' || ch == 'D')
			{
				if (chineseExercise[testWordChinese].userAnswer == 0)
					isTextedInTestChinese++;
				chineseExercise[testWordChinese].userAnswer = 4;
			}
			else if (ch == 13)
				//按键按下回车键，表示提交
			{
				trueNumInTestChinese = 0;
				isSubmitInTestChinese = 1;
				for (int i = 0; i < 10; i++)
				{
					if (chineseExercise[i].userAnswer == chineseExercise[i].trueAnswer)
					{
						trueNumInTestChinese++;
						//如果答对了减少错误次数
						notSortWords[wordsNum[i]].errorTimes--;
						if (notSortWords[wordsNum[i]].errorTimes < 0)
							notSortWords[wordsNum[i]].errorTimes = 0;
					}
					//如果错误则记录到错误次数
					else
					{
						notSortWords[wordsNum[i]].errorTimes++;
					}
				}
			}
		}

		drawTestWordsChilese();
		FlushBatchDraw();
	}
}

void drawTestWordsChilese()
{
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	putimage(0, 0, &testWordsChileseIMG);
	wsprintf(A, "%s", word[chineseExercise[testWordChinese].a].Chinese);
	wsprintf(B, "%s", word[chineseExercise[testWordChinese].b].Chinese);
	wsprintf(C, "%s", word[chineseExercise[testWordChinese].c].Chinese);
	wsprintf(D, "%s", word[chineseExercise[testWordChinese].d].Chinese);

	switch (chineseExercise[testWordChinese].trueAnswer)
	{
	case 1:wsprintf(A, "%s", word[chineseExercise[testWordChinese].index].Chinese);
		break;
	case 2:wsprintf(B, "%s", word[chineseExercise[testWordChinese].index].Chinese);
		break;
	case 3:wsprintf(C, "%s", word[chineseExercise[testWordChinese].index].Chinese);
		break;
	case 4:wsprintf(D, "%s", word[chineseExercise[testWordChinese].index].Chinese);
		break;
	}

	CHAR wordsInTestChinese[20];
	wsprintf(wordsInTestChinese, "%s", word[chineseExercise[testWordChinese].index].English);

	settextstyle(30, 0, _T("宋体"));
	settextcolor(BLACK);
	outtextxy(395, 223, A);
	outtextxy(391, 306, B);
	outtextxy(394, 399, C);
	outtextxy(393, 485, D);
	settextstyle(60, 0, _T("宋体"));
	settextcolor(RED);
	outtextxy(340, 112, wordsInTestChinese);

	CHAR tips[30];
	if (chineseExercise[testWordChinese].userAnswer == 0)
		//用户还未作答
	{
		if (isSubmitInTestChinese == 0)
			//如果还没有点击提交
		{
			wsprintf(tips, "%s", "请用键盘或者用鼠标作答");
		}
		wsprintf(tips, "%s", "这一题您没有提交答案");
	}
	else if (chineseExercise[testWordChinese].userAnswer == 1)
	{
		wsprintf(tips, "%s", "您选择的答案是：A");
	}
	else if (chineseExercise[testWordChinese].userAnswer == 2)
	{
		wsprintf(tips, "%s", "您选择的答案是：B");
	}
	else if (chineseExercise[testWordChinese].userAnswer == 3)
	{
		wsprintf(tips, "%s", "您选择的答案是：C");
	}
	else if (chineseExercise[testWordChinese].userAnswer == 4)
	{
		wsprintf(tips, "%s", "您选择的答案是：D");
	}

	settextstyle(20, 0, _T("宋体"));
	settextcolor(GREEN);
	outtextxy(332, 9, tips);

	CHAR remainTest[32];//显示用户还有几个题目没有作答
	if (isSubmitInTestChinese == 0)
		//如果还没有点击提交
	{
		if (isTextedInTestChinese == 0)
		{
			wsprintf(remainTest, "%s", "还有10题没有作答");
		}
		else if (isTextedInTestChinese == 1)
		{
			wsprintf(remainTest, "%s", "还有9题没有作答");
		}
		else if (isTextedInTestChinese == 2)
		{
			wsprintf(remainTest, "%s", "还有8题没有作答");
		}
		else if (isTextedInTestChinese == 3)
		{
			wsprintf(remainTest, "%s", "还有7题没有作答");
		}
		else if (isTextedInTestChinese == 4)
		{
			wsprintf(remainTest, "%s", "还有6题没有作答");
		}
		else if (isTextedInTestChinese == 5)
		{
			wsprintf(remainTest, "%s", "还有5题没有作答");
		}
		else if (isTextedInTestChinese == 6)
		{
			wsprintf(remainTest, "%s", "还有4题没有作答");
		}
		else if (isTextedInTestChinese == 7)
		{
			wsprintf(remainTest, "%s", "还有3题没有作答");
		}
		else if (isTextedInTestChinese == 8)
		{
			wsprintf(remainTest, "%s", "还有2题没有作答");
		}
		else if (isTextedInTestChinese == 9)
		{
			wsprintf(remainTest, "%s", "还有1题没有作答");
		}
		else if (isTextedInTestChinese == 10)
		{
			wsprintf(remainTest, "%s", "作答完毕，确认无误后请点击提交");
		}
	}
	else
		//已经提交了
	{
		settextcolor(RED);
		if (chineseExercise[testWordChinese].userAnswer == chineseExercise[testWordChinese].trueAnswer)
			//如果该题作答正确
		{
			wsprintf(remainTest, "%s", "作答正确");
		}
		else
		{
			wsprintf(remainTest, "%s", "作答错误");
		}
	}
	outtextxy(331, 39, remainTest);

	CHAR trueAnswer[32];//用户点击提交后，如果答案错误则显示正确答案，正确则显示恭喜你
	if (isSubmitInTestChinese == 0)
		//如果还没有点击提交
	{
		wsprintf(trueAnswer, "%s", "");
	}
	else
	{
		if (chineseExercise[testWordChinese].userAnswer == chineseExercise[testWordChinese].trueAnswer)
			//如果该题作答正确
		{
			wsprintf(trueAnswer, "%s", "恭喜你");
		}
		else
		{
			switch (chineseExercise[testWordChinese].trueAnswer)
			{
			case 1:wsprintf(trueAnswer, "%s", "正确答案为A");
				break;
			case 2:wsprintf(trueAnswer, "%s", "正确答案为B");
				break;
			case 3:wsprintf(trueAnswer, "%s", "正确答案为C");
				break;
			case 4:wsprintf(trueAnswer, "%s", "正确答案为D");
				break;
			}
		}
	}
	settextcolor(GREEN);
	outtextxy(331, 69, trueAnswer);

	if (isSubmitInTestChinese == 1)
	{
		CHAR score[2];
		settextcolor(RED);
		wsprintf(score, "%d", trueNumInTestChinese);
		outtextxy(10, 110, _T("得分："));
		outtextxy(70, 110, score);
	}

	settextstyle(40, 0, _T("宋体"));
	if (isSubmitInTestChinese == 0)
	{
		nowTime = time(NULL);
	}
	CHAR times[10];
	wsprintf(times, "%ld", (nowTime - beginTime));
	outtextxy(110, 60, times);
}

void testWordsEnglish()
{
	isTextedInTestEnglish = 0;
	testWordEnglish = 0;
	isSubmitInTestEnglish = 0;
	srand((unsigned)time(NULL));
	loadimage(&testWordsEnglishIMG, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\testWordsEnglish.jpg"));

	int indexNum[10];
	//产生十个不同的单词索引，当做题目
	do
	{
		for (int i = 0; i < 10; i++)
		{
			indexNum[i] = rand() % 3665;
		}
	} while (tenNumIsNotEqual(indexNum) == false);

	//给结构体数组的index赋值并且做必要的初始化
	for (int i = 0; i < 10; i++)
	{
		englishExercise[i].index = indexNum[i];
		englishExercise[i].nextLetterIndex = 0;
		englishExercise[i].userAnswer[0] = '\0';
	}

	MOUSEMSG m;
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了submit
				if (m.x > 734 && m.x < 878 && m.y > 5 && m.y < 44)
				{
					click();
					trueNumInTestEnglish = 0;
					isSubmitInTestEnglish = 1;
					for (int i = 0; i < 10; i++)
					{
						if (strcmp(englishExercise[i].userAnswer, word[englishExercise[i].index].English) == 0)
						{
							trueNumInTestEnglish++;
							//如果答对了减少错误次数
							notSortWords[indexNum[i]].errorTimes--;
							if (notSortWords[indexNum[i]].errorTimes < 0)
								notSortWords[indexNum[i]].errorTimes = 0;
						}
						//如果答案错了
						{
							notSortWords[indexNum[i]].errorTimes++;
						}
					}
				}
				//如果点击到了退出
				else if (m.x > 888 && m.x < 996 && m.y > 4 && m.y < 110)
				{
					click();
					break;
				}
				//如果点击到了LAST
				else if (m.x > 9 && m.x < 133 && m.y > 543 && m.y < 591)
				{
					click();
					testWordEnglish--;
					if (testWordEnglish < 0)
						testWordEnglish = 0;
				}
				//如果点击到了NEXT
				else if (m.x > 846 && m.x < 998 && m.y > 543 && m.y < 599)
				{
					click();
					testWordEnglish++;
					if (testWordEnglish >= 9)
						testWordEnglish = 9;
				}
				//如果点击到了删除键
				else if (m.x > 817 && m.x < 930 && m.y > 368 && m.y < 437)
				{
					click();
					englishExercise[testWordEnglish].nextLetterIndex--;
					if (englishExercise[testWordEnglish].nextLetterIndex < 0)
						englishExercise[testWordEnglish].nextLetterIndex = 0;
					//确保字符串已\0结束
					englishExercise[testWordEnglish].userAnswer[englishExercise[testWordEnglish].nextLetterIndex] = '\0';
				}
			}
		}

		//如果有键盘消息
		if (_kbhit())
		{
			char ch = _getch();
			if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
			{
				if (ch >= 'a' && ch <= 'z')
					//如果是小写
				{
					englishExercise[testWordEnglish].userAnswer[englishExercise[testWordEnglish].nextLetterIndex] = ch;
				}
				else
					//如果是大写
				{
					englishExercise[testWordEnglish].userAnswer[englishExercise[testWordEnglish].nextLetterIndex] = ch + 32;
				}
				englishExercise[testWordEnglish].nextLetterIndex++;
				//确保字符串已\0结束
				englishExercise[testWordEnglish].userAnswer[englishExercise[testWordEnglish].nextLetterIndex] = '\0';
			}
			else if (ch == 8)
				//退格
			{
				englishExercise[testWordEnglish].nextLetterIndex--;
				if (englishExercise[testWordEnglish].nextLetterIndex < 0)
					englishExercise[testWordEnglish].nextLetterIndex = 0;
				//确保字符串已\0结束
				englishExercise[testWordEnglish].userAnswer[englishExercise[testWordEnglish].nextLetterIndex] = '\0';
			}
			else if (ch == 13)
				//回车
			{
				trueNumInTestEnglish = 0;
				isSubmitInTestEnglish = 1;
				for (int i = 0; i < 10; i++)
				{
					if (strcmp(englishExercise[i].userAnswer, word[englishExercise[i].index].English) == 0)
					{
						trueNumInTestChinese++;
					}
				}trueNumInTestEnglish = 0;
				isSubmitInTestEnglish = 1;
				for (int i = 0; i < 10; i++)
				{
					if (strcmp(englishExercise[i].userAnswer, word[englishExercise[i].index].English) == 0)
					{
						trueNumInTestEnglish++;
						//如果答对了减少错误次数
						notSortWords[indexNum[i]].errorTimes--;
						if (notSortWords[indexNum[i]].errorTimes < 0)
							notSortWords[indexNum[i]].errorTimes = 0;
					}
					//如果答案错了
					{
						notSortWords[indexNum[i]].errorTimes++;
					}
				}
			}
		}

		drawTestWordsEnglish();
		FlushBatchDraw();
	}
}

void drawTestWordsEnglish()
{
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	putimage(0, 0, &testWordsEnglishIMG);

	settextstyle(20, 0, _T("宋体"));
	settextcolor(GREEN);
	CHAR tips[30];
	if (englishExercise[testWordEnglish].nextLetterIndex == 0)
		//用户答案为空
	{
		wsprintf(tips, "%s", "请从键盘键入答案");
	}
	else
	{
		wsprintf(tips, "%s", englishExercise[testWordEnglish].userAnswer);
		outtextxy(332, 9, _T("您的答案是："));
	}
	if (englishExercise[testWordEnglish].nextLetterIndex == 0 &&
		isSubmitInTestEnglish == 1)
	{
		wsprintf(tips, "%s", "此题未作答");
	}
	if (englishExercise[testWordEnglish].nextLetterIndex == 0)
		//用户答案为空
	{
		outtextxy(332, 9, tips);
	}
	else
	{
		outtextxy(445, 9, tips);
	}


	CHAR wordInBox[30];
	wsprintf(wordInBox, "%s", englishExercise[testWordEnglish].userAnswer);
	settextstyle(60, 0, _T("宋体"));
	settextcolor(BLACK);
	outtextxy(261, 352, wordInBox);

	CHAR ChineseExercise[40];
	settextstyle(50, 0, _T("宋体"));
	settextcolor(RED);
	wsprintf(ChineseExercise, "%s", word[englishExercise[testWordEnglish].index].Chinese);
	outtextxy(330, 130, ChineseExercise);

	CHAR remainTest[32];//显示用户还有几个题目没有作答
	settextstyle(20, 0, _T("宋体"));
	settextcolor(GREEN);
	isTextedInTestEnglish = 0;
	for (int i = 0; i < 10; i++)
	{
		if (englishExercise[i].nextLetterIndex != 0)
		{
			isTextedInTestEnglish++;
		}
	}
	if (isSubmitInTestEnglish == 0)
		//如果还没有点击提交
	{
		if (isTextedInTestEnglish == 0)
		{
			wsprintf(remainTest, "%s", "还有10题没有作答");
		}
		else if (isTextedInTestEnglish == 1)
		{
			wsprintf(remainTest, "%s", "还有9题没有作答");
		}
		else if (isTextedInTestEnglish == 2)
		{
			wsprintf(remainTest, "%s", "还有8题没有作答");
		}
		else if (isTextedInTestEnglish == 3)
		{
			wsprintf(remainTest, "%s", "还有7题没有作答");
		}
		else if (isTextedInTestEnglish == 4)
		{
			wsprintf(remainTest, "%s", "还有6题没有作答");
		}
		else if (isTextedInTestEnglish == 5)
		{
			wsprintf(remainTest, "%s", "还有5题没有作答");
		}
		else if (isTextedInTestEnglish == 6)
		{
			wsprintf(remainTest, "%s", "还有4题没有作答");
		}
		else if (isTextedInTestEnglish == 7)
		{
			wsprintf(remainTest, "%s", "还有3题没有作答");
		}
		else if (isTextedInTestEnglish == 8)
		{
			wsprintf(remainTest, "%s", "还有2题没有作答");
		}
		else if (isTextedInTestEnglish == 9)
		{
			wsprintf(remainTest, "%s", "还有1题没有作答");
		}
		else if (isTextedInTestEnglish == 10)
		{
			wsprintf(remainTest, "%s", "作答完毕，确认无误后请点击提交");
		}
	}
	else
		//已经提交了
	{
		settextcolor(RED);
		if (strcmp(englishExercise[testWordEnglish].userAnswer, word[englishExercise[testWordEnglish].index].English) == 0)
			//如果该题作答正确
		{
			wsprintf(remainTest, "%s", "作答正确");
		}
		else
		{
			wsprintf(remainTest, "%s", "作答错误");
		}
	}
	outtextxy(331, 39, remainTest);

	settextstyle(20, 0, _T("宋体"));
	settextcolor(GREEN);
	if (isSubmitInTestEnglish == 1)
		//如果已经点击提交了
	{
		if (strcmp(englishExercise[testWordEnglish].userAnswer, word[englishExercise[testWordEnglish].index].English) == 0)
			//如果该题作答正确
		{
			CHAR trueAnswer[15];
			wsprintf(trueAnswer, "%s", "恭喜你");
			outtextxy(331, 69, trueAnswer);
		}
		else
		{
			CHAR trueAnswer[15];
			wsprintf(trueAnswer, "%s", word[englishExercise[testWordEnglish].index].English);
			outtextxy(331, 69, "正确答案：");
			outtextxy(430, 69, trueAnswer);
		}
	}

	if (isSubmitInTestEnglish == 0)
	{
		nowTime = time(NULL);
	}
	CHAR times[10];
	wsprintf(times, "%ld", (nowTime - beginTime));
	settextstyle(30, 0, _T("宋体"));
	outtextxy(115, 55, times);

	if (isSubmitInTestEnglish == 1)
	{
		CHAR score[2];
		settextcolor(RED);
		wsprintf(score, "%d", trueNumInTestEnglish);
		outtextxy(10, 110, _T("得分："));
		outtextxy(90, 110, score);
	}
}

void loadWordTxt()
{
	readWord();//调用函数从文件中读取单词存入结构体数组
	writeWord();//调用函数把结构体数组的数据以二进制的形式存放到文件中
}

void loadHistoryData()
{
	FILE *fp;
	if ((fp = fopen("C:\\Users\\ztlzl\\Desktop\\cet4Date.txt", "r")) == NULL)
		//读文本
	{
		printf("Can not open the file!");
		exit(0);
	}
	/*
	把如下的三个数据读入
	browseIndex //单词浏览的结构体数组下标
	modeOfReciteWords //背单词的模式，0为背中文，1为背英文，默认为0
	reciteWordIndex //当前背诵的单词的下标，默认为0
	*/
	fscanf(fp, "%d %d %d %d", &browseIndex, &modeOfReciteWords, &reciteWordIndex, &wordsInWordsBook);

	int IsInWordBook = 0;
	int ErrorsTimes = 0;
	for (int i = 0; i < 3665; i++)
	{
		fscanf(fp, "%d %d", &IsInWordBook, &ErrorsTimes);
		if (IsInWordBook == 1)
		{
			word[i].isInWordsBook = true;
		}
		else
		{
			word[i].isInWordsBook = false;
		}
		notSortWords[i].errorTimes = ErrorsTimes;
	}

	fclose(fp);
}

void saveThisData()
{
	FILE *fp;
	if ((fp = fopen("C:\\Users\\ztlzl\\Desktop\\cet4Date.txt", "w")) == NULL)
		//写文本
	{
		printf("Can not open the file!");
		exit(0);
	}
	char s[5];
	/*
	把如下的三个数据分别放在文件的每一行已达到保存进度的效果
	browseIndex //单词浏览的结构体数组下标
	modeOfReciteWords //背单词的模式，0为背中文，1为背英文，默认为0
	reciteWordIndex //当前背诵的单词的下标，默认为0
	wordsInWordsBook//单词本中单词的个数
	*/
	fputs(_itoa(browseIndex, s, 10), fp);
	fputs("\n", fp);
	fputs(_itoa(modeOfReciteWords, s, 10), fp);
	fputs("\n", fp);
	fputs(_itoa(reciteWordIndex, s, 10), fp);
	fputs("\n", fp);
	fputs(_itoa(wordsInWordsBook, s, 10), fp);
	fputs("\n", fp);

	/*
	写入单词是否在单词本中  单词的错误次数
	两个数据一行，每一行当中的第一个数据是当前单词的是否在单词本中，
	第二个数据是错误了多少次
	在单词本中为1，不在单词本中为0
	*/
	int IsInWordBook = 0;
	int ErrorsTimes = 0;
	for (int i = 0; i < 3665; i++)
	{
		IsInWordBook = (word[i].isInWordsBook == true) ? 1 : 0;
		ErrorsTimes = notSortWords[i].errorTimes;
		fputs(_itoa(IsInWordBook, s, 10), fp);
		fputs(" ", fp);
		fputs(_itoa(ErrorsTimes, s, 10), fp);
		fputs("\n", fp);
	}
	fclose(fp);
}

void sortMistakeNumOfWords()
{
	for (int i = 0; i < 3665; i++)
	{
		sortedWords[i].errorTimes = notSortWords[i].errorTimes;
		sortedWords[i].index = notSortWords[i].index;
	}

	qsort(sortedWords, 3665, sizeof(sortedWords[0]), comp);
}

int comp(const void*a, const void*b)
{
	int ret = 0;
	if ((*(mistakeNumOfWords*)a).errorTimes > (*(mistakeNumOfWords*)b).errorTimes)
	{
		ret = -1;
	}
	else if ((*(mistakeNumOfWords*)a).errorTimes < (*(mistakeNumOfWords*)b).errorTimes)
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	return ret;
}

void searchInit()
{
	loadimage(&SearchInit, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\searchInit.jpg"));
	loadimage(&EnglishToChinese, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\EnglishToChinese.jpg"));
	loadimage(&ChineseToEnglish, _T("C:\\Users\\ztlzl\\Desktop\\CE-Dict\\ChineseToEnglish.jpg"));

	isReturnSearchWords = 0;
	MOUSEMSG m;
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了中文查询英文
				if (m.x > 250 && m.x < 790 && m.y > 220 && m.y < 315)
				{
					click();
					chineseToEnglish();
					if (isReturnSearchWords == 1)
					{
						isReturnSearchWords = 0;
					}
				}
				//如果点击到了英文查询中文
				else if (m.x > 240 && m.x < 778 && m.y > 350 && m.y < 447)
				{
					click();
					englishToChinese();
					if (isReturnSearchWords == 1)
					{
						isReturnSearchWords = 0;
						isSubmitSearchEnglishToChinese = false;
						lengthOfSearchEnglishToChinese = 0;
						searchEnglishToChinese[0] = '\0';
					}
				}
				//如果点击到了返回
				else if (m.x > 860 && m.x < 993 && m.y > 4 && m.y < 148)
				{
					click();
					isReturnSearchWords = 1;
					break;
				}
			}
		}
		putimage(0, 0, &SearchInit);
		FlushBatchDraw();
	}
}

void englishToChinese()
{
	MOUSEMSG m;
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				click();
				//如果点击到了返回
				if (m.x > 851 && m.x < 992 && m.y > 6 && m.y < 146)
				{
					isReturnSearchWords = 1;
					break;
				}
			}
		}

		//如果有键盘消息
		if (_kbhit())
		{
			char ch = _getch();
			if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
				//如果是字母
			{
				searchEnglishToChinese[lengthOfSearchEnglishToChinese] = ch;
				searchEnglishToChinese[lengthOfSearchEnglishToChinese + 1] = '\0';
				lengthOfSearchEnglishToChinese++;

				//不要超过数组上界
				if (lengthOfSearchEnglishToChinese >= 24)
					lengthOfSearchEnglishToChinese = 23;
			}
			//退格
			else if (ch == 8)
			{
				//不要超过数组下界
				if (lengthOfSearchEnglishToChinese == 0)
					searchEnglishToChinese[0] = '\0';
				else
				{
					searchEnglishToChinese[lengthOfSearchEnglishToChinese - 1] = '\0';
					lengthOfSearchEnglishToChinese--;
				}
			}
			//回车
			else if (ch == 13)
			{
				isSubmitSearchEnglishToChinese = true;
				if (searchEnglishToChinese[0] != '\0')
				{
					fromEnglishSearchChinese(searchEnglishToChinese);
				}
				else
				{
					wordIndexInSearchEnglishToChinese = -1;
				}
			}
		}

		drawEnglishToChinese();
		FlushBatchDraw();
	}
}

void drawEnglishToChinese()
{
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	putimage(0, 0, &EnglishToChinese);

	settextstyle(50, 0, _T("宋体"));
	settextcolor(BLACK);
	outtextxy(400, 210, searchEnglishToChinese);

	CHAR searchChinese[50];
	//如果还没提交过
	if (isSubmitSearchEnglishToChinese == false)
	{
		wsprintf(searchChinese, "%s", "按回车键发起查询");
	}
	else
	{
		if (wordIndexInSearchEnglishToChinese == -1)
		{
			wsprintf(searchChinese, "%s", "未找到该单词");
		}
		else
		{
			wsprintf(searchChinese, "%s", word[wordIndexInSearchEnglishToChinese].Chinese);
		}
	}
	settextcolor(RED);
	outtextxy(400, 350, searchChinese);
}

void drawChineseToEnglish()
{
	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	putimage(0, 0, &ChineseToEnglish);
	settextstyle(50, 0, _T("宋体"));
	settextcolor(BLACK);
	outtextxy(400, 215, str.c_str());

	CHAR searchEnglish[25];
	if (isSubmitSearchChineseToEnglish == false)
	{
		wsprintf(searchEnglish, "%s", "按回车键发起查询");
	}
	else
	{
		if (wordIndexInSearchChineseToEnglish == -1)
		{
			wsprintf(searchEnglish, "%s", "未搜索到");
		}
		else
		{
			wsprintf(searchEnglish, "%s", word[wordIndexInSearchChineseToEnglish].English);
		}
	}
	settextcolor(RED);
	outtextxy(400, 350, searchEnglish);
}

void chineseToEnglish()
{
	isSubmitSearchChineseToEnglish = false;
	HWND hWnd = GetHWnd();
	settextstyle(30, 0, _T("宋体"));
	settextcolor(RED);
	MOUSEMSG m;
	while (1)
	{
		if (MouseHit())
		{
			m = GetMouseMsg();
			//如果鼠标点击
			if (m.mkLButton == true)
			{
				//如果点击到了返回
				if (m.x > 851 && m.x < 992 && m.y > 6 && m.y < 146)
				{
					click();
					isReturnSearchWords = 1;
					break;
				}
			}
		}

		if (_kbhit()) //如果是ASCII输入
		{
			char c = _getch();
			//退格
			if (c == '\b')
			{
				//c++ style
				if (str.length() > 0)
				{
					if (str.at(str.length() - 1) & 0x8000)
						str.erase(str.end() - 1);
					str.erase(str.end() - 1);
				}
			}
			//回车
			else if (c == 13)
			{
				isSubmitSearchChineseToEnglish = true;
				wordIndexInSearchChineseToEnglish = -1;
				for (int i = 0; i < 3665; i++)
				{
					//把C语言当中的字符数组转化成c++里面的string字符串，然后再用string的find函数来查找子字符串
					string a = "";//c++ style
					//c++ style
					for (int j = 0; j < strlen(word[i].Chinese); j++)
					{
						a += (word[i].Chinese)[j];
					}
					//如果找到了
					//c++ style
					if (a.find(str.c_str()) != string::npos)
					{
						wordIndexInSearchChineseToEnglish = i;
						break;
					}
				}
			}
			//c++ style
			else 
			{
				str += c;
			}
		}
		//c++ style
		else //除此之外，检测是否有IME输入，如果有，则将输入结果添加到string中
		{
			GetIMEString(hWnd, str);
		}
		//c++ style
		if (str.length() > 100)
			str = "";

		drawChineseToEnglish();
		FlushBatchDraw();
	}
}

void fromEnglishSearchChinese(char *a)
{
	//二分法查找
	int end = 3664;
	int start = 0;
	while (start <= end)
	{
		if (_stricmp(a, word[((end + start) / 2)].English) == 0)
		{
			wordIndexInSearchEnglishToChinese = (end + start) / 2;
			break;
		}
		else if (_stricmp(a, word[((end + start) / 2)].English) > 0)
		{
			start = (end + start) / 2 + 1;
		}
		else
		{
			end = (end + start) / 2 - 1;
		}
	}

	//如果没有查询到
	if (start > end)
		wordIndexInSearchEnglishToChinese = -1;
}

void click()
{
	if (musicIsOpen == true)
	{
		mciSendString(_T("close click"), NULL, 0, NULL);
		mciSendString(_T("open C:\\Users\\ztlzl\\Desktop\\CE-Dict\\click.wma alias click"), NULL, 0, NULL);
		mciSendString(_T("play click"), NULL, 0, NULL);
	}
}

void GetIMEString(HWND hWnd, string& str)
{
	//c++ style 整个函数
	HIMC hIMC = ImmGetContext(hWnd);//获取HIMC
	if (hIMC)
	{
		//这里先说明一下，以输入“中国”为例
		//切换到中文输入法后，输入“zhongguo”，这个字符串称作IME组成字符串
		//而在输入法列表中选择的字符串“中国”则称作IME结果字符串
		static bool flag = false;//输入完成标记：在输入中时，IME组成字符串不为空，置true；输入完成后，IME组成字符串为空，置false
		DWORD dwSize = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, NULL, 0); //获取IME组成输入的字符串的长度
		if (dwSize > 0)//如果IME组成字符串不为空，且没有错误（此时dwSize为负值），则置输入完成标记为true
		{
			if (flag == false)
			{
				flag = true;
			}
		}
		else if (dwSize == 0 && flag) //如果IME组成字符串为空，并且标记为true，则获取IME结果字符串
		{
			int iSize; //IME结果字符串的大小
			LPSTR pszMultiByte = NULL;//IME结果字符串指针
			int ChineseSimpleAcp = 936;//宽字节转换时中文的编码
			WCHAR* lpWideStr = NULL;//宽字节字符数组
			dwSize = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);//获取IME结果字符串的大小
			if (dwSize > 0) //如果IME结果字符串不为空，且没有错误
			{
				dwSize += sizeof(WCHAR);//大小要加上NULL结束符
				//为获取IME结果字符串分配空间
				if (lpWideStr)
				{
					delete[]lpWideStr;
					lpWideStr = NULL;
				}
				lpWideStr = new WCHAR[dwSize];
				memset(lpWideStr, 0, dwSize); //清空结果空间
				ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, lpWideStr, dwSize);//获取IME结果字符串，这里获取的是宽字节
				iSize = WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, NULL, 0, NULL, NULL);//计算将IME结果字符串转换为ASCII标准字节后的大小
				//为转换分配空间
				if (pszMultiByte)
				{
					delete[] pszMultiByte;
					pszMultiByte = NULL;
				}
				pszMultiByte = new char[iSize + 1];
				WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, pszMultiByte, iSize, NULL, NULL);//宽字节转换
				pszMultiByte[iSize] = '\0';
				str += pszMultiByte;//添加到string中
				//释放空间
				if (lpWideStr)
				{
					delete[]lpWideStr;
					lpWideStr = NULL;
				}
				if (pszMultiByte)
				{
					delete[] pszMultiByte;
					pszMultiByte = NULL;
				}
			}
			flag = false;
		}
		ImmReleaseContext(hWnd, hIMC);//释放HIMC
	}
}