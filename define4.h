// 1. RC4������������
struct RC4Struct {
	BYTE s[256];   //����Ϊ256�����ݱ�S
	int x, y;
	
	void InitZero(void)	{   // �ÿ���Կ��ʼ��������������
		int i, j;
		BYTE uc;
		
		x = y = j = 0;
		for (i = 0; i < 256; i ++) { //1.��S�������䣬����ʱ���ݱ�T
			s[i] = i;
		}
		for (i = 0; i < 256; i ++) {
			j = (j + s[i]) & 255;
			uc = s[i];
			s[i] = s[j];
			s[j] = uc;
		}
	}
	BYTE NextByte(void) {  // ��������������һ���ֽ�
		BYTE uc;
		x = (x + 1) & 255;
		y = (y + s[x]) & 255;
		uc = s[x];
		s[x] = s[y];
		s[y] = uc;
		return s[(s[x] + s[y]) & 255];
	}
	DWORD NextLong(void) { // ���������������ĸ��ֽ�
		BYTE uc0, uc1, uc2, uc3;
		uc0 = NextByte();
		uc1 = NextByte();
		uc2 = NextByte();
		uc3 = NextByte();
		return uc0 + (uc1 << 8) + (uc2 << 16) + (uc3 << 24);
	}
};


/* 2. Zobrist У����ṹ
   ����:���ٲ���32λ�Ĺ�ϣֵ��Ϊ�û�����ֵ 
  */
struct ZobristStruct {
	//32λ: �û�����ֵ(����ظ�����ʱʹ��)
	DWORD dwKey;    
	//64λ(2��32λ): �û���ļ�����
	DWORD dwLock0;
	DWORD dwLock1;
	
	//��ʼ��ʱ��0���
	void InitZero(void) {                 // �������Zobrist
		dwKey = dwLock0 = dwLock1 = 0;
	}
	void InitRC4(RC4Struct &rc4) {        // �����������Zobrist
		dwKey   = rc4.NextLong();
		dwLock0 = rc4.NextLong();
		dwLock1 = rc4.NextLong();
	}
	void Xor(const ZobristStruct &zobr) { // ִ��XOR����
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

// 3. Zobrist��:�̶��洢�����ڲ�ͬλ�������
static struct {
	ZobristStruct Player;    //���巽�������
	//ÿ���������ڲ�ͬλ��������Ӧ���������������������ļ�ֵ����������������Ӧ����������������Ľ��
	ZobristStruct Table[14][256]; 
} Zobrist;


// ��ʼ��Zobrist��
static void InitZobrist(void) {
	int i, j;
	RC4Struct rc4;
	
	rc4.InitZero();
	Zobrist.Player.InitRC4(rc4); //���巽�����
	for (i = 0; i < 14; i ++) {  //Ϊÿ�������ڲ�ͬλ�÷ֱ���������
		for (j = 0; j < 256; j ++) {
			Zobrist.Table[i][j].InitRC4(rc4);
		}
	}
}


// ��ʷ�߷���Ϣ(ռ4�ֽ�)
//ÿ��һ���߷��Ͱѵ�ǰ�����У�����¼�������ٿ���ǰ���������У�����Ƿ��뵱ǰֵ��ȡ�
struct MoveStruct {
	WORD wmv;
	BYTE ucpcCaptured, ucbCheck; //�߷��Ƿ񽫾�
	DWORD dwKey;
	//�൱�ڹ��캯��
	void Set(int mv, int pcCaptured, BOOL bCheck, DWORD dwKey_) {
		wmv = mv;
		ucpcCaptured = pcCaptured;
		ucbCheck = bCheck;
		dwKey = dwKey_;
	}
}; // mvs

// MVV/LVAÿ�������ļ�ֵ
static const BYTE cucMvvLva[24] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	5, 1, 1, 3, 4, 3, 2, 0,
	5, 1, 1, 3, 4, 3, 2, 0
};