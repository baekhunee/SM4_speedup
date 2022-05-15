#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <time.h>
using namespace std;
//加密过程中的新S盒
string S0[16][16];
string S1[16][16];
string S2[16][16];
string S3[16][16];
//密钥扩展算法中的新S盒
string SK0[16][16];
string SK1[16][16];
string SK2[16][16];
string SK3[16][16];

string Sbox[16][16] = {
	{"D6","90","E9","FE","CC","E1","3D","B7","16","B6","14","C2","28","FB","2C","05"},
	{"2B","67","9A","76","2A","BE","04","C3","AA","44","13","26","49","86","06","99"},
	{"9C","42","50","F4","91","EF","98","7A","33","54","0B","43","ED","CF","AC","62"},
	{"E4","B3","1C","A9","C9","08","E8","95","80","DF","94","FA","75","8F","3F","A6"},
	{"47","07","A7","FC","F3","73","17","BA","83","59","3C","19","E6","85","4F","A8"},
	{"68","6B","81","B2","71","64","DA","8B","F8","EB","0F","4B","70","56","9D","35"},
	{"1E","24","0E","5E","63","58","D1","A2","25","22","7C","3B","01","21","78","87"},
	{"D4","00","46","57","9F","D3","27","52","4C","36","02","E7","A0","C4","C8","9E"},
	{"EA","BF","8A","D2","40","C7","38","B5","A3","F7","F2","CE","F9","61","15","A1"},
	{"E0","AE","5D","A4","9B","34","1A","55","AD","93","32","30","F5","8C","B1","E3"},
	{"1D","F6","E2","2E","82","66","CA","60","C0","29","23","AB","0D","53","4E","6F"},
	{"D5","DB","37","45","DE","FD","8E","2F","03","FF","6A","72","6D","6C","5B","51"},
	{"8D","1B","AF","92","BB","DD","BC","7F","11","D9","5C","41","1F","10","5A","D8"},
	{"0A","C1","31","88","A5","CD","7B","BD","2D","74","D0","12","B8","E5","B4","B0"},
	{"89","69","97","4A","0C","96","77","7E","65","B9","F1","09","C5","6E","C6","84"},
	{"18","F0","7D","EC","3A","DC","4D","20","79","EE","5F","3E","D7","CB","39","48"} };

