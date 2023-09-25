#include <stdio.h>
#include <stdlib.h>

#define NUM_CANDIDATES 3
#define NUM_VOTERS 25

typedef struct {
    int first_choice;
    int second_choice;
    int third_choice;
} Vote;

typedef struct {
    int candidate;
    int count;
} CandidateVoteCount;

// Initializes the vote counts of all candidates to zero
void initializeCandidateVotes(CandidateVoteCount candidates[]) 
{
    for (int i = 0; i < NUM_CANDIDATES; i++) 
    {
        candidates[i].candidate = i + 1;
        candidates[i].count = 0;
    }
}

// Counts the votes for each candidate based on the first choice of each voter
void countVotes(FILE *file, CandidateVoteCount candidates[]) 
{
    Vote vote;
    while (fscanf(file, "%d %d %d", &vote.first_choice, &vote.second_choice, &vote.third_choice) == 3) 
    {
        candidates[vote.first_choice - 1].count++;
    }
}

int main() 
{
    printf("*****Co-Operative Society Elections*****\n\n");
    printf("There Are 3 Candidates For The Election.... People Have Chosen And Voted Them According To Their Preferences...\n\n");
    printf("The Voters Data Is Being Read From A File......\n\n");
    printf("The Result Is .....\n\n");

    // Open the input file containing voter preferences
    FILE *file = fopen("votes.txt", "r");
    if (file == NULL) 
    {
        printf("Error opening file.\n");
        return 1;
    }

    CandidateVoteCount candidates[NUM_CANDIDATES];

    // Initialize candidate vote counts to zero
    initializeCandidateVotes(candidates);

    // Count votes based on first choices
    countVotes(file, candidates);
    fclose(file);

    // Sort candidates based on vote counts
    CandidateVoteCount temp;
    for(int i = 1; i < NUM_CANDIDATES; i++)
    {
        if(candidates[i].count > candidates[0].count)
        {
           temp = candidates[0];
           candidates[0] = candidates[i];
           candidates[i] = temp;
        }   
    }

    //if (candidates[0].count <= NUM_VOTERS / 2 )
    if (candidates[0].count == candidates[1].count || candidates[0].count == candidates[2].count) 
    {
        printf("There is a tie.... Candidate Has Not Gained The Majority.... Checking Second Preferences...\n");

        // Bring the candidate that is equal to first candidate in the second positon
        if(candidates[0].count == candidates[2].count) 
        {
            candidates[1] = candidates[2];
        }

        // Open the input file again to take second choices
        file = fopen("votes.txt", "r");

        CandidateVoteCount candidates_second[NUM_CANDIDATES];

        // Initialize candidate vote counts to zero
        initializeCandidateVotes(candidates_second);

        Vote vote;
        while (fscanf(file, "%d %d %d", &vote.first_choice, &vote.second_choice, &vote.third_choice) == 3) 
        {
            candidates_second[vote.second_choice - 1].count++;
        }

        fclose(file);

        int max_second_choice = 0;
        int max_second_choice_votes = candidates_second[0].count;

        // Find the candidate with the highest second choice votes
        if (candidates_second[1].count > max_second_choice_votes) 
        {
            max_second_choice = 1;
            max_second_choice_votes = candidates_second[1].count;
        } 
        else if (candidates_second[1].count == max_second_choice_votes) 
        {
            max_second_choice = -1; // Indicate a tie again in the second preference
        }

        if (max_second_choice != -1) 
        {
            printf("Candidate %d wins with %d second preference votes!\n\n", max_second_choice + 1, max_second_choice_votes);
        } 
        else 
        {
            printf("No candidate has won with second preference votes either. It's a tie!\n\n");
        }
    } 

    else 
    {
        printf("Candidate %d wins with %d votes!\n\n\n", candidates[0].candidate, candidates[0].count);
    }

    return 0;
}      