// A C++ solution for longest palindrome
#include <bits/stdc++.h>
using namespace std;

// Function to print a substring str[low..high]
void printSubStr(string str, int low, int high)
{
	for (int i = low; i <= high; ++i)
		cout << str[i];
}

// This function prints the
// longest palindrome substring
// It also returns the length
// of the longest palindrome
int longestPalSubstr(string str)
{
	// get length of input string
	int n = str.size();

	// All substrings of length 1
	// are palindromes
	int maxLength = 1, start = 0;

	// Nested loop to mark start and end index
	for (int i = 0; i < str.length(); i++) {
		for (int j = i; j < str.length(); j++) {
			int flag = 1;

			// Check palindrome
			for (int k = 0; k < (j - i + 1) / 2; k++)
				if (str[i + k] != str[j - k])
					flag = 0;

			// Palindrome
			if (flag && (j - i + 1) > maxLength) {
				start = i;
				maxLength = j - i + 1;
			}
		}
	}

	cout << "Longest palindrome substring is: ";
	printSubStr(str, start, start + maxLength - 1);

	// return length of LPS
	return maxLength;
}

// Driver Code
int main()
{
	string str = "B9CEC1BE05E0D3FBD16979775C41E7D6E65EFC7FAC796C8F8C0C41D6867651838ABCD9DE28E221BFF3D8E8CD7115625F8B672782A9D73225D06654BFA1F2AFF1EFDD36E7E37C4A5CFFB697006260A24ADDD0B5EDFD4EA4F7C5B003E1BA2E97E964E3B3133713674435797F3C0DF2014895945E9E6D0AF25F985847D3CC990A139D731B1E6D311880567862CEC797B2E38A62B46BE880D9614AF5AF75D3E61EBA7B08902F4F0F44E7CFC3B7A05FC1DCC97C930C8048F95BBF8F3968757572F23B90427D013633F17BA421452C0B37F6777FA8443943EAFCA8EDDDC9B9FEF6F7D4672FB7F843A943A518F256573398962B17F3BF4906B5B1254DB52B7DD93CFEF0E785C2DA8AB6CD1C5DB83CCDB53A9630CB1F203721004083947348C29FA1E7AAD5A9CE42060A041569DEF2F9704F72BA04EDB39FC24DAB10C799EE35DE1BEF532617037759F8DE7A5818630993DAA302B21B25711391391399F56C728B08BBC18CFC00C33794569A4012DE0D0E8D790FBF39B8844773BE48453ED257C4EEB5BABFED9BC02781FD7F133D0DA85BB74A86CC673FB8330F44560E32CAD81DB494E72F83F195442860DEDA16A810D8D043DE5B4F6AE41F8731F96CFE5DA897A142A158666E23C5F3DC5DF8EAFDFE6F7EA8CE68A86238BC1C5381C28E18284AB38AE8D5BF036BB81F0077C07064A8CF21A31AF2CF40";
	cout << "\nLength is: "
		<< longestPalSubstr(str);
	return 0;
}
