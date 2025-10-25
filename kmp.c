#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void get_lps(char *word, int *lps)
{
    int i = 0;
    int j = 1;

    while (j < strlen(word))
    {
        /* code */
        if (word[i] == word[j])
        {
            i++;

            lps[j] = i;

            j++;
        }
        else
        {

            if (i > 0)
            {
                i = lps[i - 1];
            }
            else
            {
                lps[j] = 0;
                j++;
            }
        }
    }
    /*
     for (int l = 0; l < j; l++)
    {
        printf("%d ", lps[l]);
    }

    printf("\n");
     */
}
int main()
{

    char word[] = "le mot de lorem ifp sim";
    char sub[] = "ip";
    int lps[30] = {0};
    bool found = false;

    get_lps(sub, lps);
    int j = 0;
    int i = 0;
    int idx=0 ;

    while (i < strlen(word))
    {
        /* code */

        if (word[i] == sub[j])
        {
            j++;
            i++;
        }

        else
        {
            if (j > 0)
                j = lps[j - 1];
            else
                i++;
        }

        if (j == strlen(sub))
        {
            found = true;
            idx=i-j ;
            break;
        }
    }

    if(found)
    printf("found at index = %d \n",idx);
    else 
    printf("not found \n") ;
}