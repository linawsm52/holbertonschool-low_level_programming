#ifndef DOG_H
#define DOG_H

/**
 * struct dog - information about a dog
 * @name: dog's name
 * @age: dog's age
 * @owner: owner's name
 *
 * Description: Basic structure that stores a dog's information.
 */
struct dog
{
	char *name;
	float age;
	char *owner;
};

/* Typedef for struct dog */
typedef struct dog dog_t;

/* Function prototypes */
void init_dog(struct dog *d, char *name, float age, char *owner);
void print_dog(struct dog *d);

#endif /* DOG_H */
