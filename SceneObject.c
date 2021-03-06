/*
 * SceneObject.c
 *
 *  Created on: 11 mars 2015
 *      Author: Gabriel
 */

#include "SceneObject.h"

SceneObject* so_create(char* name, Transform t) {
	SceneObject* so = (SceneObject*)malloc(sizeof(SceneObject));
	so->transform=t;
	so->name=name;
	so->mesh=NULL;
	so->shader=NULL;
	so->scripts=list_script_create();
	return so;
}

void so_detroy(SceneObject* so) {
	free(so->name);
	free(so);
}

SceneObject* so_duplicate(SceneObject* so, char* name, Transform t) {
	SceneObject* new_so = (SceneObject*)malloc(sizeof(SceneObject));
	new_so->name = name;
	new_so->mesh = so->mesh;
	new_so->shader = so->shader;
	new_so->transform = t;
	new_so->scripts = list_script_create();
	return new_so;
}

void so_setup(SceneObject* so) {
	node_script *iterator = so->scripts->root;
	while(iterator != NULL) {
		iterator->value->setup(iterator->value, so);
		iterator = iterator->next;
	}
}

void so_run(SceneObject* so) {
	node_script *iterator = so->scripts->root;
	while(iterator != NULL) {
		iterator->value->run(iterator->value, so);
		iterator = iterator->next;
	}
}

void so_draw(SceneObject* so, Camera* cam) {
	if(so->mesh != NULL && so->shader != NULL) {
		transform_refresh_matrix(&(so->transform));

		glUseProgram(so->shader->program); // On verouille le shader

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, so->mesh->vertices);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, so->mesh->normals);
		glEnableVertexAttribArray(1);

		glUniformMatrix4fv(glGetUniformLocation(so->shader->program, "M"), 1, GL_FALSE, mat4x4_ptr(so->transform.matrix));
		glUniformMatrix4fv(glGetUniformLocation(so->shader->program, "V"), 1, GL_FALSE, mat4x4_ptr(cam->transform.matrix));
		glUniformMatrix4fv(glGetUniformLocation(so->shader->program, "P"), 1, GL_FALSE, mat4x4_ptr(cam->perspective_matrix));
		//glUniform3fv(glGetUniformLocation(so->shader->program, "target"), 1,pos);

		glDrawArrays(GL_TRIANGLES, 0, so->mesh->f*3);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glUseProgram(0);
	}
}

SceneObject* so_from_transform(Transform* t){
	return (SceneObject*)t;
}

void so_add_script(SceneObject* so, Script* script) {
	list_script_put(so->scripts, script);
}
