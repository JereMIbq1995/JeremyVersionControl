#ifndef HASHING_UNIT
#define HASHING_UNIT

/*
    Prototype for the Hashing unit
*/
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

class HashingUnit
{

private:
    map<int, uint32_t> H1;
    map<int, uint32_t> H2;
    bool HSwitch;
    map<int, uint32_t> K;

    // Perform a bitwise computation based on what round it is
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

    // Takes in
    uint32_t rotl(uint32_t num, int pos)
    {
        return (num << pos) | (num >> (32 - pos));
    }

    void hash512(char c[])
    {
        // The 80 rounds of computation
        bool HSwitch = true;
        int roundNumber = 0;
        for (int i = 1; i <= 5; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                int itOutOf5 = roundNumber / 20;

                // Read the next 32 bits chunk
                uint32_t *chunk = (uint32_t *)&(c[j * 4]);

                // Hashing...
                if (HSwitch)
                {
                    H2[0] = (F(itOutOf5, H1[1], H1[2], H1[3]) + H1[4] + rotl(H1[0], 5) + *chunk + K.at(itOutOf5));
                    H2[1] = H1[0];
                    H2[2] = rotl(H1[1], 30);
                    H2[3] = H1[2];
                    H2[4] = H1[3];
                }
                else
                {
                    H1[0] = (F(itOutOf5, H2[1], H2[2], H2[3]) + H2[4] + rotl(H2[0], 5) + *chunk + K.at(itOutOf5));
                    H1[1] = H2[0];
                    H1[2] = rotl(H2[1], 30);
                    H1[3] = H2[2];
                    H1[4] = H2[3];
                }

                roundNumber++;
                HSwitch = HSwitch ? false : true;
            }
        }
    }

public:
    HashingUnit()
    {
        reset();
    }

    // Reset H and K to initial values
    void reset()
    {

        HSwitch = true;

        H1 = {{0, uint32_t(0x67452301)},
              {1, uint32_t(0x98BADCFE)},
              {2, uint32_t(0xEFCDAB89)},
              {3, uint32_t(0xC3D2E1F0)},
              {4, uint32_t(0xAF9C3FD6)}};

        H2 = {{0, uint32_t(0x67452301)},
              {1, uint32_t(0x98BADCFE)},
              {2, uint32_t(0xEFCDAB89)},
              {3, uint32_t(0xC3D2E1F0)},
              {4, uint32_t(0xAF9C3FD6)}};

        K = {{0, uint32_t(0x5A827999)},
             {1, uint32_t(0x21A36DE2)},
             {2, uint32_t(0x8F1BBCDC)},
             {3, uint32_t(0x9CF6132A)}};
    }

    string getHash()
    {
        stringstream ss;
        // string binaryString = "";

        for (int i = 0; i < 5; i++)
        {
            ss << hex << uppercase << setw(8) << setfill('0') << H1[i];
            // binaryString += H[i].to_string();
        }
        // cout << binaryString << endl;
        // ss << hex << uppercase << (bitset<160> (binaryString)).to_ulong();
        // cout << ss.str() << endl;
        return ss.str();
    }

    /*
        Receive a binary text with numChar == 512
        Turn original string into hashed binary string
        Return the hashed string in Hex
    */
    

    string hashFileContent(string fileName)
    {
        fstream fin;
        fin.open(fileName, ios::in | ios::binary);

        if (fin.is_open())
        {
            char s[64];

            while (fin.read(s, 64))
            {
                hash512(s);
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
                hash512(s);
                // displayH();
            }

            return getHash();
        }
        else
        {
            return "Could not open file!";
        }
    }
};

#endif