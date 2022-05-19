#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <assert.h>
#include <bitset>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;

/*

*/
map<int, uint32_t> H = {
    {0, uint32_t(0x67452301)},
    {1, uint32_t(0x98BADCFE)},
    {2, uint32_t(0xEFCDAB89)},
    {3, uint32_t(0xC3D2E1F0)},
    {4, uint32_t(0xAF9C3FD6)}};

void displayH()
{
    stringstream ss;
    // string binaryString = "";

    for (int i = 0; i < 5; i++)
    {
        ss << hex << uppercase << setw(8) << setfill('0') << H[i];
        // binaryString += H[i].to_string();
    }
    // cout << binaryString << endl;
    // ss << hex << uppercase << (bitset<160> (binaryString)).to_ulong();
    cout << ss.str() << endl;
}
/*
    K Map for the 80 rounds
*/
const map<int, uint32_t> K = {
    {0, uint32_t(0x5A827999)},
    {1, uint32_t(0x21A36DE2)},
    {2, uint32_t(0x8F1BBCDC)},
    {3, uint32_t(0x9CF6132A)}};

/*
    Some random bit operations on B, C, and D for rounds 1-20
*/
uint32_t F(int round, const uint32_t &B, const uint32_t &C, const uint32_t &D)
{
    switch (round)
    {
    case 0:
        return (B & C) | (~B & D);
    case 1:
        return ~(B ^ C ^ D);
    case 2:
        return (B & C) | (B & D) | (C & D);
    default:
        return ~(B & C) ^ (B & D) ^ ~(C & D);
    }
}

/*
    Receive a binary string of length 160 chars
*/
string binaryToHex(string binary)
{
    return "";
}

/*
    Compression function:
    Takes in a message of 512 chars and the current H (160 chars)
    Returns the new value of H
*/
string compress(string)
{
    return "";
}

/*
    RotL: rotate the bits left
*/
uint32_t rotl(uint32_t num, int pos) {
    return (num << pos) | (num >> (32-pos));
}

/*
    Receive a binary text with numChar % 512 == 0
    Turn original string into hashed binary string
    Return the hashed string in Hex
*/
void sha1hash(char c[])
{

    // Step 1: H is already intitialized in the H map
    // Step 2: Break the 512 bits into 16 chunks of length 32 bits
    // vector<bitset<32>> chunks;
    // string currentChunk = "";
    // int count8bitsChunks = 0;

    // for (int i = 0; i < 64; i++)
    // {
    //     currentChunk += bitset<8>(to_integer<int8_t>(byte(c[i]))).to_string();
    //     count8bitsChunks++;
    //     if (count8bitsChunks == 4)
    //     {
    //         chunks.push_back(bitset<32>(currentChunk));
    //         count8bitsChunks = 0;
    //         currentChunk = "";
    //     }
    // }

    // Step 3: The 80 rounds of computation
    int roundNumber = 0;
    for (int i = 1; i <= 5; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            int itOutOf5 = roundNumber / 20;

            // Read the next 32 bits chunk
            uint32_t *chunk = (uint32_t*)&(c[j*4]);

            // Hashing...
            uint32_t A_new = (F(itOutOf5, H[1], H[2], H[3]) + H[4] + rotl(H[0], 5) + *chunk + K.at(itOutOf5));
            uint32_t B_new = H[0];
            uint32_t C_new = rotl(H[1], 30);
            uint32_t D_new = H[2];
            uint32_t E_new = H[3];

            // Reassign H to new values
            H[0] = A_new;
            H[1] = B_new;
            H[2] = C_new;
            H[3] = D_new;
            H[4] = E_new;

            roundNumber++;
            // displayH();
        }
    }
    // cout << "round number: " << roundNumber << endl;
}

int main()
{
    fstream fin;

    string fileName;
    cout << "Enter file name: ";
    getline(cin, fileName);
    fin.open(fileName, ios::in | ios::binary);

    if (fin.is_open())
    {
        char s[64];

        while (fin.read(s, 64))
        {
            sha1hash(s);
            // displayH();
        }

        int remainingBytes = fin.gcount();

        if (remainingBytes > 0)
        {
            for (int i = 0; i < 64; i++)
            {
                if (i >= remainingBytes)
                {
                    s[i] = 0;
                }
            }
            sha1hash(s);
        }
        displayH();
    }
    else {
        cout << "Unable to open file!" << endl;
    }
    return 0;
}