//进制转换
string Hex(string s)//2->16
{
	string hex = "";
	int tmp = 0;
	while (s.size() % 4 != 0)
		s = "0" + s;
	for (int i = 0; i < s.size(); i += 4)
	{
		tmp = (s[i] - '0') * 8 + (s[i + 1] - '0') * 4 + (s[i + 2] - '0') * 2 + (s[i + 3] - '0') * 1;
		if (tmp < 10)
			hex += to_string(tmp);
		else
			hex += 'A' + (tmp - 10);
	}
	return hex;
}
string Bin(string s)//16->2
{
	string bin = "";
	string table[16] = {
	"0000","0001","0010","0011",
	"0100","0101","0110","0111",
	"1000","1001","1010","1011",
	"1100","1101","1110","1111" };
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] >= 'A' && s[i] <= 'F')
			bin += table[s[i] - 'A' + 10];
		else
			bin += table[s[i] - '0'];
	}
	return bin;
}
int Dec(char s)//16->10
{
	int dec = 0;
	if (s >= 'A' && s <= 'F')
		dec += (s - 'A' + 10);
	else
		dec += (s - '0');
	return dec;
}
//左移+异或
string leftshift(string s, int len)
{
	string res = Bin(s);
	res = res.substr(len) + res.substr(0, len);
	return Hex(res);
}
string XOR(string s1, string s2)
{
	string res1 = Bin(s1);
	string res2 = Bin(s2);
	string res = "";
	for (int i = 0; i < res1.size(); i++)
	{
		if (res1[i] == res2[i])
			res += "0";
		else
			res += "1";
	}
	return Hex(res);
}
//原始 SM4
string T(string s)
{
	string res = "";
	for (int i = 0; i < 4; i++)
	{
		res = res + Sbox[Dec(s[i * 2])][Dec(s[2 * i + 1])];
	}
	return res;
}
string L1(string s)
{
	return  XOR(XOR(XOR(XOR(s, leftshift(s, 2)), leftshift(s, 10)), leftshift(s, 18)), leftshift(s, 24));
}
string L2(string s)
{
	return  XOR(XOR(s, leftshift(s, 13)), leftshift(s, 23));
}
string plain_T(string s)
{
	return L1(T(s));
}
string key_T(string s)
{
	return L2(T(s));
}
string key_extent(string MK)
{
	string FK[4] = { "A3B1BAC6", "56AA3350", "677D9197", "B27022DC" };
	string CK[32] = { "00070E15", "1C232A31", "383F464D", "545B6269",
					  "70777E85", "8C939AA1", "A8AFB6BD", "C4CBD2D9",
					  "E0E7EEF5", "FC030A11", "181F262D", "343B4249",
					  "50575E65", "6C737A81", "888F969D", "A4ABB2B9",
					  "C0C7CED5", "DCE3EAF1", "F8FF060D", "141B2229",
					  "30373E45", "4C535A61", "686F767D", "848B9299",
					  "A0A7AEB5", "BCC3CAD1", "D8DFE6ED", "F4FB0209",
					  "10171E25", "2C333A41", "484F565D", "646B7279" };

	string K[36] = { XOR(FK[0],MK.substr(0,8)),XOR(FK[1],MK.substr(8,8)),XOR(FK[2],MK.substr(16,8)),XOR(FK[3],MK.substr(24,8)) };
	string rk = "";
	for (int i = 0; i < 32; i++)
	{
		K[i + 4] = XOR(key_T(XOR(XOR(XOR(K[i + 1], K[i + 2]), K[i + 3]), CK[i])), K[i]);
		rk += K[i + 4];
	}
	return rk;
}
string SM4_enc(string m, string key)
{
	string X[36] = { m.substr(0,8),m.substr(8,8), m.substr(16,8), m.substr(24,8) };
	string rk = key_extent(key);
	for (int i = 0; i < 32; i++)
	{
		X[i + 4] = XOR(X[i], plain_T(XOR(XOR(XOR(X[i + 1], X[i + 2]), X[i + 3]), rk.substr(8 * i, 8))));
	}
	return X[35] + X[34] + X[33] + X[32];
}
//加密 T-table
string padding0(string s)
{
	string tmp = s;
	for (int i = 0; i < 24; i++)
		tmp.push_back('0');
	return tmp;
}
string padding1(string s)
{
	string tmp = s;
	for (int i = 0; i < 8; i++)
		tmp.insert(tmp.begin(), '0');
	for (int i = 0; i < 16; i++)
		tmp.push_back('0');
	return tmp;
}
string padding2(string s)
{
	string tmp = s;
	for (int i = 0; i < 16; i++)
		tmp.insert(tmp.begin(), '0');
	for (int i = 0; i < 8; i++)
		tmp.push_back('0');
	return tmp;
}
string padding3(string s)
{
	string tmp = s;
	for (int i = 0; i < 24; i++)
		tmp.insert(tmp.begin(), '0');
	return tmp;
}
void plaintext_newsboxs()
{
	string s0 = "";
	string s1 = "";
	string s2 = "";
	string s3 = "";

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			s0 = Hex(padding0(Bin(Sbox[i][j])));
			S0[i][j] = XOR(XOR(XOR(XOR(s0, leftshift(s0, 2)), leftshift(s0, 10)), leftshift(s0, 18)), leftshift(s0, 24));
		}
	}
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			s1 = Hex(padding1(Bin(Sbox[i][j])));
			S1[i][j] = XOR(XOR(XOR(XOR(s1, leftshift(s1, 2)), leftshift(s1, 10)), leftshift(s1, 18)), leftshift(s1, 24));
		}
	}
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			s2 = Hex(padding2(Bin(Sbox[i][j])));
			S2[i][j] = XOR(XOR(XOR(XOR(s2, leftshift(s2, 2)), leftshift(s2, 10)), leftshift(s2, 18)), leftshift(s2, 24));
		}
	}
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			s3 = Hex(padding3(Bin(Sbox[i][j])));
			S3[i][j] = XOR(XOR(XOR(XOR(s3, leftshift(s3, 2)), leftshift(s3, 10)), leftshift(s3, 18)), leftshift(s3, 24));
		}
	}
}
string plain_T1(string s)
{
	return XOR(XOR(XOR(S0[Dec(s[0])][Dec(s[1])], S1[Dec(s[2])][Dec(s[3])]), S2[Dec(s[4])][Dec(s[5])]), S3[Dec(s[6])][Dec(s[7])]);
}
string SM4_enc_Ttable1(string m, string key)
{
	string X[36] = { m.substr(0,8),m.substr(8,8), m.substr(16,8), m.substr(24,8) };
	string rk = key_extent(key);
	for (int i = 0; i < 32; i++)
	{
		X[i + 4] = XOR(X[i], plain_T1(XOR(XOR(XOR(X[i + 1], X[i + 2]), X[i + 3]), rk.substr(8 * i, 8))));
	}
	return X[35] + X[34] + X[33] + X[32];
}
//加密+密钥 T-table
void key_newsboxs()
{
	string s0 = "";
	string s1 = "";
	string s2 = "";
	string s3 = "";

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			s0 = Hex(padding0(Bin(Sbox[i][j])));
			SK0[i][j] = XOR(XOR(s0, leftshift(s0, 13)), leftshift(s0, 23));
		}
	}
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			s1 = Hex(padding1(Bin(Sbox[i][j])));
			SK1[i][j] = XOR(XOR(s1, leftshift(s1, 13)), leftshift(s1, 23));
		}
	}
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			s2 = Hex(padding2(Bin(Sbox[i][j])));
			SK2[i][j] = XOR(XOR(s2, leftshift(s2, 13)), leftshift(s2, 23));
		}
	}
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			s3 = Hex(padding3(Bin(Sbox[i][j])));
			SK3[i][j] = XOR(XOR(s3, leftshift(s3, 13)), leftshift(s3, 23));
		}
	}
}
string key_T1(string s)
{
	return XOR(XOR(XOR(SK0[Dec(s[0])][Dec(s[1])], SK1[Dec(s[2])][Dec(s[3])]), SK2[Dec(s[4])][Dec(s[5])]), SK3[Dec(s[6])][Dec(s[7])]);
}
string key_extent1(string MK)
{
	string FK[4] = { "A3B1BAC6", "56AA3350", "677D9197", "B27022DC" };
	string CK[32] = { "00070E15", "1C232A31", "383F464D", "545B6269",
					  "70777E85", "8C939AA1", "A8AFB6BD", "C4CBD2D9",
					  "E0E7EEF5", "FC030A11", "181F262D", "343B4249",
					  "50575E65", "6C737A81", "888F969D", "A4ABB2B9",
					  "C0C7CED5", "DCE3EAF1", "F8FF060D", "141B2229",
					  "30373E45", "4C535A61", "686F767D", "848B9299",
					  "A0A7AEB5", "BCC3CAD1", "D8DFE6ED", "F4FB0209",
					  "10171E25", "2C333A41", "484F565D", "646B7279" };

	string K[36] = { XOR(FK[0],MK.substr(0,8)),XOR(FK[1],MK.substr(8,8)),XOR(FK[2],MK.substr(16,8)),XOR(FK[3],MK.substr(24,8)) };
	string rk = "";
	for (int i = 0; i < 32; i++)
	{
		K[i + 4] = XOR(key_T1(XOR(XOR(XOR(K[i + 1], K[i + 2]), K[i + 3]), CK[i])), K[i]);
		rk += K[i + 4];
	}
	return rk;
}
string SM4_enc_Ttable2(string m, string key)
{
	string X[36] = { m.substr(0,8),m.substr(8,8), m.substr(16,8), m.substr(24,8) };
	string rk = key_extent1(key);
	for (int i = 0; i < 32; i++)
	{
		X[i + 4] = XOR(X[i], plain_T1(XOR(XOR(XOR(X[i + 1], X[i + 2]), X[i + 3]), rk.substr(8 * i, 8))));
	}
	return X[35] + X[34] + X[33] + X[32];
}

