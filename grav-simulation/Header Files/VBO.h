#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include<vector>

class VBO {
	public:
		GLuint ID;
		VBO();
		VBO(const float* vertices, GLsizeiptr size);

		void Init(const float* vertices, GLsizeiptr size);

		void Bind();
		void Unbind();
		void Delete();


};
#endif // !VBO_CLASS
