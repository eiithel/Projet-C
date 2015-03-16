#include <stdlib.h>
#include <stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Mesh.h"
#include "Math.h"
#include "Controls.h"
#include "Input.h"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
    GLFWwindow* window;

    /*Init GLFW*/
    if (!glfwInit())
        exit(EXIT_FAILURE);

    /*Création d'une fenêtre 640x480 nommée Simple example*/
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    /*Si la fenêtre ne s'est pas ouverte*/
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* Création d'un contexte pour la fenêtre
     * et configuration des évènement provoquants
     * l'arrêt de cette fenêtre*/
    glfwSetErrorCallback(error_callback);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);


    /*Réglages de GLEW*/
    glewExperimental=GL_FALSE;				/*GLEW non expérimental*/
    GLenum error = glGetError();			/*Enumération des erreurs possibles par la variable error*/

    /*S'il y a une erreure*/
    if (error != GL_NO_ERROR)
    {
        printf("OpenGL Error: %d\n", error);
    }



    /*Init GLEW*/
    GLenum glewinit = glewInit();

    /*S'il y a une erreure d'init*/
    if (glewinit != GLEW_OK) {
        printf("Glew's not okay! %s\n", glewGetErrorString(glewinit));
        exit(EXIT_FAILURE);
    }

    /*Vérifie si l'ordinateur supporte l'API 2.1 de GLEW */
    if (!GLEW_VERSION_2_1)  // check that the machine supports the 2.1 API.
    	exit(EXIT_FAILURE);

    float t = 0;
    /*Utilisation des shaders (couleurs ombres...)*/
    Shader shader = {"Shaders/vertex.vert", "Shaders/fragment.frag",0,0,0};
    shader_load(&shader);

    /* Création d'une variable de type Mesh. Un mesh est une figure
     * tridimensionnel. Un mesh est basé sur un enssemble de triangles*/
    Mesh mesh;

    /*Chargement de l'objet demandé (décrit dans un fichier .obj)*/
    mesh_load_from_obj(&mesh, "Models/Tank.obj");

    /*Récupération des sommets et des normales*/
    float *vertices = mesh_get_vertices(&mesh);
    float *normals = mesh_get_normals(&mesh);

    // M : ModelView : Position du model dans l'espace
    // V : View : Position de la camera
    // P : Perspective : Perspective de la camera
    mat4x4 M, V, P, MVP, tempM;
    vec3 pos = {0,30,-20}; // Position de la camera dans l'espace
    vec3 dir = {0,0,0}; // Cible de la camera dans l'espace
    vec3 up = {0,1,0}; // Axe de la camera (tête en haut)
    vec3 target;

    /*Target = dir - pos*/
    vec3_sub(target, dir, pos);

    mat4x4_identity(M);

    /*Ce que la caméra regarde*/
    mat4x4_look_at(V,pos,dir,up);

    /*Réglage caméra :
     * Perspective
     * Angle de vue
     * Ratio de la fenêtre
     * distance d'affichage : 0.1 unités <-> 100 unités */
    mat4x4_perspective(P, 70.f, 640.f/480.f, 0.1f, 100.f);

    // Active le test de profondeur
    glEnable(GL_DEPTH_TEST);

    /*Couleur du fond (R, V, B, alpha)*/
    glClearColor(0.0f, 0.0f, 0.4f, 0.8f);

    init_controlsTable();

    controls_create("P1_up", GLFW_KEY_UP);
    controls_create("P1_down", GLFW_KEY_DOWN);
    controls_create("P1_left", GLFW_KEY_LEFT);
    controls_create("P1_right", GLFW_KEY_RIGHT);

    /*Boucle infinie permettant les interactions*/
    while (!glfwWindowShouldClose(window))
    {
    	/*Test des appuis touches et actions associées*/
    	/*
    	if(glfwGetKey(window, GLFW_KEY_LEFT)==GLFW_PRESS)
    		mat4x4_rotate_Y(M, M, -0.002f);
    	if(glfwGetKey(window, GLFW_KEY_RIGHT)==GLFW_PRESS)
    	    mat4x4_rotate_Y(M, M, 0.002f);
    	if(glfwGetKey(window, GLFW_KEY_UP)==GLFW_PRESS)
    		mat4x4_translate_in_place(M, 0,0,-0.008);
    	if(glfwGetKey(window, GLFW_KEY_DOWN)==GLFW_PRESS)
    		mat4x4_translate_in_place(M, 0,0,+0.005);
    	//t = glfwGetTime();
    	*/

    	input_update(window);

    	if(input_keypressed("P1_left"))
    	    mat4x4_rotate_Y(M, M, -0.002f);
    	if(input_keypressed("P1_right"))
    		mat4x4_rotate_Y(M, M, 0.002f);
    	if(input_keypressed("P1_up"))
    		mat4x4_translate_in_place(M, 0,0,-0.008);
    	if(input_keypressed("P1_down"))
    		mat4x4_translate_in_place(M, 0,0,+0.005);


        mat4x4_mul(MVP, V, M); // MVP = P * V * M
        mat4x4_mul(MVP, P, MVP);

    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // On vide les tampons couleurs et profondeur

    	/*Utilisation du shader*/
    	glUseProgram(shader.program); // On verouille le shader

    	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    	    glEnableVertexAttribArray(0);
    	    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, normals);
    	    glEnableVertexAttribArray(1);

    	    glUniformMatrix4fv(glGetUniformLocation(shader.program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    	    glUniformMatrix4fv(glGetUniformLocation(shader.program, "M"), 1, GL_FALSE, &M[0][0]);
    	    glUniformMatrix4fv(glGetUniformLocation(shader.program, "V"), 1, GL_FALSE, &V[0][0]);
    	    glUniformMatrix4fv(glGetUniformLocation(shader.program, "P"), 1, GL_FALSE, &P[0][0]);
    	    glUniform3fv(glGetUniformLocation(shader.program, "target"), 1,pos);

    	    /* Génère l'affichage (démarre au sommet 0 et affiche les 3*mesh.f suivants soit
    	     * tout l'objet. Ici 1 mesh est un triangle et mesh.f est le nombre de triangle.
    	     * Puisqu'il y a 3 sommets par triangles, l'affichages de tous les sommets se
    	     * fait en utilisant mesh.f*3*/
    	    glDrawArrays(GL_TRIANGLES, 0, mesh.f*3);

    	    glDisableVertexAttribArray(1);
    	    glDisableVertexAttribArray(0);

    	/*Libération du shader*/
    	glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    /*Libération de la mémoire, destruction de la fenêtre et arrêt de GLFW*/
    mesh_free(&mesh);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
