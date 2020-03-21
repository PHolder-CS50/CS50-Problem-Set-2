#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Returned if the number of words is 0, avoiding a division by zero
const int  ERROR_CONDITION = -1000;

float calculateReadability(string text);
int calculateColemanLiauIndex(int numLetters, int numWords, int numSentences);
bool isEndOfSentenceChar(char c);
void doTestCases(void);

// Get a text block from the user and calculate
// the readbility as a grade level using the
// Coleman-Liau index
int main(void)
{
    string text = get_string("Text: ");
    if (text[0] == '@')
    {
        doTestCases();
    }
    else
    {
        int level = calculateReadability(text);
        if (level <= ERROR_CONDITION)
        {
            printf("An ERROR occured, cannot calculate readability.\n");
        }
        else if (level < 1)
        {
            printf("Before Grade 1\n");
        }
        else if (level >= 16)
        {
            printf("Grade 16+\n");
        }
        else
        {
            printf("Grade %i\n", level);
        }
    }
    // Intended to return the readability index, but that breaks check50
    // return level;
}

// Count number of letters, words and sentences in text
// to be able to calculate the readability index
float calculateReadability(string text)
{
    int numLetters = 0;
    int numWords = 0;
    int numSentences = 0;
    char prevC = ' ';
    bool sentenceStarted = false;

    // Intentionally processing the '\0' so that we count the final
    // word or sentence
    for (int i = 0, s = strlen(text); i <= s; i++)
    {
        char c = text[i];
        // I thought maybe apostophe's and dashes should count, but apparently not
        // if (isalpha(c) || c == '\'' || c == '-')
        if (isalpha(c))
        {
            numLetters++;
            sentenceStarted = true;
        }
        if (prevC != ' ' && (c == ' ' || c == '\0'))
        {
            numWords++;
        }
        if (sentenceStarted && (isEndOfSentenceChar(c) || c == '\0'))
        {
            numSentences++;
            sentenceStarted = false;
        }
        prevC = c;
    }

    if (false)
    {
        printf("%i letter(s)\n", numLetters);
        printf("%i word(s)\n", numWords);
        printf("%i sentence(s)\n", numSentences);
    }

    float index = calculateColemanLiauIndex(numLetters, numWords, numSentences);
    if (index <= ERROR_CONDITION)
    {
        printf("An ERROR occured processing text, number of words was invalid.\n");
        return ERROR_CONDITION;
    }
    return index;
}

// Check if a character is considered to be the end of a sentence
bool isEndOfSentenceChar(char c)
{
    return c == '.' || c == '?' || c == '!';
}

// The formulat is
//   index = 0.0588 * L - 0.296 * S - 15.8
// where L is the average number of letters per 100 words in the text,
// and S is the average number of sentences per 100 words in the text.
int calculateColemanLiauIndex(int numLetters, int numWords, int numSentences)
{
    if (numWords < 1)
    {
        return ERROR_CONDITION;
    }

    double L = numLetters * 100.0 / numWords;
    double S = numSentences * 100.0 / numWords;
    return (int)round(0.0588 * L - 0.296 * S - 15.8);
}

// Run a test case, producing an error message if any fail (and also returing true/false)
bool expectResult(int grade, string text)
{
    int level = calculateReadability(text);
    if (grade == 0 && level < 1)
    {
        return true;
    }
    else if (grade == 16 && level >= 16)
    {
        return true;
    }
    else if (grade == level)
    {
        return true;
    }
    printf("### TEST CASE FAILED ###\n");
    printf("%s\n", text);
    printf(" => expected = %i  actual = %i\n", grade, level);
    return false;
}

// Automate the test cases provided.  Invoke them by entering '@' as the input text
void doTestCases(void)
{
    expectResult(0, "One fish. Two fish. Red fish. Blue fish.");
    expectResult(2, "Would you like them here or there? I would not like them here or there. I would not like them anywhere.");
    expectResult(3, "Congratulations! Today is your day. You're off to Great Places! You're off and away!");
    expectResult(5, "Harry Potter was a highly unusual boy in many ways. For one thing, he hated the \
                    summer holidays more than any other time of year. For another, he really wanted \
                    to do his homework, but was forced to do it in secret, in the dead of the night. \
                    And he also happened to be a wizard.");
    expectResult(7, "In my younger and more vulnerable years my father gave me some advice that I've \
                    been turning over in my mind ever since.");
    expectResult(8, "Alice was beginning to get very tired of sitting by her sister on the bank, and \
                    of having nothing to do: once or twice she had peeped into the book her sister \
                    was reading, but it had no pictures or conversations in it, \"and what is the use of \
                    a book,\" thought Alice \"without pictures or conversation?\"");
    expectResult(8, "When he was nearly thirteen, my brother Jem got his arm badly broken at the elbow. \
                    When it healed, and Jem's fears of never being able to play football were assuaged, \
                    he was seldom self-conscious about his injury. His left arm was somewhat shorter \
                    than his right; when he stood or walked, the back of his hand was at right angles \
                    to his body, his thumb parallel to his thigh.");
    expectResult(9, "There are more things in Heaven and Earth, Horatio, than are dreamt of in your philosophy.");
    expectResult(10, "It was a bright cold day in April, and the clocks were striking thirteen. Winston Smith, \
                    his chin nuzzled into his breast in an effort to escape the vile wind, slipped quickly \
                    through the glass doors of Victory Mansions, though not quickly enough \
                    to prevent a swirl of gritty dust from entering along with him.");
    expectResult(16, "A large class of computational problems involve the determination of properties of \
                    graphs, digraphs, integers, arrays of integers, finite families of finite sets, boolean \
                    formulas and elements of other countable domains.");
}
