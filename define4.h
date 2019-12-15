// 1. RC4密码流生成器
struct RC4Struct {
	BYTE s[256];   //长度为256的数据表S
	int x, y;
	
	void InitZero(void)	{   // 用空密钥初始化密码流生成器
		int i, j;
		BYTE uc;
		
		x = y = j = 0;
		for (i = 0; i < 256; i ++) { //1.对S表进行填充，无临时数据表T
			s[i] = i;
		}
		for (i = 0; i < 256; i ++) {
			j = (j + s[i]) & 255;
			uc = s[i];
			s[i] = s[j];
			s[j] = uc;
		}
	}
	BYTE NextByte(void) {  // 生成密码流的下一个字节
		BYTE uc;
		x = (x + 1) & 255;
		y = (y + s[x]) & 255;
		uc = s[x];
		s[x] = s[y];
		s[y] = uc;
		return s[(s[x] + s[y]) & 255];
	}
	DWORD NextLong(void) { // 生成密码流的下四个字节
		BYTE uc0, uc1, uc2, uc3;
		uc0 = NextByte();
		uc1 = NextByte();
		uc2 = NextByte();
		uc3 = NextByte();
		return uc0 + (uc1 << 8) + (uc2 << 16) + (uc3 << 24);
	}
};


/* 2. Zobrist 校验码结构
   作用:快速产生32位的哈希值作为置换表达键值 
  */
struct ZobristStruct {
	//32位: 置换表达键值(检查重复局面时使用)
	DWORD dwKey;    
	//64位(2个32位): 置换表的检验吗
	DWORD dwLock0;
	DWORD dwLock1;
	
	//初始化时用0填充
	void InitZero(void) {                 // 用零填充Zobrist
		dwKey = dwLock0 = dwLock1 = 0;
	}
	void InitRC4(RC4Struct &rc4) {        // 用密码流填充Zobrist
		dwKey   = rc4.NextLong();
		dwLock0 = rc4.NextLong();
		dwLock1 = rc4.NextLong();
	}
	void Xor(const ZobristStruct &zobr) { // 执行XOR操作
		dwKey ^= zobr.dwKey;
		dwLock0 ^= zobr.dwLock0;
		dwLock1 ^= zobr.dwLock1;
	}
	void Xor(const ZobristStruct &zobr1, const ZobristStruct &zobr2) {
		dwKey ^= zobr1.dwKey ^ zobr2.dwKey;
		dwLock0 ^= zobr1.dwLock0 ^ zobr2.dwLock0;
		dwLock1 ^= zobr1.dwLock1 ^ zobr2.dwLock1;
	}
};

// 3. Zobrist表:固定存储棋子在不同位置随机数
static struct {
	ZobristStruct Player;    //走棋方的随机数
	//每个棋子子在不同位置上所对应的随机数，这样整个局面的键值就是所有棋子所对应的随机数的异或运算的结果
	ZobristStruct Table[14][256]; 
} Zobrist;


// 初始化Zobrist表
static void InitZobrist(void) {
	int i, j;
	RC4Struct rc4;
	
	rc4.InitZero();
	Zobrist.Player.InitRC4(rc4); //走棋方随机数
	for (i = 0; i < 14; i ++) {  //为每个棋子在不同位置分别产生随机数
		for (j = 0; j < 256; j ++) {
			Zobrist.Table[i][j].InitRC4(rc4);
		}
	}
}


// 历史走法信息(占4字节)
//每走一个走法就把当前局面的校验码记录下来，再看看前几个局面的校验码是否与当前值相等。
struct MoveStruct {
	WORD wmv;
	BYTE ucpcCaptured, ucbCheck; //走法是否将军
	DWORD dwKey;
	//相当于构造函数
	void Set(int mv, int pcCaptured, BOOL bCheck, DWORD dwKey_) {
		wmv = mv;
		ucpcCaptured = pcCaptured;
		ucbCheck = bCheck;
		dwKey = dwKey_;
	}
}; // mvs

// MVV/LVA每种子力的价值
static const BYTE cucMvvLva[24] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	5, 1, 1, 3, 4, 3, 2, 0,
	5, 1, 1, 3, 4, 3, 2, 0
};