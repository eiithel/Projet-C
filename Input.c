/*
 * Input.c
 *
 *  Created on: 8 mars 2015
 *      Author: Nicolas
 */
#include "Input.h"

extern Controls* controlsTable[20];

/*
 * Regarde l'�tat des touches dans contr�le et vient modifier l'�tat du statut
 * des touches en fonctions de si elles sont appuy�es ou non
 */
void input_update(GLFWwindow* window)
{
	/*
	 * Regarde toutes les touches actives (associ�es � des actions)
	 * et modifie le state des "controls"
	 */
	int i;
	extern int numberOfControls;

	for(i=0; i<numberOfControls; i++)
	{
		/*controlsTable[i]->key_state = input_keypressed(controlsTable[i]->action_name);*/
		controlsTable[i]->key_state = glfwGetKey(window, controlsTable[i]->key_number);
	}
}


/*
 * Associe une touche � un contr�le
 */
/*
void input_set_controls()
{

}
*/

/*
 * R�cup�re un Controls_s � partir de son nom
 */
Controls* input_get_controls(char* nom_control)
{
	int i;
	extern int numberOfControls;
	for(i=0; i<numberOfControls; i++)
	{
		if(strcmp(nom_control, controlsTable[i]->action_name) == 0)
		{
			return controlsTable[i];
		}
	}
	fprintf(stderr, "Nonexistent key name");
	return NULL;
}

Controls* input_get_controls_index(int key_number)
{
	int i;
	extern int numberOfControls;
	for(i=0; i<numberOfControls; i++)
	{
		if(key_number == controlsTable[i]->key_number)
		{
			return controlsTable[i];
		}
	}
	fprintf(stderr, "Nonexistent key name");
	return NULL;
}

/*
 * rend l'�tat de la touche demand�e (vraie si appuy�e)
 * ex : if(keypresse(player1, up))
 */
/*
int input_keypressed(char*, char*)
{

}
*/

int input_keypressed(char* nom_control)
{
	return (input_get_controls(nom_control))->key_state;
}

/*
int input_keypressed_index(int, int)
{

}
*/

int input_keypressed_index(int key)
{
	return (input_get_controls_index(key))->key_state;
}
