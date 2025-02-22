#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void search_executable(const char *command)
{
    char *path_env = getenv("PATH");
    if (!path_env)
    {
        printf("%s: not found\n", command);
        return;
    }

    char *path = strdup(path_env);
    if (!path)
    {
        perror("strdup");
        return;
    }

    char *dir = strtok(path, ":");
    while (dir)
    {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);

        if (access(full_path, X_OK) == 0)
        {
            printf("%s\n", full_path);
            free(path);
            return;
        }
        dir = strtok(NULL, ":");
    }

    printf("%s: not found\n", command);
    free(path);
}

int main()
{
    setbuf(stdout, NULL);

    while (1)
    {
        printf("$ ");

        // Read input
        char input[100];
        if (!fgets(input, sizeof(input), stdin))
        {
            break;
        }
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit 0") == 0)
        {
            break;
        }

        char *firstPart = strtok(input, " ");
        char *secondPart = strtok(NULL, "");

        if (firstPart == NULL)
        {
            continue;
        }

        if (strcmp(firstPart, "echo") == 0)
        {
            if (secondPart)
            {
                printf("%s\n", secondPart);
            }
        }
        else if (strcmp(firstPart, "type") == 0)
        {
            if (secondPart)
            {

                if (strcmp(secondPart, "echo") == 0 || strcmp(secondPart, "type") == 0 || strcmp(secondPart, "exit") == 0)
                {
                    printf("%s is a shell builtin\n", secondPart);
                }
                else
                {
                    search_executable(secondPart);
                }
            }
            else
            {
                printf("type: missing argument\n");
            }
        }
        else
        {
            // Handle unknown command
            printf("%s: command not found\n", firstPart);
        }
    }

    return 0;
}
