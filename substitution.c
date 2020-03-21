#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool mapKeyIfValid(string key, char encryptKeyMap[], char decryptKeyMap[]);
string getPlaintext();
void crypt(string input, char keyMap[], char output[]);

// Pass the encryption key on the command line
// Supply plaintext at the prompt
// The ciphertext will be output
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        // Invalid usage, give some help
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else
    {
        string key = argv[1];
        char encryptKeyMap[26];
        char decryptKeyMap[26];
        if (!mapKeyIfValid(key, encryptKeyMap, decryptKeyMap))
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        else
        {
            // Will double check the decryption for good measure
            string plaintext = getPlaintext();
            char crypttext[strlen(plaintext) + 1];
            char decrypted[strlen(plaintext) + 1];
            crypt(plaintext, encryptKeyMap, crypttext);
            crypt(crypttext, decryptKeyMap, decrypted);
            if (strcmp(plaintext, decrypted) != 0)
            {
                printf("### decryption failed ###\n");
            }
            // printf("decrypted:  %s\n", decrypted);
            printf("ciphertext: %s\n", crypttext);
        }
    }
}

// Combines the operation of mapping the key letters to array positions
// and determining if any letters were missing or duplicated
bool mapKeyIfValid(string key, char encryptKeyMap[], char decryptKeyMap[])
{
    // Clear the letter map to a known value we can check for later
    for (int i = 0; i < 26; i++)
    {
        encryptKeyMap[i] = '~';
        decryptKeyMap[i] = '~';
    }

    // Map each letter
    for (int i = 0, s = strlen(key); i < s; i++)
    {
        char c = key[i];
        if (!isalpha(c))
        {
            return false;
        }
        int pos = tolower(c) - 'a';
        if (decryptKeyMap[pos] != '~')
        {
            // Duplicated letter
            return false;
        }
        decryptKeyMap[pos] = i;
        encryptKeyMap[i] = pos;
    }

    // Check and make sure all letters are mapped
    for (int i = 0; i < 26; i++)
    {
        if (encryptKeyMap[i] == '~' || decryptKeyMap[i] == '~')
        {
            // A letter was missed
            return false;
        }
    }
    return true;
}

// Prompts for and returns plaintext
string getPlaintext()
{
    return get_string("plaintext:  ");
}

// performs an encryption or decryption (requires the correct keyMap to be supplied)
void crypt(string input, char keyMap[], char output[])
{
    for (int i = 0, s = strlen(input); i < s; i++)
    {
        char c = input[i];
        if (islower(c))
        {
            output[i] = keyMap[c - 'a'] + 'a';
        }
        else if (isupper(c))
        {
            output[i] = keyMap[c - 'A'] + 'A';
        }
        else
        {
            output[i] = c;
        }
    }
    output[strlen(input)] = '\0';
}