int main()
{
	
	clock_t start, end;
	string c1, c2, c3;
	string p = "0123456789ABCDEFFEDCBA9876543210";
	string key = "0123456789ABCDEFFEDCBA9876543210";

    plaintext_newsboxs();
	key_newsboxs();

	start = clock();
	c1 = SM4_enc(p, key);
	end = clock();
	printf("Time_original:%.3f s\n\n", double(end - start) / CLOCKS_PER_SEC);

	start = clock();
	c2 = SM4_enc_Ttable1(p, key);
	end = clock();
	printf("Time_Ttable1:%.3f s\n\n", double(end - start) / CLOCKS_PER_SEC);
	
	start = clock();
	c3 = SM4_enc_Ttable2(p, key);
	end = clock();
	printf("Time_Ttable2:%.3f s\n\n", double(end - start) / CLOCKS_PER_SEC);

	printf("Plaintext:\n%s %s %s %s \n\n", p.substr(0, 8).c_str(), p.substr(8, 8).c_str(), p.substr(16, 8).c_str(), p.substr(24, 8).c_str());
	printf("Enc:\n%s %s %s %s \n\n", c1.substr(0, 8).c_str(), c1.substr(8, 8).c_str(), c1.substr(16, 8).c_str(), c1.substr(24, 8).c_str());
	printf("Enc_Ttable1:\n%s %s %s %s \n\n", c2.substr(0, 8).c_str(), c2.substr(8, 8).c_str(), c2.substr(16, 8).c_str(), c2.substr(24, 8).c_str());
	printf("Enc_Ttable2:\n%s %s %s %s \n\n", c3.substr(0, 8).c_str(), c3.substr(8, 8).c_str(), c3.substr(16, 8).c_str(), c3.substr(24, 8).c_str());
}

